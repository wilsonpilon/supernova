// Converts the original Supernova data files (Turbo Pascal "file of string[N]" records and the
// SM/B1 text blocks) into mapper text pools for the MSX port.
//   node mkdata.js <source dir> <out dir>
//
// Kinds (global record id = KIND_BASE[kind] + pointer used by the original game):
//   0 R  rooms       R1 + R2 concatenated      (RR)
//   1 S  specials    S1                        (RS)
//   2 L  lines       L1                        (RL)
//   3 C  clues       C1                        (default.pas: seek(C1,Prm))
//   4 M  text blocks SM, block p = lines between "(p-1)" and "(p)"  (RB)
//   5 B  text blocks B1, same                  (RB2)
//
// Outputs in <out dir>:
//   TXT<n>.BIN  one 16KB-max pool each: u16 count, u16 offs[count] (from pool start; 0 = no record),
//               then records: u16 uncompressed len, compressed bytes, 0.   Records never span pools.
//               Compressed bytes: literal ASCII, or a token code (byte values unused by the text)
//               that expands through TXT.TOK.
//   TXT.IDX     u8 nPools, u16 poolFirst[nPools] (first global id in each pool), u16 kindBase[6]
//   TXT.TOK     u16 ptr[256] (file offset, 0 = literal), then token strings (u8 len, bytes)
//   textdef.h   KIND_BASE / counts for the C code
const fs = require("fs");
const path = require("path");

const [src, out] = process.argv.slice(2);
if (!src || !out) { console.error("usage: node mkdata.js <source dir> <out dir>"); process.exit(1); }
fs.mkdirSync(out, { recursive: true });

const POOL_SIZE = 16384;

// Pascal `file of string[max]`: each record = 1 length byte + max bytes
function pascalRecs(file, max) {
	const b = fs.readFileSync(path.join(src, file));
	const size = max + 1;
	if (b.length % size) throw new Error(file + ": size " + b.length + " not a multiple of " + size);
	const r = [];
	for (let o = 0; o < b.length; o += size) r.push(b.slice(o + 1, o + 1 + b[o]).toString("latin1"));
	return r;
}

// SM / B1: "(n)" marker lines; record p = text between "(p-1)" and "(p)"; record 0 is empty
function blockRecs(file) {
	const lines = fs.readFileSync(path.join(src, file), "latin1").replace(//g, "").split(/\r?\n/);
	const r = [""];
	let cur = null;
	for (const l of lines) {
		const m = /^\((\d+)\)$/.exec(l);
		if (m) {
			const n = +m[1];
			if (cur !== null) {
				if (n !== r.length) throw new Error(file + ": block marker " + n + ", expected " + r.length);
				r.push(cur.join("\n"));
			} else if (n !== 0) throw new Error(file + ": first marker is " + n);
			cur = [];
		} else if (cur !== null) cur.push(l.replace(/\x1a/g, ""));
	}
	return r;
}

const r1 = pascalRecs("R1", 234), r2 = pascalRecs("R2", 234);
if (r1.length !== r2.length) throw new Error("R1/R2 length mismatch");
const kinds = [
	{ name: "R", recs: r1.map((s, i) => s + r2[i]) },
	{ name: "S", recs: pascalRecs("S1", 234) },
	{ name: "L", recs: pascalRecs("L1", 78) },
	{ name: "C", recs: pascalRecs("C1", 77) },
	{ name: "M", recs: blockRecs("SM") },
	{ name: "B", recs: blockRecs("B1") },
];

// flatten, remembering kind bases
const all = [];
const kindBase = [];
for (const k of kinds) { kindBase.push(all.length); for (const s of k.recs) all.push(s); }

// ---- BPE compression: unused byte values become tokens expanding to strings ----
const rawBufs = all.map(s => Buffer.from(s, "latin1"));
const usedBytes = new Set();
for (const b of rawBufs) for (const c of b) usedBytes.add(c);
const codes = [];
for (let c = 1; c < 256; c++) if (!usedBytes.has(c)) codes.push(c);

function trainBpe(bufs, maxTokens) {
	const seqs = bufs.map(b => Array.from(b));
	const merges = []; // [a, b] over symbols: <256 literal, >=256 token index+256
	for (let t = 0; t < maxTokens; t++) {
		const cnt = new Map();
		for (const s of seqs) for (let i = 0; i + 1 < s.length; i++) {
			const k = s[i] * 4096 + s[i + 1];
			cnt.set(k, (cnt.get(k) || 0) + 1);
		}
		let best = -1, bc = 0;
		for (const [k, v] of cnt) if (v > bc) { bc = v; best = k; }
		if (bc < 4) break;
		const x = Math.floor(best / 4096), y = best % 4096, sym = 256 + merges.length;
		merges.push([x, y]);
		for (let j = 0; j < seqs.length; j++) {
			const s = seqs[j], o = [];
			for (let i = 0; i < s.length; i++) {
				if (i + 1 < s.length && s[i] === x && s[i + 1] === y) { o.push(sym); i++; } else o.push(s[i]);
			}
			seqs[j] = o;
		}
	}
	return { merges, seqs };
}
const { merges, seqs } = trainBpe(rawBufs, codes.length);
const expand = sym => sym < 256 ? [sym] : expand(merges[sym - 256][0]).concat(expand(merges[sym - 256][1]));
const symCode = sym => sym < 256 ? sym : codes[sym - 256];
const tokStr = merges.map((_, i) => Buffer.from(expand(256 + i)));
const enc = seqs.map((s, i) => ({ ulen: rawBufs[i].length, c: Buffer.from(s.map(symCode)) }));

// verify round trip
const dec = Array(256).fill(null);
merges.forEach((_, i) => { dec[codes[i]] = tokStr[i]; });
enc.forEach((e, i) => {
	const o = [];
	for (const c of e.c) { if (dec[c]) o.push(...dec[c]); else o.push(c); }
	if (Buffer.from(o).compare(rawBufs[i]) !== 0) throw new Error("round trip failed for record " + i);
});

// TXT.TOK: u16 ptr[256] (offset into this file; 0 = literal byte), then blobs: u8 len, bytes
const tok = Buffer.alloc(512 + merges.reduce((a, m, i) => a + 1 + tokStr[i].length, 0));
let tpos = 512;
merges.forEach((_, i) => {
	tok.writeUInt16LE(tpos, 2 * codes[i]);
	tok[tpos++] = tokStr[i].length;
	tokStr[i].copy(tok, tpos);
	tpos += tokStr[i].length;
});
fs.writeFileSync(path.join(out, "TXT.TOK"), tok);

// pack sequentially into pools
const pools = [];
let cur = null;
function newPool(first) { cur = { first, recs: [] }; pools.push(cur); }
function poolSize(p, extra) {
	let n = 2 + 2 * p.recs.length;
	for (const e of p.recs) n += 2 + e.c.length + 1;
	return n + (extra === undefined ? 0 : 2 + 2 + extra + 1);
}
newPool(0);
enc.forEach((e, id) => {
	if (poolSize(cur, e.c.length) > POOL_SIZE) newPool(id);
	cur.recs.push(e);
});

pools.forEach((p, n) => {
	const buf = Buffer.alloc(poolSize(p));
	buf.writeUInt16LE(p.recs.length, 0);
	let pos = 2 + 2 * p.recs.length;
	p.recs.forEach((e, i) => {
		buf.writeUInt16LE(pos, 2 + 2 * i);
		buf.writeUInt16LE(e.ulen, pos);
		e.c.copy(buf, pos + 2);
		pos += 2 + e.c.length + 1;
	});
	fs.writeFileSync(path.join(out, "TXT" + n + ".BIN"), buf);
});

const idx = Buffer.alloc(1 + 2 * pools.length + 2 * kinds.length);
idx[0] = pools.length;
pools.forEach((p, i) => idx.writeUInt16LE(p.first, 1 + 2 * i));
kindBase.forEach((b, i) => idx.writeUInt16LE(b, 1 + 2 * pools.length + 2 * i));
fs.writeFileSync(path.join(out, "TXT.IDX"), idx);

let h = "// Generated by tools/mkdata.js - do not edit\n#pragma once\n";
h += "#define TXT_POOLS " + pools.length + "\n";
kinds.forEach((k, i) => { h += "#define TK_" + k.name + " " + kindBase[i] + " // " + k.recs.length + " records\n"; });
h += "#define TK_TOTAL " + all.length + "\n";
fs.writeFileSync(path.join(out, "textdef.h"), h);

let bytes = 0;
for (const s of all) bytes += s.length;
console.log("records:", all.length, " text bytes:", bytes, " pools:", pools.length);
kinds.forEach((k, i) => console.log("  " + k.name, "base", kindBase[i], "count", k.recs.length));
pools.forEach((p, i) => console.log("  pool", i, "first", p.first, "records", p.recs.length, "size", poolSize(p)));
console.log("longest record:", Math.max(...all.map(s => s.length)));
console.log("tokens:", merges.length, " compressed bytes:", enc.reduce((a, e) => a + e.c.length, 0), " token table file:", tok.length);

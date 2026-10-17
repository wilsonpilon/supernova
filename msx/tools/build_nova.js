// Builds the MSX game: a resident program (MSXgl build) plus code overlays that run at 8000h
// from MSX-DOS 2 mapper segments.  usage (from msx/nova):  node ../tools/build_nova.js
//
//  1. every overlay source is compiled to a .rel; the functions of an overlay that are called from
//     outside it (the resident program or other overlays) become "exports"
//  2. generated for the resident program: stubs (one per export: switch page 2, call, switch back),
//     the table of overlay segments and the loader
//  3. the resident program is built with the MSXgl build script
//  4. each overlay is linked at 8000h against the resident symbols (sdldz80 -g), converted to a 16KB
//     image and copied to the emulator disk directory
const fs = require("fs");
const path = require("path");
const cp = require("child_process");

const ROOT = path.resolve(__dirname, "..", "..");
const MSX = path.join(ROOT, "msx");
const PROJ = process.cwd();
const OUT = path.join(PROJ, "out");
const OVL_OUT = path.join(OUT, "ovl");
const GEN = path.join(PROJ, "gen");
const SDCC = path.join(ROOT, "MSXgl", "tools", "sdcc");
const MSXGL = path.join(ROOT, "MSXgl");
const NODE = path.join(MSXGL, "tools", "build", "Node", "node.exe");
const HEX2BIN = path.join(MSXGL, "tools", "MSXtk", "bin", "MSXhex.exe");

// overlay groups: name -> source files (in msx/game)
const OVERLAYS = [
	{ name: "PARSE", files: ["parser"] },
	{ name: "ROOMA", files: ["rooms1", "rooms2", "rooms3"] },
	{ name: "ROOMB", files: ["rooms4", "rooms5", "default1"] },
	{ name: "DEF",   files: ["default2", "default3"] },
	{ name: "TIME",  files: ["time", "ui2"] },
];
// resident sources (for the symbol scan) -> relative to msx
const RESIDENT_SCAN = ["game/globals", "game/util", "game/game", "game/rt_msx", "data/vocab_data", "txt", "textpool", "nova/main"];
const OVL_SIZE = 16384;

function run(cmd, opts) {
	return cp.execSync(cmd, Object.assign({ stdio: ["ignore", "pipe", "pipe"], env: Object.assign({}, process.env, { PATH: path.join(SDCC, "bin") + path.delimiter + process.env.PATH }) }, opts || {})).toString();
}
function mkdir(p) { fs.mkdirSync(p, { recursive: true }); }

const CFLAGS = `-c -mz80 -DTARGET=TARGET_DOS2 -DROM_SIZE=128 -DMSX_VERSION=MSX_2 -I"${PROJ}" -I"${MSXGL}/engine/src" -I"${MSXGL}/engine/content" -I"${MSXGL}/tools" --opt-code-size`;

function compile(src, rel) {
	try { run(`"${SDCC}/bin/sdcc.exe" ${CFLAGS} "${src}" -o "${rel}"`); }
	catch (e) { console.error(e.stdout ? e.stdout.toString() : "", e.stderr ? e.stderr.toString() : ""); throw new Error("compile failed: " + src); }
}

// symbols of an ASxxxx .rel: { defs: Set, refs: Set, areas: {name:size} }
function scanRel(file) {
	const t = fs.readFileSync(file, "latin1").split(/\r?\n/);
	const r = { defs: new Set(), refs: new Set(), areas: {} };
	for (const l of t) {
		let m;
		if ((m = /^S (\S+) Def/.exec(l))) r.defs.add(m[1]);
		else if ((m = /^S (\S+) Ref/.exec(l))) r.refs.add(m[1]);
		else if ((m = /^A (\S+) size ([0-9A-F]+)/.exec(l))) r.areas[m[1]] = parseInt(m[2], 16);
	}
	return r;
}

mkdir(OVL_OUT); mkdir(GEN);
console.log("== compile overlays");
const ovl = OVERLAYS.map((o, k) => Object.assign({}, o, { k, rels: [], scan: [] }));
for (const o of ovl) {
	for (const f of o.files) {
		const rel = path.join(OVL_OUT, f + ".rel");
		compile(path.join(MSX, "game", f + ".c"), rel);
		o.rels.push(rel);
		const s = scanRel(rel);
		o.scan.push(s);
		if (s.areas._INITIALIZED) throw new Error(f + ": initialised data in an overlay is not supported");
	}
	o.defs = new Set(); o.refs = new Set();
	o.scan.forEach(s => { s.defs.forEach(x => o.defs.add(x)); s.refs.forEach(x => o.refs.add(x)); });
	o.dataSize = o.scan.reduce((a, s) => a + (s.areas._DATA || 0), 0);
	o.codeSize = o.scan.reduce((a, s) => a + (s.areas._CODE || 0), 0);
}

console.log("== scan resident references");
const resRefs = new Set();
for (const f of RESIDENT_SCAN) {
	const src = path.join(MSX, f + ".c");
	if (!fs.existsSync(src)) continue;
	const rel = path.join(OVL_OUT, "res_" + path.basename(f) + ".rel");
	compile(src, rel);
	scanRel(rel).refs.forEach(x => resRefs.add(x));
}
// the stubs file and loader are generated below; the resident references what the game calls from main loop
// exports: defined in overlay k and referenced outside of it
for (const o of ovl) {
	const outside = new Set(resRefs);
	for (const p of ovl) if (p !== o) p.refs.forEach(x => outside.add(x));
	o.exports = [...o.defs].filter(d => outside.has(d)).sort();
	console.log(`  ${o.name}: code ${o.codeSize} data ${o.dataSize}, exports: ${o.exports.map(x => x.slice(1)).join(" ")}`);
	if (o.codeSize > OVL_SIZE) throw new Error(o.name + " is larger than 16KB");
}

// exported functions run through the stubs: arguments may only be passed in HL/DE (not in A) and the
// result in A/HL/DE, so only void functions or functions taking pointers are allowed
{
	const decl = fs.readFileSync(path.join(MSX, "game", "nova.h"), "latin1");
	for (const o of ovl) for (const s of o.exports) {
		const m = new RegExp("\\b(?:void|u8|u16|i16)\\s+" + s.slice(1) + "\\(([^)]*)\\)").exec(decl);
		if (!m) { console.warn("  warning: no prototype for exported " + s); continue; }
		const first = m[1].trim().split(",")[0];
		if (first !== "void" && first !== "" && first.indexOf("*") < 0) throw new Error("exported " + s + "(" + m[1] + "): the first argument would be passed in A");
	}
}

console.log("== generate stubs");
let asm = "; generated by tools/build_nova.js - do not edit\n\t.module ovl_stubs\n\t.globl\t_g_OvlSeg\n\t.globl\t_g_Page2\n\t.globl\t_DOSMapper_SetPage2\n\n";
asm += "\t.area\t_DATA\n__ovl_tmp:\n\t.ds\t2\n\n\t.area\t_CODE\n\n";
asm += "; BC = target address in page 2, [SP] = caller's BC, then the caller's return address\n";
for (const o of ovl) {
	asm += `__ovl_call_${o.k}:\n\tld\ta, (_g_Page2)\n\tpush\taf\n\tld\ta, (_g_OvlSeg + ${o.k})\n\tjp\t__ovl_go\n`;
}
asm += `
__ovl_go:
	ld	(_g_Page2), a
	push	hl
	push	de
	call	_DOSMapper_SetPage2
	pop	de
	pop	hl
	ld	(__ovl_tmp), bc
	ld	bc, #__ovl_ret
	push	bc
	ld	bc, (__ovl_tmp)
	push	bc
	ret				; "call" the target; it returns to __ovl_ret

__ovl_ret:
	ld	b, a			; keep a possible return value in A, HL, DE
	pop	af			; previous segment
	ld	(_g_Page2), a
	push	hl
	push	de
	call	_DOSMapper_SetPage2
	pop	de
	pop	hl
	ld	a, b
	pop	bc			; caller's BC
	ret

`;
for (const o of ovl) {
	o.exports.forEach((s, i) => {
		asm += `\t.globl\t${s}\n${s}::\n\tpush\tbc\n\tld\tbc, #${0x8000 + 3 * i}\n\tjp\t__ovl_call_${o.k}\n`;
	});
}
fs.writeFileSync(path.join(GEN, "ovl_stubs.s"), asm);

for (const o of ovl) {
	let h = `; generated - jump table of overlay ${o.name}\n\t.module ovl_head_${o.name}\n`;
	o.exports.forEach(s => { h += `\t.globl\t${s}\n`; });
	h += "\t.area\t_CODE\n";
	o.exports.forEach(s => { h += `\tjp\t${s}\n`; });
	fs.writeFileSync(path.join(GEN, `ovl_head_${o.name}.s`), h);
}

let c = `// generated by tools/build_nova.js - do not edit
#include "../../game/nova.h"
#include "../../game/mapper.h"
#include "dos.h"
#include "dos_mapper.h"

#define OVL_COUNT ${ovl.length}
u8 g_OvlSeg[OVL_COUNT];
static const char* const k_OvlFile[OVL_COUNT] = { ${ovl.map(o => `"NOVA${o.k}.OVL"`).join(", ")} };

// loads the overlay images into freshly allocated mapper segments
u8 OvlLoadAll(void)
{
	u8 i, ok = 1, save = g_Page2;
	for (i = 0; i < OVL_COUNT && ok; i++)
	{
		DOS_Segment seg;
		u8 f;
		if (!DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &seg)) { ok = 0; break; }
		g_OvlSeg[i] = seg.Number;
		Page2Set(seg.Number);
		f = DOS_OpenHandle(k_OvlFile[i], O_RDONLY);
		if (g_DOS_LastError) { ok = 0; break; }
		DOS_ReadHandle(f, (void*)0x8000, 16384);
		DOS_CloseHandle(f);
	}
	Page2Set(save);
	return ok;
}
`;
fs.writeFileSync(path.join(GEN, "ovl_gen.c"), c);

console.log("== build resident program");
{
	const out = cp.spawnSync(NODE, [path.join(MSXGL, "engine", "script", "js", "build.js")], { cwd: PROJ, env: Object.assign({}, process.env, { MSXGL_PATH: MSXGL }), encoding: "utf8" });
	const txt = (out.stdout || "") + (out.stderr || "");
	const lines = txt.split(/\r?\n/).filter(l => /error|warning|Seg\[|Higher|Size=/i.test(l) && !/DSK generation|msxtar/i.test(l));
	console.log(lines.join("\n"));
	if (!fs.existsSync(path.join(OUT, "nova.com"))) { console.error(txt); throw new Error("resident build failed"); }
}

// resident symbols
const sym = {};
for (const l of fs.readFileSync(path.join(OUT, "nova.noi"), "latin1").split(/\r?\n/)) {
	const m = /^DEF (\S+) 0x([0-9A-Fa-f]+)/.exec(l);
	if (m) sym[m[1]] = parseInt(m[2], 16);
}
const dataStart = sym["s__DATA"], dataLen = sym["l__DATA"], codeEnd = sym["s__CODE"] + sym["l__CODE"];
console.log(`  resident code ends at ${codeEnd.toString(16)}, data ${dataStart.toString(16)}+${dataLen.toString(16)}`);
if (Math.max(codeEnd, dataStart + dataLen + (sym["l__INITIALIZED"] || 0)) > 0x8000) console.warn("WARNING: the resident program reaches page 2!");

console.log("== link overlays");
let dataAddr = (sym["s__INITIALIZED"] + sym["l__INITIALIZED"] + 15) & ~15;    // the overlays' variables follow the resident ones
const disk = path.join(PROJ, "emul", "dos2");
mkdir(disk);
for (const o of ovl) {
	const head = path.join(OVL_OUT, `ovl_head_${o.name}`);
	run(`"${SDCC}/bin/sdasz80.exe" -plosgff -o "${head}.rel" "${path.join(GEN, `ovl_head_${o.name}.s`)}"`);
	const rels = [head + ".rel", ...o.rels];
	const defs = new Set(o.defs); o.exports.forEach(x => defs.add(x));
	const g = [];
	const missing = [];
	for (const r of o.refs) {
		if (defs.has(r)) continue;
		if (r.startsWith("__")) { missing.push(r); continue; }    // sdcc runtime helpers: linked from the library into the overlay
		if (sym[r] !== undefined) g.push(`-g ${r}=0x${sym[r].toString(16)}`);
		else missing.push(r);
	}
	const lk = ["-mjwx", `-i ${OVL_OUT}/${o.name}.ihx`, "-b _CODE = 0x8000", `-b _DATA = 0x${dataAddr.toString(16)}`,
		`-k ${SDCC}/lib/z80`, "-l z80", ...g, ...rels.map(r => r.replace(/\\/g, "/")), "", "-e", ""];
	fs.writeFileSync(path.join(OVL_OUT, o.name + ".lk"), lk.join("\n"));
	let log = "";
	try { log = run(`"${SDCC}/bin/sdldz80.exe" -nf "${path.join(OVL_OUT, o.name + ".lk")}"`); }
	catch (e) { console.error(e.stdout && e.stdout.toString(), e.stderr && e.stderr.toString()); throw new Error("link failed: " + o.name); }
	if (/Undefined|undefined/.test(log)) { console.error(log); throw new Error("undefined symbols in " + o.name); }
	run(`"${HEX2BIN}" "${OVL_OUT}/${o.name}.ihx" -e bin -s 0x8000 -o "${disk}/NOVA${o.k}.OVL" -l ${OVL_SIZE}`);
	const size = fs.statSync(path.join(disk, `NOVA${o.k}.OVL`)).size;
	console.log(`  ${o.name} -> NOVA${o.k}.OVL (${size} bytes), data at ${dataAddr.toString(16)} (+${o.dataSize}), ${g.length} resident symbols${missing.length ? ", from libs: " + missing.join(" ") : ""}`);
	dataAddr += o.dataSize;
}
for (const f of fs.readdirSync(path.join(MSX, "data"))) if (/.(BIN|IDX|TOK)$/i.test(f)) fs.copyFileSync(path.join(MSX, "data", f), path.join(disk, f));
fs.copyFileSync(path.join(OUT, "nova.com"), path.join(disk, "NOVA.COM"));
if (dataAddr > 0x7F00) console.warn("WARNING: overlay variables at " + dataAddr.toString(16));
console.log("== done");

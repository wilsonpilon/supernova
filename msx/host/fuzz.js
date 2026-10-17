// generates random command input for the host build: node fuzz.js <seed> <lines>
const fs = require("fs");
const voc = fs.readFileSync(process.argv[4] || "data/vocab_data.c", "latin1");
let seed = +process.argv[2] || 1;
const rnd = n => { seed = (seed * 1103515245 + 12345) & 0x7fffffff; return (seed >> 8) % n; };
const words = { v: [], n: [], p: [] };
for (const m of voc.matchAll(/static const char (s\d+)\[\] = "(.*)";/g)) {
	const first = m[2].split("\\\\")[0];
	if (first && /^[a-z ]+$/.test(first)) words.v.push(first);
}
const dirs = ["n", "s", "e", "w", "ne", "nw", "se", "sw", "u", "d", "in", "out", "fore", "aft", "port", "starboard"];
const preps = ["on", "with", "in", "to", "under", "at", "from", "beside", "using", "off", "for"];
let out = "ab\nbob\n";
const lines = +process.argv[3] || 500;
for (let i = 0; i < lines; i++) {
	const r = rnd(10);
	let l;
	const w = () => words.v[rnd(words.v.length)];
	if (r < 3) l = dirs[rnd(dirs.length)];
	else if (r < 7) l = w() + " " + w();
	else if (r < 9) l = w() + " " + w() + " " + preps[rnd(preps.length)] + " " + w();
	else l = w();
	out += l + "\n";
	if (rnd(8) == 0) out += "y\n";
}
process.stdout.write(out);

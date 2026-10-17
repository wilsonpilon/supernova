// Builds TEXT0.BIN / TEXT1.BIN: 16KB text-pool segments.
// Layout: u16 offsets[256] (from segment start, 0 = none), then records: u16 len, bytes, 0.
const fs = require("fs");
const path = require("path");
const out = process.argv[2];

function pool(records) {
	const buf = Buffer.alloc(16384);
	let pos = 512;
	records.forEach((s, i) => {
		buf.writeUInt16LE(pos, i * 2);
		buf.writeUInt16LE(s.length, pos);
		buf.write(s, pos + 2, "latin1");
		pos += 2 + s.length + 1;
	});
	if (pos > 16384) throw new Error("pool overflow");
	return buf;
}

const long = Array.from({ length: 40 }, (_, i) => "Line " + i + " of a long record that crosses many bytes. ").join("");
fs.writeFileSync(path.join(out, "text0.bin"), pool([
	"Record 0 (seg 0)", "Record 1 (seg 0)", "Record 2 (seg 0)",
	"Record 3 (seg 0): hello from the text pool",
	long
]));
fs.writeFileSync(path.join(out, "text1.bin"), pool([
	"Record 0 (seg 1)", "Record 1 (seg 1)",
	"Record 2 of pool 1: second segment works"
]));
console.log("long record length", long.length);

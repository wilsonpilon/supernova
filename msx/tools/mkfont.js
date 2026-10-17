// Extracts the 6x8 MSX BIOS font (256 glyphs x 8 bytes) from MSXgl's font_bios_latin.h into FONT.BIN
//   node mkfont.js <font_bios_latin.h> <out dir>
const fs = require("fs");
const path = require("path");
const args = process.argv.slice(2);
const bold = args.includes("--bold");
const [src, out] = args.filter(a => a[0] !== "-");
if (!src || !out) { console.error("usage: node mkfont.js <font_bios_latin.h> <out dir>"); process.exit(1); }
const t = fs.readFileSync(src, "latin1");
const i = t.indexOf("g_Font_BIOS_EU");
const open = t.indexOf("{", i);
const close = t.indexOf("};", open);
// drop comments, then read the hex bytes
const body = t.slice(open + 1, close).replace(/\/\*[\s\S]*?\*\//g, "").replace(/\/\/[^\n]*/g, "");
const nums = (body.match(/0x[0-9A-Fa-f]{1,2}/g) || []).map(s => parseInt(s, 16));
// 4-byte header (width, height, first, last), then 8 bytes per glyph
const glyphs = Buffer.from(nums.slice(4));
if (glyphs.length < 255 * 8) throw new Error("font too short: " + glyphs.length);
const buf = Buffer.alloc(2048);
glyphs.copy(buf, 0, 0, Math.min(2048, glyphs.length));
const thin = Buffer.from(buf);
const WIDE = new Set("mwMW@".split("").map(c => c.charCodeAt(0)));   // glyphs whose gaps would fill in
if (bold) for (let k = 0; k < buf.length; k++) if (!WIDE.has(k >> 3)) buf[k] = (buf[k] | (buf[k] >> 1)) & 0xFC;   // smear one pixel to the right inside the 6-pixel cell
fs.mkdirSync(out, { recursive: true });
fs.writeFileSync(path.join(out, "FONT.BIN"), buf);
fs.writeFileSync(path.join(out, "FONT_THIN.TXT"), thin);   // original shapes, rename to FONT.BIN to use them
console.log("FONT.BIN written, header:", nums.slice(0, 4).join(" "));

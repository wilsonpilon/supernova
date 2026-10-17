# Supernova for MSX2

An unofficial, non-commercial port of **Supernova** (Apogee Software, 1985-1990, by Scott Miller and
Terry Nagy), the classic text adventure whose Turbo Pascal source was released under the GPL in 2009,
to the **MSX2** with **MSX-DOS 2**, written in C with [SDCC](https://sdcc.sourceforge.net/) and
[MSXgl](https://github.com/aoineko-fr/MSXgl).

The original is an 80-column game in which every character cell can have its own colour. On the MSX2
this is done in **SCREEN 7** (512x212, 16 colours) with a software text engine, so all the original colours
(and blinking text) are kept.

## Requirements

| | |
|---|---|
| Machine | MSX2 or better, **128 KB VRAM** |
| RAM | memory mapper, **256 KB** or more |
| OS | **MSX-DOS 2**; the TPA must reach `D900h` (the program checks at start-up) |
| Disk | about 220 KB |

Tested with openMSX (Philips NMS 8250 + MSX-DOS 2 + 512 KB mapper). Not tested on real hardware.

## Playing

Files needed on the disk: `NOVA.COM`, `NOVA0.OVL`-`NOVA4.OVL`, `TXT0.BIN`-`TXT3.BIN`, `TXT.IDX`,
`TXT.TOK`, `FONT.BIN`. Run `NOVA`. Commands are those of the original (see
`freeware/INSTRUCT.TXT` in the original release): `LOOK`, `INVENTORY`, `OPEN THE HATCH THEN ENTER THE SHIP`...

| MSX key | Original key | Command |
|---|---|---|
| F1 / F2 | F1 / F2 | Save / Restore |
| F3 / F4 / F5 | F3 / F4 / F5 | R D / Look / Get all |
| Shift+F1..F5 | F6..F10 | Drop all / Score / Inventory / Wait / Repeat |
| Cursor keys | cursor keys | North/West/East/South (Fore/Port/Starboard/Aft on the ship) |
| Home, Select, Ins, Del | Home, PgUp, Ins, Del | NW, NE, Down, Up |

Saved games are single `NAME.SAV` files (the original wrote four files and asked for a drive).

## Building

Prerequisites: Node.js (the MSXgl one is fine), a checkout of MSXgl in `MSXgl/` (it brings SDCC and the
tools), and openMSX with the MSX-DOS 2 ROM to try it.

```
git clone https://github.com/aoineko-fr/MSXgl MSXgl
cd msx/nova
node ../tools/build_nova.js          # resident program + overlays, files go to emul/dos2
```

Data files are generated from the original source with:

```
node tools/mkdata.js  ../freeware/source/SOURCE data      # texts -> compressed pools (TXT*.BIN)
node tools/mkvocab.js ../freeware/source/SOURCE/VOCAB.PAS data   # vocabulary and object locations (C)
node tools/mkfont.js --bold ../MSXgl/engine/content/font/font_bios_latin.h data   # FONT.BIN
```

`msx/nova/drive.sh` runs the game in openMSX, types commands and takes screenshots (see its header).
`msx/host/build.sh` builds the game logic for the PC (gcc) with a plain text runtime, used to compare
behaviour while translating and for fuzzing.

## How it is built

* **Translation**: `NOVA.PAS`, `ADPAR.PAS` and `DEFAULT.PAS` were translated by hand to C in `msx/game`,
  keeping the Pascal names so the code can be compared with the original. Pascal sets are bit arrays,
  strings are 1-based through helper functions, out-of-range set tests are false as in Pascal.
* **Memory**: the Z80 code is ~75 KB. A small resident part lives below `8000h`; the rest is in five
  16 KB **overlays** that are loaded into mapper segments and mapped at `8000h` on demand through generated
  stubs (`tools/build_nova.js`). The overlays link against the resident symbols.
* **Text**: the 1103 texts (117 KB) are stored compressed (byte-pair encoding, 48%) in four 16 KB mapper
  segments, and decoded by a Z80 routine into a buffer.
* **Screen** (`msx/game/rt_msx.c`, `msx/txt.c`): Turbo-Pascal-style windows over SCREEN 7. Glyphs are drawn by a
  Z80 blitter using lookup tables per colour pair; scrolling is deferred (new lines are drawn below the visible
  area and the window is scrolled once with a VDP copy); blinking text is redrawn while waiting for a key.
* **Sound**: PSG square wave; pitch changes are limited to one per frame.

## Credits and licence

* *Supernova* is a trademark and copyright of Apogee Software, Ltd. The source code was released under the GPL
  v2 (`freeware/source/GNU.TXT`) and this port is a derivative work under the same licence. This is an unofficial
  fan port, not affiliated with or endorsed by Apogee / 3D Realms, **for non-commercial use only**. See
  `freeware/readme.txt` for the terms of the original release.
* Original game: Scott Miller (programming, story) and Terry Nagy (story).
* [MSXgl](https://github.com/aoineko-fr/MSXgl) by Guillaume "Aoineko" Blanchard; the 6x8 font in `FONT.BIN` is
  derived from its `font_bios_latin` data (CC BY-SA).
* SDCC, openMSX.

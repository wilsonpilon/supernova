# Build overlay binaries linked at 8000h, padded to 16 KB, into emul/dos2
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Join-Path $here "..\..\MSXgl"
$sdcc = Join-Path $root "tools\sdcc\bin\sdcc.exe"
$env:PATH = "$(Split-Path $sdcc);$env:PATH"
$hex  = Join-Path $root "tools\MSXtk\bin\MSXhex.exe"
New-Item -ItemType Directory -Force "$here\out" | Out-Null
foreach ($n in "ovla","ovlb") {
  & $sdcc -mz80 --no-std-crt0 --code-loc 0x8000 --data-loc 0xBF00 --opt-code-size "$here\ovl\$n.c" -o "$here\out\$n.ihx" 2>&1 | Select-String "error|warning"
  & $hex "$here\out\$n.ihx" -e bin -s 0x8000 -o "$here\out\$n.bin" -l 16384 | Out-Null
  Get-Item "$here\out\$n.bin" | Select-Object Name,Length
}

#!/bin/sh
# usage: drive.sh <end-seconds> "delay|text|shotdelay" ...
#   e.g.  drive.sh 200 "12| |-1" "3| |-1" "3|bob\r|-1" "8|look\r|6"
# delay = seconds after the previous item (the first one counts from the start of the game, i.e. Title),
# text is typed, shotdelay = seconds after typing to take a screenshot (-1: none)
cd "$(dirname "$0")"
end=$1; shift
taddr=$(awk '/^ +0000[0-9A-F]+ +_Title / {print $1}' out/nova.map | head -1 | sed 's/^0*/0x/')
rm -f out/shot*.png
q=""
for a in "$@"; do
  d=${a%%|*}; rest=${a#*|}; text=${rest%|*}; s=${rest##*|}
  esc=$(printf '%s' "$text" | sed 's/"/\\"/g')
  q="$q {$d \"$esc\" $s}"
done
{
  echo "set title_addr $taddr"
  echo "set outdir {E:/supernova/msx/nova/out}"
  echo "set endtime $end"
  echo "set script [list $q]"
  cat drive.tcl
} > out/drive_run.tcl
openmsx -machine Philips_NMS_8250 -ext msxdos2 -ext ram512k -diska emul/dos2 -script out/drive_run.tcl >/dev/null 2>&1
ls out/shot*.png

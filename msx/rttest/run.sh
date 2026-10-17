#!/bin/sh
# usage: run.sh s40 k45 s50 ...   sN = screenshot at emulated second N (out/sK.png in order), kN = type a space at N
cd "$(dirname "$0")"
cp out/rttest.com emul/dos2/
: > out/shot.tcl
echo "set throttle off" >> out/shot.tcl
n=1; last=0
for a in "$@"; do
  t=${a#?}
  case $a in
    s*) echo "after time $t { screenshot {E:/supernova/msx/rttest/out/s$n.png} }" >> out/shot.tcl; n=$((n+1)) ;;
    k*) echo "after time $t { type \" \" }" >> out/shot.tcl ;;
  esac
  last=$t
done
echo "after time $((last+1)) { exit }" >> out/shot.tcl
rm -f out/s*.png
openmsx -machine Philips_NMS_8250 -ext msxdos2 -ext ram512k -diska emul/dos2 -script out/shot.tcl >/dev/null 2>&1
ls out/s*.png

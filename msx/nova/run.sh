#!/bin/sh
# usage: run.sh s50 t52:"text" ...   sN = screenshot at emulated second N (out/sK.png), tN:text = type text at N (\r = Enter)
cd "$(dirname "$0")"
: > out/shot.tcl
echo "set throttle off" >> out/shot.tcl
n=1; last=0
for a in "$@"; do
  case $a in
    s*) t=${a#s}; echo "after time $t { screenshot {E:/supernova/msx/nova/out/s$n.png} }" >> out/shot.tcl; n=$((n+1)) ;;
    t*) t=${a#t}; t=${t%%:*}; txt=${a#*:}; echo "after time $t { type \"$txt\" }" >> out/shot.tcl ;;
  esac
  last=$t
done
echo "after time $((last+1)) { exit }" >> out/shot.tcl
rm -f out/s*.png
openmsx -machine Philips_NMS_8250 -ext msxdos2 -ext ram512k -diska emul/dos2 -script out/shot.tcl >/dev/null 2>&1
ls out/s*.png

#!/bin/sh
# Builds the PC test version of the game (run from msx/)
gcc -DHOST_TEST -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -std=gnu99 -o host/nova_host.exe \
  game/globals.c game/util.c game/time.c game/parser.c game/ui2.c game/game.c \
  game/default1.c game/default2.c game/default3.c \
  game/rooms1.c game/rooms2.c game/rooms3.c game/rooms4.c game/rooms5.c data/vocab_data.c host/rt_host.c "$@"

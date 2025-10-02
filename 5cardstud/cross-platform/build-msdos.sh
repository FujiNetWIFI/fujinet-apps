#!/bin/bash
wcl -q src/main.c src/gamelogic.c src/misc.c src/screens.c src/stateclient.c src/msdos/appkey.c src/msdos/graphics.c src/msdos/input.c src/msdos/network.c src/msdos/sound.c src/msdos/util.c && mv main.exe fcs.exe


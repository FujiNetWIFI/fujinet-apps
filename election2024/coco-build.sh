#!/bin/bash

rm -f election.bin election.dsk

cmoc -oelection.bin -Isrc/common src/main.c src/common/fetch.c src/common/ruminate.c src/common/parse.c src/coco/screen.c -lfujinet 

decb dskini election.dsk
writecocofile election.dsk election.bin
cp election.dsk ~/tnfs

rm -f election.bin election.dsk

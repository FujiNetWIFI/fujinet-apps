#!/bin/bash

rm -f election.bin election.dsk

zcc +coleco -subtype=adam -create-app -oelection.ddp -Isrc/common src/main.c -DFETCH_OVERRIDE=1 src/common/fetch.c src/common/ruminate.c src/common/parse.c src/adam/fetch.c src/adam/screen.c -leos -lsmartkeys

cp election.ddp ~/tnfs/election.adam.ddp

rm -f election.ddp


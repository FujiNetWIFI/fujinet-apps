#!/bin/bash

rm -f election.bin election.ptp election.rmm

# zcc +pmd85 -create-app -oelection.bin -Isrc/common -DFETCH_OVERRIDE=1 -DFETCH_MOCK=1 \
zcc +pmd85 -create-app -oelection.bin -Isrc/common -DFETCH_OVERRIDE=1 \
    src/main.c src/common/fetch.c \
    src/common/ruminate.c src/common/parse.c \
    src/pmd85/fetch.c src/pmd85/screen_util_pmd85.asm src/pmd85/screen.c \
    src/pmd85/dwread_becker_pmd85.asm src/pmd85/dwwrite_becker_pmd85.asm \
    src/pmd85/net.c

(/bin/printf '\xcd\x00\x8c\x00\x00\xff\x1f\x00\x00'; \
    dd if=election.bin count=4 bs=1 status=none; \
    dd if=election.bin skip=13 bs=1 status=none) > election.rmm

cp election.rmm ~/tnfs/election.pmd85.rmm

# rm -f election.rmm

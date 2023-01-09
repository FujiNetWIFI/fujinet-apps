#!/bin/bash

CC65_HOME=/usr/local
PATH=$PATH:/cygdrive/d/VICE

make -j5 clean
make -j5
make dist-apple2
make dist-apple2enh
make dist-atari
make dist-c64
make dist-c128

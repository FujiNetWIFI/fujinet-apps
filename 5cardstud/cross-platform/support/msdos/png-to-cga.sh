#!/bin/bash

# Input file should be PNG with colors mapped as:
#
# 0    0    0    Color 0 BLACK
# 64   64   64   Color 1 GREEN/CYAN
# 128  128  128  Color 2 RED/MAGENTA
# 192  192  192  Color 3 WHITE

convert tile.png -depth 2 gray:tile.raw
xxd -i tile.raw

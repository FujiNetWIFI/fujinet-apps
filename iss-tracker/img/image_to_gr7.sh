#!/bin/bash

convert map.ppm -depth 8 +dither -remap atari128.ppm pnm:- \
| convert - -depth 8 +dither -colors 4 pnm:- \
| convert - +dither -remap atari128.ppm -interpolate nearest pnm:- \
| ./ppm_to_gr7.php > map.gr7


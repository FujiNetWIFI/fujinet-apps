International Space Station (ISS) Tracker for #FujiNet
======================================================

Bill Kendrick <bill@newbreedsoftware.com>

Released under the GNU General Public License 3 (GPL-3);
see COPYING

2021-05-17 - 2024-05-21

## Purpose
View the current position of the International Space Station
over the earth, track some of it's upcoming positions,
and even see how many people are in space, and who they are.

This simple client utilizes the data available freely from
[Open Notify](http://open-notify.org/) and
[Where The ISS At?](https://wheretheiss.at/).

## Usage
Launch the program.  A map of the Earth will appear, with
title/credits drawn over it.  Press a key to continue.

Your FujiNet device will attempt to fetch the location of
the ISS, and place it on the map.  The latitude and longitude
will be shown at the bottom.

Latitude is degrees north of the equator.  (0 is at the equator,
positive is north of the equator, and negative is south.)

Longitude is degrees east of prime meridian.  (Positive is east, towards
the right side of the map (toward Asia, Australia, etc.), and negative
is west (toward the Americas).)

### Refreshing
Press the `[R]` key to fetch updated coordinates, and refresh the ISS's
position on the map.  (Or wait a while, and it will refresh automatically.)

### Who's in Space?
Press the `[P]` key to see
[how many people are in space right now](https://www.howmanypeopleareinspacerightnow.com/).
The information will be fetched from the internet, and the number of
people will be displayed over the map.

Each person's name will appear (along with what craft they're in --
e.g., the ISS -- at the bottom) one by one.  Press a key to see
the next name, or `[Esc]` to return to the map immediately.

### Track the ISS
Press the `[T]` key to have the program fetch the upcoming
positions of the ISS over the next ~100 minutes.  They will
appear as dashes on the map.

## Credits
Map image based on [Equirectangular projection SW.jpg](https://commons.wikimedia.org/wiki/File:Equirectangular_projection_SW.jpg)
created by [Daniel R. Strebe](https://commons.wikimedia.org/wiki/User:Strebe),
and licensed under the
[Creative Commons Attribution-Share Alike 3.0 Unported license](https://creativecommons.org/licenses/by-sa/3.0/deed.en).

Programming and ISS sprite by Bill Kendrick.

Data sources provided by Nathan Bergey (Open-Notify)
and Bill Shupp (Where The ISS At?).

Updates to fetch and use the FujiNet Network Lib library
by Frank Rachel <https://github.com/frachel>

Build system / Makefile reworking; more FujiNet-Lib work
by Mark Fisher (fenrock) <https://github.com/markjfisher>

## Requirements
This requires a #FujiNet with firmware capable of parsing and
allowing querying of JSON.

### Compilation Requirements

 * cc65 tools to compile, assemble, and link
   <https://cc65.github.io/>

 * Internet access is required, to fetch a release ZIP file
   of Fujinet Network Lib <https://github.com/FujiNetWIFI/fujinet-lib>
   + curl <https://curl.se/>
   + `bash`, `awk`, `head`
   + unzip (e.g. Info-ZIP <https://infozip.sourceforge.net/>)

## Links
- Homepage: http://www.newbreedsoftware.com/fujinet-iss-tracker/
- Git source code repository: https://github.com/FujiNetWIFI/fujinet-apps/tree/master/iss-tracker/atari


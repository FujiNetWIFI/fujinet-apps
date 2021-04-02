apod (Astronomy Picture of the Day) Client for #FujiNet
=======================================================

Bill Kendrick bill@newbreedsoftware.com
with help from apc & others (see credits, below)

2021-03-27 - 2021-04-02

## Purpose
Fetch and view [NASA's Astronomy Picture of the Day (APOD)](https://apod.nasa.gov/apod/),
via the "apod" server (which converts it to a format suitable for quickly loading on
an Atari, via HTTP for an Atari with a #FujiNet and its `N:` device.)

The current image of the day (fetched by the server, if it hasn't
already been during the current day (Pacific time zone)) will
be loaded and displayed on your Atari!

## Usage
From the menu, press one of the follow keys to load and render
an image in one of the following graphics modes:

- `[A]` - High resolution mono (320x192 black and white)
- `[B]` - Medium resolution greyscale (160x192 four shades of grey)
- `[C]` - Low resolution greyscale (80x192 sixteen shades of grey)
- `[D]` - Low resolution 4,096 color (via flickering; "ColorView" mode)

Once the image appears, press `[Esc]` to return to the menu.

### Samples
Press keys `[1]` through `[4]` to fetch a static sample image
from the webserver.  (A source JPEG or PNG image is loaded and
converted with the same workflow as the APOD images, which allows
us developers to tweak and test the process using known images.)

Currently, the images are:

- 1 - A 320x192 screenshot of the "gate" from the Atari 8-bit game "Alternate Reality: The City"
- 2 - Nebula NGC 2818
- 3 - A parrot
- 4 - An astronaut spacewalking above earth

Press `[5]` to render a test pattern.  In 4,096 color mode,
a colorbar battern will appear with 6 colors at the top
(red, yellow, green, cyan, blue, and purple), and sets of
16 and 8 shades of grey at the bottom.  This does not
require a #FujiNet device.

## Work in progress
This project is a work in progress!

## Credits

h/t Wrathchild @ Atari Age forums
(https://atariage.com/forums/profile/1822-wrathchild/)
for a sample .cfg file in a discussion regarding cc65
with Yaron Nir.

h/t Jan (apc @ Atari Age forums and #FujiNet discord)
(https://atariage.com/forums/profile/73813-apc/)
for further .cfg file adjustments, and getting the
ColorView DLI to work correctly, and creating the original
color bars sample.


apod (Astronomy Picture of the Day) Client for #FujiNet
=======================================================

Bill Kendrick bill@newbreedsoftware.com
with help from apc & others (see credits, below)

Released under the GNU General Public License 3 (GPL-3);
see COPYING

2021-03-27 - 2021-05-05

## Purpose
Fetch and view [NASA's Astronomy Picture of the Day (APOD)](https://apod.nasa.gov/apod/),
via the "apod" server (which converts it to a format suitable for quickly loading on
an Atari, via HTTP for an Atari with a #FujiNet and its `N:` device.)

The current image of the day (fetched by the "apod" server from the
official APOD website, if it hasn't already been during the current day)
will be loaded and displayed on your Atari!

## How to view things
This client program, and the server that powers it, offers
a variety of viewing options, using various Atari graphics modes,
both standard "hardware" modes, as well as "software" modes
that flicker between different images to provide the perception
of additional colors.

From the menu, press one of the follow keys to load and render
an image in one of the following graphics modes:

- `[H]` - High resolution mono (320x192 black and white, aka "GRAPHICS 8")
- `[M]` - Medium resolution four-color (160x192 four best colors, aka "GRAPHICS 15" aka "GRAPHICS 7+")
- `[C]` - Medium resolution 64-color (via flickering; "ColorView" mode)
- `[G]` - Low resolution greyscale (80x192 sixteen shades of grey, aka "GRAPHICS 9")
- `[F]` - Low resolution 4,096 color (via flickering; "ColorView" mode)
- `[A]` - Low resolution 256 color (via flickering; "Any Point, Any Color (APAC)" mode)

Along with the image, the title of the picture will appear at the bottom of the screen.

Once the image appears, press `[Esc]` to return to the menu.

## What to view

### Picking an photo-of-the-day date
You can opt to view a different day's APOD by chosing the date.

- `[<]`, `[>]` - Previous / next day
- `[Shift]`+`[<]`, `[Shift]`+`[>]` - Previous / next month
- `[Ctrl]`+`[<]`, `[Ctrl]`+`[>]` - Previous / next year
- `[=]` - Fetch the current APOD
- `[Ctrl]`+`[T]` - Try to fetch current date from #FujiNet again (in case of error, or you're running APOD for a long time)

Note - The APOD photos (and hence the APOD server's idea of
"what's the latest photo") is based on Eastern US time.
Your #FujiNet is probably set to your current time zone.
So if you want the latest image, get the "current" image
(the default; or press `[=]`).

### Adjusting and saving color settings
Noted below, in the "While Viewing" section, you can adjust
some color settings in some modes.  You can also change the
settings from the main menu.

You can also press the `[S]` key to save the settings
to your FujiNet (via an "app key"), and the settings will
be loaded the next time you launch APOD Viewer.

### Viewing samples
#### Server samples
Press keys `[1]` through `[5]` to fetch a static sample image
from the webserver.  (A source JPEG or PNG image is loaded and
converted with the same workflow as the APOD images, which allows
us developers to tweak and test the process using known images.)

Currently, the images are:

- 1 - A 320x192 screenshot of the "gate" from the Atari 8-bit game "Alternate Reality: The City"
- 2 - Nebula NGC 2818
- 3 - A parrot
- 4 - An astronaut spacewalking above earth
- 5 - Rainbow gradients

#### Local samples
Press `[9]` to render a test pattern.  This is done locally (and in fact
does not require a #FujiNet device).

- In high resolution mono mode, a set of bars will appear, dithered to simulate shading.
- In medium resolution four-color mode and low resolution greyscale mode, grey bars will appear.
- In low resoluton 4,096 color mode, a colorbar battern will appear with 6 colors at the top (red, yellow, green, cyan, blue, and purple), and sets of 16 and 8 shades of grey at the bottom.
- In medium resoluton 64 color mode, a colorbar battern will appear with 6 colors at the top (red, yellow, green, cyan, blue, and purple), and 4 shades of grey at the bottom.
- In low resolution 256 color mode, a grid of 256 colors (16 shades across, 16 hues down) appears.

Press `[0]` to choose to fetch an APOD photo (the default).

## While Viewing
Press `[Esc]` to return to the menu.

### Color adjustments
#### ColorView
When viewing a ColorView image, you can press `[R]`, `[G]`, and
`[B]` to adjust (increment) the Atari color palette hue that's
used for the red, green, and blue components of the image,
respectively.  Press those keys with `[Shift]` held down to adjust
the hue in the opposite direction (decrement).

Here are the Atari's hues (after trying to sort out various sources:
Compute!'s First and Second Books of Atari Graphics, and
Understanding Atari Graphics), along with which are used as the
red, green, and blue defaults by this program's ColorView modes:

- 0 - gray (no color)
- 1 - light orange (gold)
- 2 - orange -- RED
- 3 - red orange
- 4 - pink
- 5 - purple / violet / lavender
- 6 - blue-purple
- 7 - blue / purple-blue
- 8 - medium blue
- 9 - light blue
- 10 - turquoise
- 11 - green-blue -- BLUE
- 12 - green
- 13 - yellow-green -- GREEN
- 14 - orange-green
- 15 - light orange

Press `[X]` to reset the hues to these defaults.

Note: The current hue settings are shown (in decimal)
on the menu.

#### APAC (256 color)
When viewing in this mode, press `[L]` to increase the
luminence (brightness) of the hue component of the image.
Press `[Shift]`+`[L]` to decrease it.  There are 8 levels,
from 0 to 14 (stepping by 2s).

The default luminence is 2 (the second-to-lowest brightness).

Pressing `[X]` will reset this setting, as well.

Note: The current luminence setting is shown (in decimal)
on the menu.

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


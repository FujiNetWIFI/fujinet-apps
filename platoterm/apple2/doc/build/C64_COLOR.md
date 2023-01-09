C64_COLOR
---------

The Commodore 64 version of PLATOTerm has basic color support.

How it works
------------
It works by painting the COLOR RAM underneath the hi-res mode, as text is being plotted. The related code is housed in the following files:

* src/c64/color_transform.s - A fast routine by McMartin of #c64friends to translate an x,y coordinate in bitmap mode to a COLOR RAM offset.
* src/c64/screen.c - Contains in #ifdef'ed blocks, the code to change the color values when text is plotted.

What's supported
----------------

Currently, only foreground color is supported, and the following system colors are parsed:

* Black
* Blue
* Green
* Cyan
* Red
* Magenta (as purple)
* Yellow
* White

Enabling it
-----------

By default, color support is disabled. To enable it, add -D__C64_COLOR__ to your target's CFLAGS.

Making it better
----------------

Lots of things could be done to make it better, within the limits of colorizing 8x8 blocks. ;) If you have code that can improve this to approximate the 24-bit RGB color space to one of these RGB colors, please pull and add. :)


# Reversi

![Reversi](../images/reversi.png)

The original text based version was found as an example in the
z88dk examples folder called othello and has been adapted by
Norman Davie a.k.a TechCowboy.

I realized when creating a reversi program for the Coleco ADAM for the
FujiNet project, the hardware was so similar it was easy to port 
it to the NABU.

With this program, you can now play against people running 
Atari 8-bit computers, Apple, ADAM and soon many more.

Special thanks for Thomas Cherryhomes for providing the 
initial user interace and introducing me to Magellan which 
I used to create the current board screen and sprites.

If you're interested in vintage/retro computing, on various
platforms, you *need* to check out the Fujinet at 
https://fujinet.online 

## NABU Internet Adapter Settings

As per normal practice, you need to tell the IA where to find
the pak/nabu in settings.

![Interface Adapter - Source](../images/source.png)

If you're hosting a game, you need to change the IA Port to
6502 in the RetroNET settings

![Interface Adapter - RetroNet](../images/retronet.png)

Remember to turn on the TCP Server.
If you recompile the source, remember to clear the PAK cache.

## Changes to NABU-LIB.h

I modified the header file so that I can load NABU-LIB.h from
more than one file without errors from multiple definitions of
globals.  If you need to include NABU-LIB.h within more than
one file, then in all but one file create a define called 
PROTOTYPES_ONLY in those files.

## Set up requirements

You need z88dk compiler 
https://github.com/z88dk/z88dk
and gnu make. On windows I used cygwin
https://www.cygwin.com/

On linux, you will need WINE to run the emulator.

The Makefile was designed to allow you to switch between Windows
and Linux seemlessly provided you set up some environment variables. 

Set these environment variables in your .basrc or windows environment
to automagically run your code in the mame emulator
https://gtamp.com/nabu/
<br/>
NABEM_PATH  = NABU emulator executible path<br/>
NABEM_EXEC  = NABU emulator executible name<br/>
NABU_CACHE  = Directory of the local PAK folder<br/>
NABU_STORE  = Directory of the local File store folder<br/>
<br/>
Optionally if you want to copy the files to a samba server, you
can set these environment variables

TNFS_SERVER = IP Address of samba server<br/>
TNFS_SHARE  = The share name<br/>
TNFS_DIR    = the directory within the share<br/>
<br/>
## Homebrew or CP/M

CP/M isn't working YET; seems like the character set is
not being set up or the background and foreground colours
are the same -- but when I figure it out you can
set the project type within the Makefile to either build
a REVERSI.NABU file or a REVERSI.COM file

## Make options

make clean - remove previous compiled objects<br/>
make       - build either the NABU homebrew or NABU CP/M executiable<br/>
make tnfs  - copy the Homebrew or CP/M file to your samba server<br/>
make go    - With homebrew: <br/>
               copies the REVERSI.NABU file to the NABU_CACHE directory
               as 000001.nabu then launches the mame emulator
             With CP/M:<br/>
               copies the REVERSI.COM file to the NABU_STORE directory
               launches the mame emulator. See below for the next step

## Copying file into CP/M

Issue the following command to move the executible to drive C. <br/> 
ALL COMMANDS AND FILENAMES MUST BE IN UPPERCASE<br/>
``
A:RNCMD IA:REVERSI.COM C:REVERSI.COM
C:
REVERSI
``
![CP/M Command](../images/cpm.png)

## Graphics interface

The game board was created using Magellian; a TI-99/4a tile editor.
https://github.com/Rasmus-M/magellan

I wrote mag2c.py in Python 3 with takes the output of Magellian and replaces the
C source code for board.c, charset.c and spriteset.c -- a huge timesaver!

## Resources

NABU RETRONET  https://nabu.ca <br/>
NABU-LIB       [NABU-LIB](https://github.com/DJSures/NABU-LIB)<br/>
FUJINET        [Fujinet Github](https://github.com/FujiNetWIFI)<br/>
MAME Emualtor  https://gtamp.com/nabu/<br/>
Magellian      https://github.com/Rasmus-M/magellan





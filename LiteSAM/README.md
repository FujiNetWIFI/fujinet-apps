LiteSAM
=======

Contributed by Sijmen Schouten (http://www.mr-atari.com/)

## What is it?

This is a handler that can be loaded at boot in any Atari DOS, to redirect the screen editor (E:) device output to also go to #FujiNet's built-in S.A.M. speech synthesizer, essentially giving you a screen reader!

## How to use

Depending on DOS, copy the LiteSAM.DRV file onto a bootable DOS disk, renaming as needed.

* Atari DOS 2 and OS/A+/DOS XL use the file name AUTORUN.SYS
* MyDOS 4.5 can have any filename of AUTORUN.AU0 to AUTORUN.AU9
* LiteDOS simply needs the .DRV extension named to .AU1
* SpartaDOS just needs the file executed, e.g. in your STARTUP.EXC or AUTOEXEC.BAT file

Once the handler is loaded, all screen output is parsed to white space boundaries, and as each white space is encountered, it is sent to S.A.M., with a pause in screen output as S.A.M. utters the word.

Thanks, Sijmen!
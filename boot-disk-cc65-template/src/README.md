# src

* atari-boot-load.cfg - This is the linker configuration file which has been modified from atari.cfg, to remove the standard binary load segments, as well as the system check segment and replace it with an ATR header, and a boot load header. The resulting MAIN segment is exactly 32,640 bytes long, corresponding to the maximum 255 sectors allowed by a boot load program. The starting address is $0706
* atrheader.s - corresponds to a 16-byte ATR header describing a 90K single sided, single density (SS/SD) disk.
* bootheader.s - A 6 byte boot loader header as described by the Atari Operating System Manual. The Second byte in this header is derived from __NUM_SECTORS__ which is calculated from the total program size (STARTUP + ONCE + CODE + DATA). Only the low byte of this value is used, for a maximum of 255 sectors.
* cio.h - part of example program, ciov()
* cio.s - part of example program, ciov()
* data.s - Data to put in the data segment (just a tiny bit of filler data)
* fill.s - Fill data to put in fill segment (to avoid warning)
* main.c - The demo program.

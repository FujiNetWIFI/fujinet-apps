# Apple IIgs clock adjustment

This repo contains 2 programs:

* **fnclock** is a shell program which displays the FujiNet clock and optionally sets the Apple IIgs internal clock to the FN clock time.
* **adjclock** is a temporary initialization program that sets the Apple IIgs clock at boot time.

## Compiling

To compile, currently requires:

### GoldenGate built and installed on your system

Note: you'll need to buy GoldenGate before being able to build it. See the project page: https://goldengate.gitlab.io/.

A copy of the Byte Works ORCA/C and its libraries is also required. You can get it from https://juiced.gs/store/opus-ii-software/.

To build these programs, the compiler itself needs to be updated to at least 2.2.0. Get it from https://github.com/byteworksinc/ORCA-C/releases/. Note that this GitHub repo contains only the compiler, without the ORCA/Shell, C library and supporting files and utilities. You will still need to acquire a copy of Opus ][ software distribution from Juiced.GS (see above).

#### Pull down GoldenGate code

   * https://gitlab.com/GoldenGate/GoldenGate

#### Build GoldenGate:

Detailed instructions for building the GoldenGate binaries are given in the project README.md. To install GoldenGate and the ORCA/C components, see https://goldengate.gitlab.io/manual/#installation.

Now with these prerequisites out of the way:

To build the ORCA shell EXE:
```
$ cd fujinet-apps/clock/apple2gs
$ make -f Makefile.fnclock
```

Check for any errors. If successful there will be logs that say: 
```
...
cp dist.apple2/bootable.po dist.apple2/fnclock.po
java -jar dist.apple2/ac.jar -p dist.apple2/fnclock.po fnclock exe <fnclock
```

You will find the `FNCLOCK` shell EXE in the fnclock.po image. Just copy it under prefix #17, then add this to 15:SYSCMND:  

`CONFIG       U             configure Fujinet`

Restart the ORCA shell or issue a `commands 15:syscmnd` and you're ready.

To build the TIF (temporary initialization program):
```
cd fujinet-config
$ cd fujinet-apps/clock/apple2gs
$ make -f Makefile.adjclock
```

Check for any errors. If successful there will be logs that say: 
```
...
cp dist.apple2/bootable.po dist.apple2/adjclock.po
java -jar dist.apple2/ac.jar -p dist.apple2/adjclock.po adjclock tsf <adjclock
```

You will find the `ADJCLOCK TIF` in the adjclock.po image. Just copy this file on your GS/OS boot disk under System/System.Setup. Now each time GS/OS boots up, the internal clock will get synchronized with the FujiNet time.


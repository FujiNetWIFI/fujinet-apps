# srq

The following routine can be loaded into BASIC to signal when network traffic is available on an opened #FujiNet network device channel. This prevents the need to constantly poll for network traffic.

## Building SRQ

SRQ can be built using CC65, and using the following command:

```
cl65 -t c64 -C c64-asm.cfg -l srq.lst --mapfile srq.map -osrq srq.s
```

## Detecting SRQ

SRQ can be detected by doing a PEEK to location 828:

```
D=PEEK(828):IF D=120 THEN REM ALREADY LOADED
```

## Loading SRQ

If SRQ isn't loaded, it can be loaded with the following command:

```
LOAD "SRQ",8,1
```

## Activating SRQ

Once SRQ is loaded into memory, it can be activated with the following SYS command:

```
SYS828
```

This attaches the SRQ as an IRQ handler at the KERNAL IRQ vector ($0314).

## Detecting network traffic

Once a network connection is opened, e.g. with any of channels 2-14:

```
10 OPEN 2,16,2,"TCP://192.168.1.2:1234/"
```

Any received traffic will be signaled via wiggling the SRQ line on the IEC port. This causes an IRQ, and the "trip" variable to be set to 16 ($10) (this is the value of the IRQ status register)

```
20 IF NOT TRIP THEN GOTO 10:REM NO TRAFFIC
30 REM WE GOT TRAFFIC
40 INPUT #2,A$: REM GET THE DATA
50 POKE 867,0:REM RESET THE TRIP
60 GOTO 20
```

Closing the network channel, will also stop asserting SRQ.

```
70 CLOSE 2
```

## Deactivating SRQ

If SRQ is no longer needed, it can be detached (with the original vector put in place), by issuing the following SYS command.

```
SYS854
```

## Author

Thomas Cherryhomes <thom dot cherryhomes at gmail dot com>

# adm3a

This is a simple version of netcat with ADM-3A terminal emulation

## usage

adm3a can be run either interactively, on versions of DOS that do not support command line parameters:

```
SELECT ITEM OR [RETURN] FOR MENU
L
BINARY LOAD FILE?
D1:ADM3A.COM
ADM-3A TERMINAL--DEVICESPEC?
N:SSH://RASPBERRYPI:22/

pi@raspberrypi$ 
```

or it can be run via command line:

```
D1:ADM3A N:SSH://RASPBERRYPI:22/

pi@raspberrypi$
```

If no parameters are specified, the program starts interactively.

## ADM-3A compatibility

This program supports most of the ADM-3A terminal codes, omitting ones that do not make sense for modern use, such as answer-back facilities.

This program does emit the N: TERMINAL TYPE command, specifying a terminal type of "adm3a" to protocol adapters which send this information, such as the SSH and TELNET adapters.

## License

This program is licensed under the GPL version 3.0, see the file LICENSE for details.


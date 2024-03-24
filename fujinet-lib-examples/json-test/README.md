# cross platform jsontest

This application uses cc65 and fujinet-network-lib to show common code
using json network library functions to get ISS data.

## building

```shell
# make all platforms exe files
$ make

# make apple2 PO disk
make TARGETS=apple2 clean all dist po

# make atari ATR disk
make TARGETS=atari clean all dist atr
```

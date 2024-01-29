# httpbin

Tests various HTTP protocols, and json parsing using fujinet-network-library.

## Building

Running make will build the application for all targets by default.

### Creating Atari ATR DISK image of application

```shell
# Atari, build and create ATR
$ make TARGETS=atari clean all dist atr
...
creating standard SD/90k image
Added file "dist/atr\httpbin.com"
created image "dist/httpbin.atr"
```

The generated file is at `dist/httpbin.atr`

### Creating APPLE ProDos Bitsy DISK image of application

```shell
make TARGETS=apple2 clean all dist po
```

The generated file is at `dist/httpbin.po`

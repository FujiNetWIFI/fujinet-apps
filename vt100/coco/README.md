# VT100 for the CoCo

A FujiNet VT100 terminal for telnet and SSH hosts.

| Machine  | Screen                                  | Binary        |
|----------|-----------------------------------------|---------------|
| CoCo 3   | 80x24 GIME text, color                  | `VT100C3.BIN` |
| CoCo 1/2 | 42x24 hirestxt on PMODE 4, monochrome   | `VT100C2.BIN` |

The CoCo 1/2 needs 32K. The disk boots `AUTOEXEC.BAS`, which runs `VT100.BIN`, a loader that starts the right binary for the machine. `RUNM"VT100"` does the same by hand. The terminal size is sent to the host on connect.

## Building

```
make
```

This needs `cmoc`, `decb` and `python3`, or prefix the command with `defoogi`. fujinet-lib and hirestxt-mod (the no-VT52 build) are downloaded into `_cache/`. Override them with `FUJINET_LIB=` and `HIRESTXT_LIB=`, which take a version, a directory, or a git URL. The result is `vt100.dsk`.

`make test` adds `TEST.BIN`, a CoCo 3 engine test, to the disk.

## Keys

| Action           | CoCo 3                  | CoCo 1/2                  |
|------------------|-------------------------|---------------------------|
| Help             | F1                      | SHIFT+BREAK               |
| Disconnect       | CTRL+BREAK              | CLEAR+BREAK               |
| Control code     | CTRL+letter             | CLEAR+letter              |
| F1-F10           | CTRL+1..0               | CLEAR+1..0                |
| Missing symbols  | ALT+1..0                | CLEAR+SHIFT+1..0          |
| ESC              | BREAK                   | BREAK                     |
| DEL              | CLEAR                   | SHIFT+LEFT                |
| TAB / Backspace  | CTRL+RIGHT / CTRL+LEFT  | CLEAR+RIGHT / CLEAR+LEFT  |

The missing symbols on 1 to 0 are, in order: [ ] { } | \ _ ~ ` ^

On the CoCo 1/2, SHIFT+UP, DOWN, RIGHT and CLEAR also type _, [, ] and \ respectively.

## Limits

- **Both:** no 132 columns, smooth scrolling, or double-width/height lines.
- **CoCo 3:** no bold or DEC line drawing, and a backtick shows as a degree sign.
- **CoCo 1/2:** no color or blink. Bold, inverse, underline, ISO-8859-1 and DEC line drawing are supported. Output runs at about 330 characters a second.

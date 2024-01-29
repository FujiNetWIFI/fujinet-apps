# gr0

This is not a fujinet application.

This is just an ATARI specific example of doing a "graphics 0" call in asm
by closing and opening the E: device via CIOV on IOCB0.

Equivalent CC65 is to do:

```c
int fd = _graphics(0);
close(fd);
```

which will allocate a free IOCB, convert it to a FD, and then close it.

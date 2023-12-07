# chunked-test

This is a program to help us debug issues in the "chunked" transfer mode in the HTTP adapter.

It makes a call to:

```
https://fujinet.online/chunk.php
```

and proceeds to read it all in, halting on error, while printing error and status (DVSTAT) information.



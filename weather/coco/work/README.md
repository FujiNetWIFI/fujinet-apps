# work

All files in here were greyscale PNGs that were converted to a 2bpp file via:

```sh
convert -depth 2 clear.png clear.gray
xxd -i clear.gray >clear.h
```

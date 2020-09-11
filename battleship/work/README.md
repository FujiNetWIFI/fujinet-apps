# work

This folder contains original files used in production of game, such as the
binary data for the font, which can be loaded into an Atari font editor,
such as Envision.

## font

The bship.fnt is converted into font.h with the following command:

```
xxd -i bship.fnt >../src/font.h
```


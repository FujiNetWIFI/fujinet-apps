################################################################
# COMPILE FLAGS

# reserved memory for graphics
# LDFLAGS += -Wl -D,__RESERVED_MEMORY__=0x2000

CFLAGS = -Os -DBUILD_APPLE2 --include-dir src/common
LDFLAGS += -C src/apple2-hgrtext.cfg -m $(PROGRAM).map --ld-args -D,__HIMEM__=0xBF00 apple2-iobuf-0800.o

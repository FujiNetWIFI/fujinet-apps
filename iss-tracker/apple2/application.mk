################################################################
# COMPILE FLAGS

$(info >>>>Starting application.mk)


CFLAGS  = -Os --static-locals -DBUILD_APPLE2
LDFLAGS = --start-addr 0x4000 --ld-args -D,__HIMEM__=0xBF00 apple2-iobuf-0800.o


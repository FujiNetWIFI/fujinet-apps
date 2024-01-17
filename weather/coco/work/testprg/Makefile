CC=cmoc
CP=cp

all: wgtest.dsk

wgtest.dsk: wgtest.c
	$(CC) -owgtest.bin wgtest.c
	$(RM) wgtest.dsk
	decb dskini wgtest.dsk
	writecocofile wgtest.dsk wgtest.bin
	$(CP) wgtest.dsk ~/Workspace/tnfs

clean:
	$(RM) wgtest.bin wgtest.dsk



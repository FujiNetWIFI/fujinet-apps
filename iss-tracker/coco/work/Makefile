CC=cmoc
CP=cp

test.dsk: test.c
	$(CC) -otest.bin test.c
	$(RM) test.dsk
	decb dskini test.dsk
	writecocofile test.dsk test.bin
	$(CP) test.dsk ~/Workspace/tnfs

clean:
	$(RM) test.bin test.dsk

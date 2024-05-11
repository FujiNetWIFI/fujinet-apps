.phony: all clean

all: 
	gcc -ogencharset gencharset.c
	rm -rf charset
	./gencharset
	xxd -i charset >../../../src/apple2/charset.h

	ex -sc '1i|#ifdef __APPLE2__' -cx "../../../src/apple2/charset.h"
	echo "#endif /* __APPLE2__ */" >>../../../src/apple2/charset.h

.phony: all clean

# The generated charset file is included by the Apple2 source
# at $900 to save upper space

# The following can be pasted below "all:" to generate a c array instead, if desired
#xxd -i charset >../../../src/apple2/charset.h
#ex -sc '1i|#ifdef __APPLE2__' -cx "../../../src/apple2/charset.h"
#echo "#endif /* __APPLE2__ */" >>../../../src/apple2/charset.h


all: 
	gcc -ogencharset gencharset.c
	rm -rf charset
	./gencharset

 
	
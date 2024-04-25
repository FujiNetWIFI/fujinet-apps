.phony: all clean

all: 
	gcc -ogencharset gencharset.c
	rm -rf charset
	./gencharset
	xxd -i charset >../src/charset.h

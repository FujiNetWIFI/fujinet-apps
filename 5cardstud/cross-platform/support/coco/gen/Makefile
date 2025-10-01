.phony: all clean

all: 
	gcc -ogencharset gencharset.c
	rm -rf charset
	./gencharset
	xxd -c 8 -i charset >../../../src/coco/charset.c
# ex -sc '1i|#ifdef _CMOC_VERSION_' -cx "../../../src/coco/charset.c"
	ex -sc '1i|// Auto Generated in support/coco/gen' -cx "../../../src/coco/charset.c"
# echo "#endif /* _CMOC_VERSION_ */" >>../../../src/coco/charset.c

 
	
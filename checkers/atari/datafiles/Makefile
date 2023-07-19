all:	../src/checkers.h

../src/checkers.h: checkers.fnt 
	xxd -i checkers.fnt >../src/charset.h

pfdata:
	gcc -opfdata pfdata.c

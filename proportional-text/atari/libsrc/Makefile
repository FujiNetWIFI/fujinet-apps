CC=cc65 -t atari 
AS=ca65 -t atari 
AR=ar65
LIB=pfont.lib
OBJ=obj
MKDIR=mkdir

.PHONY: clean

all: obj pcursor.o font.o pputc.o pputs.o
	$(AR) r $(LIB) obj/pcursor.o
	$(AR) r $(LIB) obj/font.o
	$(AR) r $(LIB) obj/pputc.o
	$(AR) r $(LIB) obj/pputs.o

obj:
	$(MKDIR) -p obj

pcursor.o:
	$(CC) -oobj/pcursor.s pcursor.c
	$(AS) -oobj/pcursor.o obj/pcursor.s

font.o:
	$(CC) -oobj/font.s font.c
	$(AS) -oobj/font.o obj/font.s

pputc.o:
	$(CC) -oobj/pputc.s pputc.c
	$(AS) -oobj/pputc.o obj/pputc.s

pputs.o:
	$(CC) -oobj/pputs.s pputs.c
	$(AS) -oobj/pputs.o obj/pputs.s

clean:
	$(RM) -rf $(OBJ) $(LIB)

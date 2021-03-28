# Makefile for apod - #FujiNet client
# for viewing Astronomy Picture of the Day (APOD)
# images, as converted by the 'apod' web app.
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2021-03-27 - 2021-03-27

CC65=/usr/bin/cc65
CA65=/usr/bin/ca65
LD65=/usr/bin/ld65
CC65_HOME=/usr/share/cc65/
CC65_INC=/usr/share/cc65/include/
CC65_ASMINC=/usr/share/cc65/asminc/
CC65_LIB=/usr/share/cc65/lib/
CC65_CFG=/usr/share/cc65/cfg/

all:	apod.xex

clean:
	-rm apod.xex
	-rm apod.map
	-rm apod.o
	-rm apod.s
	-rm nsio.o
	-rm nsio.s
	-rm sio.o

apod.xex:	apod.o nsio.o sio.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o apod.xex \
		-t atari \
		-m apod.map \
		apod.o nsio.o sio.o atari.lib

apod.o:	apod.s
	${CA65} -I "${CC65_ASMINC}" -t atari apod.s -o apod.o

apod.s:	apod.c nsio.h
	${CC65} -I "${CC65_INC}" \
		-t atari \
		apod.c \
		-o apod.s

nsio.o:	nsio.s
	${CA65} -I "${CC65_ASMINC}" -t atari nsio.s -o nsio.o

nsio.s:	nsio.c nsio.h sio.h
	${CC65} -I "${CC65_INC}" \
		-t atari \
		nsio.c \
		-o nsio.s

sio.o:	sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari sio.s -o sio.o


# Makefile for apod - #FujiNet client
# for viewing Astronomy Picture of the Day (APOD)
# images, as converted by the 'apod' web app.
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2021-03-27 - 2021-03-30

CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
CC65_HOME=/usr/share/cc65/
CC65_INC=${CC65_HOME}/include/
CC65_ASMINC=${CC65_HOME}/asminc/
CC65_LIB=${CC65_HOME}/lib/
CC65_CFG=${CC65_HOME}/cfg/

all:	apod.xex

run:	apod.xex
	atari800 -nobasic apod.xex

clean:
	-rm apod.xex
	-rm apod.map
	-rm apod.o
	-rm apod.s
	-rm nsio.o
	-rm nsio.s
	-rm sio.o
	-rm dli.o
	-rm segments.o

apod.xex:	apod.o dli.o nsio.o sio.o segments.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o apod.xex \
		-t atari \
		-m apod.map \
		apod.o dli.o nsio.o sio.o segments.o atari.lib

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

dli.o:	dli.s
	${CA65} -I "${CC65_ASMINC}" -t atari dli.s -o dli.o

segments.o:	segments.s
	${CA65} -I "${CC65_ASMINC}" -t atari segments.s -o segments.o


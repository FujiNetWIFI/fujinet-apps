# Makefile for apod - #FujiNet client
# for viewing Astronomy Picture of the Day (APOD)
# images, as converted by the 'apod' web app.
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2021-03-27 - 2021-04-23

CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
CC65_HOME=/usr/share/cc65/
CC65_INC=${CC65_HOME}/include/
CC65_ASMINC=${CC65_HOME}/asminc/
CC65_LIB=${CC65_HOME}/lib/
CC65_CFG=${CC65_HOME}/cfg/
CC65_FLAGS=-Osir --add-source

all:	apod.xex

run:	apod.xex
	atari800 -nobasic apod.xex

clean:
	-rm apod.xex
	-rm apod.map
	# O files from S source
	-rm dli256.o
	-rm dli9.o
	-rm segments.o
	-rm sio.o
	# S assembly from C source, and corresponding O files
	-rm apod.o
	-rm apod.s
	-rm colorbars.o
	-rm colorbars.s
	-rm fetch.o
	-rm fetch.s
	-rm get_time.o
	-rm get_time.s
	-rm interrupt_helpers.o
	-rm interrupt_helpers.s
	-rm myprint.o
	-rm myprint.s
	-rm menu.o
	-rm menu.s
	-rm nsio.o
	-rm nsio.s
	-rm rgb.o
	-rm rgb.s
	-rm screen_helpers.o
	-rm screen_helpers.s
	-rm view.o
	-rm view.s

apod.xex:	apod.o dli256.o dli9.o nsio.o sio.o myprint.o get_time.o interrupt_helpers.o screen_helpers.o colorbars.o menu.o rgb.o view.o fetch.o segments.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o apod.xex \
		-t atari \
		-m apod.map \
		apod.o dli256.o dli9.o nsio.o sio.o myprint.o get_time.o interrupt_helpers.o screen_helpers.o colorbars.o menu.o rgb.o view.o fetch.o segments.o atari.lib

apod.o:	apod.s
	${CA65} -I "${CC65_ASMINC}" -t atari apod.s -o apod.o

apod.s:	apod.c get_time.h screen_helpers.h menu.h rgb.h view.h globals.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		apod.c \
		-o apod.s

myprint.o:	myprint.s
	${CA65} -I "${CC65_ASMINC}" -t atari myprint.s -o myprint.o

myprint.s:	myprint.c myprint.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		myprint.c \
		-o myprint.s

get_time.o:	get_time.s
	${CA65} -I "${CC65_ASMINC}" -t atari get_time.s -o get_time.o

get_time.s:	get_time.c get_time.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		get_time.c \
		-o get_time.s

screen_helpers.o:	screen_helpers.s
	${CA65} -I "${CC65_ASMINC}" -t atari screen_helpers.s -o screen_helpers.o

screen_helpers.s:	screen_helpers.c screen_helpers.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		screen_helpers.c \
		-o screen_helpers.s

interrupt_helpers.o:	interrupt_helpers.s
	${CA65} -I "${CC65_ASMINC}" -t atari interrupt_helpers.s -o interrupt_helpers.o

interrupt_helpers.s:	interrupt_helpers.c interrupt_helpers.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		interrupt_helpers.c \
		-o interrupt_helpers.s

colorbars.o:	colorbars.s
	${CA65} -I "${CC65_ASMINC}" -t atari colorbars.s -o colorbars.o

colorbars.s:	colorbars.c colorbars.h screen_helpers.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		colorbars.c \
		-o colorbars.s

menu.o:	menu.s
	${CA65} -I "${CC65_ASMINC}" -t atari menu.s -o menu.o

menu.s:	menu.c menu.h screen_helpers.h myprint.h version.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		menu.c \
		-o menu.s

rgb.o:	rgb.s
	${CA65} -I "${CC65_ASMINC}" -t atari rgb.s -o rgb.o

rgb.s:	rgb.c rgb.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		rgb.c \
		-o rgb.s

view.o:	view.s
	${CA65} -I "${CC65_ASMINC}" -t atari view.s -o view.o

view.s:	view.c view.h interrupt_helpers.h screen_helpers.h rgb.h dli256.h dli9.h colorbars.h menu.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		view.c \
		-o view.s

fetch.o:	fetch.s
	${CA65} -I "${CC65_ASMINC}" -t atari fetch.s -o fetch.o

fetch.s:	fetch.c fetch.h nsio.h menu.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		fetch.c \
		-o fetch.s

nsio.o:	nsio.s
	${CA65} -I "${CC65_ASMINC}" -t atari nsio.s -o nsio.o

nsio.s:	nsio.c nsio.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		nsio.c \
		-o nsio.s

sio.o:	sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari sio.s -o sio.o

dli9.o:	dli9.s dli9.h
	${CA65} -I "${CC65_ASMINC}" -t atari dli9.s -o dli9.o

dli256.o:	dli256.s dli256.h
	${CA65} -I "${CC65_ASMINC}" -t atari dli256.s -o dli256.o

segments.o:	segments.s
	${CA65} -I "${CC65_ASMINC}" -t atari segments.s -o segments.o


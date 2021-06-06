# Makefile for apod - #FujiNet client
# for viewing Astronomy Picture of the Day (APOD)
# images, as converted by the 'apod' web app.
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2021-03-27 - 2021-06-04

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

all:	apod.xex apodtest.xex

run:	apod.xex
	atari800 -nobasic apod.xex

runtest:	apodtest.xex
	atari800 -nobasic apodtest.xex

clean:
	-rm apod.xex
	-rm apodtest.xex
	-rm apod.map
	-rm apodtest.map
	# O files from S source
	-rm dlists.o
	-rm dli15.o
	-rm dli256.o
	-rm dli9.o
	-rm segments.o
	-rm sio.o
	# S assembly from C source, and corresponding O files
	-rm apod.o
	-rm apod.s
	-rm app_key.o
	-rm app_key.s
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
	-rm menutest.o
	-rm menutest.s
	-rm nsio.o
	-rm nsio.s
	-rm rgb.o
	-rm rgb.s
	-rm screen_helpers.o
	-rm screen_helpers.s
	-rm speak.o
	-rm speak.s
	-rm vblanks.o
	-rm vblanks.s
	-rm view.o
	-rm view.s
	-rm viewtest.o
	-rm viewtest.s

apod.xex:	apod.o app_key.o dlists.o dli15.o dli256.o dli9.o nsio.o sio.o myprint.o get_time.o interrupt_helpers.o screen_helpers.o menu.o rgb.o view.o vblanks.o speak.o fetch.o segments.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o apod.xex \
		-t atari \
		-m apod.map \
		apod.o app_key.o dlists.o dli15.o dli256.o dli9.o nsio.o sio.o myprint.o get_time.o interrupt_helpers.o screen_helpers.o menu.o rgb.o view.o vblanks.o speak.o fetch.o segments.o atari.lib

apod.o:	apod.s
	${CA65} -I "${CC65_ASMINC}" -t atari apod.s -o apod.o

apod.s:	apod.c app_key.h get_time.h globals.h menu.h rgb.h screen_helpers.h view.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		apod.c \
		-o apod.s

apodtest.xex:	apod.o app_key.o dlists.o dli15.o dli256.o dli9.o nsio.o sio.o myprint.o colorbars.o get_time.o interrupt_helpers.o screen_helpers.o menutest.o rgb.o viewtest.o vblanks.o speak.o fetch.o segments.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o apodtest.xex \
		-t atari \
		-m apod.map \
		apod.o app_key.o dlists.o dli15.o dli256.o dli9.o nsio.o sio.o myprint.o colorbars.o get_time.o interrupt_helpers.o screen_helpers.o menutest.o rgb.o viewtest.o vblanks.o speak.o fetch.o segments.o atari.lib

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

app_key.o:	app_key.s
	${CA65} -I "${CC65_ASMINC}" -t atari app_key.s -o app_key.o

app_key.s:	app_key.c app_key.h sio.h rgb.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		app_key.c \
		-o app_key.s

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

colorbars.s:	colorbars.c colorbars.h screen_helpers.h menu.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		colorbars.c \
		-o colorbars.s

menu.o:	menu.s
	${CA65} -I "${CC65_ASMINC}" -t atari menu.s -o menu.o

menu.s:	menu.c menu.h app_key.h get_time.h rgb.h screen_helpers.h myprint.h version.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		menu.c \
		-o menu.s

menutest.o:	menutest.s
	${CA65} -I "${CC65_ASMINC}" -t atari menutest.s -o menutest.o

menutest.s:	menu.c menu.h app_key.h get_time.h rgb.h screen_helpers.h myprint.h version.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-D APODTEST \
		-t atari \
		menu.c \
		-o menutest.s

rgb.o:	rgb.s
	${CA65} -I "${CC65_ASMINC}" -t atari rgb.s -o rgb.o

rgb.s:	rgb.c rgb.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		rgb.c \
		-o rgb.s

dlists.o:	dlists.s
	${CA65} -I "${CC65_ASMINC}" -t atari dlists.s -o dlists.o

dlists.s:	dlists.c dlists.h screen_helpers.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		dlists.c \
		-o dlists.s

vblanks.o:	vblanks.s
	${CA65} -I "${CC65_ASMINC}" -t atari vblanks.s -o vblanks.o

vblanks.s:	vblanks.c vblanks.h dli15.h dli256.h dli9.h rgb.h interrupt_helpers.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		vblanks.c \
		-o vblanks.s

speak.o:	speak.s
	${CA65} -I "${CC65_ASMINC}" -t atari speak.s -o speak.o

speak.s:	speak.c speak.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		speak.c \
		-o speak.s

view.o:	view.s
	${CA65} -I "${CC65_ASMINC}" -t atari view.s -o view.o

view.s:	view.c view.h fetch.h interrupt_helpers.h screen_helpers.h rgb.h dlists.h dli15.h dli9.h dli256.h colorbars.h menu.h vblanks.h speak.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		view.c \
		-o view.s

viewtest.o:	viewtest.s
	${CA65} -I "${CC65_ASMINC}" -t atari viewtest.s -o viewtest.o

viewtest.s:	view.c view.h fetch.h interrupt_helpers.h screen_helpers.h rgb.h dlists.h dli15.h dli9.h dli256.h colorbars.h menu.h vblanks.h speak.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-D APODTEST \
		-t atari \
		view.c \
		-o viewtest.s

fetch.o:	fetch.s
	${CA65} -I "${CC65_ASMINC}" -t atari fetch.s -o fetch.o

fetch.s:	fetch.c fetch.h nsio.h menu.h rgb.h screen_helpers.h
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

dli15.o:	dli15.s dli15.h
	${CA65} -I "${CC65_ASMINC}" -t atari dli15.s -o dli15.o

dli9.o:	dli9.s dli9.h
	${CA65} -I "${CC65_ASMINC}" -t atari dli9.s -o dli9.o

dli256.o:	dli256.s dli256.h
	${CA65} -I "${CC65_ASMINC}" -t atari dli256.s -o dli256.o

segments.o:	segments.s
	${CA65} -I "${CC65_ASMINC}" -t atari segments.s -o segments.o


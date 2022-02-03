# Makefile for cookies - #FujiNet web client demo
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2021-08-05 - 2021-08-05

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

all:	cookies.xex

run:	cookies.xex
	atari800 -nobasic cookies.xex

clean:
	-rm cookies.xex
	-rm cookies.map
	# O files from S source
	-rm sio.o
	# S assembly from C source, and corresponding O files
	-rm cookies.o
	-rm cookies.s
	-rm nsio.o
	-rm nsio.s
	-rm app_key.o
	-rm app_key.s

cookies.xex:	cookies.o nsio.o sio.o app_key.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o cookies.xex \
		-t atari \
		-m cookies.map \
		cookies.o nsio.o sio.o app_key.o atari.lib

cookies.o:	cookies.s
	${CA65} -I "${CC65_ASMINC}" -t atari cookies.s -o cookies.o

cookies.s:	cookies.c nsio.h app_key.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		cookies.c \
		-o cookies.s

nsio.o:	nsio.s
	${CA65} -I "${CC65_ASMINC}" -t atari nsio.s -o nsio.o

nsio.s:	nsio.c nsio.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		nsio.c \
		-o nsio.s

app_key.o:	app_key.s
	${CA65} -I "${CC65_ASMINC}" -t atari app_key.s -o app_key.o

app_key.s:	app_key.c app_key.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		app_key.c \
		-o app_key.s

sio.o:	sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari sio.s -o sio.o


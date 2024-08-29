# Makefile for jsontest for Atari
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2022-05-25 - 2022-06-01

CC65BIN=/usr/local/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
CC65_HOME=/usr/local/share/cc65/
CC65_INC=${CC65_HOME}/include/
CC65_ASMINC=${CC65_HOME}/asminc/
CC65_LIB=${CC65_HOME}/lib/
CC65_CFG=${CC65_HOME}/cfg/
CC65_FLAGS=-Osir --add-source

all:	randomtest.xex

clean:
	-rm randomtest.xex
	-rm randomtest.map
	# O files from S source
	-rm sio.o
	# S assembly from C source, and corresponding O files
	-rm randomtest.o
	-rm randomtest.s


randomtest.xex:	randomtest.o sio.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o randomtest.xex \
		-t atari \
		-m randomtest.map \
		randomtest.o sio.o atari.lib

sio.o:	sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari sio.s -o sio.o

randomtest.o:	randomtest.s
	${CA65} -I "${CC65_ASMINC}" -t atari randomtest.s -o randomtest.o

randomtest.s:	randomtest.c sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		randomtest.c \
		-o randomtest.s




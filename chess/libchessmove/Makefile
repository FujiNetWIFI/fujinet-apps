# Makefile for `libchessmove`
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2023-07-14 - 2023-07-14

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

all:	chsstest.xex

clean:
	-rm chsstest.xex
	-rm chsstest.map
	# S assembly from C source, and corresponding O files
	-rm chsstest.o
	-rm chsstest.s
	-rm libchessmove.o
	-rm libchessmove.s

chsstest.xex:	chsstest.o libchessmove.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o chsstest.xex \
		-t atari \
		-m chsstest.map \
		chsstest.o libchessmove.o atari.lib

chsstest.o:	chsstest.s
	${CA65} -I "${CC65_ASMINC}" -t atari chsstest.s -o chsstest.o

chsstest.s:	chsstest.c libchessmove.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		chsstest.c \
		-o chsstest.s

libchessmove.o:	libchessmove.s
	${CA65} -I "${CC65_ASMINC}" -t atari libchessmove.s -o libchessmove.o

libchessmove.s:	libchessmove.c libchessmove.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		libchessmove.c \
		-o libchessmove.s

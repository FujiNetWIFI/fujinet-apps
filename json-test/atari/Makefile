# Makefile for jsontest for Atari
#
# Bill Kendrick <bill@newbreedsoftware.com>
# 2022-05-25 - 2024-10-29

CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
CC65_HOME=/usr/local/share/cc65/
CC65_INC=${CC65_HOME}/include/
CC65_ASMINC=${CC65_HOME}/asminc/
CC65_LIB=${CC65_HOME}/lib/
CC65_CFG=${CC65_HOME}/cfg/
CC65_FLAGS=-Osir --add-source

all:	jsontest.xex uptodate.xex

clean:
	-rm jsontest.xex
	-rm jsontest.map
	-rm uptodate.xex
	-rm uptodate.map
	# O files from S source
	-rm sio.o
	# S assembly from C source, and corresponding O files
	-rm jsontest.o
	-rm jsontest.s
	-rm uptodate.o
	-rm uptodate.s
	-rm nsio.o
	-rm nsio.s

jsontest.xex:	jsontest.o nsio.o sio.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o jsontest.xex \
		-t atari \
		-m jsontest.map \
		jsontest.o nsio.o sio.o atari.lib

jsontest.o:	jsontest.s
	${CA65} -I "${CC65_ASMINC}" -t atari jsontest.s -o jsontest.o

jsontest.s:	jsontest.c nsio.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		jsontest.c \
		-o jsontest.s

uptodate.xex:	uptodate.o nsio.o sio.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o uptodate.xex \
		-t atari \
		-m uptodate.map \
		uptodate.o nsio.o sio.o atari.lib

uptodate.o:	uptodate.s
	${CA65} -I "${CC65_ASMINC}" -t atari uptodate.s -o uptodate.o

uptodate.s:	uptodate.c nsio.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		uptodate.c \
		-o uptodate.s

nsio.o:	nsio.s
	${CA65} -I "${CC65_ASMINC}" -t atari nsio.s -o nsio.o

nsio.s:	nsio.c nsio.h sio.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		nsio.c \
		-o nsio.s

sio.o:	sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari sio.s -o sio.o


# Makefile for iss-tracker - #FujiNet client
# for tracking the International Space Station (ISS)
# and seeing who is in space right now.
#
# Pulls data from "Open Notify" http://open-notify.org/
# (an open source project to provide a simple programming interface for
# some of NASA's awesome data)
#
# Bill Kendrick <bill@newbreedsoftware.com>
#
# 2021-05-17 - 2024-04-07

CURRENT_PLATFORM = atari
FUJINET_LIB_VERSION := 3.0.3
FUJINET_LIB := ./
FUJINET_LIB_VERSION_DIR = $(FUJINET_LIB)/$(FUJINET_LIB_VERSION)-$(CURRENT_PLATFORM)
FUJINET_LIB_PATH = $(FUJINET_LIB_VERSION_DIR)/fujinet-$(CURRENT_PLATFORM)-$(FUJINET_LIB_VERSION).lib
FUJINET_LIB_DOWNLOAD_URL = https://github.com/FujiNetWIFI/fujinet-lib/releases/download/v$(FUJINET_LIB_VERSION)/fujinet-lib-$(CURRENT_PLATFORM)-$(FUJINET_LIB_VERSION).zip
FUJINET_LIB_DOWNLOAD_FILE = $(FUJINET_LIB)/fujinet-lib-$(CURRENT_PLATFORM)-$(FUJINET_LIB_VERSION).zip

#CC65BIN=/usr/local/bin
#CC65=${CC65BIN}/cc65
#CA65=${CC65BIN}/ca65
#LD65=${CC65BIN}/ld65
#CC65_HOME=/usr/share/cc65
#CC65_INC=${CC65_HOME}/include/
#CC65_ASMINC=${CC65_HOME}/asminc/
#CC65_LIB=${CC65_HOME}/lib/
#CC65_CFG=${CC65_HOME}/cfg/
CC65_FLAGS=-Osir --add-source

CC65 = cc65
CA65 = ca65
LD65 = cl65
# FIXME: Use this in "iss.c" as well
VERSION=2024-04-07

all:	get_fujinet_lib iss.xex

lib: 	get_fujinet_lib

run:	iss.xex
	atari800 -nobasic iss.xex

clean:
	-rm iss.xex
	-rm iss.map
	# O files from S source
	-rm segments.o
	-rm sio.o
	# S assembly from C source, and corresponding O files
	-rm iss.o
	-rm iss.s
	-rm app_key.o
	-rm app_key.s
	-rm colors.o
	-rm colors.s
	# Fujinet-lib things we downloaded
	-rm Changelog.md
	-rm fujinet-atari-?.?.?.lib
	-rm fujinet-fuji.h
	-rm fujinet-fuji.inc
	-rm fujinet-lib-atari-?.?.?.zip
	-rm fujinet-network.h
	-rm fujinet-network.inc


release:	clean releaseclean
	mkdir release
	mkdir release/iss-tracker-atari-${VERSION}-src
	mkdir release/iss-tracker-atari-${VERSION}-src/img
	cp *.* release/iss-tracker-atari-${VERSION}-src/
	cp img/* release/iss-tracker-atari-${VERSION}-src/img/
	tar -czf release/iss-tracker-atari-${VERSION}-src.tar.gz release/iss-tracker-atari-${VERSION}-src/

releaseclean:
	-rm -rf release

iss.xex:	iss.o sio.o app_key.o colors.o segments.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}:."  \
		-o iss.xex \
		-t atari \
		-m iss.map \
		iss.o sio.o app_key.o colors.o segments.o atari.lib  fujinet-atari-3.0.3.lib

iss.o:	iss.s
	${CA65} -I "${CC65_ASMINC}" -t atari iss.s -o iss.o

iss.s:	iss.c app_key.h colors.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		iss.c \
		-o iss.s


app_key.o:	app_key.s
	${CA65} -I "${CC65_ASMINC}" -t atari app_key.s -o app_key.o

app_key.s:	app_key.c app_key.h colors.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		app_key.c \
		-o app_key.s

colors.o:	colors.s
	${CA65} -I "${CC65_ASMINC}" -t atari colors.s -o colors.o

colors.s:	colors.c colors.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		colors.c \
		-o colors.s

sio.o:	sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari sio.s -o sio.o

segments.o:     segments.s img/map.gr7
	${CA65} -I "${CC65_ASMINC}" -t atari segments.s -o segments.o

get_fujinet_lib:
	@if [ ! -f "$(FUJINET_LIB_DOWNLOAD_FILE)" ]; then \
		if [ -d "$(FUJINET_LIB_VERSION_DIR)" ]; then \
		  echo "A directory already exists with version $(FUJINET_LIB_VERSION) - please remove it first"; \
			exit 1; \
		fi; \
		HTTPSTATUS=$$(curl -Is $(FUJINET_LIB_DOWNLOAD_URL) | head -n 1 | awk '{print $$2}'); \
		if [ "$${HTTPSTATUS}" == "404" ]; then \
			echo "ERROR: Unable to find file $(FUJINET_LIB_DOWNLOAD_URL)"; \
			exit 1; \
		fi; \
		echo "Downloading fujinet-lib for $(TARGETLIST) version $(FUJINET_LIB_VERSION) from $(FUJINET_LIB_DOWNLOAD_URL)"; \
		mkdir -p $(FUJINET_LIB); \
		curl -L $(FUJINET_LIB_DOWNLOAD_URL) -o $(FUJINET_LIB_DOWNLOAD_FILE); \
		echo "Unzipping to $(FUJINET_LIB)"; \
		unzip -o $(FUJINET_LIB_DOWNLOAD_FILE); \
		echo "Unzip complete."; \
	fi

CMOC ?= cmoc
DECB ?= decb
NAME   = vt100
VPATH  = src
PLATFORM = coco

# CoCo 3: 80-col GIME text. CoCo 1/2: hirestxt 42x24 on a PMODE 4 screen at
# $0E00-$25FF, so the program starts just above it.
C3_ORG   = 4000
C2_ORG   = 2600
C2_LIMIT = 7B00

# Version shown on the F1 help screen. Scoped to vt100* tags so the monorepo's
# other app tags don't leak in; falls back to the short commit hash.
VERSION := $(shell git describe --tags --always --dirty --match 'vt100*' 2>/dev/null || echo dev)

# fnlib.py resolves where fujinet-lib comes from. FUJINET_LIB can be:
#   empty                          - latest GitHub release (downloads to _cache/)
#   a version number (e.g. 4.10.0) - that specific release
#   a directory path               - use lib + headers from there
#   a zip file path                - extract from a local zip
#   a git URL                      - clone + build from source (use #branch suffix
#                                    to pick a branch, e.g. URL#feature/foo)
FUJINET_LIB ?=

# hirestxtlib.py takes the same forms. Empty = latest release, no-VT52 build.
HIRESTXT_LIB ?=

SRCS = main.c screen.c vt100.c
C3_OBJS = $(SRCS:%.c=build/c3/%.o)
C2_OBJS = $(SRCS:%.c=build/c2/%.o)
C3_BIN  = build/c3/$(NAME)c3.bin
C2_BIN  = build/c2/$(NAME)c2.bin
LOADER  = build/loader.bin

TEST      = build/c3/test_engine.bin
TEST_OBJS = build/c3/test_engine.o build/c3/screen.o build/c3/vt100.o

all: $(NAME).dsk

.PHONY: all test clean distclean

# Run a resolver script and turn its stdout (one VAR:=VAL per line) into make
# assignments. The tr/| substitution dance is needed because $(shell) collapses
# newlines.
define _newline


endef
$(eval $(subst |,$(_newline),$(shell PLATFORM=$(PLATFORM) ./fnlib.py $(FUJINET_LIB) | tr '\n' '|')))
ifeq ($(strip $(FUJINET_LIB_LDLIB)),)
  $(error fujinet-lib not available - check FUJINET_LIB or network access)
endif
$(eval $(subst |,$(_newline),$(shell PLATFORM=$(PLATFORM) ./hirestxtlib.py $(HIRESTXT_LIB) | tr '\n' '|')))
ifeq ($(strip $(HIRESTXT_LIB_LDLIB)),)
  $(error hirestxt-mod not available - check HIRESTXT_LIB or network access)
endif
ifeq ($(HIRESTXT_LIB_NO_VT52),1)
  C2_DEFS = -DHIRESTEXT_NO_VT52
endif

CFLAGS = --coco -Isrc -I$(FUJINET_LIB_INCLUDE) -DVT100_VERSION='"$(VERSION)"'

build/c3/%.o: %.c | build/c3
	$(CMOC) $(CFLAGS) -DCOCO3 -c -o $@ $<

build/c2/%.o: %.c | build/c2
	$(CMOC) $(CFLAGS) -I$(HIRESTXT_LIB_INCLUDE) $(C2_DEFS) -c -o $@ $<

build/c3 build/c2:
	mkdir -p $@

$(C3_BIN): $(C3_OBJS)
	$(CMOC) --coco --org=$(C3_ORG) -o $@ $(C3_OBJS) -L$(FUJINET_LIB_DIR) -l$(FUJINET_LIB_LDLIB)

$(C2_BIN): $(C2_OBJS)
	$(CMOC) --coco --org=$(C2_ORG) --limit=$(C2_LIMIT) -o $@ $(C2_OBJS) \
	  -L$(FUJINET_LIB_DIR) -l$(FUJINET_LIB_LDLIB) -L$(HIRESTXT_LIB_DIR) -l$(HIRESTXT_LIB_LDLIB)

$(LOADER): loader.c | build/c3
	$(CMOC) --coco -o $@ $<

$(TEST): $(TEST_OBJS)
	$(CMOC) --coco --org=$(C3_ORG) -o $@ $(TEST_OBJS)

# AUTOEXEC runs VT100.BIN, the loader, which picks the binary for the machine.
$(NAME).dsk: $(LOADER) $(C2_BIN) $(C3_BIN)
	$(DECB) dskini $@
	echo 'RUNM"VT100"' > build/autoexec.bas
	$(DECB) copy -t -0 build/autoexec.bas $@,AUTOEXEC.BAS
	$(DECB) copy -2 -b $(LOADER) $@,VT100.BIN
	$(DECB) copy -2 -b $(C2_BIN) $@,VT100C2.BIN
	$(DECB) copy -2 -b $(C3_BIN) $@,VT100C3.BIN

# Adds TEST.BIN (CoCo 3 engine test) to the disk.
test: $(NAME).dsk $(TEST)
	$(DECB) copy -2 -b $(TEST) $(NAME).dsk,TEST.BIN

clean:
	rm -rf build $(NAME).dsk
	rm -f *.link *.map *.s *.asm

# Also wipes the fetch cache so the next build re-fetches.
distclean: clean
	rm -rf _cache

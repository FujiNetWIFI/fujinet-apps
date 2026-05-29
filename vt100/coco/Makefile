CMOC ?= cmoc
DECB ?= decb
ORG    = 4000
NAME   = vt100
VPATH  = src
PLATFORM = coco

# fnlib.py resolves where fujinet-lib comes from. FUJINET_LIB can be:
#   empty                          - latest GitHub release (downloads to _cache/)
#   a version number (e.g. 4.10.0) - that specific release
#   a directory path               - use lib + headers from there
#   a zip file path                - extract from a local zip
#   a git URL                      - clone + build from source (use #branch suffix
#                                    to pick a branch, e.g. URL#feature/foo)
FUJINET_LIB ?=

OBJS   = main.o screen.o vt100.o
TEST   = test_engine
TEST_OBJS = test_engine.o screen.o vt100.o

all: $(NAME).dsk

.PHONY: all test clean distclean

# Run fnlib.py and turn its stdout (one VAR:=VAL per line) into make
# assignments. The tr/| substitution dance is needed because $(shell) collapses
# newlines.
define _newline


endef
$(eval $(subst |,$(_newline),$(shell PLATFORM=$(PLATFORM) ./fnlib.py $(FUJINET_LIB) | tr '\n' '|')))
ifeq ($(strip $(FUJINET_LIB_LDLIB)),)
  $(error fujinet-lib not available - check FUJINET_LIB or network access)
endif

%.o: %.c
	$(CMOC) --coco -c -Isrc -I$(FUJINET_LIB_INCLUDE) -o $@ $<

$(NAME).bin: $(OBJS)
	$(CMOC) --coco --org=$(ORG) -o $@ $(OBJS) -L$(FUJINET_LIB_DIR) -l$(FUJINET_LIB_LDLIB)

$(TEST).bin: $(TEST_OBJS)
	$(CMOC) --coco --org=$(ORG) -o $@ $(TEST_OBJS)

$(NAME).dsk: $(NAME).bin
	$(DECB) dskini $@
	$(DECB) copy -2 -b $(NAME).bin $@,VT100.BIN

# Rebuilds the disk with TEST.BIN added alongside VT100.BIN.
test: $(NAME).bin $(TEST).bin
	$(DECB) dskini $(NAME).dsk
	$(DECB) copy -2 -b $(NAME).bin $(NAME).dsk,VT100.BIN
	$(DECB) copy -2 -b $(TEST).bin $(NAME).dsk,TEST.BIN

clean:
	rm -f $(OBJS) $(NAME).bin $(NAME).dsk
	rm -f $(TEST_OBJS) $(TEST).bin
	rm -f *.link *.map *.s *.asm

# Also wipes the fnlib cache so the next build re-fetches.
distclean: clean
	rm -rf _cache

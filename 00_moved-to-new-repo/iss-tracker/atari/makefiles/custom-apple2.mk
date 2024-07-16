# COMPILE FLAGS

# reserved memory for graphics
# LDFLAGS += -Wl -D,__RESERVED_MEMORY__=0x2000

#LDFLAGS += --start-addr 0x4400
#LDFLAGS += -C cfg/atari.cfg

################################################################
# DISK creation

SUFFIX =
DISK_TASKS += .po
AUTOBOOT := -l
APPLE_TOOLS_DIR := ../apple-tools

PROG_XTRA :=
ifeq ($(CURRENT_TARGET),apple2enh)
    PROG_XTRA = -enh
endif

.po:
	$(call RMFILES,$(DIST_DIR)/$(PROGRAM)$(PROG_XTRA).po)
	$(APPLE_TOOLS_DIR)/mk-bitsy.sh $(DIST_DIR)/$(PROGRAM)$(PROG_XTRA).po $(PROGRAM_TGT)$(SUFFIX)
	$(APPLE_TOOLS_DIR)/add-file.sh $(AUTOBOOT) $(DIST_DIR)/$(PROGRAM)$(PROG_XTRA).po $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) $(PROGRAM)

# Applewin debug script
.gendebug: $(PROGRAM_TGT)
	@if [ -f "build/$(PROGRAM_TGT).lbl" ]; then \
		echo "Generating debug.scr script for AppleWin"; \
		echo 'echo "Loading symbols"' > build/debug.scr; \
		awk '{printf("sym %s = %s\n", substr($$3, 2), $$2)}' < build/$(PROGRAM_TGT).lbl >> build/debug.scr; \
		echo 'bpx _main' >> build/debug.scr; \
		echo 'bpx _debug' >> build/debug.scr; \
		echo 'bpx _network_open' >> build/debug.scr; \
		echo 'bpx _sp_init' >> build/debug.scr; \
	fi

ALL_TASKS += .gendebug

################################################################
# TESTING / EMULATOR

# COMPILE FLAGS

LDFLAGS += -C apple2enh-system.cfg

################################################################
# DISK creation

# -l: autostart via SYSTEM loader
# -a: autostart via STARTUP directly
AUTOBOOT := -l

SUFFIX =
DISK_TASKS += .po
APPLE_TOOLS_DIR := ../../apple-tools

.po:
	$(call RMFILES,$(DIST_DIR)/$(APP_NAME)*.po)
	cp $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) $(DIST_DIR)/$(PROGRAM)$(SUFFIX)
	$(APPLE_TOOLS_DIR)/mk-bitsy.sh $(DIST_DIR)/$(PROGRAM).po $(PROGRAM)$(SUFFIX)
	$(APPLE_TOOLS_DIR)/add-file.sh $(AUTOBOOT) $(DIST_DIR)/$(PROGRAM).po $(DIST_DIR)/$(PROGRAM)$(SUFFIX) $(PROGRAM)

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

################################################################
# COMPILE FLAGS

LDFLAGS += -C src/atari/cater.cfg

################################################################
# DISK creation

SUFFIX = .com
DISK_TASKS += .atr

.atr:
	$(call MKDIR,$(DIST_DIR)/atr)
	cp $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) $(DIST_DIR)/atr/$(PROGRAM)$(SUFFIX)
	$(call RMFILES,$(DIST_DIR)/*.atr)
	dir2atr -S $(DIST_DIR)/$(PROGRAM).atr $(DIST_DIR)/atr

################################################################
# TESTING / EMULATOR

# Specify ATARI_EMULATOR=[ALTIRRA|ATARI800] to set which one to run, default is ALTIRRA

ALTIRRA ?= $(ALTIRRA_HOME)/Altirra64.exe \
  $(XS)/portable $(XS)/portablealt:altirra-debug.ini \

# Additional args that can be copied into the above lines
#   $(XS)/debug \
#   $(XS)/debugcmd: ".loadsym build\$(PROGRAM).$(CURRENT_TARGET).lbl" \
#   $(XS)/debugcmd: "bp _debug" \

ATARI800 ?= $(ATARI800_HOME)/atari800 \
  -xl -nobasic -ntsc -xl-rev custom -config atari800-debug.cfg -run

# split these out by all targets, this is needed so that in os.mk, it will get the right value as other platforms do run different emulators for targets
# e.g. "vice" uses "x64sc", "xpet", ... for the different machines, unlike atari that uses Altirra for all different "models" (XL/XE etc).
atari_EMUCMD := $($(ATARI_EMULATOR))
atarixl_EMUCMD := $($(ATARI_EMULATOR))

ifeq ($(ATARI_EMULATOR),)
atari_EMUCMD := $(ALTIRRA)
atarixl_EMUCMD := $(ALTIRRA)
endif


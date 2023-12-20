# Adapted from the generic cc65 makefile.
# Notible exceptions:
# - recursive dirs for src
# - final files go into build/ directory instead of polluting root folder (e.g. lbl, com file etc)

# Space or comma separated list of cc65 supported target platforms to build for.
TARGETS := atari apple2

# Name of the final, single-file library.
PROGRAM := httpbin

APP_NAME := $(PROGRAM)

APPLE_TOOLS_DIR = ../apple-tools
FUJINET_NETWORK_LIBS = ../fn_network

# Build dir for putting final built program rather than cluttering root
BUILD_DIR = build

PREEMUCMD :=
POSTEMUCMD :=
VICE_HOME :=

# Options state file name. You should not need to change this, but for those
# rare cases when you feel you really need to name it differently - here you are
STATEFILE := Makefile.options

###################################################################################
####  DO NOT EDIT BELOW THIS LINE, UNLESS YOU REALLY KNOW WHAT YOU ARE DOING!  ####
###################################################################################

###################################################################################
### Mapping abstract options to the actual compiler, assembler and linker flags ###
### Predefined compiler, assembler and linker flags, used with abstract options ###
### valid for 2.14.x. Consult the documentation of your cc65 version before use ###
###################################################################################

# Compiler flags used to tell the compiler to optimise for SPEED
define _optspeed_
  CFLAGS += -Oris
endef

# Compiler flags used to tell the compiler to optimise for SIZE
define _optsize_
  CFLAGS += -Or
endef

# Compiler and assembler flags for generating listings
define _listing_
  CFLAGS += --listing $$(@:.o=.lst)
  ASFLAGS += --listing $$(@:.o=.lst)
  REMOVES += $(addsuffix .lst,$(basename $(OBJECTS)))
endef

# Linker flags for generating map file
define _mapfile_
  LDFLAGS += --mapfile $$@.map
  REMOVES += $(PROGRAM).map
endef

# Linker flags for generating VICE label file
define _labelfile_
  LDFLAGS += -Ln $$@.lbl
  REMOVES += $(PROGRAM).lbl
endef

# Linker flags for generating a debug file
define _debugfile_
  LDFLAGS += -Wl --dbgfile,$$@.dbg
  REMOVES += $(PROGRAM).dbg
endef

###############################################################################
###  Defaults to be used if nothing defined in the editable sections above  ###
###############################################################################

# Presume the C64 target like the cl65 compile & link utility does.
# Set TARGETS to override.
ifeq ($(TARGETS),)
  TARGETS := c64
endif

# Presume we're in a project directory so name the program like the current
# directory. Set PROGRAM to override.
ifeq ($(PROGRAM),)
  PROGRAM := $(notdir $(CURDIR))
endif

# Presume the C and asm source files to be located in the subdirectory 'src'.
# Set SRCDIR to override.
ifeq ($(SRCDIR),)
  SRCDIR := src
endif

# Presume the object and dependency files to be located in the subdirectory
# 'obj' (which will be created). Set OBJDIR to override.
ifeq ($(OBJDIR),)
  OBJDIR := obj
endif

TARGETOBJDIR := $(OBJDIR)/$(TARGETS)

# On Windows it is mandatory to have CC65_HOME set. So do not unnecessarily
# rely on cl65 being added to the PATH in this scenario.
ifdef CC65_HOME
  CC := $(CC65_HOME)/bin/cl65
else
  CC := cl65
endif

# Default emulator commands and options for particular targets.
# Set EMUCMD to override.
c64_EMUCMD := $(VICE_HOME)x64 -kernal kernal -VICIIdsize -autoload
c128_EMUCMD := $(VICE_HOME)x128 -kernal kernal -VICIIdsize -autoload
vic20_EMUCMD := $(VICE_HOME)xvic -kernal kernal -VICdsize -autoload
pet_EMUCMD := $(VICE_HOME)xpet -Crtcdsize -autoload
plus4_EMUCMD := $(VICE_HOME)xplus4 -TEDdsize -autoload
# So far there is no x16 emulator in VICE (why??) so we have to use xplus4 with -memsize option
c16_EMUCMD := $(VICE_HOME)xplus4 -ramsize 16 -TEDdsize -autoload
cbm510_EMUCMD := $(VICE_HOME)xcbm2 -model 510 -VICIIdsize -autoload
cbm610_EMUCMD := $(VICE_HOME)xcbm2 -model 610 -Crtcdsize -autoload
#atari_EMUCMD := atari800 -windowed -xl -pal -nopatchall -run

ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

XS := ""
ifeq ($(detected_OS),$(filter $(detected_OS),MSYS MINGW))
# need an eXtra Slash for altirra things
	XS := /
endif

LBL_SYM := $(XS)/debugcmd: ".loadsym build\$(PROGRAM).$(TARGETS).lbl"

ALTIRRA ?= $(ALTIRRA_HOME)/Altirra64.exe \
  $(XS)/portable $(XS)/portablealt:altirra-debug.ini \
  $(XS)/debug \
  $(LBL_SYM) \
  $(XS)/debugcmd: "bp _debug" \

atari_EMUCMD := $(ALTIRRA)

ifeq ($(EMUCMD),)
  EMUCMD = $($(CC65TARGET)_EMUCMD)
endif

###############################################################################
### The magic begins                                                        ###
###############################################################################

# The "Native Win32" GNU Make contains quite some workarounds to get along with
# cmd.exe as shell. However it does not provide means to determine that it does
# actually activate those workarounds. Especially does $(SHELL) NOT contain the
# value 'cmd.exe'. So the usual way to determine if cmd.exe is being used is to
# execute the command 'echo' without any parameters. Only cmd.exe will return a
# non-empy string - saying 'ECHO is on/off'.
#
# Many "Native Win32" prorams accept '/' as directory delimiter just fine. How-
# ever the internal commands of cmd.exe generally require '\' to be used.
#
# cmd.exe has an internal command 'mkdir' that doesn't understand nor require a
# '-p' to create parent directories as needed.
#
# cmd.exe has an internal command 'del' that reports a syntax error if executed
# without any file so make sure to call it only if there's an actual argument.
ifeq ($(shell echo),)
  MKDIR = mkdir -p $1
  RMDIR = rmdir $1
  RMFILES = $(RM) $1
else
  MKDIR = mkdir $(subst /,\,$1)
  RMDIR = rmdir $(subst /,\,$1)
  RMFILES = $(if $1,del /f $(subst /,\,$1))
endif
COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

TARGETLIST := $(subst $(COMMA),$(SPACE),$(TARGETS))

ifeq ($(words $(TARGETLIST)),1)

# Strip potential variant suffix from the actual cc65 target.
CC65TARGET := $(firstword $(subst .,$(SPACE),$(TARGETLIST)))

# Set PROGRAM to something like 'myprog.c64'.
override PROGRAM := $(PROGRAM).$(TARGETLIST)

# Set SOURCES to something like 'src/foo.c src/bar.s'.
# Use of assembler files with names ending differently than .s is deprecated!

SOURCES := $(call rwildcard,$(SRCDIR)/,*.s)
SOURCES += $(call rwildcard,$(SRCDIR)/,*.c)

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))

OBJ1 := $(SOURCES:.c=.o)
OBJECTS := $(OBJ1:.s=.o)
# change from src/ -> obj/<target>/
OBJECTS := $(OBJECTS:$(SRCDIR)/%=$(OBJDIR)/$(TARGETLIST)/%)

# Set DEPENDS to something like 'obj/c64/foo.d obj/c64/bar.d'.
DEPENDS := $(OBJECTS:.o=.d)

# Add to LIBS something like 'src/foo.lib src/c64/bar.lib'.
LIBS += $(wildcard ../fn_network/fujinet-network-$(TARGETLIST)_*.lib)

# add common/inc, <target>/src/inc, and the root directory (for fujinet-network.[h|inc])
ASFLAGS += --asm-include-dir $(SRCDIR)/include --asm-include-dir $(FUJINET_NETWORK_LIBS)
CFLAGS += --include-dir $(SRCDIR)/include --include-dir $(FUJINET_NETWORK_LIBS)

# Add -DBUILD_(TARGET) to all args for the current name.
UPPER_TARGETLIST := $(shell echo $(TARGETLIST) | tr a-z A-Z)
CFLAGS += -DBUILD_$(UPPER_TARGETLIST)
ASFLAGS += -DBUILD_$(UPPER_TARGETLIST)
LDFLAGS += -DBUILD_$(UPPER_TARGETLIST)

LDFLAGS += --start-addr 0x3000

.SUFFIXES:
.PHONY: all clean dist $(PROGRAM).$(TARGETLIST)

all: $(PROGRAM)

-include $(DEPENDS)
-include $(STATEFILE)

# If OPTIONS are given on the command line then save them to STATEFILE
# if (and only if) they have actually changed. But if OPTIONS are not
# given on the command line then load them from STATEFILE. Have object
# files depend on STATEFILE only if it actually exists.
ifeq ($(origin OPTIONS),command line)
  ifneq ($(OPTIONS),$(_OPTIONS_))
    ifeq ($(OPTIONS),)
      $(info Removing OPTIONS)
      $(shell $(RM) $(STATEFILE))
      $(eval $(STATEFILE):)
    else
      $(info Saving OPTIONS=$(OPTIONS))
      $(shell echo _OPTIONS_=$(OPTIONS) > $(STATEFILE))
    endif
    $(eval $(OBJECTS): $(STATEFILE))
  endif
else
  ifeq ($(origin _OPTIONS_),file)
    $(info Using saved OPTIONS=$(_OPTIONS_))
    OPTIONS = $(_OPTIONS_)
    $(eval $(OBJECTS): $(STATEFILE))
  endif
endif

# Transform the abstract OPTIONS to the actual cc65 options.
$(foreach o,$(subst $(COMMA),$(SPACE),$(OPTIONS)),$(eval $(_$o_)))

$(OBJDIR):
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(BUILD_DIR):
	$(call MKDIR,$@)

SRC_INC_DIRS := \
	$(sort $(dir $(wildcard $(SRCDIR)/*)))

# $(info $$SOURCES = $(SOURCES))
# $(info $$OBJECTS = $(OBJECTS))
# $(info $$SRC_INC_DIRS = $(SRC_INC_DIRS))
# $(info $$ASFLAGS = $(ASFLAGS))
# $(info $$TARGETOBJDIR = $(TARGETOBJDIR))
# $(info $$TARGETLIST = $(TARGETLIST))
# $(info $$PROGRAM = $(PROGRAM))

vpath %.c $(SRC_INC_DIRS)

$(TARGETOBJDIR)/%.o: %.c | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(TARGETLIST) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

vpath %.s $(SRC_INC_DIRS)

$(TARGETOBJDIR)/%.o: %.s | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(TARGETLIST) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/$(PROGRAM): $(OBJECTS) $(LIBS) | $(BUILD_DIR)
	$(CC) -t $(TARGETLIST) $(LDFLAGS) -o $@ $(patsubst %.cfg,-C %.cfg,$^)

$(PROGRAM): $(BUILD_DIR)/$(PROGRAM) | $(BUILD_DIR)

test: $(PROGRAM)
	$(PREEMUCMD)
	$(EMUCMD) $(BUILD_DIR)\\$<
	$(POSTEMUCMD)

clean:
	$(call RMFILES,$(OBJECTS))
	$(call RMFILES,$(DEPENDS))
	$(call RMFILES,$(REMOVES))
	$(call RMFILES,$(BUILD_DIR)/$(PROGRAM))

dist: $(PROGRAM)
	$(call MKDIR,dist/)
	$(call RMFILES,dist/$(PROGRAM)*)
	cp build/$(PROGRAM) dist/$(PROGRAM).com

atr: dist
	$(call MKDIR,dist/atr)
	cp dist/$(PROGRAM).com dist/atr/$(APP_NAME).com
	$(call RMFILES,dist/*.atr)
	dir2atr -S dist/$(APP_NAME).atr dist/atr

po: dist
	$(call RMFILES,dist/$(APP_NAME)*.po)
	cp dist/$(PROGRAM).com dist/$(APP_NAME)
	$(APPLE_TOOLS_DIR)/mk-bitsy.sh dist/$(APP_NAME).po TEST_$(APP_NAME)
	$(APPLE_TOOLS_DIR)/add-file.sh dist/$(APP_NAME).po dist/$(APP_NAME) $(APP_NAME)

else # $(words $(TARGETLIST)),1

all test clean:
	$(foreach t,$(TARGETLIST),$(MAKE) TARGETS=$t --no-print-directory clean all dist$(NEWLINE))

endif # $(words $(TARGETLIST)),1

###################################################################
###  Place your additional targets in the additional Makefiles  ###
### in the same directory - their names have to end with ".mk"! ###
###################################################################
-include *.mk
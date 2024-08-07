###################################################################
# Platform Mapping, OS specifics, and Emulator settings
###################################################################

ifeq ($(CURRENT_TARGET),)
$(error Missing value for CURRENT_TARGET)
endif

CURRENT_PLATFORM_apple2 := apple2
CURRENT_PLATFORM_apple2enh := apple2
CURRENT_PLATFORM_atari := atari
CURRENT_PLATFORM_atarixl := atari
CURRENT_PLATFORM_c64 := c64
CURRENT_PLATFORM_c128 := c64
CURRENT_PLATFORM_c16 := c64
CURRENT_PLATFORM_pet := c64
CURRENT_PLATFORM_plus4 := c64
#CURRENT_PLATFORM_x16 := c64
CURRENT_PLATFORM_cbm510 := c64
CURRENT_PLATFORM_cbm610 := c64
CURRENT_PLATFORM_vic20 := c64

CURRENT_PLATFORM = $(CURRENT_PLATFORM_$(CURRENT_TARGET))

ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

# MSYS2 environment hack for extra slash (XS) needed in cmd line args
XS =
ifeq ($(detected_OS),$(filter $(detected_OS),MSYS MINGW))
	XS = /
endif

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

PREEMUCMD :=
POSTEMUCMD :=

ifeq ($(EMUCMD),)
  EMUCMD = $($(CURRENT_TARGET)_EMUCMD)
endif

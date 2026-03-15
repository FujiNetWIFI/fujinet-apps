CC_DEFAULT ?= cmoc
AS_DEFAULT ?= $(CC_DEFAULT)
LD_DEFAULT ?= $(CC_DEFAULT)
AR_DEFAULT = lwar

include $(MWD)/tc-common.mk

CFLAGS += --intdir=$(OBJ_DIR)
ASFLAGS +=
LDFLAGS +=

CFLAGS += -DGIT_VERSION='"$(GIT_VERSION)"'

# Needed because of using sed on error messages
SHELL = /bin/bash -o pipefail

define include-dir-flag
  -I$1
endef

define asm-include-dir-flag
  -I$1
endef

define library-dir-flag
  -L$1
endef

define library-flag
  -l$1
endef

define link-lib
  $(AR) -a -r $@ $^
endef

define link-bin
  $(LD) -o $1 $(LDFLAGS) $2 $(LIBS)
endef

define compile
  $(CC) -c $(CFLAGS) --deps=$(1:.o=.d) -o $1 $2
endef

define assemble
  $(AS) -c $(ASFLAGS) -o $1 $2 2>&1 | sed -e 's/^\(.*\)(\([0-9][0-9]*\)) :/\1:\2:/'
endef

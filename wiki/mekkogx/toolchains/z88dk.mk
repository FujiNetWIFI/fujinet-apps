CC_DEFAULT ?= zcc
AS_DEFAULT ?= z80asm
LD_DEFAULT ?= $(CC_DEFAULT)
AR_DEFAULT ?= $(AS_DEFAULT)

include $(MWD)/tc-common.mk

CFLAGS +=
ASFLAGS +=
LDFLAGS +=

CFLAGS += -DGIT_VERSION='\"$(GIT_VERSION)\"'

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
  $(AR) -x$1 $2
endef

define link-bin
  $(LD) $(LDFLAGS) $2 $(LIBS) -o $1
endef

define compile
  $(CC) -c $(CFLAGS) -o $1 $2 -MT $(1:.o=.d) -MMD
endef

define assemble
  $(AS) $(ASFLAGS) -o=$1 $2
endef

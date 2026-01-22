CC_DEFAULT ?= wcc
AS_DEFAULT ?= wasm
LD_DEFAULT ?= wlink OPTION quiet
AR_DEFAULT ?= wlib

include $(MWD)/tc-common.mk

CFLAGS += -0 -bt=dos -ms -s -osh -zu -fr=$(basename $@).err
ASFLAGS +=
LDFLAGS += SYSTEM dos LIBPATH $(FUJINET_LIB_DIR)

CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

define include-dir-flag
  -I$1
endef

define asm-include-dir-flag
  -I$1
endef

define library-dir-flag
endef

define library-flag
  $1
endef

define link-lib
  $(AR) -n $1 $2
endef

define link-bin
  $(LD) $(LDFLAGS) \
    disable 1014 \
    name $1 \
    file {$2} \
    library {$(LIBS)}
endef

define compile
  $(CC) $(CFLAGS) -ad=$(1:.o=.d) -fo=$1 $2
endef

define assemble
  $(AS) -c $(ASFLAGS) -fo=$1 $2 2>&1
endef

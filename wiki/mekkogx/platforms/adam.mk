EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).ddp
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

ADAM_FLAGS = +coleco -subtype=adam
CFLAGS += $(ADAM_FLAGS)
LDFLAGS += $(ADAM_FLAGS)
LIBS += -leos.lib -lsmartkeys.lib
ifneq ($(IS_LIBRARY),1)
  LDFLAGS += -create-app
endif

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

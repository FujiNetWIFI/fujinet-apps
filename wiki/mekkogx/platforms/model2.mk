EXEC_SUFFIX = .com
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

MODEL2_FLAGS = +cpm
CFLAGS += $(MODEL2_FLAGS)
LDFLAGS += $(MODEL2_FLAGS)
ifneq ($(IS_LIBRARY),1)
  LDFLAGS += -create-app
endif

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

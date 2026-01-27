# Automatically figure out TOOLCHAIN from the .mk file that included us
TOOLCHAIN_MK := $(call pop,$(MAKEFILE_LIST))
TOOLCHAIN := $(basename $(notdir $(lastword $(TOOLCHAIN_MK))))
TOOLCHAIN_UC := $(shell echo "$(TOOLCHAIN)" | tr '[:lower:]' '[:upper:]')
$(info Using TOOLCHAIN=$(TOOLCHAIN))

CC_$(TOOLCHAIN_UC) ?= $(CC_DEFAULT)
AS_$(TOOLCHAIN_UC) ?= $(AS_DEFAULT)
LD_$(TOOLCHAIN_UC) ?= $(LD_DEFAULT)
AR_$(TOOLCHAIN_UC) ?= $(AR_DEFAULT)
PC_$(TOOLCHAIN_UC) ?= $(PC_DEFAULT)

CC := $(CC_$(TOOLCHAIN_UC))
AS := $(AS_$(TOOLCHAIN_UC))
LD := $(LD_$(TOOLCHAIN_UC))
AR := $(AR_$(TOOLCHAIN_UC))
PC := $(PC_$(TOOLCHAIN_UC))

CFLAGS += $(CFLAGS_EXTRA_$(TOOLCHAIN_UC)) $(CFLAGS_EXTRA_$(PLATFORM_UC))
ASFLAGS += $(ASFLAGS_EXTRA_$(TOOLCHAIN_UC)) $(ASFLAGS_EXTRA_$(PLATFORM_UC))
LDFLAGS += $(LDFLAGS_EXTRA_$(TOOLCHAIN_UC)) $(LDFLAGS_EXTRA_$(PLATFORM_UC))

EXTRA_INCLUDE_$(TOOLCHAIN_UC) += $(EXTRA_INCLUDE) $(EXTRA_INCLUDE_$(PLATFORM_UC))
CFLAGS += $(foreach incdir,$(EXTRA_INCLUDE_$(TOOLCHAIN_UC)),$(call include-dir-flag,$(incdir)))
ASFLAGS += $(foreach incdir,$(EXTRA_INCLUDE_$(TOOLCHAIN_UC)),$(call asm-include-dir-flag,$(incdir)))

ifdef FUJINET_LIB_INCLUDE
  CFLAGS += $(call include-dir-flag,$(FUJINET_LIB_INCLUDE))
  ASFLAGS += $(call asm-include-dir-flag,$(FUJINET_LIB_INCLUDE))
endif
ifdef FUJINET_LIB_DIR
  LIBS += $(call library-dir-flag,$(FUJINET_LIB_DIR)) $(call library-flag,$(FUJINET_LIB_LDLIB))
endif

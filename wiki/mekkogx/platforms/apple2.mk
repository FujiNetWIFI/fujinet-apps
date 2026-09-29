EXEC_SUFFIX = .a2s
DISK = $(R2R_PD)/$(PRODUCT_BASE).po
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib
DISK_TOOL = ac
DISK_TOOL_X = acx
DISK_TOOL_INFO = https://github.com/AppleCommander/AppleCommander/releases/
DISK_SIZE ?= 140kb

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cc65.mk

r2r:: $(BUILD_DISK) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

PRODOS_VERSION = 2.4.3
PRODOS8_DISK ?= $(CACHE_PLATFORM)/PRODOS8-$(PRODOS_VERSION).po

# ProDOS boots the first .SYSTEM file, so a multi product disk leads with
# the selector (Bitsy Bye on 2.4.x) to let any product be chosen
ifeq ($(MEKKO_MULTI),1)
  DISK_BOOT_SYSTEM ?= QUIT.SYSTEM
endif
DISK_BASIC_SYSTEM ?= BASIC.SYSTEM
CC65_UTILS_DIR := $(shell cl65 --print-target-path --target $(PLATFORM))/$(PLATFORM)/util
LOADER_SYSTEM := loader.system

$(BUILD_DISK): $(DISK_EXECUTABLES) $(PRODOS8_DISK) $(DISK_EXTRA_DEPS) $(DISK_EXTRA_FILES) | $(R2R_PD)
	$(call require,$(DISK_TOOL),$(DISK_TOOL_INFO))
	$(call require,$(DISK_TOOL_X),$(DISK_TOOL_INFO))
	$(DISK_TOOL_X) create -d $@ --format $(PRODOS8_DISK) --prodos --size=$(DISK_SIZE) --name=$(PRODUCT_BASE)
	$(DISK_TOOL_X) export --as -d $@ BASIC.SYSTEM > $(CACHE_PLATFORM)/BASIC.SYSTEM
	$(DISK_TOOL) -d $@ BASIC.SYSTEM
	$(if $(DISK_BOOT_SYSTEM),$(DISK_TOOL_X) export --as -d $(PRODOS8_DISK) $(DISK_BOOT_SYSTEM) > $(CACHE_PLATFORM)/$(DISK_BOOT_SYSTEM); $(call copy-to-disk,-as,$(CACHE_PLATFORM)/$(DISK_BOOT_SYSTEM),$(DISK_BOOT_SYSTEM),$@);)
	$(foreach e,$(DISK_EXECUTABLES),$(call copy-to-disk,-as,$(e),$(basename $(notdir $(e))),$@); $(call copy-to-disk,-p,$(CC65_UTILS_DIR)/$(LOADER_SYSTEM),$(basename $(notdir $(e))).SYSTEM SYS 0x2000,$@);)
	$(if $(DISK_BASIC_SYSTEM),$(call copy-to-disk,-as,$(CACHE_PLATFORM)/BASIC.SYSTEM,$(DISK_BASIC_SYSTEM),$@);)
	$(foreach f,$(DISK_EXTRA_FILES),$(call copy-to-disk,-ptx,$(f),$(notdir $(f)),$@);)
	make -f $(PLATFORM_MK) $(PLATFORM)/disk-post

# Download and cache ProDOS disk if necessary
PRODOS_URL = https://releases.prodos8.com
PRODOS8_RELEASE := ProDOS_$(subst .,_,$(PRODOS_VERSION)).po
$(PRODOS8_DISK): | $(CACHE_PLATFORM)
	curl --insecure -L -o $@ $(PRODOS_URL)/$(PRODOS8_RELEASE)

# Converts AppleSingle (cc65 output) to AppleDouble (netatalk share)
UNSINGLE = unsingle
EXECUTABLE_AD = $(R2R_PD)/$(PRODUCT_BASE)

define single-to-double
  unsingle $< && mv $<.ad $@ && mv .AppleDouble/$<.ad .AppleDouble/$@
endef

$(EXECUTABLE_AD): $(BUILD_EXEC)
	if command -v $(UNSINGLE) > /dev/null 2>&1 ; then \
	  $(single-to-double) ; \
	else \
	  cp $< $@ ; \
	fi

# Arguments:
# $1 == DISK_TOOL flags
# $2 == source file
# $3 == destination name
# $4 == disk image
define copy-to-disk
    $(DISK_TOOL) $1 $4 $3 < $2
endef

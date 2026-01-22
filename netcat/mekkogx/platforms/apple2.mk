EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).a2s
DISK = $(R2R_PD)/$(PRODUCT_BASE).po
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib
DISK_TOOL = ac
DISK_TOOL_X = acx
DISK_TOOL_INFO = https://github.com/AppleCommander/AppleCommander/releases/

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cc65.mk

r2r:: $(BUILD_DISK) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

PRODOS_VERSION = 2.4.3
PRODOS8_DISK ?= $(CACHE_PLATFORM)/PRODOS8-$(PRODOS_VERSION).po
CC65_UTILS_DIR := $(shell cl65 --print-target-path --target $(PLATFORM))/$(PLATFORM)/util
LOADER_SYSTEM := loader.system

$(BUILD_DISK): $(BUILD_EXEC) $(PRODOS8_DISK) $(DISK_EXTRA_DEPS) $(DISK_EXTRA_FILES) | $(R2R_PD)
	$(call require,$(DISK_TOOL),$(DISK_TOOL_INFO))
	$(call require,$(DISK_TOOL_X),$(DISK_TOOL_INFO))
	$(DISK_TOOL_X) create -d $@ --format $(PRODOS8_DISK) --prodos --size=140kb --name=$(PRODUCT_BASE)
	$(call copy-to-disk,-as,$<,$(PRODUCT_BASE),$@)
	$(call copy-to-disk,-p,$(CC65_UTILS_DIR)/$(LOADER_SYSTEM),$(PRODUCT_BASE).SYSTEM SYS 0x2000,$@)
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

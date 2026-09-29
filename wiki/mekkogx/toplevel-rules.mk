# Executables and disk images will be placed into a platform specific
# subdirectory in a "Ready 2 Run" directory (r2r)
R2R_DIR = r2r
BUILD_DIR = build
CACHE_DIR = _cache
RELEASE_DIR = release

MAKEFILE_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

# PRODUCTS names several binaries sharing one disk, PRODUCTS_DISK_NAME
ifneq ($(strip $(PRODUCTS)),)
  PRODUCTS_DISK_NAME ?= $(if $(strip $(PRODUCT)),$(PRODUCT),$(firstword $(filter-out %.lib,$(PRODUCTS))))
  MEKKO_TARGET := $(PRODUCTS_DISK_NAME)
else
  MEKKO_TARGET := $(PRODUCT)
endif

# Make a list of the things we want to build which combine R2R dir, app name, and platform
APP_TARGETS := $(foreach p, $(PLATFORMS), $(R2R_DIR)/$(p)/$(MEKKO_TARGET))

.PHONY: all clean release FORCE

all:: $(APP_TARGETS)

clean::
	rm -rf $(R2R_DIR) $(BUILD_DIR) $(CACHE_DIR)

ifeq ($(PLATFORM),)
RELEASE_TARGETS := $(foreach p,$(PLATFORMS),$(p)/release)
$(info RELEASE_TARGETS=$(RELEASE_TARGETS))
release:: $(RELEASE_TARGETS)
endif

# Use % wildcard match to platform specific app so we don't have to
# spell out every single platform variation
$(R2R_DIR)/%/$(MEKKO_TARGET): FORCE
	$(MAKE) -f $(MAKEFILE_DIR)/platforms/$*.mk \
	  $(if $(MEKKO_CONFIG),MEKKO_CONFIG=$(MEKKO_CONFIG)) r2r

# Convenience: allow `make coco` (or apple2) as a shortcut
$(PLATFORMS): %: $(R2R_DIR)/%/$(MEKKO_TARGET)

# ------------------------------------------------------------------------
# Pattern rule to support "make <platform>/<target>" syntax.
#
# Example: "make apple2/r2r"
#   $@   = "apple2/r2r"         (the full target name)
#   $(@D) = "apple2"             (the directory part before the slash)
#   $(@F) = "r2r"                (the filename part after the slash)
#
# This runs the corresponding platform makefile:
#   make -f mekkogx/platforms/apple2.mk r2r
#
# Works for ANY target name, so:
#   make coco/clean   -> runs clean in mekkogx/platforms/coco.mk
#   make atari/debug  -> runs debug in mekkogx/platforms/atari.mk
# ------------------------------------------------------------------------
.DEFAULT:
	@target="$@" ; case "$@" in \
	  */*/*)   echo "No rule to make target '$@'"; exit 1;; \
	  */*)     platform=$${target%/*}; target=$${target##*/}; \
	           $(MAKE) -f $(MAKEFILE_DIR)/platforms/$${platform}.mk \
	             $(if $(MEKKO_CONFIG),MEKKO_CONFIG=$(MEKKO_CONFIG)) $${target} ;; \
	  *)       echo "No rule to make target '$@'"; exit 1;; \
	esac

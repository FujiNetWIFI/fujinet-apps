# Generic CC65 TARGETS makefile++
#
# Set the TARGETS and PROGRAM values as required.
# See makefiles/build.mk for details on directory structure for src files and how to add custom extensions to the build.
$(info |)
$(info |)
$(info Starting Build)

TARGETS = apple2enh apple2
PROGRAM := iss

# We need to set this here to point to the relative path to the fujinet-build-tools repo
#FUJINET-BUILD-TOOLS_DIR := ../../fujinet-build-tools

export FUJINET_BUILD_TOOLS_DIR := ../
export FUJINET_LIB_VERSION := 4.3.1


# Check if FUJINET_BUILD_TOOLS_DIR is set to a real directory
ifeq ($(wildcard $(FUJINET_BUILD_TOOLS_DIR)),)
$(error You must have FUJINET_BUILD_TOOLS_DIR set to the location of fujinet-build-tools repo)
endif

$(info >>>> Using FUJINET_BUILD_TOOLS_DIR=$(FUJINET_BUILD_TOOLS_DIR))

SUB_TASKS := clean disk test release distlocal
.PHONY: all help $(SUB_TASKS)

all:
	@for target in $(TARGETS); do \
		echo "-------------------------------------"; \
		echo "Building $$target"; \
		echo "-------------------------------------"; \
		$(MAKE) --no-print-directory -f $(FUJINET_BUILD_TOOLS_DIR)/makefiles/build.mk CURRENT_TARGET=$$target PROGRAM=$(PROGRAM) $(MAKECMDGOALS); \
	done

# if disk images were built show them
	@if [ -d ./dist ]; then \
		echo "Contents of dist:"; \
		ls -1 ./dist; \
	fi

	@echo ""; 
	@echo "Build Complete"; 
	@echo ""; 


$(SUB_TASKS): _do_all
$(SUB_TASKS):
	@:

_do_all: all

help:
	@echo "Makefile for $(PROGRAM)"
	@echo ""
	@echo "Available tasks:"
	@echo "all       - do all compilation tasks, create app in build directory"
	@echo "clean     - remove all build artifacts"
	@echo "release   - create a release of the executable in the build/ dir"
	@echo "disk      - generate platform specific disk images in dist/ dir"
	@echo "deploy    - deploy dist/ images to a local TNFS or other server platform"
	@echo "test      - run application in emulator for given platform."
	@echo "            specific platforms may expose additional variables to run with"
	@echo "            different emulators, see makefiles/custom-<platform>.mk"
	
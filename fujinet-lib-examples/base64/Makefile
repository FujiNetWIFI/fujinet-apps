# Generic CC65 TARGETS makefile++
#
# Set the TARGETS and PROGRAM values as required.
# See makefiles/build.mk for details on directory structure for src files and how to add custom extensions to the build.

TARGETS = atari apple2 c64
PROGRAM := base64

SUB_TASKS := clean disk test release
.PHONY: all help $(SUB_TASKS)

all:
	@for target in $(TARGETS); do \
		echo "-------------------------------------"; \
		echo "Building $$target"; \
		echo "-------------------------------------"; \
		$(MAKE) --no-print-directory -f ../makefiles/build.mk CURRENT_TARGET=$$target PROGRAM=$(PROGRAM) $(MAKECMDGOALS); \
	done

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
	@echo "disk      - generate platform specific disk versions of application (PO/ATR etc)"
	@echo "test      - run application in emulator for given platform."
	@echo "            specific platforms may expose additional variables to run with"
	@echo "            different emulators, see makefiles/custom-<platform>.mk"
	@echo "release   - create a release of the executable in the dist/ dir"

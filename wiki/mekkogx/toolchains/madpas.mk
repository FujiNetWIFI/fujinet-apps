# Mad-Pascal doesn't produce .o files. In this setup, we treat Mad-Pascal as the "linker":
# 1. Compile and assemble stages generate fake .o files that record
#    the source file names.
# 2. During the link stage, the madpas-build.py wrapper collects all
#    source files from the fake .o files and passes them to Mad-Pascal
#    to produce the final binary.

include $(MWD)/tc-common.mk

MADS_LIBS += \
  --unit-path /usr/local/share/mads/lib \
  --unit-path /usr/local/share/mads/blibs \
  --include-path /usr/local/share/mads/base

define library-dir-flag
  --unit-path $1
endef

define link-lib
  @echo "Mad-Pascal does not create libaries" ; exit 1
endef

define link-bin
  $(MWD)/toolchains/madpas-build.py $(MADS_LIBS) -o $1 $2
endef

define compile
  @echo "Mad-Pascal does not support C" ; exit 1
endef

define assemble
  echo "$(basename $(notdir $2)) rcasm $2" > $1
endef

define compile-pas
  echo "$(basename $(notdir $2)) rcpascal $2" > $1
endef

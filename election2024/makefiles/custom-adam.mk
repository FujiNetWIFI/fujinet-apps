###################################################################
# ADAM
###################################################################
ifeq ($(DEBUG),true)
    $(info >Starting custom-adam.mk)
endif

# COMPILE FLAGS

CC=zcc
AS=zcc

CFLAGS=+coleco -subtype=adam
LDFLAGS=+coleco -subtype=adam -o$(DIST_DIR)/$(PROGRAM).ddp -create-app -lsmartkeys -leos
ASFLAGS=+coleco -subtype=adam

################################################################
# DISK creation

SUFFIX = .prg
VICE_HOME := $(dir $(shell which xvic))

DISK_FILE = $(DIST_DIR)/$(PROGRAM).ddp



################################################################
# TESTING / EMULATOR

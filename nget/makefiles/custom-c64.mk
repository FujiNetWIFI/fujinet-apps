###################################################################
# C64
###################################################################
ifeq ($(DEBUG),true)
    $(info >Starting custom-c64.mk)
endif

# COMPILE FLAGS

################################################################
# DISK creation

SUFFIX = .prg
DISK_TASKS += .create-d64
VICE_HOME := $(dir $(shell which xvic))

DISK_FILE = $(DIST_DIR)/$(PROGRAM).d64

.create-d64:
	@which cc1541 > /dev/null 2>&1 ; \
	if [ $$? -eq 0 ] ; then \
		echo "creating $(DISK_FILE) from program $(PROGRAM_TGT)" ; \
		if [ -f "$(DISK_FILE)" ] ; then \
			rm "$(DISK_FILE)" ; \
		fi ; \
		cc1541 -q -n "fn-lib: $(PROGRAM)" -f "$(PROGRAM)" -P -w $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) -H "created by fenrock" $(DISK_FILE) ; \
	else \
		echo -e "\nERROR! You must install cc1541 from https://bitbucket.org/ptv_claus/cc1541/src/master/ to create commodore disks\n" ; \
		exit 1 ; \
	fi

################################################################
# TESTING / EMULATOR

c64_EMUCMD    := $(VICE_HOME)/x64sc
c128_EMUCMD   := $(VICE_HOME)/x128
c16_EMUCMD    := $(VICE_HOME)/xvic
pet_EMUCMD    := $(VICE_HOME)/xpet
plus4_EMUCMD  := $(VICE_HOME)/xplus4
vic20_EMUCMD  := $(VICE_HOME)/xvic
#x16_EMUCMD    := $(VICE_HOME)/xplus4 -ramsize 16
cbm510_EMUCMD := $(VICE_HOME)/xcbm5x0
cbm610_EMUCMD := $(VICE_HOME)/xcbm2
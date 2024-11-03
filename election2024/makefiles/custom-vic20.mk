###################################################################
# VIC20 
###################################################################
ifeq ($(DEBUG),true)
    $(info >Starting custom-vic20.mk)
endif

# COMPILE FLAGS
CFLAGS += -C src/vic20/cart.cfg
LDFLAGS += -C src/vic20/cart.cfg

################################################################
# CART creation

SUFFIX = .prg
CART_TASKS += .create-crt
VICE_HOME := $(dir $(shell which xvic))

CART_FILE = $(DIST_DIR)/$(PROGRAM).crt

.create-crt:
	@which cartconv > /dev/null 2>&1 ; \
	if [ $$? -eq 0 ] ; then \
		echo "creating $(CART_FILE) from program $(PROGRAM_TGT)" ; \
		if [ -f "$(CART_FILE)" ] ; then \
			rm "$(CART_FILE)" ; \
		fi ; \
		cartconv -t vic20 -i $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) -o $(CART_FILE) ; \
	else \
		echo -e "\nERROR! You must install cartconv from https://bitbucket.org/ptv_claus/cartconv/src/master/ to create commodore disks\n" ; \
		exit 1 ; \
	fi

################################################################
# TESTING / EMULATOR

vic20_EMUCMD  := $(VICE_HOME)/xvic

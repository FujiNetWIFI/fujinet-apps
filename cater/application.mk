# for Cater, on Atari only we have a Flag that needs to be set for the memory mgt

$(info >>>>> Starting application.mk for: $(CURRENT_TARGET))

# Conditional settings for LDFLAGS based on the CURRENT_TARGET
ifeq ($(CURRENT_TARGET), atari)
LDFLAGS += -C src/atari/cater.cfg
$(info >>>> LDFLAGS=$(LDFLAGS))
endif

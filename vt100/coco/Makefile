CMOC ?= cmoc
DECB ?= decb
ORG    = 4000
NAME   = vt100
VPATH  = src
FNLIB  = fujinet-lib
FNNAME = fujinet-coco-4.10.0
OBJS   = main.o screen.o vt100.o
TEST   = test_engine
TEST_OBJS = test_engine.o screen.o vt100.o

all: $(NAME).dsk

%.o: %.c
	$(CMOC) --coco -c -Isrc -I$(FNLIB) -o $@ $<

$(NAME).bin: $(OBJS)
	$(CMOC) --coco --org=$(ORG) -o $@ $(OBJS) -L$(FNLIB) -l$(FNNAME)

$(TEST).bin: $(TEST_OBJS)
	$(CMOC) --coco --org=$(ORG) -o $@ $(TEST_OBJS)

$(NAME).dsk: $(NAME).bin $(TEST).bin
	$(DECB) dskini $@
	$(DECB) copy -2 -b $(NAME).bin $@,VT100.BIN
	$(DECB) copy -2 -b $(TEST).bin $@,TEST.BIN

clean:
	rm -f $(OBJS) term.o $(NAME).bin $(NAME).dsk
	rm -f test_engine.o $(TEST).bin

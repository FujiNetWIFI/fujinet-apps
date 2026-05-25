CMOC ?= cmoc
DECB ?= decb
ORG    = 4000
NAME   = vt100
VPATH  = src
FNLIB  = fujinet-lib
FNNAME = fujinet-coco-4.10.0
OBJS   = main.o term.o screen.o vt100.o

all: $(NAME).dsk

%.o: %.c
	$(CMOC) --coco -c -Isrc -I$(FNLIB) -o $@ $<

$(NAME).bin: $(OBJS)
	$(CMOC) --coco --org=$(ORG) -o $@ $(OBJS) -L$(FNLIB) -l$(FNNAME)

$(NAME).dsk: $(NAME).bin
	$(DECB) dskini $@
	$(DECB) copy -2 -b $(NAME).bin $@,VT100.BIN

clean:
	rm -f $(OBJS) $(NAME).bin $(NAME).dsk

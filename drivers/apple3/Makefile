# Makefile for Apple /// Fujinet driver
AC = build/ac.jar
OUTPUTDISK1 = disks/sos_selector_hd.po

default:
	mkdir -p lst
	mkdir -p obj
	mkdir -p out
	ca65 src/fujinet.s -l lst/fujinet.lst -o obj/fujinet.o
	ld65 obj/fujinet.o -o out/fujinet.o65 -C build/Apple3_o65.cfg
	java -jar $(AC) -g $(OUTPUTDISK1) SOS.DRIVER > out/SOS.DRIVER#0c0000
	python build/A3Driverutil.py update out/fujinet.o65 out/SOS.DRIVER#0c0000
	java -jar $(AC) -d $(OUTPUTDISK1) SOS.DRIVER
	java -jar $(AC) -p $(OUTPUTDISK1) SOS.DRIVER SOS 0x0000 < out/SOS.DRIVER#0c0000

clean:
	rm -r lst
	rm -r obj
	rm -r out

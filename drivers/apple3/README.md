# Apple /// Fujinet driver

This is an all in one driver to support the following devices in SOS:

Unit  Name       Type
0     .PROFILE   Block
1     .S2        Block
2     .S3        Block
3     .S3        Block
4     .NETWORK   Char
5     .PRINTER   Char
6     .CPM       Char
7     .RS232     Char -> mapped to Fujinet MODEM device
8     .FN_CLOCK  Char

Update:
Included an option to build just the character devices into the driver. The driver can then be used with the standard soshdboot sos.

The following prebuilt disk images are available now:

1. sos_selector_hd.po
The FujiNet driver for this image the includes the character devices only and gives you two block devices using the internal soshdboot driver. This boots into Selector/// and includes most A3 software. This is based on the Apple3rtr image plus quite a few additions.

2. businessbasic_hd.po
The FujiNet driver for this image the includes the character devices only and boots straight into Business Basic so you can write your .network programs quickly from there.

3. sos15_selector_hd.po
The FujiNet driver includes all of the devices, block and character. This requires an updated version of the Soshdboot version of SOS to run.(so that the four Smartport HD devices are available)
This special version is limited at the moment to only using the Selector /// program selector on boot up. You can easily add more applications to this. Its very much an early version, so beware.

You will need either a soshdboot rom installed, or, use the special boot floppy available on the soshdboot site to boot these images. For more details, refer to the Fujinet Wiki.

# Driver overview

## 4 x SP Block devices
mount them via Fujinet CONFIG, or via the web interface

## .PRINTER device
Select .PRINTER as yout output device in you application, and grab the printout from the Fujinet web interface.

## .RS232 device
The standard comms programs under the 'Communications' menu will work with this, eg: Access/// or Easyterm. the Modem dev support AT command, eg ATDTcqbbs.ddns.net:6800 

## .CPM device
Included under the CPM menu is a modifed version of Access/// setup for the .CPM device. For some reason, none of these comms programs seem to support changin the serial device. This one has been hand modifed with a hex editor to use .CPM instead of .RS232. This gives ternial access to CPM running on the Fujinet. You will need the appropriate files setup on your SD card to support this, refer to the Fujinet Wiki. Its actually quite fast compared to the Softcard ///.

## .FN_CLOCK device
The driver automatically updates the SOS time from Fujinet when the device receives it INIT from SOS. Now you can rip out those batteries!

## .NETWORK device
Available as a normal character device. Its really only been tested from Business Basic so far.

Have a look at the following programs for examples of the code needed to access it. This is using the supplied invokable module "REQUEST.INV" that came with Business Basic to interface with SOS.

These are under .PROFILE/LANGUAGES/BASIC
IPV4TEST - print your external IP
READWEB - read some web content
ECHOSERVER - listen on TCP port for connections and echo data back
ISSV3 - ISS tracker (note there are a couple of older version that will show some others ways of doing the same thing. The latest feels like the simplest way.


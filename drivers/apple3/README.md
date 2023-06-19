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

These are all using Smartport and requires an updated version of the Soshdboot version of SOS to run.(so that the 4 SP HD devices are available)
This special version is limited at the moment to only using the Selector /// program selector on boot up. You can easily add more applications to this. Its very much an early version, so beware.

The included disk image includes this and the driver all ready to go. You will need either a soshdboot rom installed, or, use the special boot floppy available on the soshdboot site. For more details, refer to the Fujinet Wiki.

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
ISSV3 - ISS tracker (note there are a couple of older version that will show some others ways of doing the same thing. The latest feels like the simplest way.


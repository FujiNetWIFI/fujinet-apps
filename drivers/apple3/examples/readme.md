# Example Apple /// Programs for the network device

## Basic

These text files can be loaded onto a disk using your program of choice (Ciderpress, AppleCommander or Cadius)

Then do 'exec <program.bas>' to load it into Business Basic.

You can then do a 'save program' to save it as a business basic .ba3 file

* ipv4test.bas - print your external IP address
* readweb.bas - Read the GPL license file from the web and print.
* echoserver.bas - Listen on TCP port for connections, and accept and print then echo the data back
* issv3.bas - Track the location of the ISS

## Pascal

These text files can be loaded onto a disk using your program of choice (Ciderpress, AppleCommander or Cadius)
They then need to be converted to PTX format. 

This can be done in A3 pascal:

1. Run A3 Pascal
2. Type E to edit file, then type the name of the file to load
3. Type S (Set), E (Environment), A (ascii), T (to set to true), ESC
4. Type Q (Quit), S (Save), Y (confirm overwrite), E (exit editor)
5. Type C (Compile), enter file name eg: ISS.TEXT, enter code name eg: ISSP.CODE, RET for no listing
6. Type X (execute) to run program, enter the name ISSP.CODE

* iss_pascal.text - Track the location of the ISS ( ported from above Basic program )

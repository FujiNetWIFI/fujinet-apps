@echo off

del fcs
taskkill /im applewin.exe

REM Compile
make

REM Check if compile was successful
if not exist fcs goto skip

REM Build disk
cd dist
del fcs.po
copy bootable.po fcs.po
java -jar ac.jar -p fcs.po fcs.system sys < loader.system
java -jar ac.jar -as fcs.po fcs bin < ..\fcs

REM Run on emulator (TODO - CHANGE PATH TO EMULATOR)
start C:\Projects\AppleWin\Debug\applewin.exe -d1 fcs.po
:skip
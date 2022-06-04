/******************************************************************************
 Lynx GD Menu Loader 2. Firmware for the Lynx GD game cartridge.

 Copyright 2019 Igor Kromin (Necrocia) - Atari Gamer

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef __LYNX_GD_PROGRAM__
#define __LYNX_GD_PROGRAM__

#include "LynxGD.h"

FRESULT __fastcall__ LynxGD_Program(const char *pFilename);
FRESULT __fastcall__ LynxGD_ProgramLYX(const char *pFilename);
FRESULT __fastcall__ LynxGD_ProgramHomebrew(const char *pFilename);
void LaunchROM();

#endif // __LYNX_GD_PROGRAM__

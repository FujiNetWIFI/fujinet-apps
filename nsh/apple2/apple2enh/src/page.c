/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Page display functions
 */

#include <apple2.h>
#include <conio.h>
#include <stdio.h>

/**
 * @var text for line data 
 */
static const char _line[81] =
  {
    0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,
    0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,
    0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,
    0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,
    0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,
    0x00
  };

/**
 * @brief Re-usable text fragments
 */
const char *helpText = "For Help: Press \xC1?";
const char *selectText = "Press \xCA \xCB \xC8 \xD5 to select an option.";
const char *returnText = "Then press RETURN.";
const char *selectDeviceSubheader="Select Network Device                                             ESC: Main Menu";

/**
 * @brief display horizontal rule at specified line
 * @param y Desired horizontal line
 */
void page_line(unsigned char y)
{
  gotoxy(0,y);
  cputs(_line);
}

/**
 * @brief Display standard page
 * @param header    Header text to display
 * @param subheader Subheader text to display
 * @param footer    Footer text to display
 */
void page(char *header, char *subheader, char *footer)
{
  clrscr();

  page_line(2);
  page_line(22);
  
  cputsxy(0,0,header);
  cputsxy(0,1,subheader);
  cputsxy(0,23,footer);  
}
/**
 * Twitter - menu functions
 */

#include <string.h>
#include "menu.h"

const char menu[] =
  "\x00\x00\x80\xA8\xEF\xED\xE5\x80"                         // Home
  "\x25\x78\x70\x6C\x6f\x72\x65\x00"                         // Explore
  "\x2E\x6f\x74\x69\x66\x69\x63\x61\x74\x69\x6f\x6e\x73\x00" // Notifications
  "\x2D\x65\x73\x73\x61\x67\x65\x73\x00"                     // Messages
  "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
  "\x22\x6f\x6f\x6B\x6d\x61\x72\x6b\x73\x00"                 // Bookmarks
  "\x2C\x69\x73\x74\x73\x00"                                 // Lists
  "\x30\x72\x6f\x66\x69\x6c\x65\x00";                        // Profile

extern unsigned char *video_ptr;

void menu_main(unsigned char i)
{
  memcpy(video_ptr,&menu,sizeof(menu));
}

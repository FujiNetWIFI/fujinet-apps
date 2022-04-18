/**
 * Wikipedia for Adam
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3 
 */

#include <msx.h>
#include <eos.h>
#include <smartkeys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "input.h"

#define NET 0x09
#define READ 4
#define WRITE 8
#define LF 2
#define ACK 0x80
#define CONNECTED 2

struct _oc
{
  char cmd;
  char mode;
  char trans;
  char url[256];
} OC;

DCB *dcb;

char term[128], current_url[256];
char article[16];

char page[8];
int p=1;

char response[1024];

bool keyVI = false;

/**
 * Replace spaces in a string
 */
void space_replace( char * old )
{
    int length = strlen(old);
    int i = 0; 
    for (; i < length; i++) {
        if( old[i] == ' ' )
            old[i] = '+';
        }
}

bool connected(void)
{
  while (eos_request_device_status(NET,dcb) < ACK);
  return eos_get_device_status(NET) & CONNECTED;
}

void display_contents(void)
{
  char r=eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC));

  clrscr();
  
  if (r != ACK)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("  COULD NOT OPEN URL.");
      exit(1);
    }
  
  while (!connected())
    {
      cprintf(".");
      csleep(2);
    }

  clrscr();

  while (connected())
    {
      memset(response,0,sizeof(response));
      
      if (eos_read_character_device(NET,response,1024) != 0x80)
	break;

      cprintf("%s",response);
    }

  // Close the connection
  eos_write_character_device(NET,"C",1);
}

void main(void)
{
 another_term:
  smartkeys_set_mode();
  smartkeys_sound_init();

  dcb = eos_find_dcb(NET);

  if (dcb == NULL)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("  FUJINET NOT FOUND.");
      return 1;
    }
  
  OC.cmd = 'O';
  OC.mode = READ|WRITE;
  OC.trans = 0;  

  memset(term,0,sizeof(term));
  
  while (term[0] == 0x00)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("  WELCOME TO WIKIPEDIA\n  ENTER SEARCH TERM, THEN PRESS [RETURN].");
      
      input_line(0,19,0,term,128,NULL);
    }

  space_replace(term);

 another_article:
  strcpy(OC.url,"N:HTTPS://fujinet.online/8bitwiki/wiki.php?t=lf&l=10&ps=31x17");
  strcpy(current_url,OC.url);
  strcat(OC.url,"&s=");
  strcat(OC.url,term);
  
  display_contents();

  smartkeys_display(NULL,NULL,NULL,NULL,NULL," ANOTHER\n  TERM");
  smartkeys_status("  SELECT ARTICLE ID, OR...");

  memset(article,0,sizeof(article));
  
  input_line(0,19,0,article,16,&keyVI);

  if (keyVI)
    goto another_term;
  
  strcpy(OC.url,current_url);
  strcat(OC.url,"&a=");
  strcat(OC.url,article);

  display_contents();

  while(1)
    {      
      strcpy(current_url,OC.url);

      smartkeys_display(NULL,NULL,NULL,NULL,NULL," ANOTHER\n ARTICLE");
      smartkeys_status("  ENTER PAGE #\n  OR [RETURN] FOR NEXT PAGE");

      memset(page,0,sizeof(page));
      
      input_line(0,19,0,page,16,&keyVI);

      if (keyVI)
	goto another_article;
      
      if (page[0]==0x00)
	p++;
      else
	p=atoi(page);

      itoa(p,page,10);
      
      strcat(OC.url,"&p=");
      strcat(OC.url,page);
      
      display_contents();
    }
}
  

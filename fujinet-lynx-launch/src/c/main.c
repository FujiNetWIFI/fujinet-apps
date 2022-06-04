/**
 * Quick and dirty fujinet lynx launcher
 */

#include <6502.h>
#include <tgi.h>
#include <serial.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Program.h"
#include "fujinet.h"
#include "LynxGD.h"

char buffer[256];
struct _esm
{
  char cmd;
  unsigned short port;
  char hostname[64];
} esm;

void main(void)
{
  unsigned long blockNum=0;
  int r;
  
  fujinet_init();
  joy_install(lynx_stdjoy_joy);
  LynxGD_Init();

  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_setframerate(60);
    CLI();
  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_clear();

  tgi_outtextxy(0,0,"OPENING");

  // Mount all devices.
  fujinet_send(0x0F,"\xD7",1);
  sleep(2);
  if ((r = LynxGD_OpenFile("/netload.lnx")) != FR_OK)
    {
      char tmp[3];
      sprintf(tmp,"%u",r);
      tgi_outtext(tmp);
      return;
    }

  tgi_outtextxy(0,8,"LOADING");

  for (blockNum=0;blockNum<1024;blockNum++)
    {
      unsigned short len;
      fujinet_send(0x04,(char *)&blockNum,5);
      fujinet_recv(0x04,(unsigned char *)buffer,&len);
      fujinet_recv(0x04,(unsigned char *)buffer,&len);
      LynxGD_WriteFile(buffer,256);
    }
  
  LynxGD_CloseFile();
  tgi_outtext(" CLO");

  esm.cmd = 0xF0;
  esm.port = 6502;
  strcpy(esm.hostname,"140.141.153.57");
  fujinet_send(0x0F,(char *)&esm,sizeof(esm));
  
  LynxGD_Program("netload.lnx");
  tgi_outtext(" PRO");
  LaunchROM();
}

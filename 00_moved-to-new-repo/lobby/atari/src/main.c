
/**
 * @brief   Lobby program for #FujiNet
 * @author  Thomas Cherryhomes, Eric Carr
 * @email   thom dot cherryhomes at gmail dot com,  eric dot carr at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include "appkey.h"
#include "nio.h"
#include "fuji.h"
#include "bar.h"

#define CREATOR_ID 0x0001 /* FUJINET  */
#define APP_ID     0x01   /* LOBBY    */
#define KEY_ID     0x00   /* USERNAME */
#define SERVER     "N2:TCP://fujinet.online:7373/"
#define LOBBY_ENDPOINT "N:http://fujinet.online:8080/view?platform=atari"
#define PAGE_SIZE  14   /* # of results to show per page of servers */
#define SCREEN_WIDTH 40
#define CHAT_Y 17

unsigned char username[64];
void* old_vprced;               // old PROCEED vector, restored on exit.
bool old_enabled=false;         // were interrupts enabled for old vector
bool running=false;
unsigned short bw=0;            // # of bytes waiting.
unsigned char chat_rx_buf[256]; // Chat RX buffer.
unsigned char rx_buf[4096];     // RX buffer.
unsigned char tx_buf[128];      // TX buffer.
unsigned char txbuflen;         // TX buffer length
unsigned char trip=0;           // if trip=1, fujinet is asking us for attention.
unsigned char buf[128];         // Temporary buffer use
extern void ih();               // defined in intr.s
extern void bar_setup_regs();   // defined in bar-setup-regs.s

bool skip_offline_check = false;        // Do not check if server is offline before mounting the client
bool skip_server_instructions = false;  // Do not show mount instructions

unsigned char server_count = 0;       // Number of servers available
unsigned char selected_server = 0;    // Currently selected server


const char error_138[]="FUJINET NOT RESPONDING\x9B";
const char error_139[]="FUJINET NAK\x9b";
const char error[]="SIO ERROR\x9b";

char host_slots[FUJI_HOST_SLOT_COUNT][FUJI_HOST_SLOT_NAME_LENGTH];
char instance_endpoint[64];

typedef struct {
  unsigned char game_type;    // 3 json chars max
  char * game;                // 12
  char * server;              // 32
  char * url;                 // 64
  char * client_url;          // 64
  char * region;              // 2
  unsigned char online;       // 1
  unsigned char players;      // 3
  unsigned char max_players;  // 3
  unsigned short ping_age;    // 5
} ServerDetails;

ServerDetails serverList[PAGE_SIZE];

/* Blip sound frequencies */
unsigned char blipFreq[4] = { 255, 128, 64, 32 };

void wait(unsigned char s)
{
  OS.rtclok[2]=0;
  while (OS.rtclok[2] < s);
}

void pause(void)
{
  cputs("\r\nPress ");
  revers(1);
  cputs("RETURN");
  revers(0);
  cputs(" to continue.");
  cgetc();
}

/**
 * @brief The initial banner
 */
void banner(void)
{
  clrscr();
  printf("#FUJINET GAME LOBBY \n\n");
}

void chatBlip(void)
{
  unsigned char i=0;

  POKEY_WRITE.audc1=0xA8; // Square at volume 8

  for (i=0;i<sizeof(blipFreq);i++)
  {
    POKEY_WRITE.audf1 = blipFreq[i];
    waitvsync();
  }

  POKEY_WRITE.audf1 = 0;
  POKEY_WRITE.audc1 = 0;
}

void connectChat(void)
{
  unsigned char err;
  char login[80];

  if ((err = nopen(SERVER,2)) != SUCCESS)
  {
    nstatus(SERVER);

    printf("Could not connect.\nError: %u\n",OS.dvstat[3]);
    exit(1);
  }

  // Open successful, set up interrupt
  old_vprced  = OS.vprced;     // save the old interrupt vector
  old_enabled = PIA.pactl & 1; // keep track of old interrupt state
  PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
  OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
  PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.

  // Send login
  sprintf(login,"/login @%s\n\n",username);
  nwrite(SERVER,(unsigned char *)login,strlen(login));
  nwrite(SERVER,(unsigned char *)"/join #Lobby\n",13);
}

void chat_clear()
{
  bzero(&OS.savmsc[40*CHAT_Y],128);
}

void chat_send()
{
  memset(tx_buf,0,sizeof(tx_buf));

  chat_clear();
  gotoxy(0,CHAT_Y);
  cputs(">> ");

  cursor(1);

  gets((char *)tx_buf);

  strcat((char *)tx_buf,"\n");

  // Temporary
  nwrite(SERVER,(unsigned char *)"#Lobby ",7);
  nwrite(SERVER,tx_buf,strlen((char *)tx_buf));

  cursor(0);
}

void chatZoomed(void)
{
  unsigned char err;
  char *p;

  cursor(1);
  clrscr();
  cputs("You are now zoomed into chat.\r\nPress any key to send.\r\n");
  revers(1);
  cputs(" SELECT ");
  revers(0);
  cputs(" to return to game list.\r\n\r\n");

  while (!CONSOL_SELECT(GTIA_READ.consol))
    {
      // Keyboard pressed, send something.
      if (kbhit())
      {
        cursor(1);

        cputs(">> ");

        gets((char *)tx_buf);

        strcat((char *)tx_buf,"\n");
        nwrite(SERVER,(unsigned char *)"#Lobby ",7);
        nwrite(SERVER,tx_buf,strlen((char *)tx_buf));
      }

      // We received something, display it.
      if (trip)
      {
        err = nstatus(SERVER);

        if (err==136)
        {
          puts("Chat server disconnected.");
          return; // FIXME: handle better.
        }
        else if (err>1)
        {
          printf("Status error: %u",err);
          return; // FIXME: handle better.
        }
        else
          bw = OS.dvstat[1]*256+OS.dvstat[0];

        if (bw>sizeof(chat_rx_buf))
          bw=sizeof(chat_rx_buf);

        if (bw>0)
        {
          memset(chat_rx_buf,0,sizeof(chat_rx_buf));
          err = nread(SERVER,chat_rx_buf,bw);

          if (err!=1)
          {
            printf("READ ERROR: %u",err);
            return; // FIXME, handle better.
          }
        }

        p = strtok((char *)chat_rx_buf,"\n");

        while (p)
        {
          if (strstr(p,(char *)username) != NULL)
            chatBlip();

          puts((const char *)p);
          p = strtok(NULL,"\n");
          if (p)
            wait(1);
        }

        trip=0;
        PIA.pactl |= 1; // Interrupt serviced, ready again.
      }
    }
}

void chat()
{
  unsigned char err;
  char *p = NULL;

  if (!trip)
    return;

  err = nstatus(SERVER);

  if (err==136)
  {
    chat_clear();
    gotoxy(0,CHAT_Y);
    cputs("Chat server disconnected.");
    return; // FIXME: handle better.
  }
  else if (err>1)
  {
    chat_clear();
    gotoxy(0,CHAT_Y);
    printf("Status error: %u",err);
    return; // FIXME: handle better.
  }
  else
    bw = OS.dvstat[1]*256+OS.dvstat[0];

  if (bw>sizeof(chat_rx_buf))
    bw=sizeof(chat_rx_buf);

  if (bw>0)
  {
    memset(chat_rx_buf,0,sizeof(chat_rx_buf));
    err = nread(SERVER,chat_rx_buf,bw);

    if (err!=1)
    {
      chat_clear();
      gotoxy(0,CHAT_Y);
      printf("READ ERROR: %u",err);
      return; // FIXME, handle better.
    }
  }

  p = strtok((char *)chat_rx_buf,"\n");

  while (p)
  {
    if (strstr(p,(char *)username) != NULL)
      chatBlip();

    chat_clear();
    gotoxy(0,CHAT_Y);
    cputs((const char *)p);
    p = strtok(NULL,"\n");
    if (p)
      wait(1);
  }

  trip=0;
  PIA.pactl |= 1; // Interrupt serviced, ready again.
}

void display_servers(int old_server)
{
  ServerDetails* server;
  unsigned char j,y;
  char * prevGame = NULL;
  y=0;

  for (j=0;j<server_count;j++ )
  {

    server = &serverList[j]; 
  
    if (prevGame == NULL || strcmp(server->game, prevGame) !=0)
    {
      y+=2;
      prevGame = server->game;

      if (old_server<0)
      {
        cputsxy(0,y,prevGame);
        if (j>0) 
        {
           cclear(39-strlen(prevGame));
        }
        else 
        {
          cclear(33-strlen(prevGame));
          cputs("PLAYERS");
        }
      }

    }
    y++;
        // If just moving the selection, only redraw the old and new server
    // Also temp guard for servers until paging is implemented
    if (y>PAGE_SIZE || (old_server>=0 && j != old_server && j != selected_server))
      continue;

    // Show the selected server in reverse
    // Printing full space to overwrite the existing server, a bit convoluted but
    // prevents flickering. TODO: Use PMG like the config screen.
    revers(j == selected_server ? 1 : 0);
    cputcxy(0,y,' ');
    cputs(server->server);    

    sprintf((char *)buf, "%i/%i ", server->players, server->max_players);
    cclear(39-strlen(server->server)-strlen((char *)buf));
    cputs((char *)buf);

     // Reset reverse
     if (j == selected_server)
        revers(0);
  }

 
  //gotoxy(0,19);

  if (skip_server_instructions)
    return;

  cclearxy(0,20,SCREEN_WIDTH*4);
  /* gotoxy(0,19); */
  /* printf("________________________________________"); */
  gotoxy(0,21);
  if (server_count>0)
  {
    //printf("Select a server, ");
    //revers(1); cputs("OPTION"); revers(0);
    //printf(" to boot client\n\n");

    printf("Pick a server, hold ");
    revers(1); cputs("OPTION"); revers(0);
    printf(" to boot game\n");
  }
  revers(1); cputs("R"); revers(0);
  printf("efresh list - ");
  revers(1); cputs("C"); revers(0);
  printf("hange your name\n");
  revers(1); cputs("S"); revers(0);
  printf("hout ");
  revers(1); cputs("Z"); revers(0);
  printf("oom into chat");

  skip_server_instructions = true;
}

void refresh_servers()
{
  int data_len;
  char *key, *value;
  signed char i;
  bool lobby_error = false;

  skip_server_instructions = false;
  server_count = 0;

  cursor(0);

  if(
    (njsonparse(LOBBY_ENDPOINT, 2)) != SUCCESS ||
    (njsonquery(LOBBY_ENDPOINT, "N:\x9b")) != SUCCESS ||
    nstatus(LOBBY_ENDPOINT) > 128 ||
    (data_len = (OS.dvstat[1] << 8) + OS.dvstat[0]) == 0
  )
  {
    lobby_error = true;
  }
  else
  {
    if (data_len>sizeof(rx_buf))
        data_len=sizeof(rx_buf);

    if (nread(LOBBY_ENDPOINT, rx_buf, data_len) != 1)
    {
      lobby_error = true;
    }
    else
    {

      key = strtok((char *)rx_buf, "\n");
      i=-1;
      while( key != NULL)
      {
        value = strtok(NULL, "\n");

        switch (key[0])
        {
          case 'g':
                    // Assume "g" is the first property of a new server
                    // Only read up to the page size to avoid buffer overrun
                    

                    // If previous server is offline, ignore it
                    if (i>0 && serverList[i].online == 0)
                    {
                      i--;
                    }

                    if (i >= PAGE_SIZE)
                      break;

                    i++;

                    serverList[i].game = strupper(value); break;

          case 't': serverList[i].game_type = atoi(value); break;
          case 's': serverList[i].server = value; break;
          case 'u': serverList[i].url = value; break;
          case 'c': serverList[i].client_url = value; break;
          case 'r': serverList[i].region = value; break;
          case 'o': serverList[i].online = atoi(value); break;
          case 'p': serverList[i].players = atoi(value); break;
          case 'm': serverList[i].max_players = atoi(value); break;
          case 'a': serverList[i].ping_age = atoi(value); break;
        }

        key = strtok(NULL, "\n");
      }

      // If last server is offline, ignore it              
      if (i>0 && serverList[i].online == 0) {
        i--;
      }
      server_count = i+1;
      nclose(LOBBY_ENDPOINT);
    }
  }

  banner();
  cputsxy(40-strlen((char *)username),0, (char *)username);

  if (server_count>0)
  {
    if (selected_server >= server_count)
    {
      selected_server = server_count-1;
    }
  }
  else
  {
    if (lobby_error)
    {
      nstatus(LOBBY_ENDPOINT);
      printf("\nCould not query Lobby!\nError: %u\n",OS.dvstat[3]);
    }
    else
    {
      printf("\nNo servers are online at the moment.");
    }
  }

  display_servers(-1);
}


/**
 * @brief Get username and set key
 */
void get_username(bool clearUsername)
{

  if (clearUsername)
    memset(username,0,sizeof(username));

  while (strlen((char *)username)<1 || strlen((char *)username)>10)
  {
    printf("\nEnter a user name and press RETURN\n");

    // TODO - Discuss imposing limitations like Alpha Numeric only, so 8-Bit clients only have to
    // support rendering a minimum # of characters, since character sets are limited.
    // printf("Letters and numbers are acceptable.\n");

    if (strlen((char *)username)>10)
      printf("It must be 10 characters or less.\n");

    // TODO - Restrict input to AlphaNumeric only
    printf(">");

    gets((char *)username);
  }

  cursor(0);

  sio_writekey(CREATOR_ID,APP_ID,KEY_ID, username);

}


/**
 * @brief Set user name, either from appkey or via input
 */
void register_user(void)
{
  sio_readkey(CREATOR_ID,APP_ID,KEY_ID,username);
  get_username(false);
  printf("Welcome, %s.\n",username);
}


/**
 * @brief Mount the selected server's client and reboot
*/
void mount()
{
  char *client_path, *host, *filename;
  int i, host_slot;

  cclearxy(0,20,SCREEN_WIDTH*4);
  gotoxy(0,20);

  // Sanity check 1 - a server was selected
  if (server_count==0 || selected_server >= server_count)
  {
    return;
  }

  // Sanity check 2 - the game type is greater than 0
  if (serverList[selected_server].game_type == 0)
  {
    printf("ERROR: Invalid client game type. Inform the owner of the server.");
    skip_server_instructions = false;
    return;
  }

  // Offline warning
  /* Removing warning for now
    if (!skip_offline_check && serverList[selected_server].online != 1) {
    printf("\nThis server is reportedly offline!\n\nPress ");
    revers(1);cputs("OPTION");revers(0);
    cputs(" again to try anyway.");
    skip_offline_check = true;
    skip_server_instructions = false;
    return;
  }*/


  // Remove the protocol for now, assume TNFS://
  if (client_path = strstr(serverList[selected_server].client_url, "://"))
    client_path+=3;
  else
    client_path = serverList[selected_server].client_url;

  printf("Mounting:\n%s\n", client_path);


  // Get the host and filename
  if (filename = strstr(client_path,"/"))
  {
    filename+=1;
  }

  // Get the host
  host = strtok(client_path,"/");

  if (filename == NULL && host == NULL)
  {
    printf("ERROR: Invalid client file");
    pause();
    refresh_servers();
    return;
  }

  // Read current list of hosts from FujiNet
  host_read((char *)host_slots);

  // Pick the host slot to use. Default to the last, but choose an existing slot
  // if it already has the same host
  host_slot = 7;
  for(i=0;i<8;i++)
  {
    if (strcasecmp(host, host_slots[i]) == 0)
    {
      host_slot = i;
      break;
    }
  }

  // Update the host slot 7 if needed
  if (host_slot==7)
  {
    strcpy(host_slots[7], host);
    host_write((char *)host_slots);
  }

  // Mount host slot (test connectivity)
  host_mount(host_slot);
  if (OS.dcb.dstats != 1)
  {
    host_unmount(host_slot);
    printf("ERROR %i: Unable to connect to host", OS.dcb.dstats);

    // Reset the fujinet to abort the mounting process
    fujinet_reset();
    OS.rtclok[1]=OS.rtclok[2]=0;
    pause();

    // Make sure at least 5 seconds has passed before continuing
    while (OS.rtclok[1]<1 && OS.rtclok[2] < 50);
    refresh_servers();
    return;
  }

  // Point device slot 0 to the host_slot
  disk_set_host_slot(0, host_slot, FUJI_DEVICE_MODE_READ);

  // Set and mount the filename in device slot 0
  set_filename(0, host_slot, 0, filename);
  disk_mount(0, FUJI_DEVICE_MODE_READ);

  // Set the server url in this game type's app key:
  sio_writekey(CREATOR_ID,APP_ID,serverList[selected_server].game_type, (unsigned char *)serverList[selected_server].url);

  // Cold boot the computer after a second
  wait(1);
  asm("JMP $E477");
}

void change_selection(signed char delta)
{
    int old_server = selected_server;

    if (delta < 0 && selected_server == 0)
      selected_server = server_count - 1;
    else
      selected_server = (selected_server + delta) % server_count;

    display_servers(old_server);

    skip_offline_check = false;
}

void event_loop()
{
  signed char selection_change;
  while (true)
  {
    selection_change = CONSOL_SELECT(GTIA_READ.consol) ? 1 : 0;

    chat();

    if (kbhit())
    {
      switch (cgetc())
      {
        case 'c':
        case 'C':
          cursor(1);
          clrscr();
          printf("Your current username is: %s\n", username);
          get_username(true);
        case 'r':
        case 'R':
          refresh_servers();
          break;
        case 0x1C:
        case '-':
          selection_change = -1;
          break;
        case 0x1D:
        case '=':
          selection_change = 1;
          break;
        case 'S':
        case 's':
          cursor(1);
          chat_send();
          break;
        case 'Z':
        case 'z':
          bar_clear();
          chatZoomed();
          bar_show(CHAT_Y);
          refresh_servers();
          break;
      }
    }

    // Arrow keys select the server
    if ( selection_change != 0 )
    {
      change_selection(selection_change);
    }

    // Pressing Option mounts the client for the server
    if (CONSOL_OPTION(GTIA_READ.consol))
    {
      mount();
       // Wait until OPTION is released
      while (CONSOL_OPTION(GTIA_READ.consol));
    }

  }
}

void main(void)
{
  OS.soundr=0; // Silent noisy SIO
  cursor(1);
  bordercolor(0x90);
  bgcolor(0x90);

  banner();

  register_user();
  printf("\nConnecting..\n");

  connectChat();

  refresh_servers();

  bar_setup_regs();
  bar_clear();
  bar_show(CHAT_Y);
  chat_clear();

  event_loop();

  // term(); TODO: Integrate chatting into lobby
}

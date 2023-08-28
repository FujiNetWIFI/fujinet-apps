
/**
 * @brief   Lobby program for #FujiNet
 * @author  Thomas Cherryhomes, Eric Carr
 * @email   thom dot cherryhomes at gmail dot com,  eric dot carr at gmail dot com
 * @license gpl v. 3
 */


/**
 * PLEASE NOTE -
 * Progress is still very early and each platform is in various states of working condition.
 * This does not yet contain the recently added chat functionality. Eric
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include <joystick.h>

#include "fuji.h"
#include "io.h"
#include "appkey.h"

// Include platform specific keymaps before the input include
#include "atari/keymap.h"
#include "c64/keymap.h"
#include "apple2/keymap.h"

#include "input.h"

#define CREATOR_ID 0x0001 /* FUJINET  */
#define APP_ID     0x01   /* LOBBY    */
#define KEY_ID     0x00   /* USERNAME */

#define LOBBY_ENDPOINT "http://fujinet.online:8080/view"

// Initial chat code commented out - need to fold back in the latest chat functionality
#define CHAT_SERVER     "N:TCP://IRATA.ONLINE:1512/" 

// Common defines. Undef and redefine for specific platform below
#define PAGE_SIZE  6
#define PAGE_SIZE_STR  "6"
#define SCREEN_WIDTH 40
#define BOOT_KEY "RETURN"

// Set the platform send to the lobby server based on the platform specified in the makefile

#ifdef __ATARI__
  #define PLATFORM "atari"
  #undef BOOT_KEY
  #define BOOT_KEY "OPTION"
  #define BACKGROUND_COLOR 0x90
  #define FOREGROUND_COLOR 0xff
#endif

#ifdef __C64__
  #define PLATFORM "atari"
  #define BACKGROUND_COLOR 6
  #define FOREGROUND_COLOR 1
#endif

#ifdef __APPLE2__
  #define PLATFORM "apple2"
#endif

#ifdef __VIC20__
  #undef SCREEN_WIDTH
  #define SCREEN_WIDTH 22
#endif


#define FUJI_HOST_SLOT_COUNT 8
#define FUJI_HOST_SLOT_NAME_LENGTH 32

// gotoxy + cput* saves 4 bytes over cput*xy, so why not optimize?
#define cputsxy(x,y,s) gotoxy(x,y); cputs(s);
#define cputcxy(x,y,c) gotoxy(x,y); cputc(c);
  
unsigned char username[64];
void* old_vprced;               // old PROCEED vector, restored on exit.
bool old_enabled=false;         // were interrupts enabled for old vector
bool running=false;
unsigned short bw=0;            // # of bytes waiting.
unsigned char rx_buf[2048];     // RX buffer.
unsigned char tx_buf[128];      // TX buffer.
unsigned char txbuflen;         // TX buffer length
unsigned char trip=0;           // if trip=1, fujinet is asking us for attention.
unsigned char buf[128];         // Temporary buffer use
unsigned char page=1;           // Default to first page of Lobby results
//extern void ih();               // defined in intr.s

bool skip_offline_check = false;        // Do not check if server is offline before mounting the client
bool skip_server_instructions = false;  // Do not show mount instructions

unsigned char server_count = 0;       // Number of servers available
unsigned char selected_server = 0;    // Currently selected server

const char error_138[]="FUJINET NOT RESPONDING\x9B";
const char error_139[]="FUJINET NAK\x9b";
const char error[]="SIO ERROR\x9b"; 
 
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

void pause(void) { 
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
void banner(void) {
  clrscr();
  cputs("#FUJINET GAME LOBBY \r\n");
}

// void term(void)
// {
//   unsigned char err;
//   unsigned char i;
//   running = true;
  
//   while (running==true)
//   {
//     if (kbhit())
//       {
// 	memset(tx_buf,0,sizeof(tx_buf));
// 	cputs(">> ");
// 	cursor(1);
// 	gets((char *)tx_buf);
// 	if (strlen((char *)tx_buf))
// 	  {
// 	    nwrite(SERVER,tx_buf,strlen((char *)tx_buf));
// 	    nwrite(SERVER,"\x9b",1);
// 	  } 
//     } 
    
//     if (trip==0) // is nothing waiting for us?
//       continue;

//     // Something waiting for us, get status and bytes waiting.
//     err=nstatus(SERVER);

//     if (err==136)
//     {
//       cputs("DISCONNECTED.\x9b");
//       running=false;
//       continue;
//     }
//     else if (err!=1)
//     {
//       cprintf("STATUS ERROR: %u\r\n",err);
//       running=false;
//       continue;
//     }

//     // Get # of bytes waiting, no more than size of rx_buf
//     bw=OS.dvstat[1]*256+OS.dvstat[0];

//     if (bw>sizeof(rx_buf))
//       bw=sizeof(rx_buf);
    
//     if (bw>0)
//     {
//       err=nread(SERVER,rx_buf,bw);

//       if (err!=1)
//         {
//           cprintf("READ ERROR: %u\r\n",err);
//           running=false;
//           continue;
//         }

//       // Print the buffer to screen.
//       for (i=0;i<bw;i++)
// 	      putchar(rx_buf[i]);
      
//       trip=0;
//       PIA.pactl |= 1; // Flag interrupt as serviced, ready for next one.
//     } // if bw > 0
//   } // while running
// }

// void connectChat(void)
// {
//   unsigned char err;
//   char login[80];
  
//   if ((err = nopen(SERVER,2)) != SUCCESS)
//     {
//       nstatus(SERVER);

//       cprintf("Could not connect.\r\nError: %u\r\n",OS.dvstat[3]);
//       exit(1);
//     }

//   // Open successful, set up interrupt
//   old_vprced  = OS.vprced;     // save the old interrupt vector 
//   old_enabled = PIA.pactl & 1; // keep track of old interrupt state
//   PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
//   OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
//   PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.
  
//   // Send login
//   sprintf(login,"/login %s\r\n\r\n",username);
//   nwrite(SERVER,(unsigned char *)login,strlen(login));
// }


void display_servers(int old_server) {
  ServerDetails* server;
  unsigned char j,y;

  for (j=0;j<server_count;j++ ) {
    // If just moving the selection, only redraw the old and new server
    if (old_server>=0 && j != old_server && j != selected_server)
      continue;

    server = &serverList[j];
    y = 3*j+2;

    // Show the selected server in reverse
    // Printing full space to overwrite the existing server, a bit convoluted but
    // prevents flickering. TODO: Use PMG like the config screen.
    revers(j == selected_server ? 1 : 0);

    cputcxy(0,y,' ');
    cputs(server->game);
    
    cclear(server->online + SCREEN_WIDTH-strlen(server->game)-9);
    cputs( server->online == 1 ? "ONLINE " : "OFFLINE ");

    cputcxy(0,y+1,' ');
    cputs(server->server);

    if (server->online == 1) {
      // 12756
      itoa(server->players, buf, 10);
      strcat(buf, "/");
      itoa(server->max_players, buf+strlen(buf), 10);
      
      //sprintf(buf, "%i/%i ", server->players, server->max_players);
      cclear(39-strlen(server->server)-strlen(buf));
      cputs(buf);
    } else {
      cclear(39-strlen(server->server));
    }
  }
  
  // Reset cursor and reverse
  revers(0);

  if (skip_server_instructions)
    return;

  cclearxy(0,20,SCREEN_WIDTH*4);
  gotoxy(0,19);
  cputs("________________________________________");
  gotoxy(0,21);
  if (server_count>0) {
    cputs("Pick a server, press ");
    revers(1); cputs(BOOT_KEY); revers(0);
    cputs(" to boot game\r\n");
  }

  revers(1); cputs("R"); revers(0);
  cputs("efresh list - ");
  revers(1); cputs("C"); revers(0);
  cputs("hange your name");
  
  skip_server_instructions = true;
}

void refresh_servers() { 
  char *key, *value;
  signed char i;
  unsigned char lobby_error;
  skip_server_instructions = false;
  server_count = 0;  

  cursor(0);
  strcpy(buf, LOBBY_ENDPOINT "?platform=" PLATFORM "&pagesize=" PAGE_SIZE_STR "&page=");
  itoa(page, buf+strlen(buf), 10);

  if ((lobby_error = getJsonResponse(buf, rx_buf, sizeof(rx_buf))) == 0) {
      i=-1;
      key = strtok(rx_buf, "\r\n");
      while( key != NULL) {
          value = strtok(NULL, "\r\n");

          switch (key[0]) {
            
            case 'g': 
              // Assume "g" is the first property of a new server  
              // Only read up to the page size to avoid buffer overrun

              if (i+1 >= PAGE_SIZE)
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

          key = strtok(NULL, "\r\n");
      }
      server_count = i+1;

    
  } 
  
  banner();
  cputsxy(40-strlen(username),0, username);

  if (server_count>0) {
    if (selected_server >= server_count) {
      selected_server = server_count-1;
    }
  } else {
    if (lobby_error) {
      cputs("\r\nCould not query Lobby!\r\nError: ");
      itoa(lobby_error, buf, 10);
      cputs(buf);
      cputs("\r\n");
      
    } else {
      cputs("\r\nNo servers are online at the moment.");
    }
  }

  display_servers(-1);

}


/**
 * @brief Get username and set key
 */
void get_username(bool clearUsername) {

  if (clearUsername)
    memset(username,0,sizeof(username));
  
  while (strlen(username)<1 || strlen(username)>10) {
    cputs("\r\nEnter a user name and press RETURN\r\n");

    // TODO - Discuss imposing limitations like Alpha Numeric only, so 8-Bit clients only have to
    // support rendering a minimum # of characters, since character sets are limited.
    // cputs("Letters and numbers are acceptable.\r\n");     

    if (strlen(username)>10)
      cputs("It must be 10 characters or less.\r\n");

    // TODO - Restrict input to AlphaNumeric only
    cputs(">");
    
    gets((char *)username);
  }

  cursor(0);
  write_appkey(CREATOR_ID,APP_ID,KEY_ID, username);  
}


/**
 * @brief Set user name, either from appkey or via input
 */
void register_user(void) {  
  read_appkey(CREATOR_ID,APP_ID,KEY_ID,username);
  get_username(false);
  cputs("Welcome, ");
  cputs(username);
  cputs("\r\n");
}


/**
 * @brief Mount the selected server's client and reboot
*/
void mount() {
  char *client_path, *host, *filename; 
  unsigned char i, slot;

  cclearxy(0,20,SCREEN_WIDTH*4);
  gotoxy(0,20);

  // Sanity check 1 - a server was selected
  if (server_count==0 || selected_server >= server_count) {
    return;
  } 

  // Sanity check 2 - the game type is greater than 0
  if (serverList[selected_server].game_type == 0) {
    cputs("ERROR: Invalid client game type. Inform the owner of the server.");
    skip_server_instructions = false;
    return;
  } 

  // Remove the protocol for now, assume TNFS://
  if (client_path = strstr(serverList[selected_server].client_url, "://"))
    client_path+=3;
  else
    client_path = serverList[selected_server].client_url;

  // Get the host and filename
  if (filename = strstr(client_path,"/")) {
    filename+=1;
  }

  // Get the host
  host = strtok(client_path,"/");

  if (filename == NULL && host == NULL) {
    cputs("ERROR: Invalid client file");
    pause();
    refresh_servers();
    return;
  }

  // Read current list of hosts from FujiNet
  io_get_host_slots(hostSlots);

  // Pick the host slot to use. Default to the last, but choose an existing slot
  // if it already has the same host
  slot = 99;
  for(i=0;i<8;i++) {
    if (strcasecmp(host, hostSlots[i]) == 0) {
      slot = i;
      break;
    }
  }
  // Update the host slot 7 if needed
  if (slot == 99) {
    slot=7;
    strcpy(hostSlots[7], host);
    io_put_host_slots(hostSlots);
  }

  // Mount host slot
  io_mount_host_slot(slot);

  // Point device slot 0 to the host_slot
  io_get_device_slots(deviceSlots);
  deviceSlots[0].hostSlot = slot;
  deviceSlots[0].mode = FUJI_DEVICE_MODE_READ;
  io_put_device_slots(deviceSlots);

  // Set and mount the filename in device slot 0
  io_set_device_filename(0, filename);
  
  io_mount_disk_image(0, FUJI_DEVICE_MODE_READ);

  // For some reason, it's booting to the config instead of the disk image, unless mount_all is
  io_mount_all();

  // Set the server url in this game type's app key:
  write_appkey(CREATOR_ID,APP_ID,serverList[selected_server].game_type, serverList[selected_server].url);  

  io_boot();
}

void change_selection(signed char delta) {
    int old_server = selected_server;

    if (delta < 0 && selected_server == 0)
      selected_server = server_count - 1;
    else
      selected_server = (selected_server + delta) % server_count;

    display_servers(old_server);

    skip_offline_check = false;
}
 
void event_loop() {
  signed char selection_change;
  unsigned char input;
  while (true) { 
 //   selection_change = CONSOL_SELECT(GTIA_READ.consol) ? 1 : 0;
    selection_change = 0;

    input = readCommonInput();
    
    switch (input) {
      // Arrow keys/joystick to move
      case JOY_UP_MASK:
      case JOY_LEFT_MASK:
        selection_change = -1;
        break;
      case JOY_DOWN_MASK:
      case JOY_RIGHT_MASK:
        selection_change = 1;
        break;
      case JOY_BTN_1_MASK:
        // Fire (enter, space, option) triggers a mount
        mount();
        break;
      case 'c':
      case 'C':
        cursor(1);
        clrscr();
        cputs("Your current username is: ");
        cputs(username);
        cputs("\r\n");
        get_username(true);
      case 'r':
      case 'R':
        refresh_servers();
        break;
     
    }
  
    // Arrow keys select the server
    if ( selection_change != 0 ) {
      change_selection(selection_change);
    } 
    
  }
}

void main(void)
{
  initialize();
  cursor(1);
  
  #ifdef BACKGROUND_COLOR
  bordercolor(BACKGROUND_COLOR);
  bgcolor(BACKGROUND_COLOR);
  #endif

  #ifdef FOREGROUND_COLOR
  textcolor(FOREGROUND_COLOR);
  #endif

  banner();
  
  register_user();
  cputs("\r\nConnecting..\r\n");

  refresh_servers();
  event_loop();

  // term(); TODO: Integrate chatting into lobby
}

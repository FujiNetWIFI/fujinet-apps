
/**
 * @brief   Lobby program for #FujiNet
 * @author  Thomas Cherryhomes, Eric Carr
 * @email   thom dot cherryhomes at gmail dot com,  eric dot carr at gmail dot com
 * @license gpl v. 3
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>

#include "fujinet-fuji.h"
#include "fujinet-network.h"

#include "platform.h"
#include "io.h"

#define CREATOR_ID 0x0001 /* FUJINET  */
#define APP_ID     0x01   /* LOBBY    */
#define KEY_ID     0x00   /* USERNAME */

#define LOBBY_ENDPOINT "N:http://fujinet.online:8080/view"
//#define LOBBY_ENDPOINT "N:http://localhost:8081/view"

// Common defines. Undef and redefine for specific platform below
#define PAGE_SIZE  12
#define PAGE_SIZE_STR  "12"
#define SCREEN_WIDTH 40
#define BOTTOM_PANEL_ROWS 3
#define PANEL_SPACER "--------"
#define FUJI_HOST_SLOT_COUNT 8
#define FUJI_DEVICE_SLOT_COUNT 8
#define BOTTOM_PANEL_Y (screen_height-BOTTOM_PANEL_ROWS)
#define BOTTOM_PANEL_LEN SCREEN_WIDTH*BOTTOM_PANEL_ROWS

HostSlot host_slots[FUJI_HOST_SLOT_COUNT];
DeviceSlot device_slots[FUJI_DEVICE_SLOT_COUNT];

// Set the platform send to the lobby server based on the platform specified in the makefile

#ifdef __ATARI__
  #define PLATFORM "atari"
  #define ACTION_VERB " & hold"
  #define BOOT_KEY "OPTION"
  #define BACKGROUND_COLOR 0x90
  #define FOREGROUND_COLOR 0xff
#endif

#ifdef __APPLE2__
  #define PLATFORM "apple2"
  #define ACTION_VERB ", press"
  #define BOOT_KEY "RETURN"
#endif

#ifdef _CMOC_VERSION_
#define ACTION_VERB ", press"
#define BOOT_KEY "ENTER"
#define PLATFORM "coco"
#undef SCREEN_WIDTH
#define SCREEN_WIDTH 32
//char panel_spacer_string[] = {0x83,0x93,0xA3,0xB3,0xC3,0xD3,0xE3,0xF3,0};
char panel_spacer_string[] = {0xA4,0xE4,0xE4,0xB4,0xB4,0xE4,0xE4,0xA4,0};
#define PANEL_SPACER panel_spacer_string
#undef BOTTOM_PANEL_ROWS
#define BOTTOM_PANEL_ROWS 2

#endif

#ifdef __C64__
  #define PLATFORM "c64"
  #define BACKGROUND_COLOR 6
  #define FOREGROUND_COLOR 1
  #define ACTION_VERB ", press"
  #define BOOT_KEY "RETURN"

#endif

#ifdef __VIC20__
  #define PLATFORM "vic20"
  #undef SCREEN_WIDTH
  #define SCREEN_WIDTH 22
#endif


// gotoxy + c* saves a little space
#define cputsxy(x,y,s) gotoxy(x,y); cputs(s);
#define cputcxy(x,y,c) gotoxy(x,y); cputc(c);
#define cclearxy(x,y,c) gotoxy(x,y); cclear(c);

//char rx_buf[2048];  
char username[66];

char buf[128];         // Temporary buffer use
uint8_t page=0;        // Default to first page of Lobby results

unsigned char selected_server = 0;    // Currently selected server

uint8_t screen_height;

typedef struct { // 189 bytes
  uint8_t game_type;   
  char game[17];       
  char server[33];     
  char url[65];        
  char client_url[65]; 
  char region[3];      
  uint8_t online;         
  uint8_t players;     
  uint8_t max_players; 
  uint16_t ping_age;   
} ServerDetails;

typedef struct {
  uint8_t server_count;
  uint8_t current_page;
  bool more_pages;
  ServerDetails servers[PAGE_SIZE];
} LobbyResponse;

LobbyResponse lobby;

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
  uint8_t j;
  clrscr();
  cputs("#FUJINET GAME LOBBY");
  gotoxy(0,1);
  for(j=0;j<SCREEN_WIDTH/8;j++)
    cputs(PANEL_SPACER);
  
}


void display_servers(int old_server) {
  ServerDetails* server;
  unsigned char j,y;
  char * prevGame = NULL;
  y=0;

  for (j=0;j<lobby.server_count;j++ )
  {

    server = &lobby.servers[j]; 
  
    if (prevGame == NULL || strcmp(server->game, prevGame) !=0)
    {
      y+=2;
      prevGame = server->game;

      if (old_server<0)
      {
        cputsxy(0,y,prevGame);
        if (j>0) 
        {
           cclear(SCREEN_WIDTH-1-strlen(prevGame));
        }
        else 
        {
          cclear(SCREEN_WIDTH-7-strlen(prevGame));
          cputs("PLAYERS");
        }
      }

    }
    y++;
    // If just moving the selection, only redraw the old and new server
    // Also temp guard for servers until paging is implemented
    if (j>PAGE_SIZE || (old_server>=0 && j != old_server && j != selected_server))
      continue;

    // Show the selected server in reverse
    // Printing full space to overwrite the existing server, a bit convoluted but
    // prevents flickering.
    revers(j == selected_server ? 1 : 0);
    cputcxy(0,y,' ');
    cputs(server->server);    

    itoa(server->players, buf, 10);
    strcat(buf, "/");
    itoa(server->max_players, buf+strlen(buf), 10);
    
    cclear(SCREEN_WIDTH-1-strlen(server->server)-strlen(buf));
    cputs((char *)buf);
    
     // Reset reverse
     if (j == selected_server)
      revers(0);
  }
  
  // Reset cursor and reverse
  revers(0);

  if (old_server>=0)
    return;

  cclearxy(0,BOTTOM_PANEL_Y,BOTTOM_PANEL_LEN);
  gotoxy(0,BOTTOM_PANEL_Y-1);
  for(j=0;j<SCREEN_WIDTH/8;j++)
    cputs(PANEL_SPACER);
  
  if (lobby.server_count>0)
  {
    gotoxy(0,BOTTOM_PANEL_Y);
    cputs("Select game" ACTION_VERB " ");
    revers(1); cputs(BOOT_KEY); revers(0);
    cputs(" to boot\r\n");
  }

  gotoxy(0,screen_height-1);
  revers(1); cputs("R"); revers(0);
  cputs("efresh list   ");
  gotoxy(SCREEN_WIDTH-16,screen_height-1);
  revers(1); cputs("C"); revers(0);
  cputs("hange your name");
}

void refresh_servers(bool clearScreen) { 
  int16_t api_read_result;
  uint8_t i;
  
  cclearxy(0,BOTTOM_PANEL_Y,BOTTOM_PANEL_LEN);
  cputsxy(SCREEN_WIDTH/2-12,BOTTOM_PANEL_Y+1,"Refreshing Server List..");

  strcpy(buf, LOBBY_ENDPOINT "?bin=1&platform=" PLATFORM "&pagesize=" PAGE_SIZE_STR "&page=");
  itoa(page, buf+strlen(buf), 10);

  #ifndef _CMOC_VERSION_ // remove ifndef to enable fujinet-lib in coco
  network_open(buf, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  api_read_result = network_read(buf, (uint8_t*)&lobby, sizeof(lobby));
  network_close(buf);
  #else
  // mock result for coco - to test UI
  api_read_result = 321;
  lobby.server_count = 6;
  lobby.current_page=0;
  for (i=0;i<3;i++) {
    lobby.servers[i].online=1;
    lobby.servers[i].game_type=1;
    strcpy(lobby.servers[i].game, "5 card Stud");
    sprintf(lobby.servers[i].server,"Bot Table %d", i+1);
    strcpy(lobby.servers[i].client_url,"ec.tnfs.io/coco/fcs.dsk");
    lobby.servers[i].players=0;
    lobby.servers[i].max_players=4;
  }

  for (i=3;i<6;i++) {
    lobby.servers[i].online=1;
    lobby.servers[i].game_type=2;
    strcpy(lobby.servers[i].game, "Fujitzee");
    sprintf(lobby.servers[i].server,"AI game %d", i-2);
    strcpy(lobby.servers[i].client_url,"ec.tnfs.io/coco/fujitzee.dsk");
    lobby.servers[i].players=0;
    lobby.servers[i].max_players=6;
  }
  #endif


  if (clearScreen)
    banner();

  cputsxy(SCREEN_WIDTH-strlen(username),0, username);

  if (api_read_result<0) {
    cputs("\r\nCould not query Lobby!\r\nError: ");
    itoa(api_read_result, buf, 10);
    cputs(buf);
  } else if (api_read_result < sizeof(ServerDetails) || lobby.server_count == 0 || lobby.server_count >= PAGE_SIZE) {
    cputs("\r\nNo servers are online.");
    lobby.server_count = 0;
  } else {
    if (selected_server >= lobby.server_count) {
      selected_server = lobby.server_count-1;
    }
  }

  display_servers(-1);
}



/**
 * @brief Get username and set key
 */
void get_username() {

  cputsxy(0,3,"Enter your username:\r\n\r\n[         ]");
  
  while (1) {
    inputField(1,5,8,username);
    if (strlen(username)>=2 && strlen(username)<=8)
      return;
  }

  write_appkey(CREATOR_ID,APP_ID,KEY_ID,strlen(username), username);  
}


/**
 * @brief Set user name, either from appkey or via input
 */
void register_user(void) {  
  read_appkey(CREATOR_ID,APP_ID,KEY_ID,username);
  
  if (strlen(username)==0)
    get_username();

  cputs("\r\n\r\nWelcome, ");
  cputs(username);
  cputs(".");
}


/**
 * @brief Mount the selected server's client and reboot
*/
void mount() {
  static char *client_path, *host, *filename; 
  static uint8_t i, slot;

  // // Sanity check - a valid server index was selected, and the game type is > 0
  if (lobby.server_count==0 || selected_server >= lobby.server_count || lobby.servers[selected_server].game_type == 0) {
    return;
  } 

  // // Remove the protocol for now, assume TNFS://
  if (client_path = strstr(lobby.servers[selected_server].client_url, "://"))
    client_path+=3;
  else
    client_path = lobby.servers[selected_server].client_url;

  if (strlen(client_path)>SCREEN_WIDTH) {
    strcpy(buf,client_path);
    buf[SCREEN_WIDTH/2-1]=buf[SCREEN_WIDTH/2]='.';
    strcpy(buf+SCREEN_WIDTH/2+1,client_path+strlen(client_path)-SCREEN_WIDTH/2+1);
    client_path = buf;
  }
  cclearxy(0,BOTTOM_PANEL_Y,BOTTOM_PANEL_LEN);

  cputsxy(0,BOTTOM_PANEL_Y, "Mounting\r\n");
  cputs(client_path);

  // // Get the host and filename
  if (filename = strstr(client_path,"/")) {
    filename+=1;
  }

  // // Get the host
  host = strtok(client_path,"/");

  if (filename == NULL || host == NULL) {
    cclearxy(0,BOTTOM_PANEL_Y,BOTTOM_PANEL_LEN);
    cputsxy(0,BOTTOM_PANEL_Y,"ERROR: Invalid client file");
    pause();
    refresh_servers(false);
    return;
  }

  // Read current list of hosts from FujiNet
  fuji_get_host_slots((unsigned char*) host_slots, FUJI_HOST_SLOT_COUNT);

  // Pick the host slot to use. Default to the last, but choose an existing slot
  // if it already has the same host
  slot = FUJI_HOST_SLOT_COUNT;
  for(i=0;i<FUJI_HOST_SLOT_COUNT;i++) {
    if (stricmp(host, (char*)host_slots[i]) == 0) {
      slot = i;
      break;
    }
  }

  // Update the bottom host slot if needed
  if (slot == FUJI_HOST_SLOT_COUNT) {
    slot=FUJI_HOST_SLOT_COUNT-1;
    strcpy((char*)host_slots[slot], host);
    fuji_put_host_slots((unsigned char*) host_slots, FUJI_HOST_SLOT_COUNT);
  }

  // Mount the file to the device/host slot
  fuji_mount_host_slot(slot);

  // Mount the file to device slot 0
  device_slots[0].hostSlot = slot;
  device_slots[0].mode = 0;
  strcpy((char*)device_slots[0].file, filename);
  fuji_put_device_slots(device_slots, FUJI_DEVICE_SLOT_COUNT);
  fuji_set_device_filename(0, slot, 0, filename);
  fuji_mount_disk_image(0, 1);

  // // Set the server url in this game type's app key:
  write_appkey(CREATOR_ID,APP_ID,lobby.servers[selected_server].game_type, strlen(lobby.servers[selected_server].url), lobby.servers[selected_server].url);  
  
  reboot();
}

void change_selection(int8_t delta) {
    int old_server = selected_server;

    if (delta < 0 && selected_server == 0)
      selected_server = lobby.server_count - 1;
    else
      selected_server = (selected_server + delta) % lobby.server_count;

    display_servers(old_server);
}
 
void event_loop() {
  while (true) { 
    readCommonInput();
    
    switch (input.key) {
      case 'c':
      case 'C':
        banner();
        get_username();
        refresh_servers(true);
        break;
      case 'r':
      case 'R':
        refresh_servers(false);
        break;
     
    }
    
    waitvsync();
    // Arrow keys select the server
    if ( input.dirY ) {
      change_selection(input.dirY);
    } 
  
  // ifdef hack for Atari, for now
  #ifdef __ATARI__
    // Pressing Option mounts the client for the server
    if (CONSOL_OPTION(GTIA_READ.consol))
    {
      mount();
       // Wait until OPTION is released
      while (CONSOL_OPTION(GTIA_READ.consol));
    }
  #else
    if (input.trigger) {
      mount();
    }    
  #endif
    
  }
}

void main(void)
{
  uint8_t i;

  initialize();
  
  screensize(&i, &screen_height);

  #ifdef BACKGROUND_COLOR
  bordercolor(BACKGROUND_COLOR);
  bgcolor(BACKGROUND_COLOR);
  #endif

  #ifdef FOREGROUND_COLOR
  textcolor(FOREGROUND_COLOR);
  #endif

  banner();
  
  register_user();
  for(i=0;i<30;i++) waitvsync();

  refresh_servers(true);
  event_loop();
}

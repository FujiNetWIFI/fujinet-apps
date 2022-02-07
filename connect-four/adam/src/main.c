/**
 * Connect Four
 */

#include <msx.h>
#include <sys/ioctl.h>
#include <conio.h>
#include <smartkeys.h>
#include <eos.h>

// GLOBALS ///////////////////////////////////////////////////////////////////

unsigned char response[1024]; // Response buffer used by recv()

// CONSTANTS /////////////////////////////////////////////////////////////////

#define BOARD_X 8     // Start column of board
#define BOARD_Y 5     // Start row of board

#define HOVER_X 64    // Start column pixel of hover sprite
#define HOVER_Y 22    // Start row pixel of hover sprite

#define NET 0x09      // Adamnet ID for network device
#define ACK 0x80      // Return value for AdamNet ACK

#define CONNECTED 2   // Bit 1 of returned status is whether we are connected.

#define RETRY_COUNT 128 // # of times to retry

#define READ_WRITE  12  // protocol channel needs both read and write
#define LF           2  // Translation mode = LF

#define PLAYER1_COLOR 8  // Color for player 1
#define PLAYER2_COLOR 10 // Color for player 2
#define BOARD_COLOR 4    // Color for board
#define BORDER_COLOR 7   // color for border

// PROTOTYPES ////////////////////////////////////////////////////////////////

unsigned char check_win(unsigned char x, unsigned char y, unsigned char p);
unsigned char piece_color(unsigned char column, unsigned char row);

// PATTERNS //////////////////////////////////////////////////////////////////

static const char udg[] =
  {
   0x00,0x0F,0x1F,0x3F,0x7F,0x7F,0xFF,0xFF,
   0x00,0xF0,0xF8,0xFC,0xFE,0xFE,0xFF,0xFF,
   0xFF,0xFF,0x7F,0x7F,0x3F,0x1F,0x0F,0x00,
   0xFF,0xFF,0xFE,0xFE,0xFC,0xF8,0xF0,0x00,
   
   0x00,0x0F,0x1F,0x3F,0x7F,0x7F,0x7F,0x7F,
   0x00,0xF0,0xF8,0xFC,0xFE,0xFE,0xFE,0xFE,
   0x7F,0x7F,0x7F,0x7F,0x3F,0x1F,0x0F,0x00,
   0xFE,0xFE,0xFE,0xFE,0xFC,0xF8,0xF0,0x00,
  };

// SMARTKEYS DISPLAY ///////////////////////////////////////////////////////

void status(char *s)
{
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status(s);
}

// INPUT FUNCTIONS /////////////////////////////////////////////////////////

/**
 * @brief Clear the input line
 */
void clear_line(void)
{
  msx_vfill(0x1300,0x00,256); // Clear the input line
}

/**
 * @brief input line of text from user
 * @param c Char buffer
 */
void get_line(char *c)
{
  clear_line();
  gotoxy(0,19);
  cgets(c);
  clear_line();
}

// FUJINET FUNCTIONS /////////////////////////////////////////////////////////

/**
 * Connect to host, wait for connection until timeout.
 * @param h char pointer to host name
 * @return true if connected.
 */
bool connect(char *h)
{
  unsigned char retries=RETRY_COUNT;
  DCB *dcb = eos_find_dcb(NET);
  
  struct
  {
    char cmd;
    char mode;
    char translation;
    char url[96];
  } c;

  c.cmd = 'O';
  c.mode = READ_WRITE;
  c.translation = 0;
  sprintf(c.url,"N:TCP://%s:6502/",h);

  eos_write_character_device(NET,(char *)c, sizeof(c));

  while (retries > 0)
    {
      csleep(5);
      
      while (eos_request_device_status(NET,dcb) < ACK); // sit and spin; Wait for status

      if (eos_get_device_status(NET) & CONNECTED) // Are we connected?
	return true;
      else
	retries--;      
    }

  return false; // Unable to connect.  
}

/**
 * @brief ask FujiNet to wait for a TCP connection on port 6502
 */
void listen(void)
{
  struct
  {
    char cmd;
    char mode;
    char translation;
    char url[16];
  } listenCmd;

  listenCmd.cmd = 'O';
  listenCmd.mode = READ_WRITE;
  listenCmd.translation = 0;
  strcpy(listenCmd.url,"N:TCP://:6502/");

  eos_write_character_device(NET,(char *)listenCmd,sizeof(listenCmd));
}

/**
 * @brief is there a connection waiting?
 * @return true if connection waiting.
 */
bool connection_waiting(void)
{
  DCB *dcb = eos_find_dcb(NET);
  
  while (eos_request_device_status(NET,dcb) < 0x80);
  return eos_get_device_status(NET) & 2; // Return connected bit.
}

/**
 * @brief Send data to other host
 * @param buf The buffer to send
 */
void send(char *buf)
{
  struct
  {
    char cmd;
    char buf[64];
  } s;

  s.cmd = 'W';

  strcpy(s.buf,buf);
  strcat(s.buf,"\n"); // Terminate with newline.

  eos_write_character_device(NET,(char *)s,strlen(s.buf)+1); // To account for command byte
}

/**
 * @brief receive data from other host
 * @param buf The receiving buffer.
 */
void recv(char *buf)
{
  char *p=NULL;
  DCB *dcb = eos_find_dcb(NET);
  
  while (eos_read_character_device(NET,response,sizeof(response)) != ACK); // We have to always ask for receive in 1024 bytes.

  strncpy(buf,response,dcb->len);

  p = strchr(buf,'\n');

  if (p!=NULL)
    *p=0; // remove LF
}

// BOARD DRAWING /////////////////////////////////////////////////////////////

/**
 * @brief Plots a piece on the board.
 * @param x Column (0-6)
 * @param y Row (0-5)
 */
unsigned char plot_piece(unsigned char x, unsigned char y, unsigned char p)
{
  unsigned char ox=x;
  unsigned char oy=y;
  p = ( p==0 ? PLAYER1_COLOR : PLAYER2_COLOR);
  x = (x << 1) + BOARD_X;
  y = (y << 1) + BOARD_Y;
  msx_color(p,BOARD_COLOR,BORDER_COLOR);
  gotoxy(x,y);   cputs("\x80\x81");
  gotoxy(x,y+1); cputs("\x82\x83");
  return check_win(ox,oy,p);
}

/**
 * @brief Draw top portion of next board cell
 */
void board_line1(void)
{
  cputs("\x84\x85\x84\x85\x84\x85\x84\x85\x84\x85\x84\x85\x84\x85");
}

/**
 * @brief Draw bottom portion of next board cell
 */
void board_line2(void)
{
  cputs("\x86\x87\x86\x87\x86\x87\x86\x87\x86\x87\x86\x87\x86\x87");
}

/**
 * @brief Plot an empty board
 */
void board(char *p1, char *p2)
{
  // Draw the board
  msx_color(INK_BLACK,BOARD_COLOR,BORDER_COLOR);
  gotoxy(BOARD_X, BOARD_Y   ); board_line1(); 
  gotoxy(BOARD_X, BOARD_Y+1 ); board_line2();
  gotoxy(BOARD_X, BOARD_Y+2 ); board_line1();
  gotoxy(BOARD_X, BOARD_Y+3 ); board_line2();
  gotoxy(BOARD_X, BOARD_Y+4 ); board_line1();
  gotoxy(BOARD_X, BOARD_Y+5 ); board_line2();
  gotoxy(BOARD_X, BOARD_Y+6 ); board_line1();
  gotoxy(BOARD_X, BOARD_Y+7 ); board_line2();
  gotoxy(BOARD_X, BOARD_Y+8 ); board_line1();
  gotoxy(BOARD_X, BOARD_Y+9 ); board_line2();
  gotoxy(BOARD_X, BOARD_Y+10); board_line1();
  gotoxy(BOARD_X, BOARD_Y+11); board_line2();

  // Draw player names, if set. This allows board to be used on splash screen.
  if (p1 != NULL)
    {
      msx_color(PLAYER1_COLOR,INK_TRANSPARENT,BORDER_COLOR);
      gotoxy(0,18); cprintf("\x80\x81");
      gotoxy(0,19); cprintf("\x82\x83");
      gotoxy(2,19); msx_color(INK_BLACK,INK_TRANSPARENT,BORDER_COLOR); cprintf("%s",p1);
    }

  if (p2 != NULL)
    {
      msx_color(PLAYER2_COLOR,INK_TRANSPARENT,BORDER_COLOR);
      gotoxy(18,18); cprintf("\x80\x81");
      gotoxy(18,19); cprintf("\x82\x83 %s",p2);
      gotoxy(21,19); msx_color(INK_BLACK,INK_TRANSPARENT,BORDER_COLOR); cprintf("%s",p2);
    }
}

/**
 * @brief Hover player piece over column X
 * @param x Column (0-6)
 */
void hover(unsigned char x, unsigned char p)
{
  x <<= 4;
  x += HOVER_X;
  
  p = ( p == 0 ? PLAYER1_COLOR : PLAYER2_COLOR);

  // Copy pattern data from CPU to VDP
  msx_vwrite(udg,0x3800,32);

  // Set up the four sprites that make up the hovering piece.
  vpoke(0x1b00,HOVER_Y);
  vpoke(0x1b01,x);
  vpoke(0x1B02,0);
  vpoke(0x1B03,p);

  vpoke(0x1b04,HOVER_Y);
  vpoke(0x1b05,x+8);
  vpoke(0x1B06,1);
  vpoke(0x1B07,p);

  vpoke(0x1b08,HOVER_Y+8);
  vpoke(0x1b09,x);
  vpoke(0x1B0a,2);
  vpoke(0x1B0b,p);
  
  vpoke(0x1b0c,HOVER_Y+8);
  vpoke(0x1b0d,x+8);
  vpoke(0x1B0e,3);
  vpoke(0x1B0f,p);
}

/**
 * @brief clears the hovering piece sprite
 */
void hover_done(void)
{
  msx_vfill(0x1b00,0x00,16);
}

/**
 * @brief draw the board on the splash screen
 */
void board_splash(void)
{
  cprintf("     #FUJINET CONNECT FOUR");
  board(NULL,NULL);
  plot_piece(0,5,0);
  plot_piece(1,5,1);
  plot_piece(1,4,0);
  plot_piece(2,5,1);
  plot_piece(2,4,0);
  plot_piece(0,4,1);
  plot_piece(2,3,0);
  plot_piece(2,2,1);
  plot_piece(1,3,0);
  plot_piece(1,2,1);
  plot_piece(3,5,0);
  plot_piece(0,3,1);
  hover(4,0);
}

// CHECK FOR WIN /////////////////////////////////////////////////////////////

/**
 * @brief scans VRAM pieces and determines winning player
 * @param x column of piece dropped
 * @param y row of piece dropped
 * @param p current player (0 or 1)
 * @return 0xFF = No win, otherwise player 0 or 1.
 */
unsigned char check_win(unsigned char x, unsigned char y, unsigned char p)
{
  if (p==PLAYER1_COLOR)
    p=0;
  else if (p==PLAYER2_COLOR)
    p=1;
  
  if ((piece_color(x,y) == p) &&          // HORIZONTAL CHECK
      (piece_color(x+1,y) == p) &&
      (piece_color(x+2,y) == p) &&
      (piece_color(x+3,y) == p))
    return p;
  else if (piece_color(x,y) == p &&
	   (piece_color(x-1,y) == p) &&
	   (piece_color(x-2,y) == p) &&
	   (piece_color(x-3,y) == p))
    return p;
  else if (piece_color(x,y) == p &&    // VERTICAL CHECK
	   (piece_color(x,y+1) == p) &&
	   (piece_color(x,y+2) == p) &&
	   (piece_color(x,y+3) == p))
    return p;
  else if ((piece_color(x,y) == p) &&
	   (piece_color(x,y-1) == p) &&
	   (piece_color(x,y-2) == p) &&
	   (piece_color(x,y-3) == p))
    return p;
  else if ((piece_color(x,y) == p) &&    // DIAGONAL 1 CHECK
	   (piece_color(x+1,y+1) == p) &&
	   (piece_color(x+2,y+2) == p) &&
	   (piece_color(x+3,y+3) == p))
    return p;
  else if ((piece_color(x,y) == p) &&
	   (piece_color(x-1,y-1) == p) &&
	   (piece_color(x-2,y-2) == p) &&
	   (piece_color(x-3,y-3) == p))
    return p;
  else if ((piece_color(x,y) == p) &&    // DIAGONAL 2 CHECK
	   (piece_color(x-1,y+1) == p) &&
	   (piece_color(x-2,y+2) == p) &&
	   (piece_color(x-3,y+3) == p))
    return p;
  else if ((piece_color(x,y) == p) &&
	   (piece_color(x+1,y-1) == p) &&
	   (piece_color(x+2,y-2) == p) &&
	   (piece_color(x+3,y-3) == p))
    return p;
  
  return 0xFF; // No checking for now.
}

// MAIN //////////////////////////////////////////////////////////////////////

/**
 * @brief get player name
 * @param pointer to buffer that holds player name
 */
void get_player_name(char *n)
{
  status("  ENTER PLAYER NAME:");
  get_line(n);
}

/**
 * @brief connect to host pointed by n
 * @param n pointer to hostname
 * @return true if connection successful.
 */
bool connect_to_host(char *n)
{
  bool s=false;
  
  status("  CONNECTING TO HOST, PLEASE WAIT...");
  if (connect(n))
    {
      status("  CONNECTION SUCCESSFUL.");
      s=true;
    }
  else
    {
      status("  COULD NOT CONNECT.");
      s=false;
    }
  
  sleep(1);  
  return s;
}

/**
 * @brief Listen for connection on TCP port 6502 (server mode)
 */
bool listen_for_connection(void)
{
  listen();

  status("  WAITING FOR CONNECTION TO TCP PORT 6502...");

  while (connection_waiting()==false);
  
  return true;
}

/**
 * @brief get hostname, and pass to listen or connect
 * @param n pointer to hostname
 */
bool get_host_name(char *n)
{
  bool successful = false;
  
  while (!successful)
    {
      status("  ENTER HOSTNAME\n  OR [RETURN] TO LISTEN ON TCP PORT 6502");
      get_line(n);
      
      if (n[0]==0x00)
	successful=listen_for_connection();
      else
	successful=connect_to_host(n);
   }

  return true;
}

/**
 * @brief send move to other host
 * @param x column
 * @param drop 1 = piece is dropped
 */
void send_move(unsigned char x, bool drop)
{
  char s[4];
  sprintf(s,"%d,%d",x,drop);
  send(s);
}

/**
 * @brief Hover piece over board for local player
 * @param turn The current player
 * @return the column dropped
 */
unsigned char hover_piece_local(unsigned char turn)
{
  char c=0;
  char x=0;
  
  while (c!=0x0A)
    {
      hover(x,turn);
      
      send_move(x,false);
      
      c=cgetc();
      switch(c)
	{
	case 0xA1: // RIGHT
	  if (x<6)
	    x++;
	  break;
	case 0xA3: // LEFT
	  if (x>0)
	    x--;
	  break;
	case 0x0A:
	  send_move(x,true);
	  return x;
	}
    }
}

/**
 * @brief Hover piece over board for remote player
 * @param turn The current player
 * @return the column dropped
 */
unsigned char hover_piece_remote(unsigned char turn)
{
  int x=0;
  int drop=false;
  char s[4];
  
  while (drop==false)
    {
      recv(s);
      sscanf(s,"%d,%d",&x,&drop);
      hover(x,turn);
    }

  return x;
}

/**
 * @brief Hover piece over board, pick local orremote depending on listen and turn
 * @param turn The current player
 * @param listen 1 = we are the server
 */
unsigned char hover_piece(unsigned char turn, bool listen)
{
  if (listen==true && turn==0)
    return hover_piece_local(turn);
  else if (listen==false && turn==0)
    return hover_piece_remote(turn);
  else if (listen==true && turn==1)
    return hover_piece_remote(turn);
  else
    return hover_piece_local(turn);
}

/**
 * @brief return player number of requested slot
 * @verbose this checks the color table of VRAM for a given row,column
 * @param column X position to check
 * @param row Y position to check
 * @return FF = no color, 0 = player 1, 1 = player 2
 */
unsigned char piece_color(unsigned char column, unsigned char row)
{
  unsigned short a;
  unsigned char p;
  unsigned char c;
  
  // Calculate address in color table for desired row/column
  a = MODE2_ATTR + (((row << 9) + (BOARD_Y << 8)) + 4) + ((BOARD_X << 3) + (column << 4));
  
  c=vpeek(a) >> 4; // We only want the top nibble.

  if (c==PLAYER1_COLOR)
    p = 0;
  else if (c==PLAYER2_COLOR)
    p = 1;
  else
    p = 0xFF;

  return p;
}

/**
 * @brief is there a piece below this column,row?
 * @param column present X position
 * @param row present Y position
 * @return 1 = there is a piece below.
 */
bool piece_below(unsigned char column, unsigned char row)
{
  return (piece_color(column,row+1) != 0xFF);
}

/**
 * @brief Drop piece onto board.
 * @param turn the current player.
 * @param listen 1 = we are the server.
 * @param column The column dropped.
 */
unsigned char drop_piece(unsigned char turn, bool listen, unsigned char column)
{
  char row=0;
  char ypos = vpeek(0x1B00) + 2;

  // Scoot piece to edge of board
  vpoke(0x1B00,ypos);
  vpoke(0x1B04,ypos);
  vpoke(0x1B08,ypos+8);
  vpoke(0x1B0C,ypos+8);
  
  while (piece_below(column,row)==false)
    {
      if (row == 5)
	break;

      for (char i=0;i<16;i++)
	{
	  vpoke(0x1b00,ypos+i);
	  vpoke(0x1b04,ypos+i);
	  vpoke(0x1b08,ypos+i+8);
	  vpoke(0x1b0c,ypos+i+8);
	}
      
      ypos += 16;
      row++;
    }
  
  hover_done();
  return plot_piece(column,row,turn);
}

/**
 * Display player's turn in status bar
 * @param turn the current player
 * @param player1 Player 1 name
 * @param player2 Player 2 name 
 */
void display_turn(unsigned char turn, char *player1, char *player2)
{
  char status[64];
  char statusfmt[]="\n  %s'S TURN.";
  
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);

  if (turn==1)
    sprintf(status,statusfmt,player2);
  else
    sprintf(status,statusfmt,player1);

  smartkeys_status(status);
}


/**
 * @brief main game
 * @param player starting player (0-1)
 * @param listen are we the listening server?
 * @param player1 Player 1 name
 * @param player2 Player 2 name
 */
void game(char player, bool listen, char *player1, char *player2)
{
  char turn=player;
  char winner=0xFF;
  char winfmt[]="%s WINS!\nPLAY AGAIN?";
  char winmsg[40];
  char c=0x85;
  
  while (c==0x85)
    {
      board(player1,player2);

      do
	{
	  display_turn(turn,player1,player2);
	  winner=drop_piece(turn,listen,hover_piece(turn,listen));
	  turn = !turn; // Next player turn.
	} while (winner == 0xFF);
      
      smartkeys_display(NULL,NULL,NULL,NULL,"  YES","   NO");
      
      if (winner==1)
	sprintf(winmsg,winfmt,player2);
      else
	sprintf(winmsg,winfmt,player1);
      
      smartkeys_status(winmsg);
      
      c=cgetc();
    }
}

/**
 * @brief Program entry point
 */
void main(void)
{
  void *param = &udg;
  char hostname[64];
  char player1[16], player2[16];
  bool listen=false;
  char player=0;

  memset(hostname,0,sizeof(hostname));
  memset(player1,0,sizeof(player1));
  memset(player2,0,sizeof(player2));
  
  console_ioctl(IOCTL_GENCON_SET_UDGS,&param);  
  smartkeys_set_mode();

  board_splash();

  get_player_name(player1);
  listen=get_host_name(hostname);    

  if (listen)
    {
      send(player1);
      recv(player2);
      player=0;
    }
  else
    {
      recv(player2);
      send(player1);
      player=1;
    }

  game(player,listen,player1,player2);
}

/**
 * @brief   Light Cycles example game
 * @verbose Shows example of two player networked game with #FujiNet
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 */

#include <apple2.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include "nio.h"
#include "die.h"
#include "sp.h"

/* Joystick directions from the Atari version */
#define CENTER 15
#define UP     14
#define DOWN   13
#define LEFT   11
#define RIGHT  7

/* Colors */
#define MY_COLOR     TGI_COLOR_ORANGE
#define OTHER_COLOR  TGI_COLOR_GREEN
#define BORDER_COLOR TGI_COLOR_BLUE

int8_t net; /* SmartPort Network Device ID */
char my_name[12], other_name[12], hostname[64], url[64];
int my_score, other_score;
unsigned short bw; /* bytes waiting to read */
unsigned char error; /* network error code */
bool connected=false; /* Are we connected? */
bool listening=false; /* Are we listening? (server) */
unsigned char mx,my,ox,oy; /* Current position of players */
unsigned char md,od;       /* Current player direction */

/**
 * @brief setup graphics
 */
void setup(void)
{
  if (!sp_init())
    die("SMARTPORT NOT FOUND");

  net = sp_find_network();

  if (!net)
    die("NETWORK DEVICE NOT FOUND");

  /* Everything ok, set up mixed lo-res graphics. */
  
  tgi_install(a2_lo_tgi);
  tgi_init();
  tgi_apple2_mix(1);
  tgi_clear();
}

/**
 * @brief Plot a double high pixel
 */
void plot(unsigned char x, unsigned char y)
{
  y <<= 1; // Double
  tgi_setpixel(x,y);
  tgi_setpixel(x,y+1);
}

/**
 * @brief Get pixel color at x,y
 */
unsigned char locate(unsigned char x, unsigned char y)
{
  y <<=1; // Double
  return tgi_getpixel(x,y);
}

/**
 * @brief draw the board
 */
void draw_board(void)
{
  unsigned char i;
  
  tgi_clear();

  tgi_setcolor(BORDER_COLOR);
  
  for (i=0;i<40;i++)
    plot(i,0);

  for (i=0;i<20;i++)
    plot(39,i);

  for (i=0;i<40;i++)
    plot(i,19);

  for (i=0;i<20;i++)
    plot(0,i);
}

/**
 * @brief Get the player name
 */
void get_player(void)
{  
  printf("WHAT IS YOUR NAME? ");
  gets(my_name);
}

/**
 * @brief get the host name
 */
void get_host(void)
{
  printf("\nENTER HOST NAME, OR ");
  revers(1);
  printf(" RETURN ");
  revers(0);
  printf(" TO LISTEN:\n");
  gets(hostname);
}

/**
 * @brief listen for connection, if hostname empty.
 */
void listen(void)
{
  if (nopen(net,"N:TCP://:6502/",0x0C,0x00))
    die("COULD NOT OPEN LISTENING CONNECTION");


  printf("LISTENING ON PORT 6502...");
  
  while (!connected)
    nstatus(net,&bw,&connected,&error);

  // We have a connection, accept it.
  if (ncontrol(net,'A',NULL,3))
    die("COULD NOT ACCEPT CONNECTION");

  listening=true;
}

/**
 * @brief establish TCP connection to host
 */
void connect(void)
{
  /* Build N:TCP://hostname:6502 */
  strcat(url,"N:TCP://");
  strcat(url,hostname);
  strcat(url,":6502/");

  printf("CONNECTING TO:\n%s",url);
  
  if (nopen(net,url,0x0C,0x00))
    die("COULD NOT MAKE CONNECTION");  
}

/**
 * @brief Get player name s
 */
void get_names(void)
{
  /* Send my name, and ATASCII EOL */
  /* This could be \r\n if trans = 3, or just \n if trans = 2, or just \r if trans = 1 */
  nwrite(net,my_name,strlen(my_name));
  nwrite(net,"\x9b",1);

  bw=0;
  
  /* Get opponent name, and strip away EOL */
  while (bw==0)
    nstatus(net,&bw,&connected,&error);
  nread(net,other_name,bw);
  other_name[strlen(other_name)-1]='\0';
}

/**
 * @brief reset player positions to corners of board
 */
void player_reset(void)
{
  mx=4;
  ox=35;
  my=9;
  oy=9;
}

/**
 * @brief print score
 */
void print_score(void)
{
  printf("      ME:%12s %2d\n",my_name,my_score);
  printf("OPPONENT:%12s %2d\n",other_name,other_score);
}

/**
 * @brief Plot each player's current position to screen
 */
void plot_players(void)
{
  tgi_setcolor(MY_COLOR);
  plot(mx,my);
  tgi_setcolor(OTHER_COLOR);
  plot(ox,oy);
}

/**
 * @brief move a player 
 * @param *x Pointer to player X position
 * @param *y Pointer to player Y position
 * @param *d Pointer to player direction
 */
void move_player(unsigned char *x, unsigned char *y, unsigned char *d)
{
  unsigned char cx,cy,cd;

  cx=*x;
  cy=*y;
  cd=*d;

  switch (cd)
    {
    case UP:
      cy--;
      break;
    case DOWN:
      cy++;
      break;
    case LEFT:
      cx--;
      break;
    case RIGHT:
      cx++;
      break;
    }

  *x=cx;
  *y=cy;
  *d=cd;
}

/**
 * @brief Call move_player to move each player
 */
void move_players(void)
{
  move_player(&mx,&my,&md);
  move_player(&ox,&oy,&od);
}

/**
 * @brief Check for a player collision
 * @param *x = mx or ox (X position)
 * @param *y = my or oy (Y position)
 * @param *d = md or od (direction)
 * @return The color at that position
 */
unsigned char player_check(unsigned char *x, unsigned char *y, unsigned char *d)
{
  unsigned char cx,cy,cd;

  cx=*x;
  cy=*y;
  cd=*d;

  if (cd==UP)
    cy--;
  else if (cd==DOWN)
    cy++;
  else if (cd==LEFT)
    cx--;
  else if (cd==RIGHT)
    cx++;

  return locate(cx,cy);
}

/**
 * @brief Check for player collisions
 * @return 0 = none, 1 = me, 2 = other, 3 = draw
 */
unsigned char check_for_collisions(void)
{
  unsigned char mc,oc; /* Pixel colors at collision check */
  bool m,o;            /* Combinatorial collision check */

  m=o=false;
  
  mc=player_check(&mx,&my,&md);
  oc=player_check(&ox,&ox,&od);

  if (oc==MY_COLOR ||
      oc==BORDER_COLOR ||
      mc==MY_COLOR ||
      mc==BORDER_COLOR)
    m=true; /* My collision */

  if (mc==OTHER_COLOR ||
      mc==BORDER_COLOR ||
      oc==OTHER_COLOR ||
      oc==BORDER_COLOR)
    o=true; /* other collision */

  if (m && o) /* Draw? */
    return 3;
  else if (o)
    return 2;
  else if (m)
    return 1;
  else
    return 0;
}

/**
 * @brief   Send our player's direction; retrieve the other's
 * @verbose Other than the setup, this is the extent of the network code.
 */
void handle_player_interaction(void)
{
  char my[4], other[4];

  /* Send my position */
  sprintf(my,"%u\x9b",md);
  nwrite(net,my,strlen(my));

  /* Get other position, waiting until it is sent. */
  bw=0;
  while (bw==0)
    {
      nstatus(net,&bw,&connected,&error);
      if (!connected)
	die("DISCONNECTED.");
    }
  nread(net,other,bw);
  od=atoi(other);
}

/**
 * @brief the main game loop
 */
void game(void)
{
  unsigned char winner=0;
  
  player_reset();
  print_score();

  while (winner==0)
    {
      handle_player_interaction();
      plot_players();
      move_players();
      winner = check_for_collisions();
    }
}

/**
 * @brief Show Instructions
 */
void instructions(void)
{
  clrscr();

  printf("      ");
  
  revers(1);
  printf(" #FUJINET LIGHT CYCLES \n\n");
  revers(0);

  printf("IN THIS GAME, YOU CONNECT WITH A REMOTE\n");
  printf("PLAYER VIA TCP PORT 6502. BE SURE TO\n");
  printf("FORWARD THIS PORT, SO YOU CAN CONNECT.\n\n");

  printf("AS YOU MOVE, YOUR LIGHTCYCLE LEAVES A\n");
  printf("TRAIL. MANEUVER YOUR OPPONENT INTO THE\n");
  printf("LIGHT WALLS.\n\n");
}

void main(void)
{
  uint8_t sp_init();

  setup();
  instructions();
  get_player();
  get_host();

  if (hostname[0]=='\0')
    listen();
  else
    connect();

  printf("CONNECTED!\n");

  get_names();


  while (connected==1)
    {
      draw_board();
      game();
      nstatus(net,&bw,&connected,&error);
    }

  if (listening)
    ncontrol(net,'c',NULL,0); /* Close the client connection. */
  
  nclose(net); /* Close the network adapter */

  die("PRESS ANY KEY TO EXIT");
}

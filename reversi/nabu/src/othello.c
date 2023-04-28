/*
 *   $Id: othello.c $
 * 
 *   z88dk port of the 'historical' game by Leor Zolman
 *
 *   This adaption shows how to translate a bi-dimensional
 *   array (not supported by sccz80) into a vector.
 * 
 *   Note that this program is recursive and requires
 *   a lot of stack space, thus it could need to be properly located.
 * 
 *   Examples on how to compile in text mode:
 *   zcc +zx -clib=ansi -lndos -O3 -create-app -zorg=50000 othello.c
 *   zcc +zx81 -O3 -create-app othello.c
 *
 *   Examples on how to compile in graphics mode:
 *   zcc +zx -lndos -O3 -create-app -zorg=50000 -DGRAPHICS othello.c
 *   zcc +ts2068 -clib=ansi -O3 -lndos -create-app -zorg=45000 -DWIDEGRAPHICS -pragma-define:CLIB_DEFAULT_SCREEN_MODE=0x3e -DANSITEXT othello.c
 *   (16K, WRX HRG mode)
 *   zcc +zx81 -O3 -subtype=_wrx64 -clib=wrx64ansi -create-app -DSMALLGRAPHICS othello.c
 *   (32K + WRX HRG, add the '#pragma output hrgpage = 36096' line)
 *   zcc +zx81 -O3 -subtype=_wrx -clib=wrxansi -create-app -DSMALLGRAPHICS othello.c
 * 
 *   Examples on how to compile in redefinded font mode:
 *   zcc +zx81 -O3 -create-app -DREDEFINED_FONT -DZX81_FONT othello.c
 *   zcc +zx80 -O3 -create-app -DREDEFINED_FONT -DZX80_FONT othello.c
 *   zcc +ace -O3 -lndos -create-app -DREDEFINED_FONT othello.c
 *   zcc +srr -O3 -lndos -create-app -DREDEFINED_FONT othello.c
 *   zcc +zx -O3 -lndos -create-app -DREDEFINED_FONT othello.c
 *   zcc +trs80 -subtype=eg2000disk -O3 -lndos -create-app -DREDEFINED_FONT othello.c
 */


/*

	OTHELLO -- The Game of Dramatic Reversals

	written by Bert Halstead
	modified for BDS C by Leor Zolman

This program is a good example of:

	a) structured, heirarchical function organization
	b) arrays as formal parameters
	c) use of the "qsort" library function

   Object of the game is for two players to alternate
placing their marker someplace on an 8 by 8 grid, so that
at least one of the opponent's pieces becomes surrounded
by the moving player's peices -- causing the flanked pieces
to flip 'color' and belong to the moving player. After 60
moves have been played (or if no player has a legal move left),
the player with the most of his own pieces on the board wins.

   The playing pieces are '*' and '@'. You may choose to play
either '*' or '@' for the first game; thereafter, you and the
computer will alternate going first for each game. Whoever
goes first always plays `*'.

   You enter a move as a two digit number, each digit being
from 1 to 8, first digit representing row and second representing
column. For example: if playing '*', your first move might be '46',
meaning 4th row down, 6th position across.

   As an alternative to entering a move, one of the following
commands may be typed:

	g	causes computer to play both sides until game
		is over

	a	causes computer to print out an analysis of
		each of your possible moves. A letter from A
		to Z will appear at each of your legal move
		positions, where A is the machine's opinion
		of an excellant move and Z is a real loser.

	hn	sets handicap. n is 1,2,3, or 4. If n is
		positive, gives n free pieces to the computer.
		If n is negative, gives YOU the free pieces.

	f	forfeit the current move. This happens
		automatically if you have no legal moves.

	q	quit the current game.

	b	prints out board again.

	s	prints out the score, and tells who is winning.

*/
//#define BUILD_ADAM

int my_mov(char b[64], char p, char o, char e, int *m, int *n);

#ifdef BUILD_ADAM
  #define ADAM_OR_NABU
  //#error ADAM build selected.
#endif

#ifdef BUILD_NABU
  #define ADAM_OR_NABU
  //#error NABU build selected.
#endif

#ifdef ADAM_OR_NABU
#include <msx.h>
#include <graphics.h>
#include <games.h>
#include "board.h"
#include "charset.h"
#include "nhandler.h"
#include "spriteset.h"
#include "joystick.h"
#include "sound.h"

#ifdef BUILD_ADAM
#include "smartkeys.h"
#include "eos.h"
#endif

	/*
	DefPatternTable EQU     0h
	DefNameTable    EQU     1800h
	DefSprAttrTable EQU     1b00h
	DefColorTable   EQU     2000h
	DefSprPatTable  EQU     3800h
	*/

	// graphics mode 1
	// 32 x 24

char url[255];
char host[255];
char board[768];



#define LINE_START  7
#define LINE_STOP  17
#define LINE_WIDTH  6

int white_line = LINE_START;
int black_line = LINE_START;
#define WHITE_START_X  0
#define BLACK_START_X  25




#define CURSOR_TOP_LEFT      0
#define CURSOR_MIDDLE_LEFT   1
#define CURSOR_BOTTOM_LEFT   2

#define CURSOR_TOP_MIDDLE    3
#define CURSOR_MIDDLE_MIDDLE 4
#define CURSOR_BOTTOM_MIDDLE 5

#define CURSOR_TOP_RIGHT     6
#define CURSOR_MIDDLE_RIGHT  7
#define CURSOR_BOTTOM_RIGHT  8

int status_y = 22;

int line = 0;

int trace_on = 1;
int trig=1;

int print_trace(char *message);
int delay(int d);

char my_name[16];
char their_name[16];

#define COMPUTER_OPPONENT 1
#define LOCAL_OPPONENT	  2
#define OTHER_HOSTED      3
#define HOSTING_GAME	  4

#endif


/* z88dk specific opt */
#pragma printf = "%c %u"
#ifdef SCCZ80
	void prtbrd(char b[64]) __z88dk_fastcall;
int prtscr(char b[64]) __z88dk_fastcall;
#endif

#define BLACK '*'
#define WHITE 'O'
#define EMPTY '-'

#ifdef __SPECTRUM__
#define G_BLACK 128
#define G_WHITE 129
#define G_EMPTY 130
#endif

#ifdef __TRS80__
#define G_BLACK 128
#define G_WHITE 129
#define G_EMPTY 130
#endif

#ifdef __ACE__
#define G_BLACK 0
#define G_WHITE 1
#define G_EMPTY 2
#endif

#ifdef __SORCERER__
#define G_BLACK 193
#define G_WHITE 194
#define G_EMPTY 195
#endif

#ifdef __SHARPMZ__
#define G_BLACK 0xf1
#define G_WHITE 0xf7
#define G_EMPTY 0xda
#endif

#ifdef ZX81_DKTRONICS
#define G_BLACK 6
#define G_WHITE 14
#define G_EMPTY 62
#endif

#ifndef G_BLACK
#define G_BLACK '*'
#define G_WHITE 'O'
#define G_EMPTY '.'
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>   /* Needed just for srand seed */

#ifdef WIDEGRAPHICS
#include <graphics.h>
#include <games.h>
#endif

#ifdef GRAPHICS
#include <graphics.h>
#include <games.h>
/* Declare GFX bitmap location for the expanded ZX81 */
//#pragma output hrgpage = 36096
#endif

#ifdef SMALLGRAPHICS
#include <graphics.h>
#include <games.h>
#endif

#ifndef fputc_cons
#define fputc_cons putchar
#endif

#ifndef getk
#define getk getchar
#endif

int handicap;
char selfplay;		/* true if computer playing with itself */
/* int h[4][2];	*/	/* handicap position table */
int h[8];		/* handicap position table */
char mine, his;		/* who has black (*) and white (@) in current game */
char mefirst;		/* true if computer goes first in current game */

struct mt {
		int x;
		int y;
		int c;
		int s;
	 };

#if defined (REDEFINED_FONT)

extern char whitepiece[];
extern char blackpiece[];
extern char frame[];

#asm
._whitepiece
 defb    @11111110
 defb    @11000011
 defb    @10001000
 defb    @10000101
 defb    @10000100
 defb    @10000001
 defb    @11000010
 defb    @01010101

._blackpiece
 defb    @11111110
 defb    @11000011
 defb    @10110100
 defb    @10111001
 defb    @10111100
 defb    @10111101
 defb    @11000010
 defb    @01010101

._frame
 defb    @11111110
 defb    @11111111
 defb    @11111110
 defb    @11111111
 defb    @11111110
 defb    @11111111
 defb    @11111110
 defb    @01010101
#endasm

#endif


#ifdef WIDEGRAPHICS

char numbers[] = {
        3, 5, 0xE0 , 0xA0 , 0xA0 , 0xA0 , 0xE0,
        3, 5, 0x40 , 0x40 , 0x40 , 0x40 , 0x40,
        3, 5, 0xE0 , 0x20 , 0xE0 , 0x80 , 0xE0,
        3, 5, 0xE0 , 0x20 , 0xE0 , 0x20 , 0xE0,
        3, 5, 0xA0 , 0xA0 , 0xE0 , 0x20 , 0x20,
        3, 5, 0xE0 , 0x80 , 0xE0 , 0x20 , 0xE0,
        3, 5, 0xE0 , 0x80 , 0xE0 , 0xA0 , 0xE0,
        3, 5, 0xE0 , 0x20 , 0x20 , 0x20 , 0x20,
        3, 5, 0xE0 , 0xA0 , 0xE0 , 0xA0 , 0xE0,
        3, 5, 0xE0 , 0xA0 , 0xE0 , 0x20 , 0xE0 };

// Generated by Daniel McKinnon's z88dk Sprite Editor
char frame[] = { 24, 16, 0xAA , 0xAA , 0xAA , 0x00 , 0x00 , 0x01 , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 
, 0x01 , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x80 , 0x00 , 0x00 , 0x00 
, 0x00 , 0x01 , 0x80 , 0x00 , 0x00
, 0x00 , 0x00 , 0x01 , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x80 , 0x00 
, 0x00 , 0x00 , 0x00 , 0x01 , 0x80 , 0x00 , 0x00 , 0x55 , 0x55 , 0x55  };

char whitepiece[] = { 24, 16, 0x00 , 0x00 , 0x00 , 0x00 , 0xFF , 0x00 , 0x03 , 0x00 , 0xC0 , 0x0C , 0x00 
, 0x30 , 0x10 , 0x0E , 0x08 , 0x20 , 0x01 , 0x84 , 0x20 , 0x00 , 0x44 , 0x40 
, 0x00 , 0x22 , 0x40 , 0x00 , 0x02
, 0x20 , 0x00 , 0x04 , 0x20 , 0x00 , 0x04 , 0x10 , 0x00 , 0x08 , 0x0C , 0x00 
, 0x30 , 0x03 , 0x00 , 0xC0 , 0x00 , 0xFF , 0x00 , 0x00 , 0x00 , 0x00  };

char blackpiece[] = { 24, 16, 0x00 , 0x00 , 0x00 , 0x00 , 0xFF , 0x00 , 0x03 , 0xFF , 0xC0 , 0x0F , 0xFF 
, 0xF0 , 0x1F , 0xF1 , 0xF8 , 0x3F , 0xFC , 0x7C , 0x3F , 0xFF , 0xBC , 0x7F 
, 0xFF , 0xDE , 0x7F , 0xFF , 0xFE
, 0x3F , 0xFF , 0xFC , 0x3F , 0xFF , 0xFC , 0x1F , 0xFF , 0xF8 , 0x0F , 0xFF 
, 0xF0 , 0x03 , 0xFF , 0xC0 , 0x00 , 0xFF , 0x00 , 0x00 , 0x00 , 0x00  };

#endif


#ifdef GRAPHICS

extern char whitepiece[];
extern char blackpiece[];
extern char frame[];

char numbers[] = {
        3, 5, 0xE0 , 0xA0 , 0xA0 , 0xA0 , 0xE0,
        3, 5, 0x40 , 0x40 , 0x40 , 0x40 , 0x40,
        3, 5, 0xE0 , 0x20 , 0xE0 , 0x80 , 0xE0,
        3, 5, 0xE0 , 0x20 , 0xE0 , 0x20 , 0xE0,
        3, 5, 0xA0 , 0xA0 , 0xE0 , 0x20 , 0x20,
        3, 5, 0xE0 , 0x80 , 0xE0 , 0x20 , 0xE0,
        3, 5, 0xE0 , 0x80 , 0xE0 , 0xA0 , 0xE0,
        3, 5, 0xE0 , 0x20 , 0x20 , 0x20 , 0x20,
        3, 5, 0xE0 , 0xA0 , 0xE0 , 0xA0 , 0xE0,
        3, 5, 0xE0 , 0xA0 , 0xE0 , 0x20 , 0xE0 };


#asm

._frame
 defb    16,16
 defb    @10101010, @10101010
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @00000000, @00000001
 defb    @10000000, @00000000
 defb    @01010101, @01010101

._whitepiece
 defb    16,16
 defb    @00000000, @00000000
 defb    @00000000, @00000000
 defb    @00000011, @11000000
 defb    @00001100, @00110000
 defb    @00010000, @11001000
 defb    @00010000, @00101000
 defb    @00100000, @00010100
 defb    @00100000, @00010100
 defb    @00100000, @00000100
 defb    @00100000, @00000100
 defb    @00010000, @00001000
 defb    @00010000, @00001000
 defb    @00001100, @00110000
 defb    @00000011, @11000000
 defb    @00000000, @00000000
 defb    @00000000, @00000000

._blackpiece
 defb    16,16
 defb    @00000000, @00000000
 defb    @00000000, @00000000
 defb    @00000011, @11000000
 defb    @00001111, @11110000
 defb    @00011111, @00111000
 defb    @00011111, @11011000
 defb    @00111111, @11101100
 defb    @00111111, @11101100
 defb    @00111111, @11111100
 defb    @00111111, @11111100
 defb    @00011111, @11111000
 defb    @00011111, @11111000
 defb    @00001111, @11110000
 defb    @00000011, @11000000
 defb    @00000000, @00000000
 defb    @00000000, @00000000

#endasm
#endif

#ifdef SMALLGRAPHICS

extern char whitepiece[];
extern char blackpiece[];
extern char frame[];

char numbers[] = {
        3, 5, 0xE0 , 0xA0 , 0xA0 , 0xA0 , 0xE0,
        3, 5, 0x40 , 0x40 , 0x40 , 0x40 , 0x40,
        3, 5, 0xE0 , 0x20 , 0xE0 , 0x80 , 0xE0,
        3, 5, 0xE0 , 0x20 , 0xE0 , 0x20 , 0xE0,
        3, 5, 0xA0 , 0xA0 , 0xE0 , 0x20 , 0x20,
        3, 5, 0xE0 , 0x80 , 0xE0 , 0x20 , 0xE0,
        3, 5, 0xE0 , 0x80 , 0xE0 , 0xA0 , 0xE0,
        3, 5, 0xE0 , 0x20 , 0x20 , 0x20 , 0x20,
        3, 5, 0xE0 , 0xA0 , 0xE0 , 0xA0 , 0xE0,
        3, 5, 0xE0 , 0xA0 , 0xE0 , 0x20 , 0xE0 };


#asm

._frame
 defb    6,5
 defb    @00000100
 defb    @00000000
 defb    @00000100
 defb    @00000000
 defb    @01010100

._whitepiece
 defb    4,4
 defb    @01100000
 defb    @10010000
 defb    @10010000
 defb    @01100000

._blackpiece
 defb    4,4
 defb    @01100000
 defb    @11110000
 defb    @11110000
 defb    @01100000
 
#endasm
#endif

/*
void shift_right()
{
	printf("%c[%uC",27,17);
}
*/

char skipbl()
{
	char c;
	while ((c = toupper(getchar())) == ' ' || c=='\t');
	//fputc_cons(c);
	return c;
}

int chkmv1( char b[64], char p, int x, int y, int m, int n )
{
	int k;
	k=0;
	while ((x += m) >= 0 && x < 8 && (y += n) >= 0 && y<8)
	{
		if (b[x*8+y]==EMPTY) return 0;
		if (b[x*8+y]== p ) return k;
		if (x==0 || x==7 || y==0 || y==7)
			k += 10;
		 else k++;
	}
	return 0;
}

int chkmov( char b[64], char p, int x, int y )
{
	if (b[x*8+y] != EMPTY) return 0;
	return	chkmv1(b,p,x,y, 0, 1) + chkmv1(b,p,x,y, 1, 0) +
		    chkmv1(b,p,x,y, 0,-1) + chkmv1(b,p,x,y,-1, 0) +
			chkmv1(b,p,x,y, 1, 1) + chkmv1(b,p,x,y, 1,-1) +
			chkmv1(b,p,x,y,-1, 1) + chkmv1(b,p,x,y,-1,-1);
}


int chkmvs( char b[64], char p )
{
	int i,j,k;

	k=0;
	for (i=0; i<8; i++) for (j=0; j<8; j++)
		k += chkmov(b,p,i,j);

	return k;
}


void cpybrd(char a[64], char b[64])
{
	memcpy(a,b,64);
}


int cntbrd(char b[64], char p)
{
	int i,k;
	i=64;k=0;
	while (i--)
		if (b[i] == p) ++k;
	return (k);
}


void clrbrd(char b[64])
{
	int i,j;
	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			b[i*8+j]= EMPTY;
	b[27] = b[36] = BLACK;
	b[28] = b[35] = WHITE;

#ifdef ADAM_OR_NABU
	memcpy(board, newboard, sizeof(newboard));
#endif

}

#ifdef ADAM_OR_NABU

int delay(int d)
{
	long i, s = 3000; // one second

	d--;
	for (i = 0; i < s * d + 500; i++)
		;

	return 0;
}

int print(int x, int y, char *message)
{
	int pos = y * 32 + x;
	unsigned int addr;

	for (x = 0; x < strlen(message); x++)
		board[pos + x] = message[x];

	addr = NameTable;
	msx_vwrite(board, addr, 768);
	return 0;
}

int print_down(int x, int y, char *message)
{
	int pos = y * 32 + x;
	unsigned int addr;

	for (x = 0; x < strlen(message); x++)
	{
		if (y > 20)
			break;

		board[pos] = message[x];
		pos += 32;
		y++;

	}

	addr = NameTable;
	msx_vwrite(board, addr, 768);
	return 0;
}

int print_trace(char *message)
{
	int x,y, pos;

	if (trace_on)
	{
		line++;
		if (line >22)
		{
			line = 0;
			for (y = 0; y < 22 + 1; y++)
			{
				for (x = 0; x < WHITE_START_X + LINE_WIDTH; x++)
				{
					pos = y * 32 + x;
					board[pos] = ' ';
				}
			}
		}

		print(0, line, message);
	}
	return 0;
}

int print_error(char *message)
{
	int x, y, pos;

	for (y = 21; y < 24; y++) for (x = 0; x < 32; x++)
	{
		pos = y * 32 + x;
		board[pos] = ' ';
	}
	x = strlen(message) / 2 + 16;

	print(x, y, message);
	
	delay(5);

	return 0;
}

int print_info(char *message)
{
	int x, y, pos;

	for (y = 21; y < 24; y++) for (x = 0; x < 32; x++)
	{
		pos = y * 32 + x;
		board[pos] = ' ';
	}
	x = 16 - strlen(message) / 2;
	if (x < 0)
		x = 0;
	y = status_y;

	print(x, y, message);

	return 0;

}

int print_no_clear(int y, char *message)
{
	int x;

	x = 16 - strlen(message) / 2;
	if (x < 0)
		x = 0;

	print(x, y, message);

	return 0;
}

int print_white_line(char *message)
{
	int x,y,pos;
	char c;
	if (white_line > LINE_STOP)
	{
		for(y=LINE_START; y<LINE_STOP+1; y++)
		{
			for (x = WHITE_START_X; x < WHITE_START_X + LINE_WIDTH; x++)
			{
				pos = y * 32 + x;
				board[pos] = WHITE_LINE_COLOUR;
			}
		}
		white_line = LINE_START;
	}

	for (x = 0; x < strlen(message); x++)
	{
		c = message[x];
		if (c >= '1' && c <= '8')
		{
			c = c - '0';
			c = white_numbers[c];
		}
		else if (c == '-')
			c = white_numbers[0];
		message[x] = c;
	}

	print(WHITE_START_X +(LINE_WIDTH/2)-strlen(message)/2, white_line, message);
	white_line++;

	return 0;
}

int print_black_line(char *message)
{
	int x, y, pos;
	char c;
	if (black_line > LINE_STOP)
	{
		for (y = LINE_START; y < LINE_STOP + 1; y++)
		{
			for (x = BLACK_START_X; x < BLACK_START_X + LINE_WIDTH; x++)
			{
				pos = y * 32 + x;
				board[pos] = BLACK_LINE_COLOUR;
			}
		}
		black_line = LINE_START;
	}

	for(x=0; x<strlen(message); x++)
	{
		c = message[x];
		if (c>='1' && c<='8')
		{
			c = c - '0';
			c = black_numbers[c];
		} else
			if (c == '-')
				c = black_numbers[0];
		message[x] = c;
	}

	print(BLACK_START_X + (LINE_WIDTH / 2) - strlen(message) / 2, black_line, message);
	black_line++;

	return 0;
}

void init_msx_graphics()
{
	init_character_set();

	init_adam_sprites();

	vdp_color(BACKGROUND_COLOUR_GRAPHICS);
}

void newbrd()
{
	memcpy(board, newboard, sizeof(newboard));
}

void prtbrd(char b[64])
{
	unsigned int addr;
	int x,y, pos;
	char black_count[3], white_count[3];
	sprintf(black_count, "%2d", cntbrd(b, BLACK));
	sprintf(white_count, "%2d", cntbrd(b, WHITE));

	print(WHITE_START_X + 2, LINE_STOP+1, white_count);
	print(BLACK_START_X + 2, LINE_STOP+1, black_count);

	for (x = 0; x < 8; x++)
	{
		for(y=0; y<8; y++)
		{
			pos = y*64+x*2 + BOARD_START_Y*32 + BOARD_START_X;

			switch(b[y*8+x])
			{
				case BLACK:
					board[pos] 		= (char) (BLACK_TOP_LEFT     & 0xFF);
					board[pos + 1]  = (char) (BLACK_TOP_RIGHT    & 0xFF);
					board[pos + 32] = (char) (BLACK_BOTTOM_LEFT  & 0xFF);
					board[pos + 33] = (char) (BLACK_BOTTOM_RIGHT & 0xFF);
					break;
				case WHITE:
					board[pos] 		= (char) (WHITE_TOP_LEFT     & 0xFF);
					board[pos + 1]  = (char) (WHITE_TOP_RIGHT    & 0xFF);
					board[pos + 32] = (char) (WHITE_BOTTOM_LEFT  & 0xFF);
					board[pos + 33] = (char) (WHITE_BOTTOM_RIGHT & 0xFF);
					break;
				default:
					break;
			}
		}
	}

	addr = NameTable;
	msx_vwrite(board, addr, 768);
}
#else

void prtbrd(char b[64])
{
	int i,j;
#define TEXT 1
#ifdef GRAPHICS
#undef TEXT
	clg();
	printf("%c",12);
	for (i=0; i<8; i++) {
		putsprite(spr_or,127+i*16,2,&numbers[(i+1)*7]);
		putsprite(spr_or,112,16+i*16,&numbers[(i+1)*7]);
		for (j=0; j<8; j++) {
			putsprite(spr_or,120+i*16,10+j*16,frame);
			switch(b[i*8+j]) {
				case BLACK:
					putsprite(spr_or,120+j*16,10+i*16,blackpiece);
					break;
				case WHITE:
					putsprite(spr_or,120+j*16,10+i*16,whitepiece);
					break;
			}
		 }
	 }
#endif
#ifdef WIDEGRAPHICS
#undef TEXT
	clg();
	printf("%c",12);
	for (i=0; i<8; i++) {
		putsprite(spr_or,227+i*27,2,&numbers[(i+1)*7]);
		putsprite(spr_or,212,15+i*17,&numbers[(i+1)*7]);
		for (j=0; j<8; j++) {
			putsprite(spr_or,220+i*27,10+j*17,frame);
			switch(b[i*8+j]) {
				case BLACK:
					putsprite(spr_or,220+j*27,10+i*17,blackpiece);
					break;
				case WHITE:
					putsprite(spr_or,220+j*27,10+i*17,whitepiece);
					break;
			}
		 }
	 }
#endif
#ifdef SMALLGRAPHICS
#undef TEXT
	clg();
	printf ("%c",12);

	for (i=0; i<8; i++) {
		putsprite(spr_or,157+i*7,0,&numbers[(i+1)*7]);
		putsprite(spr_or,151,6+i*7,&numbers[(i+1)*7]);
		for (j=0; j<8; j++) {
			putsprite(spr_or,155+i*7,7+j*7,frame);
			switch(b[i*8+j]) {
				case BLACK:
					putsprite(spr_or,155+j*7,6+i*7,blackpiece);
					break;
				case WHITE:
					putsprite(spr_or,155+j*7,6+i*7,whitepiece);
					break;
			}
		 }
	 }
#endif
#ifdef TEXT
	#if defined (REDEFINED_FONT) || defined (ZX81_FONT) || defined (ZX80_FONT)
		printf("  12345678\n");
		for (i=0; i<8; i++) {
			printf(" %u",i+1);
			for (j=0; j<8; j++) {
			#if defined (ZX80_FONT) || defined (ZX81_FONT) || defined (ZX81_DKTRONICS) 
				zx_asciimode(0);
				switch(b[i*8+j]) {
					#ifdef ZX81_DKTRONICS
					case BLACK:
						putchar (G_BLACK);
						break;
					case WHITE:
						putchar (G_WHITE);
						break;
					default:
						putchar (G_EMPTY);
						break;
					#else
						#ifdef ZX80_FONT
						case BLACK:
							putchar (148);
							break;
						case WHITE:
							putchar (180);
							break;
						default:
							putchar (128+9*((i+j)&1));
							break;
						#else
						case BLACK:
							putchar (151);
							break;
						case WHITE:
							putchar (180);
							break;
						default:
							putchar (128+8*((i+j)&1));
							break;
						#endif
					#endif
				}
				zx_asciimode(1);
			#else
				switch(b[i*8+j]) {
					case BLACK:
						putchar (G_BLACK);
						break;
					case WHITE:
						putchar (G_WHITE);
						break;
					default:
						putchar (G_EMPTY);
						break;
				}
			#endif
			}
			putchar('\n');
		 }
	#else
	printf("   1 2 3 4 5 6 7 8\n");
	for (i=0; i<8; i++) {
		printf(" %u",i+1);
		for (j=0; j<8; j++) {
//#ifdef ANSITEXT
//#endif
			putchar(' ');
			putchar(b[i*8+j]);
		 }
		putchar('\n');
	 }
	 #endif
#endif
	putchar('\n');
}
#endif

#ifdef ADAM_OR_NABU

int prtscr(char b[64])
{
	int i, j;
	//char message[20];
	//sprintf(message, "%u-%u", i = cntbrd(b, his), j = cntbrd(b, mine));
	//print_info(message);

	i = cntbrd(b, his);
	j = cntbrd(b, mine); 
	return i - j;
}


char getmov_local(int *i, int *j)
{
	int joy = 0;
	int trig = 1;
//	int c;
	long joytimer = 0;

	movsprite(*i, *j, MOVING_COLOR);

	while(true)
	{
		joy = read_joystick(&trig);
		if (joytimer > JOYSTICK_DELAY)
		{
			joytimer = 0;

			joy = read_joystick(&trig);

			if (joy & UP)
				(*i)--;
			if (joy & DOWN)
				(*i)++;
			if (joy & LEFT)
				(*j)--;
			if (joy & RIGHT)
				(*j)++;
		} else
			joytimer++;

		//c = in_Inkey();
		/*
		c = 0;
		switch(c)
		{
			case 'I':
				(*y)--;
				break;
			case 'K':
				(*y)++;
				break;
			case 'J':
				(*x)--;
				break;
			case 'L':
				(*x)++;
				break;
			case 13:
				trig = 0;
				break;
			default:
				break;
		}
*/
		if (*j < 0)
			*j = 7;

		if (*j > 7)
			*j = 0;

		if (*i < 0)
			*i = 7;

		if (*i > 7)
			*i = 0;

		movsprite(*i, *j, MOVING_COLOR);

		if (trig == 0)
		{
			break;
		}
	}
	movsprite(*i, *j, SELECTED_COLOR);

	return 'M';
}

char getmov_remote(char b[64], int *i, int *j) 
{
	my_mov(b, his, mine, EMPTY, i, j);
	return 'M';
}

char getmov(int *i, int *j)
{
	char a, c;

	//selfplay = 'G';
	if (selfplay == 'G')
	{
		if (getk() == 0)
			return 'G';
		selfplay = ' ';
		getchar();
	}
	
	while (1)
		switch (c = skipbl())
		{
		case '\n':
			print_info("Move?  ");
			continue;
		case 'G':
			if ((c = skipbl()) != '\n')
				goto flush;
			selfplay = 'G';
			return 'G';
		case 'B':
		case 'S':
		case 'Q':
		case 'F':
		case 'A':
			a = c;
			if ((c = skipbl()) != '\n')
				goto flush;
			return a;
		case 'H':
			if ((a = c = skipbl()) == EMPTY)
				c = getchar();
			if (c < '1' || c > '4' || skipbl() != '\n')
				goto flush;
			*i = a == EMPTY ? -(c - '0') : (c - '0');
			return 'H';
		case 4:
			return c;
		default:
			if (c < '1' || c > '8')
				goto flush;
			*i = c - '1';
			c = skipbl();
			if (c < '1' || c > '8')
				goto flush;
			*j = c - '1';
			if ((c = skipbl()) == '\n')
				return 'M';
		flush:
			while (c != '\n' && c != 4)
				c = getchar();
			if (c == 4)
				return c;
			print_info("  Huh?? ");
		}
}

#ifdef BUILD_ADAM

char ask()
{
	char c=0, key;

	smartkeys_set_mode();

	smartkeys_display(NULL, NULL, NULL, NULL, "\n YES", "\n  NO");
	smartkeys_status("\n  Play again?");
	while (c == 0)
	{
		key = eos_read_keyboard();

		switch (key)
		{
		case SMARTKEY_V:
			c = 'Y';
			break;
		case SMARTKEY_VI:
			c = 'N';
			break;
		default:
			sound_negative_beep();
			break;
		}
	}

	smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);

	return c;
}

#endif

#ifdef BUILD_NABU

char ask()
{
	char c;

	vdp_color(BACKGROUND_COLOUR_TEXT);

	vdp_set_mode(mode_2);
	printf("Another game? (Y/N)");
	while (1)
	{
		c = toupper(getchar());
		if ((c == 'Y') || (c == 'N'))
			break;
	}
	return c;
}

#endif


#else

int prtscr(char b[64])
{
	int i, j;

	printf("%u-%u", i = cntbrd(b, his), j = cntbrd(b, mine));

	return i - j;
}

char getmov(int *i, int *j)
{
	char a,c;
	//int n;
	//char *p;
	/* char skipbl(); */
	//shift_right();
	
	if (selfplay == 'G') {
		if (getk()==0) return 'G';
		selfplay = ' ';
		getchar();
	}
	printf("Move: ");
	while(1) switch (c=skipbl()) {
		case '\n':
				printf("Move?  ");
				continue;
		case 'G': if ((c = skipbl()) != '\n')
				goto flush;
			selfplay='G';
			return 'G';
		case 'B': case 'S': case 'Q':
		case 'F': case 'A':
		  a=c;
		  if (( c = skipbl()) != '\n') goto flush;
		  return a;
		case 'H': if ((a=c=skipbl()) == EMPTY)
				c=getchar();
			if (c<'1' || c>'4' || skipbl() !='\n')
				goto flush;
			*i = a==EMPTY? -(c-'0') : (c-'0');
			return 'H';
		case 4: return c;
		default: if (c<'1' || c>'8') goto flush;
			*i = c-'1';
			c = skipbl();
			if (c<'1' || c>'8') goto flush;
			*j = c- '1';
			if ((c=skipbl()) == '\n') return 'M';
		flush:	while (c != '\n' && c != 4)
				c=getchar();
			if (c==4) return c;
			printf ("  Huh?? ");
		}
}

char ask()
{
	char a, c;
	printf("Another game? ");
	a = skipbl();
	while (c != '\n' && c != 4)
		c = getchar();
	return a;
}
#endif




void putmv1(char b[64], char p,int x, int y, int m, int n)
{
	while ((x += m) >= 0 && x<8 && (y += n)>=0 && y<8) {
		if (b[x*8+y] == EMPTY || b[x*8+y] == p) return;
		b[x*8+y] = p;
	 }
}


void putmov(char b[64], char p, int x, int y)
{
	int i,j;
	b[x*8+y] = p;
	for (i= -1; i<=1; i++) for (j= -1; j<=1; j++) {
		if ((i != 0 || j!=0)&&chkmv1(b,p,x,y,i,j)>0)
			putmv1(b,p,x,y,i,j);
	 }
}



char notak2(char b[64], char p,char o,char e, int x, int y,int m,int n)
{
	x += m; y +=n;
	if (x>=0 && x<=7 && y>=0 && y<=7)
		while(b[x*8+y] == 0) {
		 x += m; y+=n;
		 if (x<0 || x>7 || y<0 || y>7 || b[x*8+y]==e)
			return o;
		 }
	while (x>=0 && x<=7 && y>=0 && y<=7 && b[x*8+y]==p)
			{ x +=m; y+=n; }
	if (x<0 || x>7 || y<0 || y>7) return p;
	return b[x*8+y];
}

char notak1(char b[64], char p,char o,char e,int x,int y,int m,int n)
{
	int c1,c2;
	c1 = notak2(b,p,o,e,x,y,m,n);
	c2 = notak2(b,p,o,e,x,y,-m,-n);
	return !(c1==o && c2==e || c1==e && c2==o);
}


char notake(char b[64],char p,char o,char e,int x,int y)
{
	return notak1(b,p,o,e,x,y,0,1)&&
		notak1(b,p,o,e,x,y,1,1)&&
		notak1(b,p,o,e,x,y,1,0)&&
		notak1(b,p,o,e,x,y,1,-1);
}


char s_move(char b[64], char p, char o, char e, int i, int j)
{
	char a[64];
	int ok,s,k,l,side,oside;
	int c,dkl;
	cpybrd(a,b);
	putmov(a,p,i,j);
	side = 0;
	if (i==0 || i==7) side++;
	if (j==0 || j==7) side++;
	s = 0;
	ok = 0;
	if (side==2 || notake(b,p,o,e,i,j)) ok++;
	oside = 0;
	for (k=0; k<8; k++) for(l=0; l<8; l++)
	 {
		c=chkmov(a,o,k,l);
		if (c==0) continue;
		dkl = 1;
		if (k==0 || k==7) { dkl+=2; oside|=4;}
		if (l==0 || l==7) {dkl+=2; oside|=4; }
		if (dkl==5) {dkl = 10; oside |= 16; }
			else if (!notake(a,o,p,e,k,l))
					continue;
		oside |= 1;
		s -= dkl;
		if (c>=10) { s -= 4; oside |= 8; }
		}
	if (s< -oside) s= -oside;
	if (side>0) return s+side-7+10*ok;
	if (i==1 || i==6) {s--; side++;}
	if (j==1 || j==6) {s--; side++;}
	if (side>0) return s;
	if (i==2 || i==5) s++;
	if (j==2 || j==5) s++;
	return s;
}


int fillmt(char b[64], char p, char o, char e, struct mt t[64])
{
	int i,j,k;
	k = 0;
	for (i=0; i<8; i++) for(j=0; j<8; j++)
	   if (t[k].c = chkmov(b,p,i,j)) {
			t[k].x =i;
			t[k].y =j;
			t[k].s = s_move(b,p,o,e,i,j);
			++k;
		}
	return k;
}

void analyze(char b[64], char p, char o, char e)
{
	struct mt  t[64];
	char a[64];
	int i,k,c;
	k = fillmt(b,p,o,e,t);
	cpybrd(a,b);
	for (i=0; i<k; i++)
	  a[t[i].x*8+t[i].y] = ((c = 'F' - t[i].s) <= 'Z')?c:'Z';
	prtbrd(a);
}


int cmpmov(struct mt  *a,struct mt  *b)
{
	if ((*a).s > (*b).s) return -1;
	if ((*a).s < (*b).s) return 1;
	if ((*a).c > (*b).c) return -1;
	if ((*a).c < (*b).c) return 1;
	return 0;
}


int my_mov(char b[64], char p,char o,char e,int *m,int *n)
{
	struct mt  t[64];
	int i,k;

	k = fillmt(b,p,o,e,t);
	if (!k) return 0;
	qsort (t, k, sizeof(struct mt), cmpmov);
	for (i=1; i<k; i++)
		if (t[i].s != t[0].s || t[i].c != t[0].c)
			break;
	k = rand() % i;
	*m = t[k].x;
	*n = t[k].y;
	return 1;
}

#ifdef ADAM_OR_NABU

int game(char b[64], int game_type)
{
	char c;
	int ff;
	int i=7, j=7;
	int BlacksTurn = true;
	int blacks_moves, whites_moves;
	char temp[32];
	handicap = 0;
	selfplay = ' ';
	ff = 0;



	if (mefirst)
	{
		mine = BLACK;
		his = WHITE;
	}
	else
	{
		mine = WHITE;
		his = BLACK;
	}

	while (1)
	{
		blacks_moves = chkmvs(b, BLACK);
		whites_moves = chkmvs(b, WHITE);

		if (blacks_moves + whites_moves == 0)
			break;
		
		// black
		if (BlacksTurn)
		{
			print(TITLE_X, TITLE_Y,  black_reversi);
			print(TITLE_X, TITLE_Y+1,mirror_white_reversi);

			print_info("Black's Turn");

			if (blacks_moves == 0)
			{
				print_black_line("Forfeit");
			} else
			{
				if (mine == BLACK)
				{
					sprintf(temp, "%s's Move", my_name);
					print_info(temp);
					c = getmov_local(&i,&j);
				} else
				{
					sprintf(temp, "%s's Move", their_name);
					print_info(temp);
					switch(game_type)
					{
						case COMPUTER_OPPONENT:
							c = my_mov(b, his, mine, EMPTY, &i, &j);
							break;
						case LOCAL_OPPONENT:
							c = getmov_local(&i, &j);
							break;
						case OTHER_HOSTED:
						case HOSTING_GAME:
							c = getmov_remote(b, &i, &j);
							break;
					}
				}

				if (chkmov(b, BLACK, i, j) > 0)
				{
					sprintf(temp, "%u-%u", j + 1, i + 1);
					print_black_line(temp);

					movsprite(i, j, SELECTED_COLOR);
					putmov(b, BLACK, i, j);
				} else
				{
					print_info("***Illegal Move ***");
					sound_negative_beep();
					delay(2);
					continue;
				}
			}
		} else
		{
			print(TITLE_X, TITLE_Y, white_reversi);
			print(TITLE_X, TITLE_Y + 1, mirror_black_reversi);

			print_info("White's Turn");

			if (whites_moves == 0)
			{
				print_white_line("Forfeit");
			}
			else
			{
				if (mine == WHITE)
				{
					sprintf(temp, "%s's Move", my_name);
					print_info(temp);
					c = getmov_local(&i, &j);
				}
				else
				{

					sprintf(temp, "%s's Move", their_name);
					print_info(temp);
					switch (game_type)
					{
					case COMPUTER_OPPONENT:
							c = my_mov(b, his, mine, EMPTY, &i, &j);
							break;
					case LOCAL_OPPONENT:
							c = getmov_local(&i, &j);
							break;
					case OTHER_HOSTED:
					case HOSTING_GAME:
							c = getmov_remote(b, &i, &j);
							break;
					}
				}
				if (chkmov(b, WHITE, i, j) > 0)
				{
					sprintf(temp, "%u-%u", j + 1, i + 1);
					print_white_line(temp);

					movsprite(i, j, SELECTED_COLOR);
					putmov(b, WHITE, i, j);
				}
				else
				{
					print_info("*** Illegal Move ***");
					sound_negative_beep();
					delay(2);
					continue;
				}
			}
		} // if BlacksTurn
		prtbrd(b);
		BlacksTurn = ! BlacksTurn;
	} // while

	return 0;
}


#else
int game(char b[64],int n)
{
	char c;
	int ff;
	int i,j;
	handicap = 0;
	selfplay = ' ';
	ff=0;
	
	if (mefirst) {
		mine = BLACK; his = WHITE;
		printf("I go first:\n");
	}
	else {
		mine = WHITE; his = BLACK;
		printf("You go first:\n");
	}


	while(1) {
		if (cntbrd(b,EMPTY)==0) return 'D';
		if (cntbrd(b,EMPTY)==60 && mine == BLACK) goto Istart;
		if (chkmvs(b,his)==0) {
                printf(!mefirst ? "Forfeit" : "   ...Forfeit\n");
							ff |= 1;
		}
		else switch (c = getmov(&i,&j)) {
		case 'B': prtbrd(b); continue;
		case 'S': i= prtscr(b);
			if (i>0)
		printf(" You're winning\n");
			else 
				if (i < 0)
		printf(" You're losing!\n");
				//else 
				//putchar('\n');
			continue;
		case 'Q': case 4: return c;

		case 'H': if (n>(unsigned int)(handicap)+4)
		printf("Illegal!\n");
						else for (j = 0; i != 0; j++)
					{
						b[h[j * 2] * 8 + h[j * 2 + 2]] = i > 0 ? BLACK : WHITE;
						handicap += i > 0 ? 1 : -1;
						++n;
						i += i > 0 ? -1 : 1;
					}
			prtbrd(b); continue;
		case 'A': analyze(b,his,mine,EMPTY);
			continue;
		case 'G': my_mov(b,his,mine,EMPTY,&i,&j);
		case 'M': if (chkmov(b,his,i,j)>0) {
		printf(!mefirst ? "%u-%u" : "   ...%u-%u\n",
				i+1,j+1);
	putmov(b, his, i, j);
			}
			else {
		printf("Illegal!\n");
				continue;
			}
			break;
		case 'F': if (n>(unsigned int)(handicap)+4) {
		printf("Illegal!\n");
				continue;
			}
			else
		printf(!mefirst ? "Forfeit":
						 "   ...Forfeit\n");
		}
Istart:		if (cntbrd(b,EMPTY) == 0) return 'D';
		if (chkmvs(b,mine)==0) {
		printf(!mefirst ? "...Forfeit\n" : "Forfeit...\n");
			ff |= 2;
		}
		else {
			my_mov(b,mine,his,EMPTY,&i,&j);
		printf(!mefirst ? "...%u-%u\n" : "%u-%u...\n",
				i+1,j+1);
	putmov(b, mine, i, j);
	++n;
		}
		if (ff==3 || n>64) return 'D';
		if (!(ff & 1)) prtbrd(b);
		ff = 0;
	}
}
#endif

#ifdef ADAM_OR_NABU
void translate_from_atari(char *atari_input, int *x, int *y, int *trig)
{
	char message[32];
	sscanf(atari_input, "%d,%d,%d", x,y,trig);

	print_info(atari_input);
	sprintf(message, "x:%d y:%d trig:%d",*x,*y,*trig);
	print_no_clear(status_y-1,message);
}

#endif


int main()
{
	char b[64];
	int i;
	char key;
	char message[32];
	int  game_type;
	int  waiting;
	int  connection = 0;
	char waitingstr[] = {"|/-\\"};
	

/*
	h[0][0] = h[0][1] = h[2][0] = h[3][1] = 0;
	h[1][0] = h[1][1] = h[2][1] = h[3][0] = 7;
*/

	h[0] = h[1] = h[4] = h[7] = 0;
	h[2] = h[3] = h[5] = h[6] = 7;

#ifdef  REDEFINED_FONT

	#ifdef __SPECTRUM__
		/* set console driver for 32 columns mode */
		printf("%c%c",1,32);
		/* INK 7 */
		printf("%c7",16);
		/* PAPER 0 */
		printf("%c0",17);
		/* Copy graphics in UDG area */
		memcpy(65368, whitepiece,24);	
		zx_border(0);	
	#endif

	#ifdef __ACE__
		memcpy(0x2c00, whitepiece,24);
	#endif

	#ifdef __SORCERER__
		memcpy(0xfe08, whitepiece,24);
	#endif

	#ifdef __TRS80__
		memcpy(0xf400, whitepiece,24);
	#endif

#endif

#ifdef ADAM_OR_NABU

		vdp_color(BACKGROUND_COLOUR_TEXT);

		vdp_set_mode(mode_2);

#ifdef BUILD_ADAM
		smartkeys_set_mode();
		smartkeys_sound_init();
#endif

		printf(
#ifdef BUILD_ADAM
			"#FUJINET REVERSI - ADAM EDITION\n"
#else
			"#FUJINET REVERSI - NABU EDITION\n"
#endif
			"      Adapted for Fujinet\n"
			"        by Norman Davie\n\n"
			" Reversi is a strategy game\n"
			"invented during the Victorian\n"
			"era.  The goal: to have the\n"
			"majority of pieces on the board\n"
			"your own colour.  Trapping\n"
			"opposing pieces between your\n"
			"coloured pieces converts them\n"
			"to your colour."
			"\n\n"
			"HOST/BLACK is always the FIRST\n"
			"player\n");
#else
#ifdef ANSITEXT
	printf("%c\nWelcome to the %c[7m OTHELLO %c[27m program!\n",12,27,27);
	printf("\nNote: %c[4m BLACK ALWAYS GOES FIRST\n    %c[24m ...Good luck!!!\n\n\n",27,27);
#else
	printf("%c\nWelcome to the OTHELLO program!\n",12);
	printf("\nNote: BLACK ALWAYS GOES FIRST\n     ...Good luck!!!\n\n\n");
#endif
#endif

#ifdef ADAM_OR_NABU

	printf("\nThis program uses TCP port 6502.\n\n");

#ifdef BUILD_ADAM

	sound_mode_change();

	smartkeys_display("1 Player\n  Local", "2 Player\n  Local", "  Host\n  Game", " Remote\n  Host", NULL, "  QUIT");

	game_type = 0;
	while (game_type == 0)
	{
		key = eos_read_keyboard();

		switch (key)
		{
		case SMARTKEY_I:
			game_type = COMPUTER_OPPONENT;
			break;
		case SMARTKEY_II:
			game_type = LOCAL_OPPONENT;
			break;
		case SMARTKEY_III:
			game_type = HOSTING_GAME;
			strcpy(host, "");
			break;
		case SMARTKEY_IV:
			game_type = OTHER_HOSTED;
			break;
		case SMARTKEY_VI:
			game_type = 0;
			break;
		default:
			sound_negative_beep();
			break;
		}
	}

	smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
#else

	printf("Press any key to continue");
	getchar();

	vdp_color(BACKGROUND_COLOUR_TEXT);
	vdp_set_mode(mode_2);

	sound_mode_change();
	printf("\nPress Number to make your\nselection:\n");
	printf("(1) 1 Player vs Computer\n");
	printf("(2) 2 Players locally\n");
	//printf("(3) 2 Players, I will host (You will be Black)\n");
	//printf("(4) 2 Players, They will host (They will be Black)\n");
	printf("(5) Quit\n");

	game_type = 0;
	while (game_type == 0)
	{
		key = getchar();

		switch (key)
		{
		case '1':
			game_type = COMPUTER_OPPONENT;
			break;
		case '2':
			game_type = LOCAL_OPPONENT;
			break;
		case '3':
			game_type = HOSTING_GAME;
			strcpy(host, "");
			break;
		case '4':
			game_type = OTHER_HOSTED;
			break;
		case '5':
			game_type = 0;
			break;
		default:
			sound_negative_beep();
			break;
		}
	}

#endif

#if BUILD_ADAM
	if ((game_type == HOSTING_GAME) || (game_type == OTHER_HOSTED))
	{

		if (game_type == OTHER_HOSTED)
		{

			smartkeys_status("Enter the IP of the host computer:");
			gets(host);
			host[strlen(host) - 1] = '\0';
		}
		sprintf(url, "TCP://%s:6502/", host);

		vdp_color(BACKGROUND_COLOUR_GRAPHICS);

		vdp_set_mode(mode_2);

		smartkeys_status("WAITING FOR CONNECTION...");

		if (strcmp(host, "") == 0)
		{
			waiting = 0;
			connection = false;
			while (!connection)
			{
				printf("%c%c", waitingstr[waiting], 8);
				connection = true;
				waiting++;
				if (waiting > 4)
					waiting = 0;
			}
			mefirst = 1;
		}
		else
		{
			mefirst = 0;
		}
	}
#else
	if ((game_type == HOSTING_GAME) || (game_type == OTHER_HOSTED))	
	{

		if (game_type == OTHER_HOSTED)
		{
			
			printf("Enter the IP of the host:\n");
			gets(host);
			host[strlen(host)-1] = '\0';
		}
		sprintf(url, "TCP://%s:6502/", host);

		vdp_color(BACKGROUND_COLOUR_GRAPHICS);

		vdp_set_mode(mode_2);

		printf("Opening %s\n", url);

		printf("WAITING FOR CONNECTION... ");
		
		if (strcmp(host, "") == 0)
		{
			waiting = 0;
			connection = false;
			while(! connection)
			{
				printf("%c%c", waitingstr[waiting], 8);
				connection = true;
				waiting++;
				if (waiting > 4)
					waiting = 0;
			}
			mefirst = 1;
		}
		else
		{
			mefirst = 0;
		}
	}
#endif
	
#else
	printf("Do you want to go first? ");
	if (toupper(getchar()) == 'Y') 
		mefirst = 1;
	else
		mefirst = 0;

#endif

#ifdef ADAM_OR_NABU
#ifdef BUILD_ADAM

	if (game_type == LOCAL_OPPONENT)
	{
		smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
		smartkeys_status("What is the name of Player 1?");
		gets(my_name);
		my_name[strlen(my_name) - 1] = '\0';
		my_name[8] = '\0';

		smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
		smartkeys_status("What is the name of Player 2?");
		gets(their_name);
		their_name[strlen(their_name) - 1] = '\0';
		their_name[8] = '\0';
	}
	else
	{
		smartkeys_status("What is your name?");
		gets(my_name);
		my_name[strlen(my_name) - 1] = '\0';
		my_name[8] = '\0';

		if (game_type == COMPUTER_OPPONENT)
		{
			strcpy(their_name, "Computer");
		}
		else
		{
			strcpy(their_name, "Remote");
		}
	}

	if ((game_type == LOCAL_OPPONENT) || (game_type == COMPUTER_OPPONENT))
	{
		char temp[32];
		sprintf(temp, "\n  Does %s want to play first?", my_name);
		smartkeys_display(NULL, NULL, NULL, NULL,"YES","NO");
		smartkeys_status(temp);
		key = eos_read_keyboard();

		while(1)
		{
			if (key == SMARTKEY_V)
			{
				mefirst = 1;
				break;
			}
			if (key == SMARTKEY_VI)
			{
				mefirst = 0;
				break;
			}
		}

	}

#else
	printf("\n");
	strcpy(their_name, "Computer");
	if (game_type == LOCAL_OPPONENT)
	{
		printf("What is the name of Player 1?\n");
		gets(my_name);
		my_name[strlen(my_name)-1] = '\0';
		my_name[8] = '\0';

		printf("What is the name of Player 2?\n");
		gets(their_name);
		their_name[strlen(their_name) - 1] = '\0';
		their_name[8] = '\0';

	}
	else
	{
		printf("What is your name?\n");
		gets(my_name);
		my_name[strlen(my_name) - 1] = '\0';
		my_name[8] = '\0';

		if (game_type == COMPUTER_OPPONENT)
		{
			strcpy(their_name, "Computer");
		}
		else
		{
			strcpy(their_name, "Remote");
		}
	}

	if ((game_type == LOCAL_OPPONENT) || (game_type == COMPUTER_OPPONENT))
	{
		printf("Does %s want to play first? (Y/N)", my_name);
		mefirst = (toupper(getchar()) == 'Y');
	}

#endif
#endif

#ifdef ZX81_DKTRONICS
#asm
		ld a,
		$20
			ld i,
		a
#endasm
#endif

		// srand( (unsigned)clock() );

	do 
	{
#ifdef ADAM_OR_NABU
		init_msx_graphics();
		newbrd();
#endif

		clrbrd(b);
#ifdef ADAM_OR_NABU
		if (mefirst)
		{
			print_down(0,  LINE_START + 1, their_name);
			print_down(31, LINE_START + 1, my_name);
		}
		else
		{
			print_down(0,  LINE_START + 1, my_name);
			print_down(31, LINE_START + 1, their_name);
		}

#else
		printf("\n");
#endif
		prtbrd(b);

#ifdef ADAM_OR_NABU
		i = game(b, game_type);
#else
		i = game(b, 4);
#endif


		mefirst = !mefirst;
		if (i==4) break;
		if (i=='Q') continue;
		//printf("\n");
		i = prtscr(b);

#ifdef ADAM_OR_NABU


		if (i>0) 
			sprintf(message, "You lost by %u", i);
		else 
			if (i < 0)
				sprintf(message, "You won by %u", -i);
			else 
				sprintf(message, " A draw");

		print_info(message);
		delay(5);

#else
		if (i > 0)
			printf(" You won by %u\n", i);
		else if (i < 0)
				printf(" You lost by %u\n", -i);
			 else
				printf(" A draw\n");
#endif
	} while (ask()=='Y');

	return 0;
}



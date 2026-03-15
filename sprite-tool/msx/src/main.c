/**
 * @brief   sprite-tool, a sprite editor.
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 * @verbose demonstrates how to use FujiNet in a Development Workflow
 */

#include <fujinet-network.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HANDLE_CURSOR 0
#define COLOR_CURSOR  9

#define DEL 0x7F

unsigned char cursor[] = {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// Example sprites
uint8_t spaceship[] = {3, 3, 3, 7, 7, 0xc7, 0xc7, 0xe7, 0xee, 0xee, 0xfe, 0xff, 0xe7, 0xc3, 0xf0, 0x90,
                       0xc0, 0xc0, 0xc0, 0xe0, 0xe0, 0xe3, 0xe3, 0xe7, 0x77, 0x77, 0x7f, 0xff, 0xe7, 0xc3, 0x0f, 9 };

uint8_t fire[] = {0x18, 0x3C, 0x3C, 0x7E, 0x7E, 0x7E, 0x76, 0x54, 0x10, 0x10, 0x10, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char sprite_buffers[8][32];
unsigned char sprite_colors[8] = {2,3,4,5,6,7,8,9};

char *url[256];

struct _pos
{
    unsigned char x;
    unsigned char y;
} sprite_pos[8] =
    {
        {160,0},
        {192,0},
        {160,32},
        {192,32},
        {160,64},
        {192,64},
        {160,96},
        {192,96},
    };

int cursor_x, cursor_y, active_sprite=0;

enum _displayQuadrant
    {
        UPPER_LEFT,
        LOWER_LEFT,
        UPPER_RIGHT,
        LOWER_RIGHT
    } displayQuadrant;

enum _direction
    {
        RIGHT = 0x1C,
        LEFT,
        UP,
        DOWN
    } direction;

void video_default_color()
{
    vdp_color(3,1,1);
}

void video_alt_color()
{
    vdp_color(9,1,1);
}

void video_inverse_color()
{
    vdp_color(1,3,1);
}

void video_status_color()
{
    vdp_color(5,1,1);
    gotoxy(0,19);
    cprintf("%-64s"," ");
    gotoxy(0,19);
}

void video_setup()
{
    // Set up video mode
    vdp_set_mode(2);
    video_default_color();
    clrscr();

    // Set up cursor
    vdp_set_sprite_mode(2);
    vdp_set_sprite_8(HANDLE_CURSOR,cursor);
}

void display_palette()
{
    for (unsigned char i=0;i<16;i++)
    {
        video_default_color();
        gotoxy(29,i);

        if (i==sprite_colors[active_sprite])
            vdp_color(9,1,1);

        cprintf("%2d",i);

        vdp_color(15,i,1);

        if (i==0)
            cputc('T');
        else
            cputc(0x20); // Space.
    }
}

void clear_command_display(void)
{
    video_default_color();
    gotoxy(0,21);
    cprintf("%-64s","");
}

void display_command(const char *key, const char *verb)
{
    video_alt_color();
    cputs(key);

    video_default_color();
    cputs(verb);

    cputc(' ');
}

void display_commands()
{
    gotoxy(0,21);

    display_command("ARROWS","MOVE CURSOR");
    display_command("1-8","SEL SPRITE");
    display_command("M","OVE SPRITE");
    display_command("S","IZE");
    display_command("I","MPORT ");
    display_command("E","XPORT");
}

void print_active_pixel(bool isActive)
{
    if (isActive)
    {
        vdp_color(sprite_colors[active_sprite],sprite_colors[active_sprite],1);
        cputc(' ');
    }
    else
    {
        vdp_color(sprite_colors[active_sprite],1,1);
        cputc('.');
    }
}

void update_display_quadrant(enum _displayQuadrant dq)
{
    int _x,_y,_offset;

    switch(dq)
    {
    case UPPER_LEFT:
        _x = _y = 0;
        _offset=0;
        break;
    case LOWER_LEFT:
        _x = 0;
        _y = 8;
        _offset=8;
        break;
    case UPPER_RIGHT:
        _x = 8;
        _y = 0;
        _offset=16;
        break;
    case LOWER_RIGHT:
        _x = _y = 8;
        _offset=24;
        break;
    }

    for (unsigned char iy=0; iy<8; iy++)
    {
        unsigned char b = sprite_buffers[active_sprite][iy+_offset];

        gotoxy(_x,_y++);

        for (unsigned char ix=0; ix<8; ix++)
        {
            if (b & 0x80) // is leftmost bit set?
            {
                print_active_pixel(true);
            }
            else
            {
                print_active_pixel(false);
            }

            b <<= 1; // Shift next bit into leftmost spot.
        }
    }
}

void update_sprite(unsigned char spriteNo)
{
    vdp_set_sprite_16(spriteNo+1,sprite_buffers[spriteNo]);
    vdp_put_sprite_16(spriteNo+1,
                      sprite_pos[spriteNo].x,
                      sprite_pos[spriteNo].y,
                      spriteNo+1,
                      sprite_colors[spriteNo]);
}

void update_all_sprites()
{
    for (int i=0;i<8;i++)
        update_sprite(i);
}

void update_display()
{
    gotoxy(0,0);

    update_display_quadrant(UPPER_LEFT);
    update_display_quadrant(LOWER_LEFT);
    update_display_quadrant(UPPER_RIGHT);
    update_display_quadrant(LOWER_RIGHT);
    update_sprite(active_sprite);
    display_palette();
}

void update_cursor()
{
    vdp_put_sprite_16(0,cursor_x<<3,cursor_y<<3,HANDLE_CURSOR,COLOR_CURSOR);
}

void move_cursor(enum _direction direction)
{
    switch (direction)
    {
    case UP:
        cursor_y--;
        break;
    case DOWN:
        cursor_y++;
        break;
    case LEFT:
        cursor_x--;
        break;
    case RIGHT:
        cursor_x++;
        break;
    }

    // wrap around if needed.
    if (cursor_y<0)
        cursor_y=15;
    else if (cursor_y>15)
        cursor_y=0;

    if (cursor_x<0)
        cursor_x=15;
    else if (cursor_x>15)
        cursor_x=0;

    update_cursor();
}

void toggle_bit()
{
    enum _displayQuadrant currentQuadrant;
    int offset = 0;
    int _x = cursor_x;
    int _y = cursor_y;
    unsigned char x_xor_table[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

    if (_x<8 && _y<8)
    {
        currentQuadrant = UPPER_LEFT;
        // No coordinate adjustments needed.
    }
    else if (_x>7 && _y<8)
    {
        currentQuadrant = UPPER_RIGHT;
        offset = 16;
        _x -= 8;
    }
    else if (_x<8 && _y>7)
    {
        currentQuadrant = LOWER_LEFT;
        offset = 8;
        _y -= 8;
    }
    else if (_x>7 && _y>7)
    {
        currentQuadrant = LOWER_RIGHT;
        offset = 24;
        _x -= 8;
        _y -= 8;
    }

    sprite_buffers[active_sprite][_y+offset] ^= x_xor_table[_x];
    update_display_quadrant(currentQuadrant); // so it's faster.
    update_sprite(active_sprite);
}

void set_sprite_color()
{
    char tmp[8] = {0,0,0,0,0,0,0,0};

    clear_command_display();
    gotoxy(0,21);
    cprintf("SET SPRITE %d COLOR TO (0-15): ",active_sprite+1);
    cgets(tmp);
    sprite_colors[active_sprite] = atoi(tmp);
    display_commands();
    gotoxy(0,19);
    video_status_color();
    cprintf("SPRITE %d COLOR SET TO %d.",active_sprite,sprite_colors[active_sprite]);
    update_display();
    display_palette();
}

void erase_sprite()
{
    int k=0;

    clear_command_display();
    gotoxy(0,21);
    cprintf("PRESS 'Y' TO CLEAR SPRITE\nANY OTHER KEY ABORTS.");

    k=cgetc();

    if (k == 'y' || k == 'Y')
    {
        gotoxy(0,19);
        video_status_color();
        cprintf("SPRITE %d CLEARED.",active_sprite+1);
        memset(sprite_buffers[active_sprite],0x00,32);
        update_display();
    }
    else
    {
        gotoxy(0,19);
        video_status_color();
        cprintf("SPRITE %d CLEAR ABORTED.",active_sprite+1);
    }

    display_commands();
}

void invert_sprite()
{
    for (int i=0;i<32;i++)
        sprite_buffers[active_sprite][i] ^= 0xFF;

    update_display();

    gotoxy(0,19);
    video_status_color();
    cprintf("SPRITE %d INVERTED.",active_sprite+1);
}

void set_active_sprite(int newSprite)
{
    active_sprite = newSprite;

    gotoxy(0,19);
    video_status_color();
    cprintf("ACTIVE SPRITE SET TO #%d",active_sprite+1);

    update_display();
}

void spaceship_to_current_sprite()
{
    gotoxy(0,19);
    video_status_color();
    cprintf("EXAMPLE SPRITE TO SPRITE #%d",active_sprite+1);

    memcpy(sprite_buffers[active_sprite],spaceship,sizeof(spaceship));
    update_display();
}

void move_sprite()
{
    int _x=0;
    int _y=0;

    clear_command_display();
    gotoxy(0,21);
    video_default_color();
    cprintf("ARROWS TO MOVE. ENTER CONFIRMS.");

    while(1)
    {
        int k = cgetc();

        _x = sprite_pos[active_sprite].x;
        _y = sprite_pos[active_sprite].y;

        switch(k)
        {
        case UP:
            _y--;
            break;
        case DOWN:
            _y++;
            break;
        case LEFT:
            _x--;
            break;
        case RIGHT:
            _x++;
            break;
        case '\n':
            goto end_move;
        }

        vdp_put_sprite_16(active_sprite+1,_x,_y,active_sprite+1,sprite_colors[active_sprite]);

        sprite_pos[active_sprite].x = _x;
        sprite_pos[active_sprite].y = _y;
    }

 end_move:
    display_commands();
}

void populate_example_sprites()
{
    memcpy(sprite_buffers[0],spaceship,32);
    memcpy(sprite_buffers[1],spaceship,32);
    memcpy(sprite_buffers[2],spaceship,32);
    memcpy(sprite_buffers[3],spaceship,32);
    memcpy(sprite_buffers[4],spaceship,32);
    memcpy(sprite_buffers[5],spaceship,32);
    memcpy(sprite_buffers[6],spaceship,32);
    memcpy(sprite_buffers[7],spaceship,32);
    sprite_colors[0]=0x0F;
}

void export(void)
{
    int k=0;
    unsigned char *buf=NULL;
    unsigned short len=0;

    clear_command_display();
    video_default_color();
    gotoxy(0,21);
    cprintf("EXPORT (C)URRENT OR (A)LL?");

    k=cgetc();

    switch(k)
    {
    case 'C':
    case 'c':
        gotoxy(0,19);
        video_status_color();
        cprintf("EXPORT SPRITE #%d",active_sprite+1);
        buf=(unsigned char *)&sprite_buffers[active_sprite];
        len=32;
        break;
    case 'A':
    case 'a':
        gotoxy(0,19);
        video_status_color();
        cprintf("EXPORT ALL SPRITES");
        buf=(unsigned char *)&sprite_buffers[0];
        len=256;
        break;
    default:
        break;
    }

    clear_command_display();
    video_default_color();
    gotoxy(0,21);
    cprintf("N: URL? ");
    cgets(url);

    clear_command_display();
    video_default_color();
    gotoxy(0,21);
    cprintf("EXPORTING...");

    network_open(url,8,0);
    network_write(url,buf,len);
    network_close(url);

    gotoxy(0,19);
    video_status_color();
    cprintf("EXPORTED TO: %s",url);
    display_commands();
}

void import(void)
{
    int k=0;
    unsigned char buf[256];
    unsigned short len=0;

    memset(buf,0,sizeof(buf));

    clear_command_display();
    video_default_color();
    gotoxy(0,21);
    cprintf("IMPORT (C)URRENT OR (A)LL?");

    k=cgetc();

    switch(k)
    {
    case 'C':
    case 'c':
        gotoxy(0,19);
        video_status_color();
        cprintf("IMPORT SPRITE #%d",active_sprite+1);
        len=32;
        break;
    case 'A':
    case 'a':
        gotoxy(0,19);
        video_status_color();
        cprintf("EXPORT ALL SPRITES");
        len=256;
        break;
    default:
        break;
    }

    clear_command_display();
    video_default_color();
    gotoxy(0,21);
    cprintf("N: URL? ");
    cgets(url);

    clear_command_display();
    video_default_color();
    gotoxy(0,21);
    cprintf("IMPORTING...");

    network_open(url,12,0);
    network_read(url,&buf,len);
    network_close(url);

    if (k=='A' || k=='a')
    {
        memcpy(&sprite_buffers[0],&buf,256);
    }
    else
    {
        memcpy(&sprite_buffers[active_sprite],&buf,32);
    }

    gotoxy(0,19);
    video_status_color();
    cprintf("IMPORTED FROM: %s",url);
    update_display();

    if (k=='A' || k=='a')
        update_all_sprites();

    display_commands();
}

void main()
{
    video_setup();
    populate_example_sprites();
    update_all_sprites();
    display_commands();
    update_display();
    update_cursor();

    while(1)
    {
        int k = cgetc();

        switch (k)
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            set_active_sprite(k-'1');
            break;
        case UP:
            move_cursor(UP);
            break;
        case DOWN:
            move_cursor(DOWN);
            break;
        case LEFT:
            move_cursor(LEFT);
            break;
        case RIGHT:
            move_cursor(RIGHT);
            break;
        case 'c':
        case 'C':
            set_sprite_color();
            break;
        case '!':
            invert_sprite();
            break;
        case DEL:
            erase_sprite();
            break;
        case ' ':
            toggle_bit();
            break;
        case '^':
            spaceship_to_current_sprite();
            break;
        case 'm':
        case 'M':
            move_sprite();
            break;
        case 'e':
        case 'E':
            export();
            break;
        case 'i':
        case 'I':
            import();
            break;
        }
    }
}

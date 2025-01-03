/**
 * @brief Playfield test.
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <conio.h>
#include <video/tms99x8.h>
#include <sys/ioctl.h>
#include <eos.h>
#include <smartkeys.h>
#include "udgs.h"

void load_udgs()
{
    void *param = &udgs;
    console_ioctl(IOCTL_GENCON_SET_UDGS,&param);
}

void main(void)
{
	smartkeys_set_mode();
        load_udgs();
        vdp_color(VDP_INK_WHITE,VDP_INK_DARK_GREEN,VDP_INK_DARK_GREEN);
        clrscr();

        smartkeys_puts(64,64,"Here is an example of SmartKeys Output.");
        
        cputs("\xa0\xa1 \xa2\xa3\xa4"); // BET

	cputs(" NORMAL TEXT");

	gotoxy(2,2);
	textcolor(RED);
	textbackground(WHITE);
	cputs("\x92");
	
	textcolor(BLACK);
	cputs("\x8F");




	while(1);
}

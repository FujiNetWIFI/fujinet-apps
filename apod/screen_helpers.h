#ifndef SCREEN_HELPERS_H
#define SCREEN_HELPERS_H

/* Screen block size is exactly 8KB; enough for
   the screen data (40 x 192 = 7680 bytes),
   a starting offset (see below; to help with 4K boundary limitation),
   and a display list */
#define SCR_BLOCK_SIZE 8192

/* Start image 16 bytes into screen memory, so when we
   hit the 102nd line, we've viewed exactly 4KB */
#define SCR_OFFSET 16

/* Tuck display list at the end of screen memory
   (each screen + display list block has 8192 bytes;
   so the display list gets the last 496 bytes of it) */
#define DLIST_OFFSET (7680 + SCR_OFFSET)

extern unsigned char * scr_mem1, * scr_mem2, * scr_mem3;
extern unsigned char * dlist1, * dlist2, * dlist3;

void screen_off(void);
void screen_on(void);
void wait_for_vblank(void);
void set_screen_and_dlist_pointers(void);

/* A block of space to store the graphics & display lists */
extern unsigned char scr_mem[];

/* A block of space to store descriptive text */
extern unsigned char txt_mem[];

#endif // SCREEN_HELPERS_H

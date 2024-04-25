/**
 * @brief Hires graphics functions
 * @author Oliver Schmidt
 */

#ifndef HIRES_H
#define HIRES_H

#define ROP_CONST(val)          0xA980|(val)
#define ROP_BLACK               0xA980
#define ROP_WHITE               0xA9FF
#define ROP_XOR(val)            0x4900|(val)
#define ROP_CPY                 0x4980
#define ROP_INV                 0x49FF
#define ROP_AND(val)            0x2900|(val)
#define ROP_OR(val)             0x0900|(val)

/* char rop_Line[2][7] = {{0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F}, */
/*                        {0x00, 0x40, 0x60, 0x70, 0x78, 0x7C, 0x7E}}; */

/* char rop_Color[2][2] = {{0x55, 0x2A}, {0xD5, 0xAA}}; */

void hires_Init(void);
void hires_Done(void);
void hires_Text(char flag);
void hires_Draw(char xpos,    char ypos,
                char xsize,   char ysize,
                unsigned rop, char *src);
void hires_Mask(char xpos,    char ypos,
                char xsize,   char ysize,
                unsigned rop);

void hires_Fill(void);

#endif /* HIRES_H */

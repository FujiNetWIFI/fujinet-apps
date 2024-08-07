#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include <stddef.h>

extern int current_frame;

typedef struct
{
  uint8_t modulus;
  uint8_t current;
  uint8_t* x_data;
  uint8_t* y_data;
  uint8_t data_len;
  uint8_t x_offset;
  uint8_t y_offset;
  char pixel;
  uint8_t trail_len;
} BoxAnimation;

void animate(BoxAnimation* b);

extern uint8_t b1_x[];
extern uint8_t b1_y[];
extern uint8_t b2_x[];
extern uint8_t b2_y[];
extern uint8_t b3_x[];
extern uint8_t b3_y[];
extern uint8_t b4_x[];
extern uint8_t b4_y[];

extern size_t b1_x_size;
extern size_t b1_y_size;
extern size_t b2_x_size;
extern size_t b2_y_size;
extern size_t b3_x_size;
extern size_t b3_y_size;
extern size_t b4_x_size;
extern size_t b4_y_size;

#endif
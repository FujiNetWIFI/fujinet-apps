/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifdef BUILD_ADAM

#include <eos.h>
#include "io.h"
#include "die.h"

#define FUJI_DEV 0x0F
#define RESPONSE_SIZE 1024
#define ACK 0x80
#define READ_HOST_LIST "\xF4"

HostSlots hs;

void io_init(void)
{
  eos_start_read_keyboard(); // might as well. :)
}

HostSlots *io_select_host(void)
{
  unsigned char r = eos_write_character_device(FUJI_DEV,READ_HOST_LIST,1);

  if (r != ACK)
    die("COULD NOT SEND HOST LIST CMD");

  r = eos_read_character_device(FUJI_DEV,&hs,RESPONSE_SIZE);

  if (r != ACK)
    die("COULD NOT READ HOST LIST");
  
  return &hs;
}

#endif /* BUILD_ADAM */

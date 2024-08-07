#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "spinfo.h"

#define BASE_ADDRESS 0xC000

char *version = "v1.0.4";

void debug() {}

int main(void) {
    clrscr();
    printf("spinfo %s\n", version);
    sp_info();
    printf("\n");
    return 0;
}

void sp_print_devices() {
  int8_t status_r = 0;
	uint8_t device_count = 0;
  int i = 0;
  uint8_t char_count = 0;

  status_r = sp_status(0, 0);
  printf("status: %d\n", status_r);
  if (status_r != 0) return;

  device_count = sp_payload[0];
  printf("device count: %d\n", device_count);
  if (device_count == 0) return;

  // DIB request on each device to get the name back
  for (i = 1; i <= device_count; i++) {
    status_r = sp_status(i, 3);
    printf("%d/%d/%u/", i, status_r, sp_count);
    if (status_r == 0 && sp_count > 0) {
      char_count = sp_payload[4];
      printf(">");
      fwrite(&sp_payload[5], sizeof(char), char_count, stdout);
      printf("<\n");
    } else {
      printf("\n");
    }
  }
  printf("press a key to continue.\n");
  cgetc();
}

void sp_info() {
  // for each SP card found, get its dispatch address, then call sp_print_devices for each
  uint8_t i = 0;
  uint8_t *p;
  unsigned int dispatch_address;

  for (i = 1; i < 8; i++) {
    p = (uint8_t *)(BASE_ADDRESS + (i * 0x100));
    // printf("Checking 0x%04x\n", (unsigned int) p);
    if (p[1] == 0x20 && p[3] == 0x00 && p[5] == 0x03 && p[7] == 0x00) {
      dispatch_address = (unsigned int) &p[p[0xff]] + 3;
      debug();
      sp_dispatch_address[0] = dispatch_address & 0xff;
      sp_dispatch_address[1] = (dispatch_address >> 8) & 0xff;
      printf("SP card at %d, dispatch: 0x%02x%02x\n", i, sp_dispatch_address[1], sp_dispatch_address[0]);
      sp_print_devices();
    }
  }
}
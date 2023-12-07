#ifdef __APPLE2__
/**
 * FujiNet CONFIG for #Apple2
 *
 * I/O Routines
 */

#include "../io.h"
#include <stdint.h>
#include <conio.h>
#include <stdlib.h>
#include <peekpoke.h> // For the insanity in io_boot()

#define FUJICMD_RESET 0xFF
#define FUJICMD_GET_SSID 0xFE
#define FUJICMD_SCAN_NETWORKS 0xFD
#define FUJICMD_GET_SCAN_RESULT 0xFC
#define FUJICMD_SET_SSID 0xFB
#define FUJICMD_GET_WIFISTATUS 0xFA
#define FUJICMD_MOUNT_HOST 0xF9
#define FUJICMD_MOUNT_IMAGE 0xF8
#define FUJICMD_OPEN_DIRECTORY 0xF7
#define FUJICMD_READ_DIR_ENTRY 0xF6
#define FUJICMD_CLOSE_DIRECTORY 0xF5
#define FUJICMD_READ_HOST_SLOTS 0xF4
#define FUJICMD_WRITE_HOST_SLOTS 0xF3
#define FUJICMD_READ_DEVICE_SLOTS 0xF2
#define FUJICMD_WRITE_DEVICE_SLOTS 0xF1
#define FUJICMD_UNMOUNT_IMAGE 0xE9
#define FUJICMD_GET_ADAPTERCONFIG 0xE8
#define FUJICMD_NEW_DISK 0xE7
#define FUJICMD_UNMOUNT_HOST 0xE6
#define FUJICMD_GET_DIRECTORY_POSITION 0xE5
#define FUJICMD_SET_DIRECTORY_POSITION 0xE4
//#define FUJICMD_SET_HSIO_INDEX 0xE3
#define FUJICMD_SET_DEVICE_FULLPATH 0xE2
#define FUJICMD_SET_HOST_PREFIX 0xE1
#define FUJICMD_GET_HOST_PREFIX 0xE0
//#define FUJICMD_SET_SIO_EXTERNAL_CLOCK 0xDF
#define FUJICMD_WRITE_APPKEY 0xDE
#define FUJICMD_READ_APPKEY 0xDD
#define FUJICMD_OPEN_APPKEY 0xDC
#define FUJICMD_CLOSE_APPKEY 0xDB
#define FUJICMD_GET_DEVICE_FULLPATH 0xDA
#define FUJICMD_CONFIG_BOOT 0xD9
#define FUJICMD_COPY_FILE 0xD8
#define FUJICMD_MOUNT_ALL 0xD7
#define FUJICMD_SET_BOOT_MODE 0xD6
#define FUJICMD_ENABLE_DEVICE 0xD5
#define FUJICMD_DISABLE_DEVICE 0xD4
#define FUJICMD_DEVICE_STATUS 0xD1
#define FUJICMD_STATUS 0x53

#define UNUSED(x) (void)(x);

#include <string.h>
#include "sp.h"
#ifdef __ORCAC__
#include <texttool.h>
#endif

unsigned char io_create_type;

/* Test Fixtures, remove when actual I/O present */
// static DeviceSlot _ds[8];
// static HostSlot _hs[8];
// static char de[36][8]={
//   {"Entry 1"},
//   {"Entry 2"},
//   {"Entry 3"},
//   {"Entry 4"},
//   {"Entry 5"},
//   {"Entry 6"},
//   {"Entry 7"},
//   {"\x7F"}
// };
// static char dc=0;

void io_init(void)
{
  clrscr();
  sp_init();
}

uint8_t io_status(void)
{
  return io_error();
}

bool io_error(void)
{
  return sp_error;
}


char *io_get_device_filename(uint8_t ds)
{
  sp_payload[0] = 1; // 1 byte, device slot.
  sp_payload[1] = 0;
  sp_payload[2] = ds; // the device slot.
  sp_error = sp_status(sp_dest, FUJICMD_GET_DEVICE_FULLPATH);
  if (!sp_error)
    return (char *)&sp_payload[0];
  else
    return 0;
}

void io_get_device_slots(DeviceSlot *d)
{
  sp_status(sp_dest, FUJICMD_READ_DEVICE_SLOTS);
  memcpy(d,sp_payload,sp_count); // 38x8 = 304 bytes
}

void io_get_host_slots(HostSlot *h)
{
  sp_status(sp_dest, FUJICMD_READ_HOST_SLOTS);
  memcpy(h, sp_payload, sp_count); // 32x8 = 256 bytes
}

void io_put_host_slots(HostSlot *h)
{
  sp_payload[0] = 0;
  sp_payload[1] = 1; // 256 bytes
  memcpy(&sp_payload[2], h, 256);
  sp_error = sp_control(sp_dest, FUJICMD_WRITE_HOST_SLOTS);
}

void io_put_device_slots(DeviceSlot *d)
{
  sp_payload[0] = 304 & 0xFF;
  sp_payload[1] = 304 >> 8;
  memcpy(&sp_payload[2],d,304);

  sp_error = sp_control(sp_dest, FUJICMD_WRITE_DEVICE_SLOTS);
  // sleep(1);
}

void io_mount_host_slot(uint8_t hs)
{
  sp_payload[0] = 1;
  sp_payload[1] = 0;
  sp_payload[2] = hs;
  sp_error = sp_control(sp_dest, FUJICMD_MOUNT_HOST);
}

void io_set_boot_config(uint8_t toggle)
{
  #ifndef __ORCAC__
  sp_payload[0] = 1;
  sp_payload[1] = 0;
  sp_payload[2] = toggle;

  sp_error = sp_control(sp_dest, FUJICMD_CONFIG_BOOT);
  #endif
}

void io_umount_disk_image(uint8_t ds)
{
  sp_payload[0] = 1;
  sp_payload[1] = 0;
  sp_payload[2] = ds;
  sp_error = sp_control(sp_dest, FUJICMD_UNMOUNT_IMAGE);
}

void io_update_devices_enabled(bool *e)
{
  char i;

  for (i=0;i<6;i++)
    {
      e[i]=io_get_device_enabled_status(io_device_slot_to_device(i));
    }
}

void io_enable_device(unsigned char d)
{
  sp_payload[0] = 1;
  sp_payload[1] = 0;
  sp_payload[2] = d;
  sp_error = sp_control(sp_dest,FUJICMD_ENABLE_DEVICE);
}

void io_disable_device(unsigned char d)
{
  sp_payload[0] = 1;
  sp_payload[1] = 0;
  sp_payload[2] = d;
  sp_error = sp_control(sp_dest,FUJICMD_DISABLE_DEVICE);
}

bool io_get_device_enabled_status(unsigned char d)
{
  sp_payload[0] = 1;
  sp_payload[1] = 0;
  sp_payload[2] = d;
  sp_error = sp_status(sp_dest,FUJICMD_DEVICE_STATUS);
  if (!sp_error)
    return (bool)sp_payload[0];

  return false;
}

unsigned char io_device_slot_to_device(unsigned char ds)
{
  return ds;
}

void io_set_device_filename(uint8_t ds, char* e)
{
}

void io_mount_disk_image(uint8_t ds, uint8_t mode)
{
}

/**
 * Mount all hosts and devices
 */
unsigned char io_mount_all(void)
{
 return 0;
}

void io_boot(void)
{
  #ifdef __ORCAC__
  sp_done();
	#ifndef BUILD_A2CDA
  WriteChar(0x8c);  // Clear screen
  WriteChar(0x92);  // Set 80 col
  WriteChar(0x86);  // Cursor on
  TextShutDown();
  exit(0);
	#endif

  #else
  char ostype;

  ostype = get_ostype() & 0xF0;
  clrscr();
  cputs("BOOTING...\r\n");

  if (ostype == APPLE_IIIEM)
  {
    asm("STA $C082");  // disable language card (Titan3+2)
    asm("LDA #$77");   // enable A3 primary rom
    asm("STA $FFDF");
    asm("JMP $F4EE");  // jmp to A3 reset entry
  }
  else  // Massive brute force hack that takes advantage of MMU quirk. Thank you xot.
  {
    // Make the simulated 6502 RESET result in a cold start.
    // INC $03F4
    POKE(0x100,0xEE);
    POKE(0x101,0xF4);
    POKE(0x102,0x03);

    // Make sure to not get disturbed.
    // SEI
    POKE(0x103,0x78);

    // Disable Language Card (which is enabled for all cc65 programs).
    // LDA $C082
    POKE(0x104,0xAD);
    POKE(0x105,0x82);
    POKE(0x106,0xC0);

    // Simulate a 6502 RESET, additionally do it from the stack page to make the MMU
    // see the 6502 memory access pattern which is characteristic for a 6502 RESET.
    // JMP ($FFFC)
    POKE(0x107,0x6C);
    POKE(0x108,0xFC);
    POKE(0x109,0xFF);

    asm("JMP $0100");
  }
  #endif /* __ORCAC__ */
}

#endif /* __APPLE2__ */

#ifdef __C64__
/**
 * FujiNet CONFIG for #C64
 *
 * I/O Routines
 */
#include "../io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h> // For the insanity in io_boot()
#include <c64.h>
#include <cbm.h>


#define UNUSED(x) (void)(x);

#define FUJICMD_RESET "\xFF"
#define FUJICMD_GET_SSID "\xFE"
#define FUJICMD_SCAN_NETWORKS "\xFD"
#define FUJICMD_GET_SCAN_RESULT "\xFC"
#define FUJICMD_SET_SSID "\xFB"
#define FUJICMD_GET_WIFISTATUS "\xFA"
#define FUJICMD_MOUNT_HOST "\xF9"
#define FUJICMD_MOUNT_IMAGE "\xF8"
#define FUJICMD_OPEN_DIRECTORY "\xF7"
#define FUJICMD_READ_DIR_ENTRY "\xF6"
#define FUJICMD_CLOSE_DIRECTORY "\xF5"
#define FUJICMD_READ_HOST_SLOTS "\xF4"
#define FUJICMD_WRITE_HOST_SLOTS "\xF3"
#define FUJICMD_READ_DEVICE_SLOTS "\xF2"
#define FUJICMD_WRITE_DEVICE_SLOTS "\xF1"
#define FUJICMD_UNMOUNT_IMAGE "\xE9"
#define FUJICMD_GET_ADAPTERCONFIG "\xE8"
#define FUJICMD_NEW_DISK "\xE7"
#define FUJICMD_UNMOUNT_HOST "\xE6"
#define FUJICMD_GET_DIRECTORY_POSITION "\xE5"
#define FUJICMD_SET_DIRECTORY_POSITION "\xE4"
#define FUJICMD_SET_DEVICE_FULLPATH "\xE2"
#define FUJICMD_SET_HOST_PREFIX "\xE1"
#define FUJICMD_GET_HOST_PREFIX "\xE0"
#define FUJICMD_WRITE_APPKEY "\xDE"
#define FUJICMD_READ_APPKEY "\xDD"
#define FUJICMD_OPEN_APPKEY "\xDC"
#define FUJICMD_CLOSE_APPKEY "\xDB"
#define FUJICMD_GET_DEVICE_FULLPATH "\xDA"
#define FUJICMD_CONFIG_BOOT "\xD9"
#define FUJICMD_COPY_FILE "\xD8"
#define FUJICMD_MOUNT_ALL "\xD7"
#define FUJICMD_SET_BOOT_MODE "\xD6"
#define FUJICMD_ENABLE_DEVICE "\xD5"
#define FUJICMD_DISABLE_DEVICE "\xD4"
#define FUJICMD_DEVICE_STATUS "\xD1"
#define FUJICMD_STATUS "\x53"

#define LFN 15
#define DEV 30  // The FujiNet device #
#define SAN 15

#include <string.h>

char response[1024];

unsigned char io_create_type;

void io_init(void)
{
}

uint8_t io_status(void)
{
  return io_error();
}

bool io_error(void)
{
  return 0; 
}

char *io_get_device_filename(uint8_t ds)
{
  char c[3];

  c[0] = FUJICMD_GET_DEVICE_FULLPATH;
  c[1] = ds;
  c[2] = 0;

  cbm_open(LFN,DEV,SAN,c);
  cbm_read(LFN,&response,sizeof(response));
  cbm_close(LFN);
  
  return (char *)response;
}


void io_get_device_slots(DeviceSlot *d)
{
  cbm_open(LFN,DEV,SAN,FUJICMD_READ_DEVICE_SLOTS);
  cbm_read(LFN,d,152); // 38 * 4
  cbm_close(LFN);
}

void io_get_host_slots(HostSlot *h)
{
  cbm_open(LFN,DEV,SAN,FUJICMD_READ_HOST_SLOTS);
  cbm_read(LFN,h,256);
  cbm_close(LFN);
}

void io_put_host_slots(HostSlot *h)
{
  memset(response,0,sizeof(response));
  
  response[0] = FUJICMD_WRITE_HOST_SLOTS;

  memcpy(&response[1],h,256);

  cbm_open(LFN,DEV,SAN,response);
  cbm_close(LFN);
}

void io_put_device_slots(DeviceSlot *d)
{
  memset(response,0,sizeof(response));
  
  response[0] = FUJICMD_WRITE_DEVICE_SLOTS;

  memcpy(&response[1],d,152);

  cbm_open(LFN,DEV,SAN,response);
  cbm_close(LFN);
}

void io_mount_host_slot(uint8_t hs)
{
  memset(response,0,sizeof(response));
  
  response[0] = FUJICMD_MOUNT_HOST;
  response[1] = hs;

  cbm_open(LFN,DEV,SAN,response);
  cbm_close(LFN);
}

void io_set_device_filename(uint8_t ds, char* e)
{
  memset(response,0,sizeof(response));

  response[0] = FUJICMD_SET_DEVICE_FULLPATH;
  response[1] = ds;
  strcpy((char *)&response[2],e);

  cbm_open(LFN,DEV,SAN,response);
  cbm_close(LFN);
}

void io_mount_disk_image(uint8_t ds, uint8_t mode)
{
  response[0] = FUJICMD_MOUNT_IMAGE;
}

unsigned char io_mount_all(void)
{ 
  return 0;
}

void io_set_boot_config(uint8_t toggle)
{
}

void io_umount_disk_image(uint8_t ds)
{
}

void io_update_devices_enabled(bool *e)
{
}

void io_enable_device(unsigned char d)
{
}

void io_disable_device(unsigned char d)
{
}

bool io_get_device_enabled_status(unsigned char d)
{
  return false;
}

unsigned char io_device_slot_to_device(unsigned char ds)
{
  return ds;
}

void io_boot(void)
{
  // Soft reboot
  //__asm__("JMP $FCE2");
  // instead of boot, load the program
  //cbm_load(...);
}


#endif /* __C64__ */

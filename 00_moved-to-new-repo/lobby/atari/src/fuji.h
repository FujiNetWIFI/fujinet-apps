/**
 * Fuji Device ($70) Commands
 */

#ifndef FUJI_H
#define FUJI_H

#define FUJI_HOST_SLOT_COUNT 8
#define FUJI_HOST_SLOT_NAME_LENGTH 32

#define FUJI_DEVICE_MODE_READ 1
#define FUJI_DEVICE_MODE_WRITE 2


void host_read(char* host_slots);
void host_write(char* host_slots);
void disk_read(void);
void disk_write(void);
void disk_set_host_slot(unsigned char device_slot, unsigned char host_slot, unsigned char mode);
void set_filename(unsigned char mode, unsigned char host_slot, unsigned char device_slot, char* filename);
void host_mount(unsigned char c);
void host_unmount(unsigned char c);
void disk_mount(unsigned char c, unsigned char o);
void fujinet_reset();

#endif

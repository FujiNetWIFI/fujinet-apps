#ifdef _CMOC_VERSION_

/*
  Platform specific utilities that don't fit in any category
*/
#include <cmoc.h>
#include <coco.h>
#include "hires.h"
#include "../fujinet-fuji.h"
#include "../fujinet-network.h"


#define FUJI_HOST_SLOT_COUNT 8
#define FUJI_DEVICE_SLOT_COUNT 8

HostSlot host_slots[FUJI_HOST_SLOT_COUNT];
DeviceSlot device_slots[FUJI_DEVICE_SLOT_COUNT];
int lastCoCoKey=0;


void resetTimer()
{
  setTimer(0);
}

int getTime()
{
  return getTimer();
}

void waitvsync() {
  uint16_t i=getTimer();
  while (i==getTimer()) {
    if (!lastCoCoKey)
      lastCoCoKey=inkey();
  }
}

/// @brief Invokes the CoCo BASIC RUNM command
/// @param filename 
void runm(char * filename) 
{
    // This reproduces the state when executing RUNM"FILE in BASIC
    // by filling in the command line buffer and jumping directly
    // to the Basic RUN procedure

    // Set beginning of (compressed) command: M"
    *((uint16_t*)0x2dd)=0x4D22;

    // Add the filename to the command
    strcpy(0x2df,filename);

    // Set command pointer
    *((uint16_t*)0xa6)=0x2dd; // set CHARAD
    
    // Jump to "RUNM" command
    asm
    {
        // Observed this value in actual RUNM. Not working unless 4Dxx is set
        ldd     #$4D1C

        // Jump to RUN procedure
        jmp     $AE75
    }
}

void mount() {
  static uint8_t i, slot;
  char host[] = "ec.tnfs.io";
  char filename[] ="coco/lobby.dsk";
  char file[] ="LOBBY";

  // Read current list of hosts from FujiNet
  fuji_get_host_slots((unsigned char*) host_slots, FUJI_HOST_SLOT_COUNT);
  
  // Pick the host slot to use. Default to the last, but choose an existing slot
  // if it already has the same host
  slot = FUJI_HOST_SLOT_COUNT;
  for(i=0;i<FUJI_HOST_SLOT_COUNT;i++) {
    if (stricmp(host, (char*)host_slots[i]) == 0) {
      slot = i;
      break;
    }
  }

  // Update the bottom host slot if needed
  if (slot == FUJI_HOST_SLOT_COUNT) {
    slot=FUJI_HOST_SLOT_COUNT-1;
    strcpy((char*)host_slots[slot], host);
    fuji_put_host_slots((unsigned char*) host_slots, FUJI_HOST_SLOT_COUNT);
  }
  
  // Mount the file to the device/host slot
  fuji_mount_host_slot(slot);

  // Mount the file to device slot 0
  device_slots[0].hostSlot = slot;
  device_slots[0].mode = 0;
  strcpy((char*)device_slots[0].file, filename);
  fuji_put_device_slots(device_slots, FUJI_DEVICE_SLOT_COUNT);
  fuji_set_device_filename(0, slot, 0, filename);
  fuji_mount_disk_image(0, 1);
  
  // Load the lobby
  waitvsync();
  runm(file);
}

void quit()
{
  memset(0x400,0x60,32*24);
  memcpy((void*)0x0509,(void*)"LOADING", 7);
  memcpy((void*)0x0512,(void*)"LOBBY", 5);
  
  waitvsync();
  pmode(0, 0x400);
  screen(0,0);
  
  mount();
}


unsigned char kbhit (void) {
    return (char)(lastCoCoKey || (lastCoCoKey=inkey()) || (lastCoCoKey=inkey()));
}

char cgetc (void) {
  int key=lastCoCoKey;

  lastCoCoKey=0;

  while (!key) {
    key=inkey();
  }

  return (char)key;
}



#endif

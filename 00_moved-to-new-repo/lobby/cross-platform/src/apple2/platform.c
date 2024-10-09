#ifdef __APPLE2__

#include <stdint.h>
#include <apple2.h>
#include <joystick.h>
#include <peekpoke.h>
#include <conio.h>

static uint8_t installedDriver = 0, canReadJoystick=0;

unsigned char readJoystick() {

  if (!installedDriver) {
    installedDriver=1;

    if (joy_install(joy_static_stddrv) == JOY_ERR_OK) {
      canReadJoystick = joy_read(JOY_1) == 0;
    }
  }
  
  return canReadJoystick ? joy_read(JOY_1) : 0;
}


void initialize() {
  // Nothing for Apple II
}

void waitvsync() {
  static uint16_t i;
  // Aproximate a jiffy for the timer countdown
  for ( i=0;i<628;i++);
}


// Copied from https://github.com/FujiNetWIFI/fujinet-config/blob/main/src/apple2/io.c
// on 2024-10-2
void reboot(void)
{
  char ostype;
  int i;

  ostype = get_ostype() & 0xF0;
  
  if (ostype == APPLE_II ||
    ostype == APPLE_IIPLUS ||
    ostype == APPLE_IIE ||
    ostype == APPLE_IIEENH)
  {
    // Wait for fujinet disk ii states to be ready
    for (i = 0; i < 2000; i++)
    {
      if (i % 250 == 0)
      {
        cputs(".");
      }
    }
  }

  if (ostype == APPLE_IIIEM)
  {
    asm("STA $C082");  // disable language card (Titan3+2)
    asm("LDA #$77");   // enable A3 primary rom
    asm("STA $FFDF");
    asm("JMP $F4EE");  // jmp to A3 reset entry
  }
  else  // Massive brute force hack that takes advantage of MMU quirk. Thank you xot.
  {
    POKE(0xC00E,0); // CLRALTCHAR

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
}

#endif /* __APPLE2__ */
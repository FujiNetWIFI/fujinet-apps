#ifdef _CMOC_VERSION_

#include <stdint.h>
#include <coco.h>
#include <joystick.h>

extern char panel_spacer_string[];
unsigned char readJoystick() {
return 0;
}


void initialize() {
  initCoCoSupport();

  if (!isCoCo3) {
    // Colored dashes don't look good on non coco3
    strcpy(panel_spacer_string, "--------");
  }
}

void waitvsync() {
  //static uint16_t i;
  // Aproximate a jiffy for the timer countdown - untested
  //for ( i=0;i<628;i++);
}


void reboot(void)
{
  sleep(2);
  coldStart();
}

#endif /* _CMOC_VERSION_ */
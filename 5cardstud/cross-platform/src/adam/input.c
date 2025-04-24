#ifdef __ADAM__
#include <eos.h>
/**
 * @brief Adam input routines
 * @author Thomas Cherryhomes, Geoff Oltmans
 * @license gpl v.3
 */

static GameControllerData cont;

unsigned char readJoystick()
{
  unsigned char temp = 0;
  eos_read_game_controller(0x03,&cont);
  temp = cont.joystick1;
  temp |= (cont.joystick1_button_left || cont.joystick1_button_right)<<4;

  return temp;
}


#ifdef USE_PLATFORM_SPECIFIC_INPUT
void initPlatformKeyboardInput(void)
{
  eos_start_read_keyboard();
}

int getPlatformKey(void)
{
  int ch;
  ch=eos_end_read_keyboard();

  if (ch>1)
  {
    // smartkeys_sound_play(SOUND_TYPEWRITER_CLACK);
    eos_start_read_keyboard();
  }
  return ch;
}
#endif
#endif /* __ADAM__ */

#ifdef __APPLE2__

/**
 * @brief   Cbm App Key Functions
 * @author  Eric Carr
 * @license gpl v. 3
 */


/**
 * @brief Open a key for reading or writing
 * @param open_mode 0 = read, 1 = write
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @return error code
 */
unsigned char open_appkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, unsigned char key_id)
{ 
  static unsigned char appkey_error;

  return appkey_error;
}



unsigned char read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* data)
{
  static unsigned char appkey_error;
  return appkey_error;
}

unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, const char *data)
{
  static unsigned char appkey_error;
  return appkey_error;
}

#endif /* __C64__ */
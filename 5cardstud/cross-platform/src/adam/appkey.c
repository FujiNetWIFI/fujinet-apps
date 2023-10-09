#ifdef __ADAM__

/**
 * @brief Coleco Adam App Key Functions
 * @author Thomas Cherryhomes
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
unsigned char open_appkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, char key_id)
{
  return 0;
}

unsigned char read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* data)
{
  return 0;
}

unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, const char *data)
{
  return 0;
}

#endif /* __ADAM__ */

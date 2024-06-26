#ifdef BUILD_APPLE2
/**
 * CONFIG custom types
 */

#ifndef FUJI_TYPEDEFS_H
#define FUJI_TYPEDEFS_H

#define FILE_MAXLEN 36

#define MODE_READ 1
#define MODE_WRITE 2

typedef unsigned short DirectoryPosition;

/**
 * Returned info for a single SSID entry
 * from a WiFi scan
 */
typedef struct {
  char ssid[33];
  signed char rssi;
} SSIDInfo;

/**
 * The current wifi SSID and password
 */
typedef struct
{
  char ssid[33];
  char password[64];
} NetConfig;

/**
 * The current network adapter configuration
 */
typedef struct
{
  char ssid[33];
  char hostname[64];
  unsigned char localIP[4];
  unsigned char gateway[4];
  unsigned char netmask[4];
  unsigned char dnsIP[4];
  unsigned char macAddress[6];
  unsigned char bssid[6];
  char fn_version[15];
} AdapterConfig;

typedef unsigned char HostSlot[32];

typedef struct {
  unsigned char hostSlot;
  unsigned char mode;
  unsigned char file[FILE_MAXLEN];
} DeviceSlot;

typedef struct
{
  unsigned short numSectors;
  unsigned short sectorSize;
  unsigned char hostSlot;
  unsigned char deviceSlot;
  char filename[256];
} NewDisk;

#endif /* FUJI_TYPEDEFS_H */
#endif

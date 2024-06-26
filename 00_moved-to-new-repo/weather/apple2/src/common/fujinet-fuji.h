#ifndef FN_FUJI_H
#define FN_FUJI_H

// TODO: this header file needs more documentation

// In general, bools return the "success" status, so true is good, false is bad.

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __CBM__
#include <cbm.h>
#define FUJI_CBM_DEV 30
#define FUJI_CMD_CHANNEL 15
#endif // __CBM__

#define FILE_MAXLEN 36
#define SSID_MAXLEN 33 /* 32 + NULL */
#define MAX_APPKEY_LEN    64

#ifdef __CBM__
#define MAX_PASSWORD_LEN  65
#else
#define MAX_PASSWORD_LEN  64
#endif


enum WifiStatus {
    no_ssid_available   = 1,
    connected           = 3,
    connect_failed      = 4,
    connection_lost     = 5
};

typedef struct {
  char ssid[SSID_MAXLEN];
  signed char rssi;
} SSIDInfo;

typedef struct
{
  char ssid[SSID_MAXLEN];
  char password[MAX_PASSWORD_LEN];
} NetConfig;

typedef struct
{
  char ssid[SSID_MAXLEN];
  char hostname[64];
  uint8_t localIP[4];
  uint8_t gateway[4];
  uint8_t netmask[4];
  uint8_t dnsIP[4];
  uint8_t macAddress[6];
  uint8_t bssid[6];
  char fn_version[15];
} AdapterConfig;

typedef struct
{
  char ssid[SSID_MAXLEN];
  char hostname[64];
  uint8_t localIP[4];
  uint8_t gateway[4];
  uint8_t netmask[4];
  uint8_t dnsIP[4];
  uint8_t macAddress[6];
  uint8_t bssid[6];
  char fn_version[15];
  char sLocalIP[16];
  char sGateway[16];
  char sNetmask[16];
  char sDnsIP[16];
  char sMacAddress[18];
  char sBssid[18];
} AdapterConfigExtended;

typedef uint8_t HostSlot[32];

typedef struct {
  uint8_t hostSlot;
  uint8_t mode;
  uint8_t file[FILE_MAXLEN];
} DeviceSlot;


// Disks have different structures / parameters

#ifdef __ATARI__
typedef struct
{
  uint16_t numSectors;
  uint16_t sectorSize;
  uint8_t hostSlot;
  uint8_t deviceSlot;
  char filename[256];
} NewDisk;
#endif

#ifdef __APPLE2__
typedef struct
{
  uint8_t hostSlot;
  uint8_t deviceSlot;
  uint8_t createType;
  uint32_t numBlocks;
  char filename[256];
} NewDisk;


#endif

#ifdef __CBM__
// TODO: what is this for commodore?
typedef struct
{
  uint16_t numSectors;
  uint16_t sectorSize;
  uint8_t hostSlot;
  uint8_t deviceSlot;
  char filename[256];

} NewDisk;
#endif

// WIP, only 64 fully supported at the moment.
enum AppKeySize {
    DEFAULT  // 64 original size
    // SIZE_256
};

typedef struct
{
    unsigned char value[4];
} FNStatus;

enum HashType {
    MD5,
    SHA1,
    SHA256,
    SHA512
};

/*
 * Closes the currently open directory
 * @return Success status, true if all OK.
 */
bool fuji_close_directory();

/*
 * Copies a file from given src to dst, with supplied path in copy_spec
 * @return Success status, true if all OK.
 */
bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec);

/*
 * Creates a new disk from the given structure.
 * @return Success status, true if all OK.
 */
bool fuji_create_new(NewDisk *new_disk);

bool fuji_disable_device(uint8_t d);
bool fuji_enable_device(uint8_t d);

// TODO: document and fix atari
bool fuji_enable_udpstream(uint16_t port, char *host);

/*
 * Returns true if last operation had an error.
 * @return ERROR status, true if there was an error in last operation.
 */
bool fuji_error();
/*
 * Gets adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Raw version that returns bytes for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config(AdapterConfig *ac);

/*
 * Gets extended adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Extended version that returns strings in addition to raw for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac);


/*
 * THIS IS BOGUS. Apple and Atari both just return "true" for any device.
 * TODO: remove this if it isn't going to be properly implemented on FN.
 *       which I think it could be by looking on FN side by looking at Config
 * @return the enabled status of the given device: NOTE: ALWAYS RETURNS TRUE AT MOMENT
 */
bool fuji_get_device_enabled_status(uint8_t d);

/*
 * Sets the buffer to the device's filename in device id `ds`
 * @return Success status, true if all OK.
 */
bool fuji_get_device_filename(uint8_t ds, char *buffer);

/*
 * Sets ALL device slot information into pointer d.
 * `size` is the receiving array size, and the returned data size is checked against this before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
bool fuji_get_device_slots(DeviceSlot *d, size_t size);

/*
 * Fetch the current directory position for paging through directories into pos.
 * @return success status of request
 */
bool fuji_get_directory_position(uint16_t *pos);

/*
 * Fetch the host prefix for given host slot id.
 * @return success status of request
 */
bool fuji_get_host_prefix(uint8_t hs, char *prefix);

/*
 * Sets ALL host slot information into pointer h.
 * `size` is the receiving array size, and the returned data size is checked against this before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
bool fuji_get_host_slots(HostSlot *h, size_t size);

/*
 * Fills ssid_info with wifi scan results for bssid index n.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info);

/*
 * Fills net_config with current SSID/password values.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
bool fuji_get_ssid(NetConfig *net_config);

/*
 * @return Checks if WIFI is enabled or not. Any device errors will return false also.
 */
bool fuji_get_wifi_enabled();

/*
 * @return Sets status to the wifi status value.
 * WL_CONNECTED (3), WL_DISCONNECTED (6) are set if there are no underyling errors in FN.
 * @return Success status, true if all OK.
 */
bool fuji_get_wifi_status(uint8_t *status);

/*
 * Mount all devices
 * @return true if successful, false otherwise
 */
bool fuji_mount_all();

/*
 * Mount specific device slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_disk_image(uint8_t ds, uint8_t mode);

/*
 * Mount specific host slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_host_slot(uint8_t hs);

/*
 * Open the given directory on the given host slot.
 * The path_filter is a buffer (not a string) of 256 bytes, with a separator of the \0 char between the path and filter parts.
 * @return true if successful, false otherwise
 */
bool fuji_open_directory(uint8_t hs, char *path_filter);

/*
 * Open the given directory on the given host slot.
 * path and filter are separate strings. If filter is not set, it is NULL
 * @return true if successful, false otherwise
 */
bool fuji_open_directory2(uint8_t hs, char *path, char *filter);

/*
 * Save `size` device slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_device_slots(DeviceSlot *d, size_t size);

/*
 * Save `size` hosts slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_host_slots(HostSlot *h, size_t size);

/*
 * Fill buffer with directory information.
 * @return success status of request
 */
bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer);

/*
 * Reset FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_reset();

/*
 * Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
bool fuji_scan_for_networks(uint8_t *count);

/*
 * Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
bool fuji_set_boot_config(uint8_t toggle);

/*
 * Sets the booting mode of the FN device (e.g. lobby).
 * @return success status of request.
 */
bool fuji_set_boot_mode(uint8_t mode);

/*
 * Sends the device/host/mode information for devices to FN
 * @return success status of request.
 */
bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer);

/*
 * Sets current directory position
 * @return success status of request.
 */
bool fuji_set_directory_position(uint16_t pos);

#ifdef __ATARI__
/*
 * Fetch the current HSIO index value.
 * @return success status of request
 */
bool fuji_get_hsio_index(uint8_t *index);

/*
 * Sets HSIO speed index
 * @return success status of request.
 */
bool fuji_set_hsio_index(bool save, uint8_t index);

/*
 * Sets SIO external clock speed
 * @return success status of request.
 */
bool fuji_set_sio_external_clock(uint16_t rate);

#endif

/*
 * Set the host prefix for given host slot id for platforms that support it.
 * @return success status of request
 */
bool fuji_set_host_prefix(uint8_t hs, char *prefix);

/*
 * Set the SSID information from NetConfig structure
 * @return success status of request
 */
bool fuji_set_ssid(NetConfig *nc);

/*
 * Gets the FNStatus information from FUJI device.
 * @return success status of request
 */
bool fuji_status(FNStatus *status);

/*
 * Unmounts the device in slot ds
 * @return success status of request
 */
bool fuji_unmount_disk_image(uint8_t ds);

/*
 * Unmounts the host in slot hs
 * @return success status of request
 */
bool fuji_unmount_host_slot(uint8_t hs);

/*
 * @brief  Opens and reads from appkey using the provided details
 * @param  key_id the specific key id of this application
 * @param  count a pointer to an int for the number of bytes that were read
 * @param  data a pointer to the memory to write the data back to.
 * @return success status of the call. If either the initial OPEN or subsequent READ fail, will return false.
 */
bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data);

/*
 * @brief  Writes to an appkey using the provided details previously setup with fuji_set_appkey_details
 * @param  key_id the specific key id of this application
 * @param  count the number of bytes in the buffer to write to the appkey.
 * @param  data a pointer to the memory to write from.
 * @return success status of the call. If either the initial OPEN or subsequent WRITE fail, will return false.
 */
bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data);

/*
 * @brief  Sets the base details for appkeys. This must be called before using read or write operations on appkeys.
 * @param  creator_id the id of the creator of the appkey
 * @param  app_id the id of the application from the creator
 * @param  keysize type: AppKeySize, set to DEFAULT for 64 byte appkeys, or SIZE_256 for 256 byte keys
 */
void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize);

// Base64
// ALL RETURN VALUES ARE SUCCESS STATUS VALUE, i.e. true == success 
bool fuji_base64_decode_compute();
bool fuji_base64_decode_input(char *s, uint16_t len);
bool fuji_base64_decode_length(unsigned long *len);
bool fuji_base64_decode_output(char *s, uint16_t len);

bool fuji_base64_encode_compute();
bool fuji_base64_encode_input(char *s, uint16_t len);
bool fuji_base64_encode_length(unsigned long *len);
bool fuji_base64_encode_output(char *s, uint16_t len);

// Hash
// ALL RETURN VALUES ARE SUCCESS STATUS VALUE, i.e. true == success 
bool fuji_hash_compute(uint8_t type);
bool fuji_hash_input(char *s, uint16_t len);
bool fuji_hash_length(uint8_t mode);
bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len);

#endif /* FN_FUJI_H */
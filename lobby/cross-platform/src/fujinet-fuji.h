#ifndef FN_FUJI_H
#define FN_FUJI_H

// TODO: this header file needs more documentation

// In general, bools return the "success" status, so true is good, false is bad.

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


#ifdef __CBM__

// For DATA transfers, we want to undo some of the charmap settings in CC65.
// So map the characters in the range 5b-60, 7b-7f back to themselves, as we want to send them as-is to FujiNet.
// This undoes some "look alike" character that move into 0x80+ range, which in FN are converted to 3 byte utf-8 chars incorrectly.

#pragma charmap (0x5B, 0x5B)
#pragma charmap (0x5C, 0x5C)
#pragma charmap (0x5D, 0x5D)
#pragma charmap (0x5E, 0x5E)
#pragma charmap (0x5F, 0x5F)
#pragma charmap (0x60, 0x60)
#pragma charmap (0x7B, 0x7B)
#pragma charmap (0x7C, 0x7C)
#pragma charmap (0x7D, 0x7D)
#pragma charmap (0x7E, 0x7E)
#pragma charmap (0x7F, 0x7F)

#endif

#define FILE_MAXLEN    36
#define SSID_MAXLEN    33 /* 32 + NULL */
#define MAX_APPKEY_LEN 64

#ifdef __CBM__
    #define MAX_PASSWORD_LEN 65
#else
#define MAX_PASSWORD_LEN 64
#endif

#define FUJICMD_RESET                      0xFF
#define FUJICMD_GET_SSID                   0xFE
#define FUJICMD_SCAN_NETWORKS              0xFD
#define FUJICMD_GET_SCAN_RESULT            0xFC
#define FUJICMD_SET_SSID                   0xFB
#define FUJICMD_GET_WIFISTATUS             0xFA
#define FUJICMD_MOUNT_HOST                 0xF9
#define FUJICMD_MOUNT_IMAGE                0xF8
#define FUJICMD_OPEN_DIRECTORY             0xF7
#define FUJICMD_READ_DIR_ENTRY             0xF6
#define FUJICMD_CLOSE_DIRECTORY            0xF5
#define FUJICMD_READ_HOST_SLOTS            0xF4
#define FUJICMD_WRITE_HOST_SLOTS           0xF3
#define FUJICMD_READ_DEVICE_SLOTS          0xF2
#define FUJICMD_WRITE_DEVICE_SLOTS         0xF1
#define FUJICMD_GET_WIFI_ENABLED           0xEA
#define FUJICMD_UNMOUNT_IMAGE              0xE9
#define FUJICMD_GET_ADAPTERCONFIG          0xE8
#define FUJICMD_NEW_DISK                   0xE7
#define FUJICMD_UNMOUNT_HOST               0xE6
#define FUJICMD_GET_DIRECTORY_POSITION     0xE5
#define FUJICMD_SET_DIRECTORY_POSITION     0xE4
#define FUJICMD_SET_DEVICE_FULLPATH        0xE2
#define FUJICMD_WRITE_APPKEY               0xDE
#define FUJICMD_READ_APPKEY                0xDD
#define FUJICMD_OPEN_APPKEY                0xDC
#define FUJICMD_CLOSE_APPKEY               0xDB
#define FUJICMD_GET_DEVICE_FULLPATH        0xDA
#define FUJICMD_CONFIG_BOOT                0xD9
#define FUJICMD_COPY_FILE                  0xD8
#define FUJICMD_MOUNT_ALL                  0xD7
#define FUJICMD_SET_BOOT_MODE              0xD6
#define FUJICMD_STATUS                     0x53
#define FUJICMD_ENABLE_DEVICE              0xD5
#define FUJICMD_DISABLE_DEVICE             0xD4
#define FUJICMD_RANDOM_NUMBER              0xD3
#define FUJICMD_GET_TIME                   0xD2
#define FUJICMD_DEVICE_ENABLE_STATUS       0xD1
#define FUJICMD_BASE64_ENCODE_INPUT        0xD0
#define FUJICMD_BASE64_ENCODE_COMPUTE      0xCF
#define FUJICMD_BASE64_ENCODE_LENGTH       0xCE
#define FUJICMD_BASE64_ENCODE_OUTPUT       0xCD
#define FUJICMD_BASE64_DECODE_INPUT        0xCC
#define FUJICMD_BASE64_DECODE_COMPUTE      0xCB
#define FUJICMD_BASE64_DECODE_LENGTH       0xCA
#define FUJICMD_BASE64_DECODE_OUTPUT       0xC9
#define FUJICMD_HASH_INPUT                 0xC8
#define FUJICMD_HASH_COMPUTE               0xC7
#define FUJICMD_HASH_LENGTH                0xC6
#define FUJICMD_HASH_OUTPUT                0xC5
#define FUJICMD_GET_ADAPTERCONFIG_EXTENDED 0xC4
#define FUJICMD_HASH_COMPUTE_NO_CLEAR      0xC3
#define FUJICMD_HASH_CLEAR                 0xC2
#define FUJICMD_SET_STATUS                 0x81

enum WifiStatus
{
    no_ssid_available = 1,
    connected = 3,
    connect_failed = 4,
    connection_lost = 5
};

typedef struct
{
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

typedef struct
{
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

#ifdef _CMOC_VERSION_
typedef struct
{
    uint8_t numDisks;
    uint8_t hostSlot;
    uint8_t deviceSlot;
    char filename[256];
} NewDisk;
#endif /* _CMOC_VERSION_ */

#ifdef __CBM__
// TODO: what is this for commodore? IEC firmware does not support new disk yet
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
enum AppKeySize
{
    DEFAULT // 64 original size
    // SIZE_256
};

#ifndef __CBM__
typedef struct
{
    unsigned char value[4];
} FNStatus;
#else
typedef struct
{
    // needs some thought, at the moment it's just values from iecStatus, but could be made generic
    // the Atari/Apple2 versions aren't being set with anything in firmware
    union {
        struct {
            uint8_t error;          // the error code from FN
            uint8_t cmd;            // the cmd that was being run
            uint8_t connected;      // the connected status
            uint8_t channel;        // the channel that was being used
            char msg[41];           // error message: 40 + null
        } value;
        uint8_t raw[45];
    };
} FNStatus;

#endif

extern FNStatus _fuji_status;

/**
 * @brief Closes the currently open directory
 * @return Success status, true if all OK.
 */
bool fuji_close_directory();

/**
 * @brief Copies a file from given src to dst, with supplied path in copy_spec
 * @return Success status, true if all OK.
 */
bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec);

/**
 * @brief Creates a new disk from the given structure.
 * @return Success status, true if all OK.
 */
bool fuji_create_new(NewDisk *new_disk);

bool fuji_disable_device(uint8_t d);
bool fuji_enable_device(uint8_t d);

// TODO: document and fix atari
bool fuji_enable_udpstream(uint16_t port, char *host);

/**
 * @brief Returns true if last operation had an error.
 * @return ERROR status, true if there was an error in last operation.
 */
bool fuji_error();

/**
 * @brief Gets adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Raw version that returns bytes for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config(AdapterConfig *ac);

/**
 * @brief Gets extended adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Extended version that returns strings in addition to raw for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac);

/**
 * THIS IS BOGUS. Apple and Atari both just return "true" for any device.
 * TODO: remove this if it isn't going to be properly implemented on FN.
 *       which I think it could be by looking on FN side by looking at Config
 * @return the enabled status of the given device: NOTE: ALWAYS RETURNS TRUE AT MOMENT
 */
bool fuji_get_device_enabled_status(uint8_t d);

/**
 * @brief Sets the buffer to the device's filename in device id `ds`
 * Note: BUFFER MUST BE ABLE TO ACCEPT UP TO 256 BYTE STRING
 * @return Success status, true if all OK.
 */
bool fuji_get_device_filename(uint8_t ds, char *buffer);

/**
 * @brief Sets ALL device slot information into pointer d.
 * `size` is the receiving array size, and the returned data size is checked against this before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
bool fuji_get_device_slots(DeviceSlot *d, size_t size);

/**
 * @brief Fetch the current directory position for paging through directories into pos.
 * @return success status of request
 */
bool fuji_get_directory_position(uint16_t *pos);

/**
 * @brief Fetch the host prefix for given host slot id.
 * @return success status of request
 */
bool fuji_get_host_prefix(uint8_t hs, char *prefix);

/**
 * @brief Sets ALL host slot information into pointer h.
 * `size` is the number of host slots, and the returned data size is checked against this multiple of HostSlot structs before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
bool fuji_get_host_slots(HostSlot *h, size_t size);

/**
 * @brief Fills ssid_info with wifi scan results for bssid index n.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info);

/**
 * @brief Fills net_config with current SSID/password values.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
bool fuji_get_ssid(NetConfig *net_config);

/**
 * @brief Checks if WIFI is enabled or not. Any device errors will return false also.
 * @return enabled status 
 */
bool fuji_get_wifi_enabled();

/**
 * @brief  Sets status to the wifi status value.
 * WL_CONNECTED (3), WL_DISCONNECTED (6) are set if there are no underyling errors in FN.
 * @return Success status, true if all OK.
 */
bool fuji_get_wifi_status(uint8_t *status);

/**
 * @brief Mount all devices
 * @return true if successful, false otherwise
 */
bool fuji_mount_all();

/**
 * @brief Mount specific device slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_disk_image(uint8_t ds, uint8_t mode);

/**
 * @brief Mount specific host slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_host_slot(uint8_t hs);

/**
 * @brief Open the given directory on the given host slot.
 * The path_filter is a buffer (not a string) of 256 bytes, with a separator of the \0 char between the path and filter parts.
 * @return true if successful, false otherwise
 */
bool fuji_open_directory(uint8_t hs, char *path_filter);

/**
 * @brief Open the given directory on the given host slot.
 * path and filter are separate strings. If filter is not set, it is NULL
 * @return true if successful, false otherwise
 */
bool fuji_open_directory2(uint8_t hs, char *path, char *filter);

/**
 * @brief Save `size` device slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_device_slots(DeviceSlot *d, size_t size);

/**
 * @brief Save `size` hosts slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_host_slots(HostSlot *h, size_t size);

/**
 * @brief Fill buffer with directory information.
 * @return success status of request
 */
bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer);

/**
 * @brief Reset FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_reset();

/**
 * @brief Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
bool fuji_scan_for_networks(uint8_t *count);

/**
 * @brief Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
bool fuji_set_boot_config(uint8_t toggle);

/**
 * @brief Sets the booting mode of the FN device (e.g. lobby).
 * @return success status of request.
 */
bool fuji_set_boot_mode(uint8_t mode);

/**
 * @brief Sends the device/host/mode information for devices to FN
 * @return success status of request.
 */
bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer);

/**
 * @brief Sets current directory position
 * @return success status of request.
 */
bool fuji_set_directory_position(uint16_t pos);

#ifdef __ATARI__
/**
 * @brief Fetch the current HSIO index value.
 * @return success status of request
 */
bool fuji_get_hsio_index(uint8_t *index);

/**
 * @brief Sets HSIO speed index
 * @return success status of request.
 */
bool fuji_set_hsio_index(bool save, uint8_t index);

/**
 * @brief Sets SIO external clock speed
 * @return success status of request.
 */
bool fuji_set_sio_external_clock(uint16_t rate);

#endif

/**
 * @brief Set the host prefix for given host slot id for platforms that support it.
 * @return success status of request
 */
bool fuji_set_host_prefix(uint8_t hs, char *prefix);

/**
 * @brief Set the SSID information from NetConfig structure
 * @return success status of request
 */
bool fuji_set_ssid(NetConfig *nc);

/**
 * @brief Gets the FNStatus information from FUJI device.
 * @return success status of the status request
 * NOTE: The actual status VALUE is in 'status', the return is just whether the command to fetch the status succeeded, it could succeed, but the status value holds an error.
 */
bool fuji_status(FNStatus *status);

#ifdef __CBM__
// DEBUGGING
bool fuji_set_status();
#endif

/**
 * @brief Unmounts the device in slot ds
 * @return success status of request
 */
bool fuji_unmount_disk_image(uint8_t ds);

/**
 * @brief Unmounts the host in slot hs
 * @return success status of request
 */
bool fuji_unmount_host_slot(uint8_t hs);

/**
 * @brief  Opens and reads from appkey using the provided details. Writes to the data buffer, and sets count to the amount of data read if it is less than the full keysize.
 * @param  key_id the specific key id of this application.
 * @param  count a pointer to an int for the number of bytes that were read.
 * @param  data a pointer to the memory to write the data back to. WARNING: The data buffer needs to be at least 2 more bytes larger than the keysize.
 * @return success status of the call. If either the initial OPEN or subsequent READ fail, will return false.
 */
bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data);

/**
 * @brief  Writes to an appkey using the provided details previously setup with fuji_set_appkey_details.
 * @param  key_id the specific key id of this application.
 * @param  count the number of bytes in the buffer to write to the appkey.
 * @param  data a pointer to the memory to write from.
 * @return success status of the call. If either the initial OPEN or subsequent WRITE fail, will return false.
 */
bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data);

/**
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

////////////////////////////////////////////////////////////////
// These are very low level functions and should only be used internally.
// Please use the new interface functions below this section.
////////////////////////////////////////////////////////////////
// All return success status (true = worked)
bool fuji_hash_compute(uint8_t type);
bool fuji_hash_compute_no_clear(uint8_t type);
bool fuji_hash_input(char *s, uint16_t len);
// this requires compute to have been called to set the hashing algorithm - don't use!
// ALSO, there is no way to get the return value with this signature

// output_type is 1 for hex, 0 of binary. the len is the length of data, which currently is only up to 128 if hex, but future proofing with word.
bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len);

/**
 * \deprecated Use fuji_hash_size instead, as this function is broken.
 */
bool fuji_hash_length(uint8_t mode);

///////////////////////////////////////////////////////
// New hashing interface

enum HashType
{
    MD5,
    SHA1,
    SHA256,
    SHA512
};

typedef enum HashType hash_alg_t;

/**
 * @brief  Returns the size of the hash that will be produced for the given hash_type and whether hex output is required or not. This should be used to calculate the memory needed for the output of \ref fuji_hash_data
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @return the length of the hash that will be computed for this algorithm depending on whether hex is being returned or not.
 */
uint16_t fuji_hash_size(hash_alg_t hash_type, bool as_hex);

/**
 * @brief  Computes the hash of the given input data in a single operation. This is a simpler interface than using \ref fuji_hash_clear, \ref fuji_hash_add, \ref fuji_hash_calculate, and can be used instead of those 3 operations if there is only 1 piece of data to hash.
 *         This will also clear any data previously add using fuji_hash_add, and also clears any memory associated with hashing in the FujiNet at the end of the operation. It is the one stop shop, if you use it with the other 3 functions, you must sequence them correctly so this function doesn't clear the existing sent data.
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  input The binary data that requires hash computed on
 * @param  length The length of binary data in "input" to compute a hash on
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @param  output The buffer to write the hash value to. This must be allocated by the application itself, it is not done in the library. \ref fuji_hash_value "fuji_hash_value()".
 * @returns sucess status of the operation
 */
bool fuji_hash_data(hash_alg_t hash_type, uint8_t *input, uint16_t length, bool as_hex, uint8_t *output);

/**
 * @brief  Clear any data associated with hashing in the Fujinet. Should be called before calculating new hashes when using \ref fuji_hash_add and \ref fuji_hash_calculate. Can also be called to discard any data previously sent to free memory on the FujiNet used for any previous data sent with \ref fuji_hash_add.
 * @return success status of the operation
 */
bool fuji_hash_clear();

/**
 * @brief  Adds data that needs to be hashed.
 * @param  data Pointer to the byte data
 * @param  length the length of data to add for hashign
 * @return success status of the operation
 */
bool fuji_hash_add(uint8_t *data, uint16_t length);

/**
 * @brief  Calculates the hash of the accumulated data. Can be called multiple times with different hash_type values on the same data if discard_data is false. If different data is required to be hashed, call \ref fuji_hash_clear to start over, then add data with \ref fuji_hash_add again.
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @param  discard_data If true the data will be freed from FujiNet memory after the hash is calculated. Use false if you are calculating more than one hash type on the data, and end with discard_data is true, or call \ref fuji_hash_clear to also clean up.
 * @param  output The buffer to write the hash to. The caller is responsible for allocating enough memory for this (based on \ref fuji_hash_length)
 * @return the success status of the operation.
 */
bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output);

#endif /* FN_FUJI_H */

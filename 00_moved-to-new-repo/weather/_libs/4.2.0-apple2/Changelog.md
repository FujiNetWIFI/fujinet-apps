# Changelog

## [Unreleased]

## [4.2.0] - 2024-06-19

- Implement commodore fujinet-lib functions
- Add fuji_open_directory2 prototype for applications to send path and
  filter separately rather than in fixed 256 byte buffer with \0 delimiter. Used by CBM.
- Rework Hash API, provide simpler interface
- Fill in some COCO parameters in FUJI calls

## [4.1.4] - 2024-06-05

- Fix sp_init again! Thanks again to @shunkita for noticing I'd broken their update.

## [4.1.3] - 2024-06-03

- Fix sp_init to find any SP device with network device, skipping past any higher cards if they don't have it

## [4.1.2] - 2024-05-20

Bugfix:

- [apple2] fix sp_find_device to not stop on error devices when finding names (thanks Eric Carr)

## [4.1.1] - 2024-05-14

- Enhance build to produce target specific versions of the library, e.g. apple2enh from apple2 src.
  The appkey application was failing in apple2enh without the specific version of the library.

- [apple2] convert network_open src from asm to c

Bugfix:

- [apple] sp_read was incorrectly calling sp_open :scream:

## [4.1.0] - 2024-05-12

### AppKeys Changes

Appkeys have been redesigned for simpler usecases.
Using appkeys now involves first calling `fuji_set_appkey_details` to define the creator, appid, and key size.

Then you call either "fuji_appkey_read" and "fuji_appkey_write".
The old "open" phase is done internally by the API and not exposed.

Clients must provide a pointer for the read/write data buffer to read from and write to.
All structs have been removed in preference of calling functions with parameters directly.

There is a paramter for the appkey size, which is a placeholder for future changes to support larger key sizes
than just 64 bytes.

### [apple2] Asm to C

I've replaced most of the SmartPort asm bus code with C versions.

This makes it way easier to support. The ASM was getting quite tricky to follow, and I wrote the bloody thing.
The only one that shouldn't change to C is sp_dispatch, as it uses self modifying code techniques.

## [4.0.0] - 2024-05-06

- [appkey] Update signature of appkey functions to return bool instead of uint8_t
- [appkey] Change return values of appkey to indicate SUCCESS status to be consistent with other functions
- [hashing] All hashing/base64 functions have been updated similarly to return bool status of success instead of error

## [3.0.4] - 2024-05-05

- [apple2] Add appkey support (thanks Eric Carr)
- [cbm] Start of commodore support in fujinet
- [coco] Start of coco support

## [3.0.3] - 2024-04-02

Bugfix release:

- [apple2] fix network_status on apple2enh by setting y specifically
- replaced BUILD_APPLE2 and BUILD_ATARI in favour of cc65 macros __APPLE2__ and __ATARI__

## [3.0.2] - 2024-03-24

This should be considered the BASE 3 release.

- network_read signature changed to indicate the bytes fetched instead of status, and errors returned as negative
- network_json_query signature similarly changed
- added network_read_nb for non-blocking reads that only fetch up to the amount available in the buffer.
  It is up to the client to loop to fetch the total required rather than the library.

## [3.0.1] - 2024-03-19

This release has been removed, as starting with 3.0.2 the network_read signature has changed and I didn't want to bump to 4.0
given noone has used 3 yet. Sorry purists!

Complete Apple2 fuji device support for CONFIG.

- [apple2] fixes for mount_host_slot and mount_disk_image
- [apple2] don't call FN for get_device_enabled_status, as it's broken
- [apple2] add fuji_disable_device and fuji_enable_device implementations
- [apple2] fix control commands that were not setting the payload parameters
- [apple2] fix fuji_set_device_filename payload length

## [3.0.0] - 2024-03-18

This release has been removed, as starting with 3.0.2 the network_read signature has changed and I didn't want to bump to 4.0
given noone has used 3 yet. Sorry purists!

- Contains signature breaking changes for:
  - network_read, fuji_scan_for_networks, fuji_get_hsio_index, fuji_get_wifi_status, fuji_get_directory_position,
  - fuji_get_device_slots, fuji_get_host_slots
- The great rename fn_io_ to fuji_ to reflect device in FN being used, version bump to 3.0.0 to reflect huge name changes
- Made most fuji_ functions return success status rather than void.
- fuji routines to Apple2 - all but hashing/base64 and appkey stuff
- Add find methods for clock, cpm, modem, printer
- Clean up atari error handling

## [2.2.1] - 2024-02-21

- Renamed libs and archives to reflect name change from "fujinet-network" to "fujinet-lib"
- Libs are now "fujinet-{target}-{version}.lib
- Zips is now fujinet-lib-{target}-{version}.zip
- Header and include files are still fujinet-io.{h,inc} and fujinet-network.{h,inc} to represent their sub-function within fujinet-lib

## [2.2.0] - 2024-02-20

- Change signature of fn_io_mount_disk_image, and fn_io_mount_host_slot to return an error code, so all fn_io_mount* functions are consistent (fixes #2).
- [atari] fn_io_mount_disk_image returns error code from BUS
- [atari] fn_io_mount_host_slot returns error code from BUS
- [atari] added test cases for BUS errors for fn_io_mount_disk_image, fn_io_mount_host_slot, fn_io_mount_all

## [2.1.5] - 2024-01-07

### Fixed

- [apple2] sp_init multiple SP device detection (thanks @ShunKita)

## [2.1.4] - 2024-01-06

### Fixed

- [apple2] network_json_query removes any trailing CR/LF/0x9b in results as last character

### Changed

- [apple2] reduced payload memory from 1024 to 512.

## [2.1.3] - 2024-01-03

### Changed

- [apple2] sp_init looks from slot 7 down to 1 instead of up from 1 to 7
- [apple2] sp_init now additionally looks for an SP card WITH the network adapter on it, which should skip other installed SP devices
- [apple2] sp_init only runs once and stores network id, close no longer resets it.
- lots of tests fixed (cycle count errors mostly)
- add network_init to detect network errors early. Implemented on APPLE2, nop on atari.
  Note: network_open still checks if appropriate init has happened on apple, but network_init will do same thing first, and then open will use the results from init.

## [2.1.2] - 2024-01-03

### Changed

- network_read handles chunked and delayed loading, and is common cross platform C file
- refactored subdirectories of apple code in preparation for fn_io for apple2

## [2.1.1] - 2023-12-07

### Changed

- Set `_sp_network` after calling `sp_find_network` in apple2 so ioctl can work after finding a network without calling open

### Added

- Tests for network_http_* functions

## [2.1.0] - 2023-11-15

### Added

- Added network_http_add_header
- Added network_http_start_add_headers
- Added network_http_end_add_headers
- Added network_http_post
- Added network_http_put
- Added network_http_delete

- Combined fn_io.lib with fn_network.lib for single library

### Changed

- apple2 - Check if there are any devices before issuing DIB command in sp_find_device

## [2.0.0] - 2023-09-24

### Added

- Moved BDD src to separate project as it shared over multiple projects
- Added apple-single testing feature
- Added Smart Port emulator for feature tests to be able to run apple2 fujinet code
- Added network_open for apple2
- Added network_close for apple2
- Added network_status for apple2
- Added network_read for apple2
- Added network_write for apple2
- Added network_ioctl for apple2
- Create set of standard error codes that device specific values get converted to for both apple2 and atari
- Added network_json_query for atari and apple2
- Added network_json_parse for atari and apple2

Test applications that run on hardware can be found at <https://github.com/markjfisher/fujinet-network-lib-tests>
Unit tests are still run from testing/bdd-testing directory.

### Breaking

- network_status: add bw, c, err parameters
- network_open signature change: add mode

## [1.0.0] - 2023-09-24

### Added

- This changelog to instil a process of clean version documentation for the library.
- Common Makefile
- Initial implementations of atari functions:
  - network_close, network_open, network_read, network_write, network_status, network_ioctl
- cross platform BDD testing framework, atari implementation done, c64 skeleton created

## Notes

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Use Added, Removed, Changed in triple headings.

Keep entries to lists and simple statements.

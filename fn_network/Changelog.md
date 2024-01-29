# Changelog

## [Unreleased]

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

# `fujinet-apps`

The following repository is full of older fujinet application examples.

This repository will eventually be archived, as it contains older examples before things like fujinet-lib came along.
The examples are useful to read, but a better "source of truth" is in the `fujinet-lib-examples` repo (see list below).

## Applications that have moved to their own repos

Various applications have been moved out into their own repositories including:

- [news](https://github.com/FujiNetWIFI/fujinet-news)
- [fujinet-lib-examples](https://github.com/FujiNetWIFI/fujinet-lib-examples)
- [weather](https://github.com/FujiNetWIFI/fujinet-weather)
- [cater](https://github.com/FujiNetWIFI/fujinet-cater)
- [iss-tracker](https://github.com/FujiNetWIFI/fujinet-iss-tracker)
- [contiki-webbrowser](https://github.com/FujiNetWIFI/fujinet-contiki-webbrowser)

The existing code here was moved into 00_moved-to-new-repo for archival purposes, please made all new commits in the new repos above if you are going to contribute to that project.


## Creating new applications

If you want to create a new fujinet application, your best bet is using cc65 and fujinet-lib, as it contains the common code
allowing you to easily write Network or FujiNet device applications for many platforms (as of writing this: atari, apple2, c64, and coco).

Use the [fujinet-build-tools](https://github.com/FujiNetWIFI/fujinet-build-tools) repository as a basis for your new project,
it has documentation on how to use it (which is basically to copy the files, and copy the Makefile sample to the root of your folder).

## Discord

Come and talk to us in discord! Use the [following link to join](https://discord.gg/4FzSnXBJFR)
and then ask us anything you like regarding the fujinet!

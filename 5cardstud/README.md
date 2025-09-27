# 5-card-stud

Client implementations for the 5CS Server. You can read more about how the server works by viewing the readme at: https://github.com/FujiNetWIFI/servers/tree/main/fujinet-game-system/5cardstud/server

## Platforms

#### Cross Platform
The `cross-platform` folder contains a shared C project that supports the following platforms. Please add new platforms to this project:
* Adam
* Apple II
* Coco (32KB+)
* C64
* Ms-Dos

#### Atari
The `atari` folder contains a FastBasic project that supports Atari, and was the original implimentation before moving to the above C cross platform.

#### Archived
Previous iterations have been pushed into `archived`

## Testing in Emulators

#### FujiNet-PC
FujiNet-PC is available on the following platforms to test the game on emulators with no special changes.
* Atari
* Apple II
* Coco

#### C64

Open misc.h and follow the instructions above the `USE_EMULATOR` define
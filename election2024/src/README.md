# source files

Filenames that the build understands are:

- `*.c` C Source Files
- `*.s` Assembler files

You can add `*.inc` or `*.h` files into any path detailed below for inclusion from assembler or C files.

## src dir structure

In your source folder, only the top `src` directory must exist, any other folder under this is purely optional to allow
for cross-platform compilation and separation, and shared files between different platforms or specific targets.

The full `src` dir has the following structure:

```text
src
├── main.c              # all targets compile files in src/ as part of their build
├── main.h
├── *.c|h|s             #  ... and any other C, H or S file you wish to include
│
├── common/             # common to all targets, including its sub-dirs
│   ├── utils/
│   └──── fn1.c         # doesn't have to be in just common, can be a subdir
│
├── apple2/             # "platform" level, so apple2/apple2enh
│   └── foo/            # ... and subfolders
│
├── atari/              # all atari platforms (atari/atarixl)
│   └── bar/            # ... and subfolders
│
├── c64/                # all commodore platforms, pet etc
│   └── qux/            # ... and subfolders
│
└── current-target/
    ├── apple2/         # only used when building "apple2" 
    │   └── baz/        # ... and subfolders
    │
    └── apple2enh/      # only used when building "apple2enh"
        └── xxx/        # ... and subfolders
```

You can keep it simple and put every file in `src/` if you have no platform or target specific code.

AFTER READING THE FOLDER STRUCTURE BELOW, IT IS COMPLETELY SAFE TO DELETE ALL THE SUB-DIRECTORIES UNDER src/.
IF YOU ONLY WISH TO USE A SINGLE src/ FOLDER FOR ALL CODE. THE FOLLOWING IS OPTIONAL AND GIVES FLEXIBILTY
BUT IS NOT AT ALL REQUIRED. HOWEVER IT ALSO ALLOWS YOUR CODE TO EXPAND AS YOU GET DEEPER INTO CROSS PLATFORM
APPLICATIONS.

A breakdown of each folder:

- `src/`

Any file in this top level only directory is available to all platforms at compile time.

- `src/common`

Any file in this directory or any sub-directory of it will be available at compile time to all platforms.
This differs from the root directory in that any sub-dir folder is recursively available for the compiler, in order to
allow you to structure your C code or Assembley code how you want.

- `src/<platform>`

The `platform` folders allow code for a particular platform to be compiled together, no matter what the specific target
of that platform is.

Like the common dir, sub-directories are recursively included in compilation.

The platforms are:

1. apple2 (apple2, and apple2enh)
2. atari (atari, and atarixl)
3. c64 (c64, c128, c16, pet, plus4, vic20)

Thus anything in the `src/apple2` directory and sub-directories will compile for both the "apple2" and "apple2enh" targets.

- `src/current-target/<target>`

This directory and sub-folders are for specific targets, e.g. code in `apple2` will not be included when compiling
`apple2enh` target, allowing a separation by folder for different targets within a platform.
It is highly unlikely you will need this, but the build will cope with it if you need it.

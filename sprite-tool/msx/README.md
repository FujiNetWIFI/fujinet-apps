# MekkoGX

[MekkoGX](https://github.com/fozzTexx/MekkoGX) is a cross-platform
build template for retro and classic computers. It provides a
collection of modular Makefiles and a top-level template project to
simplify compiling, linking, and building disk images across multiple
platforms.

A major feature is **FujiNet-lib integration**. You can point
`FUJINET_LIB` to a version, a directory, a zip archive, or a git
repository, and it will be fetched if needed. Include paths and
libraries are passed to your compiler and linker seamlessly.

Another feature is **GitHub CI builds**. Push your project to GitHub
and the CI will automatically compile for all configured
platforms. This is particularly valuable because retro projects often
require **many different toolchains**. Even though
[defoogi](https://github.com/FozzTexx/defoogi) avoids the headache of
setting up the needed toolchains, it relies on Docker — which many
developers don’t have installed or don’t want to use.

## Features

- Cross-platform Makefile library for retro computers
- Build a single platform on demand by running `make <platform>`,
  without modifying the `PLATFORMS=` list in the Makefile
- Top-level Makefile where all project-specific customization can be done
- Support for platform-specific and compiler-specific customization hooks
- FujiNet-lib integration for automated library fetching and linking
- One-off customization by passing variable definitions on the `make`
  command line, allowing developers to tweak a single build without
  editing the Makefile
- Post-build hooks to modify executables, disk images, or r2r (Ready 2
  Run) outputs without changing reusable makefiles
- GitHub CI builds for configured platforms automatically

## Getting Started

### 1. Create a New Project from the Template

Click the **Use this template** button on GitHub to create a new
repository from MekkoGX. This ensures you start with a clean history
and all required build scaffolding.

### 2. Configure the Top-Level Makefile

Edit the `Makefile` in your project root:

- Set `PRODUCT` to your program name.
- Specify the platforms you want to target in `PLATFORMS`.
- Optionally set `EXTRA_INCLUDE` to include additional header directories.
- Optionally set `FUJINET_LIB` to point to a version, directory, zip file, or repository.

Example:

```
PRODUCT = hello
PLATFORMS = coco apple2

# Extra include directories for project-specific headers
EXTRA_INCLUDE = ../my-extra-headers

# FujiNet-lib can be a version number, a directory, a zip archive, or a git URL.
FUJINET_LIB = 4.7.6
```

### 3. Build

- Build for all configured platforms:

```
make
```

- Build a specific platform:

```
make coco
make apple2/r2r
```

### 4. Post-Build Customization

You can use post hooks to modify build outputs without altering reusable platform makefiles:

Example (adding extra files to a CoCo disk image):

```
DISK_POSTDEPS_COCO := r2r/coco/extra.bin basic/coco/song.bas
coco/disk-post::
        decb copy -b -2 r2r/coco/extra.bin "$(DISK),EXTRA.BIN"
        decb copy -t -0 basic/coco/song.bas "$(DISK),SONG.BAS"
```

> Note: Extra dependencies do **not** automatically get added to the
  output. You are responsible for handling the files appropriately for
  your platform. FujiNet-lib includes and libraries are automatically
  passed to the compiler/linker if configured.

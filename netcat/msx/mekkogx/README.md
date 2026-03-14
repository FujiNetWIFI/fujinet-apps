# MekkoGX Modular Makefile Framework

[MekkoGX](https://github.com/fozzTexx/MekkoGX) is a cross-platform
build template for retro and classic computers. It provides a
collection of modular Makefiles and a top-level template project to
simplify compiling, linking, and building disk images across multiple
platforms.

The main goal is to make it easy to:

* Easily switch between different FUJINET_LIB versions by setting a
  single variable (supports directories, releases, or Git URLs).
* Keep platforms and toolchains fully modular: each uses a small .mk
  file with shared logic in common.mk and tc-common.mk, making it
  simple to add new platforms or toolchains.
* **Keep all project-specific customization in the top-level Makefile,
  where it’s visible and easy to maintain.**

Think of this as a library of Makefiles.

* `platforms/*.mk` files provide reusable rules — how to build
  executables, how to create disk images, and other platform-specific
  steps.
* `toolchains/*.mk` files define compiler-specific flags and helper
  functions.
* The project’s top-level Makefile ties everything together, declaring
  what to build while reusing the shared rules.

The result is a modular system: platforms and toolchains know *how* to
build, and the top-level Makefile defines *what* to build.

## 1. The Top-Level Makefile

The **project’s Makefile** is the only file you should need to edit
when starting a new project. It contains:

- Your project’s `PRODUCT` name.
- The `PLATFORMS` you want to build for.
- The `SRC_DIRS` containing your source code.
- Optional project-specific rules or custom targets.

You can run:

```
make
```

This will build for every platform listed in the `PLATFORMS` variable.

Or, you can build for a single platform or platform-specific target:

```
make <platform>
make <platform>/<target>
```

Examples:

```
make coco
make apple2/disk
```

### FujiNet library (`FUJINET_LIB`)

This variable specifies which FujiNet library to use when building your project. It supports several formats:

* **Version number** — e.g., `4.7.6`
* **Directory** — a path containing the libraries for each platform
* **Zip archive** — a locally built or downloaded zip file containing the library
* **Git repository URL** — a remote repo to clone
* **Empty** — automatically uses the latest available library
* **Undefined** — disables use of a FujiNet library entirely

#### Example

```
# Use FujiNet library version 4.7.6
FUJINET_LIB = 4.7.6
```

### Source directories (`SRC_DIRS`)

`SRC_DIRS` lists the directories that `make` should search for source files.
It supports two special features:

#### 1. **Platform-aware directory expansion**

Use the literal `%PLATFORM%` token to have directories expand based on the active platform.

Example:

```
SRC_DIRS = src src/%PLATFORM%
```

> **Note:** `%PLATFORM%` is used instead of `$(PLATFORM)` to avoid accidental
> expansion inside unrelated directory names.

#### 2. **Recursive directory search with `/**`**

Appending `/**` to any entry makes `make` recursively search *all* subdirectories.

Example:

```
# Recursively search inside src/apple2/
SRC_DIRS = src src/apple2/**
```

You can also combine both features:

```
# All dirs named "exact" under the platform prefix, recursively
SRC_DIRS = src/%PLATFORM%/**/exact
```

---

### Include directories (`INCLUDE_DIRS`)

`INCLUDE_DIRS` works exactly like `SRC_DIRS`.
It supports `%PLATFORM%` and `/**`, and all expanded paths are added to both
`CFLAGS` and `ASFLAGS` include search paths.

Example:

```
INCLUDE_DIRS = include include/%PLATFORM% include/common/**
```

### Platform Combos (`PLATFORM_COMBOS`)

`PLATFORM_COMBOS` defines extra directories (or “combo” platforms)
that are automatically included whenever a main platform is
built. This lets a single platform pull in multiple source directories.

Format:

```
PLATFORM_COMBOS = \
  platform1+=combo1,combo2 \
  platform2+=comboA,comboB
```

Example:

```
PLATFORM_COMBOS = \
  c64+=commodore,eightbit \
  atarixe+=atari
```

With `SRC_DIRS = src src/%PLATFORM%`, building for `c64` would expand `%PLATFORM%` into:

- `src/c64`
- `src/commodore`
- `src/eightbit`

### The `r2r` "Ready 2 Run" Target

The `r2r` target is the **default build output** for a platform. It
will always build the platform’s executable. For some platforms, it
also creates a bootable disk image.

For example:

- On **CoCo**, `r2r` builds a `.bin` file and a `.dsk` disk image.
- On **Apple II**, `r2r` builds an AppleSingle file and a `.po` disk image.
- Other platforms behave similarly depending on their toolchains.

#### Output Location

All build artifacts go under the `r2r/<platform>/` directory:

- Executables: `r2r/<platform>/<product>.<ext>`
- Optional disk images: `r2r/<platform>/<product>.dsk`

### Customizing Per Platform or Compiler

There are two kinds of per-platform/per-compiler variables:

1. Extra include directories – add paths for the compiler and assembler to search:
  * Use `EXTRA_INCLUDE` or `EXTRA_INCLUDE_<platform>`to specify one or
    more directories. They are automatically added to `-I` flags
    during compilation and assembly.
2. Extras flags – add flags or options without removing the defaults:
  * Example: `LDFLAGS_EXTRA_COCO` adds extra linker flags when building for CoCo.
3. Overrides – completely replace the default for a platform:
  * Example: `CC_APPLE2` sets the compiler to use for Apple II,
    replacing the default `cc65` setup.

### Platform-specific r2r or disk steps

You can add platform-specific actions by defining double-colon targets:

```
<platform>/r2r:: <extra-dep1> <extra-dep2>
<platform>/disk:: <extra-dep1> <extra-dep2>
```

Examples:

```
coco/r2r:: coco/custom-step1
coco/r2r:: coco/custom-step2
```

Or define a full recipe:

```
coco/r2r::
    echo "Doing something special for Coco r2r"
    ./tools/special-process coco
```

Notes:

* Double-colon `::` allows multiple isolated definitions for the same target without overwriting existing ones.
* You can also use single-colon `:` if you want a single line with multiple dependencies:

```
coco/r2r: coco/custom-step1 coco/custom-step2
```

This mechanism allows you to build additional things for a specific
platform when building standard targets.

### Post-build hooks and extra dependencies

For advanced customization, you can define extra dependencies and
post-build targets. These allow you to:

* Ensure extra files are built or up to date before a main build step.
* Run additional commands **after** a build step, without modifying the main recipe.

#### Key points

* The extra dependencies variables (e.g., `EXECUTABLE_EXTRA_DEPS_COCO`,
  `DISK_EXTRA_DEPS_COCO`, `R2R_EXTRA_DEPS_COCO`) **do not automatically
  add files to the output**. They only make sure that your custom
  files are rebuilt when modified.
* You are responsible for handling these extra dependencies
  appropriately in your post-build steps. For instance, adding files
  to a disk requires platform-specific commands.

#### Example: Adding files to a COCO disk

```
DISK_EXTRA_DEPS_COCO := r2r/coco/4voice.bin basic/coco/song.bas

coco/disk-post::
        decb copy -b -2 r2r/coco/4voice.bin "$(DISK),4VOICE.BIN"
        decb copy -t -0 basic/coco/song.bas "$(DISK),SONG.BAS"
```

Notes:

* Each `<platform>/disk-post` target must be defined if you want
  post-processing for that platform.
* Multiple post targets for the same platform can coexist thanks to
  double-colon `::`.
* This mechanism allows you to safely manipulate build outputs like
  executables or disk images without changing the original recipes.

---

## 2. Mid-Level Files

There are three “mid-level” Makefiles that provide shared rules:

- **`common.mk`**
  Defines shared variables and helper rules.

- **`toplevel-rules.mk`**
  Provides project-wide rules for building, cleaning, and delegating
  into the platform Makefiles. This file is what ties the
  `platforms/*.mk` and `toolchains/*.mk` files into your project.

- **`tc-common.mk`**
  Provides common things for the `toolchains/*.mk` files.

---

## 3. Platform Makefiles (`platforms/*.mk`)

Each file in `platforms/` defines how to build for a **specific computer platform**.
This includes:

- How to build the main executable (`EXECUTABLE`).
- How to package artifacts (like creating a disk image).
- Any special quirks for that system’s toolchain.

---

## 4. Tool Chain Makefiles (`toolchains/*.mk`)

These files define toolchain-specific behavior, such as:

- How to compile `.c` or `.s` files.
- What flags to use for linking.
- What commands to use for `LD`, `CC`, or `AR`.

For example, you can override the linker for a project like this in your top-level Makefile:

```
LD_COCO = ./my-custom-ld-wrapper.sh
```

Then your wrapper script can call the real linker and perform additional steps on the output.

This keeps toolchain logic reusable across multiple platforms.

---

## Summary

- **Top-level Makefile**: project-specific: define `PLATFORM`, `PRODUCT`, and any extra targets.
- **common.mk, toplevel-rules.mk, tc-common.mk**: glue: connect top-level, platforms, and toolchains.
- **platforms/*.mk**: platform-specific: rules for executables, disk images, quirks.
- **toolchains/*.mk**: toolchain-specific: flags and build rules.

All customization should happen in the **top-level Makefile**.
The rest of the framework is designed to be reused without modification.

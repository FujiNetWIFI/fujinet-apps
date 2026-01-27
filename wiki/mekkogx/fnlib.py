#!/usr/bin/env python3
import argparse
import os, sys
import re
import zipfile
import urllib.request
import json
import subprocess

FUJINET_REPO = "FujiNetWIFI/fujinet-lib"
GITHUB_API = "https://api.github.com/repos"
GITHUB_URL = "https://github.com"
CACHE_DIR = "_cache"
FUJINET_CACHE_DIR = os.path.join(CACHE_DIR, "fujinet-lib")

VERSION_NUM_RE = r"([0-9]+[.][0-9]+[.][0-9]+)"
VERSION_NAME_RE = fr"v?{VERSION_NUM_RE}"
LDLIB_REGEX = r"lib(.*)[.]a$"

# FIXME - this is really toolchains, not platforms
LDLIB_PLATFORMS = ["coco", "dragon"]

def build_argparser():
  parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument("file", nargs="?", help="input file")
  parser.add_argument("--platform", help="platform building for")
  parser.add_argument("--skip-if-missing", action="store_true",
                      help="don't error if fujinet-lib isn't available")
  return parser

class MakeVariables:
  def __init__(self, varList):
    for key in varList:
      setattr(self, key, "")
    return

  @staticmethod
  def escapeForMake(val):
    if not val:
      return ""
    return (
      val
      .replace("\\", "\\\\")  # escape backslashes first
      .replace('"', '\\"')
      .replace("$", "$$")
      .replace(" ", "\\ ")
      .replace(":", "\\:")
      .replace("#", "\\#")
    )

  def printValues(self):
    attrs = self.__dict__.keys()
    for key in attrs:
      print(f"{key}:={self.escapeForMake(getattr(self, key))}")
    return

class LibLocator:
  def __init__(self, FUJINET_LIB, PLATFORM, COMBOS, skipIfMissing=False):
    """
    FUJINET_LIB can be
      - a version number such as 4.7.4
      - a directory which contains the libs for each platform
      - a zip file with an archived fujinet-lib
      - a URL to a git repo
      - empty
    """

    self.skipIfMissing = skipIfMissing

    self.MV = MakeVariables([
      "FUJINET_LIB_DIR",
      "FUJINET_LIB_FILE",
      "FUJINET_LIB_INCLUDE",
      "FUJINET_LIB_LDLIB",
      "FUJINET_LIB_PLATFORM",
      "FUJINET_LIB_VERSION",
      "FUJINET_LIB_ZIP",
    ])

    self.PLATFORM = PLATFORM
    self.combos = self.combosToDict(COMBOS)
    self.possiblePlatforms = [self.PLATFORM, *self.combos.get(self.PLATFORM, [])]

    # Several possible library filename patterns:
    #   - fujinet-coco-4.7.6.lib
    #   - fujinet.apple2.lib
    #   - fujinet.lib.c64
    #   - libfujinet.coco.a
    self.LIBRARY_REGEX = []
    for platform in self.possiblePlatforms:
      self.LIBRARY_REGEX.extend([
        fr"fujinet[-.]({platform})(-{VERSION_NUM_RE})?[.]lib$",
        fr"fujinet[.]lib[.]({platform})$",
        fr"libfujinet[.]({platform})[.]a$",
      ])

    if FUJINET_LIB:
      rxm = re.match(VERSION_NAME_RE, FUJINET_LIB)
      if rxm:
        self.MV.FUJINET_LIB_VERSION = rxm.group(1)
      elif any(sub in FUJINET_LIB for sub in ("://", "@")):
        self.gitClone(FUJINET_LIB)
      elif os.path.isfile(FUJINET_LIB):
        _, ext = os.path.splitext(FUJINET_LIB)
        if ext == ".zip":
          self.MV.FUJINET_LIB_ZIP = FUJINET_LIB
        else:
          self.MV.FUJINET_LIB_DIR = os.path.dirname(FUJINET_LIB)
          self.MV.FUJINET_LIB_FILE = os.path.basename(FUJINET_LIB)
      elif os.path.isdir(FUJINET_LIB):
        self.findLibraryDir(FUJINET_LIB)
        if not self.MV.FUJINET_LIB_DIR:
          error_exit(f"\"{FUJINET_LIB}\" does not appear to contain a library")

    if not self.MV.FUJINET_LIB_VERSION:
      self.getVersion()

    if not self.MV.FUJINET_LIB_DIR:
      self.getDirectory()

    if not self.MV.FUJINET_LIB_FILE \
       and (not self.MV.FUJINET_LIB_ZIP or not os.path.exists(self.MV.FUJINET_LIB_ZIP)):
      self.downloadZip()

    if not self.MV.FUJINET_LIB_INCLUDE:
      self.getInclude()

    if not self.MV.FUJINET_LIB_PLATFORM:
      self.MV.FUJINET_LIB_PLATFORM = self.PLATFORM

    self.MV.FUJINET_LIB_FILE = self.fixupLibraryFilename(self.MV.FUJINET_LIB_FILE)
    self.MV.FUJINET_LIB_LDLIB = self.MV.FUJINET_LIB_FILE

    # If FUJINET_LIB_LDLIB is specially named for linker to find, make
    # sure FUJINET_LIB_LDLIB is fixed up appropriately
    rxm = re.match(LDLIB_REGEX, self.MV.FUJINET_LIB_LDLIB)
    if rxm:
      self.MV.FUJINET_LIB_LDLIB = rxm.group(1)

    return

  def checkLibraryFilename(self, filename):
    for pattern in self.LIBRARY_REGEX:
      rxm = re.match(pattern, filename)
      if rxm:
        return rxm
    return None

  def findLibrary(self, filelist):
    for filename in filelist:
      rxm = self.checkLibraryFilename(filename)
      if rxm:
        self.setPlatformVersion(rxm)
        return rxm
    return None

  def fixupLibraryFilename(self, filename):
    # Some linkers require the library to be named specially in order
    # to be used with the `-l` flag. Create symlink if necessary.

    symlink_file = None
    version = ""
    if self.MV.FUJINET_LIB_VERSION:
      version = f"-{self.MV.FUJINET_LIB_VERSION}"

    if self.PLATFORM in LDLIB_PLATFORMS:
      if not re.match(LDLIB_REGEX, filename):
        symlink_file = f"libfujinet-{self.MV.FUJINET_LIB_PLATFORM}{version}.a"

    elif not filename.endswith(".lib"):
      symlink_file = f"fujinet-{self.MV.FUJINET_LIB_PLATFORM}{version}.lib"

    if symlink_file:
      symlink_path = os.path.join(self.MV.FUJINET_LIB_DIR, symlink_file)
      if not os.path.exists(symlink_path):
        os.symlink(filename, symlink_path)
      filename = symlink_file

    return filename

  def setPlatformVersion(self, rxm):
    if len(rxm.groups()) >= 1:
      self.MV.FUJINET_LIB_PLATFORM = rxm.group(1)
    if len(rxm.groups()) >= 3:
      self.MV.FUJINET_LIB_VERSION = rxm.group(3)
    return

  def getVersion(self):
    if self.MV.FUJINET_LIB_ZIP:
      with zipfile.ZipFile(self.MV.FUJINET_LIB_ZIP, "r") as zf:
        rxm = self.findLibrary(zf.namelist())
        if rxm:
          return

      raise ValueError("Which file is the newest?")

    if not self.MV.FUJINET_LIB_DIR:
      cached = self.findCacheDir()
      if cached:
        self.MV.FUJINET_LIB_DIR = cached

    if self.MV.FUJINET_LIB_DIR:
      rxm = self.findLibrary(os.listdir(self.MV.FUJINET_LIB_DIR))
      if rxm:
        self.MV.FUJINET_LIB_FILE = rxm.group(0)
        return
      if self.skipIfMissing:
        exit(0)
      #error_exit(f"No library found for \"{self.PLATFORM}\"")
      if not self.MV.FUJINET_LIB_FILE:
        raise ValueError
      return

    # No version was specified, so any version is fine
    if self.MV.FUJINET_LIB_VERSION:
      return

    latest_url = f"{GITHUB_API}/{FUJINET_REPO}/releases/latest"
    with urllib.request.urlopen(latest_url) as response:
      response = response.read().decode("UTF-8")
      release_info = json.loads(response)

    latest_version = release_info.get("tag_name") or release_info.get("name")
    if not latest_version:
      raise ValueError("Can't find version")

    rxm = re.match(VERSION_NAME_RE, latest_version)
    if not rxm:
      raise ValueError("Not a FujiNet-lib version", latest_version)

    self.MV.FUJINET_LIB_VERSION = rxm.group(1)
    return

  def findCacheDir(self):
    pattern = fr"{VERSION_NUM_RE}-{self.PLATFORM}"
    if not os.path.isdir(FUJINET_CACHE_DIR):
      return None
    for path in os.listdir(FUJINET_CACHE_DIR):
      rxm = re.match(pattern, path)
      if rxm:
        return os.path.join(FUJINET_CACHE_DIR, path)
    return None

  def getDirectory(self):
    global FUJINET_CACHE_DIR
    self.MV.FUJINET_LIB_DIR = os.path.join(FUJINET_CACHE_DIR,
                                           f"{self.MV.FUJINET_LIB_VERSION}-{self.PLATFORM}")
    return

  def downloadZip(self):
    global FUJINET_CACHE_DIR
    os.makedirs(self.MV.FUJINET_LIB_DIR, exist_ok=True)

    for platform in self.possiblePlatforms:
      self.MV.FUJINET_LIB_FILE = f"fujinet-{platform}-{self.MV.FUJINET_LIB_VERSION}.lib"
      if not os.path.exists(os.path.join(self.MV.FUJINET_LIB_DIR, self.MV.FUJINET_LIB_FILE)):
        zip_path = f"fujinet-lib-{platform}-{self.MV.FUJINET_LIB_VERSION}.zip"

        if not self.MV.FUJINET_LIB_ZIP:
          self.MV.FUJINET_LIB_ZIP = os.path.join(FUJINET_CACHE_DIR, zip_path)

        if not os.path.exists(self.MV.FUJINET_LIB_ZIP):
          release_url = f"{GITHUB_URL}/{FUJINET_REPO}/releases/download" \
            f"/v{self.MV.FUJINET_LIB_VERSION}/{zip_path}"
          try:
            urllib.request.urlretrieve(release_url, self.MV.FUJINET_LIB_ZIP)
          except:
            continue

        with zipfile.ZipFile(self.MV.FUJINET_LIB_ZIP, "r") as zf:
          zf.extractall(self.MV.FUJINET_LIB_DIR)

        return

      #error_exit("Unable to download FujiNet library from", release_url)
      return

  def gitClone(self, url):
    global FUJINET_CACHE_DIR
    os.makedirs(FUJINET_CACHE_DIR, exist_ok=True)
    branch = ""
    if "#" in url:
      url, branch = url.split("#")
    base = url.rstrip("/").split("/")[-1]
    if base.endswith(".git"):
      base = base.rsplit(".", 1)[0]
    repoDir = os.path.join(FUJINET_CACHE_DIR, base)

    if not os.path.exists(repoDir):
      cmd = ["git", "clone", url]
      if branch:
        cmd.extend(["-b", branch])
      subprocess.run(cmd, cwd=FUJINET_CACHE_DIR, check=True, stdout=sys.stderr)

    self.findLibraryDir(repoDir)
    if not self.MV.FUJINET_LIB_FILE:
      cmd = ["make", ]
      subprocess.run(cmd, cwd=repoDir, check=True, stdout=sys.stderr)
      self.findLibraryDir(repoDir)

    return

  def findLibraryDir(self, baseDir):
    dirsToCheck = ["", "build", *[f"r2r/{p}" for p in self.possiblePlatforms]]
    for pdir in dirsToCheck:
      pdir = os.path.join(baseDir, pdir)
      if os.path.isdir(pdir):
        self.MV.FUJINET_LIB_DIR = pdir
        rxm = self.findLibrary(os.listdir(self.MV.FUJINET_LIB_DIR))
        if rxm:
          if not self.MV.FUJINET_LIB_PLATFORM:
            raise ValueError("What platform?", rxm)
          self.MV.FUJINET_LIB_FILE = rxm.group(0)
          return rxm
    return None

  def getInclude(self):
    parent = os.path.dirname(self.MV.FUJINET_LIB_DIR.rstrip("/"))
    checkDirs = [self.MV.FUJINET_LIB_DIR, parent, os.path.join(parent, "include")]
    components = self.MV.FUJINET_LIB_DIR.split(os.path.sep)
    if components[-1] == self.MV.FUJINET_LIB_PLATFORM and components[-2] == "r2r":
      checkDirs.append(os.path.join(os.path.dirname(parent), "include"))
    for idir in checkDirs:
      if os.path.exists(os.path.join(idir, "fujinet-fuji.h")):
        self.MV.FUJINET_LIB_INCLUDE = idir
        return
    if self.skipIfMissing:
      exit(0)
    error_exit("Unable to find include directory", self.MV.FUJINET_LIB_DIR)
    return

  def printMakeVariables(self):
    self.MV.printValues()
    if self.MV.FUJINET_LIB_LDLIB:
      print(f"CFLAGS_EXTRA_{self.PLATFORM.upper()}+=-DUSING_FUJINET_LIB")
    return

  @staticmethod
  def combosToDict(combos):
    if not combos:
      return {}

    comboDict = {}
    for part in combos.split():
      if "+=" not in part:
        continue
      platform, others = part.split("+=", 1)
      platform = platform.strip()
      others = [c.strip() for c in others.split(',') if c.strip()]
      if platform not in comboDict:
        comboDict[platform] = []
      comboDict[platform].extend(others)
    return comboDict

# Print errors to stderr so that `make` doesn't try to interpret them in `$(eval)`
def error_exit(*args):
  print(*args, file=sys.stderr)
  exit(1)

def main():
  global CACHE_DIR, FUJINET_CACHE_DIR

  args = build_argparser().parse_args()

  PLATFORM = os.getenv("PLATFORM")
  if args.platform:
    PLATFORM = args.platform

  if not PLATFORM:
    error_exit("Please specify PLATFORM")

  PLATFORM_COMBOS = os.getenv("PLATFORM_COMBOS")

  FUJINET_LIB = args.file
  if not FUJINET_LIB:
    FUJINET_LIB = os.getenv("FUJINET_LIB")

  env_cache_dir = os.getenv("CACHE_DIR")
  if env_cache_dir:
    CACHE_DIR = env_cache_dir
    FUJINET_CACHE_DIR = os.path.join(CACHE_DIR, os.path.basename(FUJINET_CACHE_DIR))

  fujinetLib = LibLocator(FUJINET_LIB, PLATFORM, PLATFORM_COMBOS, args.skip_if_missing)
  fujinetLib.printMakeVariables()

  return

if __name__ == "__main__":
  exit(main() or 0)

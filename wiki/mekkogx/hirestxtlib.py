#!/usr/bin/env python3
import argparse
import os, sys
import re
import tarfile
import urllib.request
import json
import subprocess

sys.path.insert(0, os.path.dirname(__file__))
from fnlib import MakeVariables, error_exit

HIRESTXT_REPO = "RichStephens/hirestxt-mod"
GITHUB_API = "https://api.github.com/repos"
GITHUB_URL = "https://github.com"
CACHE_DIR = "_cache"
HIRESTXT_CACHE_DIR = os.path.join(CACHE_DIR, "hirestxt-mod")

VERSION_NUM_RE = r"([0-9]+(?:[.][0-9]+)+)"
VERSION_NAME_RE = fr"v?{VERSION_NUM_RE}"

LIBRARY_FILE = "libhirestxt.a"
HEADER_FILE = "hirestxt.h"

VT52_SUFFIX = "+vt52"
# First release that ships a separate no-VT52 archive
NOVT52_MIN_VERSION = (0, 5, 1, 7)


def build_argparser():
  parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument("file", nargs="?", help="input file")
  parser.add_argument("--platform", help="platform building for")
  return parser


class HirestxtLocator:
  def __init__(self, HIRESTXT_LIB):
    self.MV = MakeVariables([
      "HIRESTXT_LIB_DIR",
      "HIRESTXT_LIB_INCLUDE",
      "HIRESTXT_LIB_LDLIB",
      "HIRESTXT_LIB_VERSION",
      "HIRESTXT_LIB_NO_VT52",
    ])

    self.version = ""
    self.vt52 = False

    if HIRESTXT_LIB in ("vt52", VT52_SUFFIX):
      self.vt52 = True
      HIRESTXT_LIB = ""

    if HIRESTXT_LIB:
      rxm = re.fullmatch(fr"{VERSION_NAME_RE}({re.escape(VT52_SUFFIX)})?", HIRESTXT_LIB)
      if rxm:
        self.version = rxm.group(1)
        self.vt52 = bool(rxm.group(2))
      elif any(sub in HIRESTXT_LIB for sub in ("://", "@")):
        self.gitClone(HIRESTXT_LIB)
      elif os.path.isdir(HIRESTXT_LIB):
        self.findLibrary(HIRESTXT_LIB)
        if not self.MV.HIRESTXT_LIB_DIR:
          error_exit(f"\"{HIRESTXT_LIB}\" does not appear to contain {LIBRARY_FILE}")
      else:
        error_exit(f"Unrecognized HIRESTXT_LIB \"{HIRESTXT_LIB}\"")

    if not self.version and not self.MV.HIRESTXT_LIB_DIR:
      self.getLatestVersion()

    if self.version and not self.MV.HIRESTXT_LIB_DIR:
      self.downloadRelease()

    if not self.MV.HIRESTXT_LIB_INCLUDE:
      self.getInclude()

    if self.MV.HIRESTXT_LIB_DIR:
      self.MV.HIRESTXT_LIB_LDLIB = "hirestxt"

    self.MV.HIRESTXT_LIB_VERSION = self.version

  def findLibrary(self, baseDir):
    for subdir in ["", "build"]:
      path = os.path.join(baseDir, subdir)
      if os.path.isdir(path) and os.path.exists(os.path.join(path, LIBRARY_FILE)):
        self.MV.HIRESTXT_LIB_DIR = path
        return True
    return False

  def getLatestVersion(self):
    latest_url = f"{GITHUB_API}/{HIRESTXT_REPO}/releases/latest"
    with urllib.request.urlopen(latest_url) as response:
      release_info = json.loads(response.read().decode("UTF-8"))

    latest_version = release_info.get("tag_name") or release_info.get("name")
    if not latest_version:
      error_exit("Can't find latest hirestxt-mod version")

    rxm = re.match(VERSION_NAME_RE, latest_version)
    if not rxm:
      error_exit("Not a valid hirestxt-mod version:", latest_version)

    self.version = rxm.group(1)

  def downloadRelease(self):
    global HIRESTXT_CACHE_DIR
    versionTuple = tuple(int(x) for x in self.version.split("."))
    variant = ""
    if not self.vt52 and versionTuple >= NOVT52_MIN_VERSION:
      variant = "novt52-"
      self.MV.HIRESTXT_LIB_NO_VT52 = "1"

    versionDir = os.path.join(HIRESTXT_CACHE_DIR, variant + self.version)

    if os.path.exists(os.path.join(versionDir, LIBRARY_FILE)):
      self.MV.HIRESTXT_LIB_DIR = versionDir
      return

    os.makedirs(HIRESTXT_CACHE_DIR, exist_ok=True)

    tarball_name = f"hirestxt-mod-bin-{variant}{self.version}.tar.gz"
    tarball_path = os.path.join(HIRESTXT_CACHE_DIR, tarball_name)

    if not os.path.exists(tarball_path):
      release_url = f"{GITHUB_URL}/{HIRESTXT_REPO}/releases/download" \
        f"/{self.version}/{tarball_name}"
      try:
        urllib.request.urlretrieve(release_url, tarball_path)
      except Exception as e:
        error_exit(f"Unable to download hirestxt-mod {self.version}:", e)

    os.makedirs(versionDir, exist_ok=True)
    with tarfile.open(tarball_path, "r:gz") as tf:
      tf.extractall(versionDir)

    self.MV.HIRESTXT_LIB_DIR = versionDir

  def gitClone(self, url):
    global HIRESTXT_CACHE_DIR
    os.makedirs(HIRESTXT_CACHE_DIR, exist_ok=True)
    branch = ""
    if "#" in url:
      url, branch = url.split("#")
    base = url.rstrip("/").split("/")[-1]
    if base.endswith(".git"):
      base = base.rsplit(".", 1)[0]
    repoDir = os.path.join(HIRESTXT_CACHE_DIR, base)

    if not os.path.exists(repoDir):
      cmd = ["git", "clone", url]
      if branch:
        cmd.extend(["-b", branch])
      subprocess.run(cmd, cwd=HIRESTXT_CACHE_DIR, check=True, stdout=sys.stderr)

    if not self.findLibrary(repoDir):
      clean_env = {k: v for k, v in os.environ.items() if k != 'HIRESTXT_LIB'}
      clean_env['MAKEFLAGS'] = re.sub(
        r'\bHIRESTXT_LIB=\S*', '', clean_env.get('MAKEFLAGS', '')
      ).strip()
      subprocess.run(
        ["make", LIBRARY_FILE], cwd=repoDir, check=True, stdout=sys.stderr, env=clean_env
      )
      self.findLibrary(repoDir)

  def getInclude(self):
    if not self.MV.HIRESTXT_LIB_DIR:
      return
    checkDirs = [self.MV.HIRESTXT_LIB_DIR,
                 os.path.dirname(self.MV.HIRESTXT_LIB_DIR.rstrip("/")),
                 os.path.join(self.MV.HIRESTXT_LIB_DIR, "include")]
    for idir in checkDirs:
      if os.path.exists(os.path.join(idir, HEADER_FILE)):
        self.MV.HIRESTXT_LIB_INCLUDE = idir
        return
    error_exit("Unable to find hirestxt.h in", self.MV.HIRESTXT_LIB_DIR)

  def printMakeVariables(self):
    self.MV.printValues()


def main():
  global CACHE_DIR, HIRESTXT_CACHE_DIR

  args = build_argparser().parse_args()

  PLATFORM = os.getenv("PLATFORM")
  if args.platform:
    PLATFORM = args.platform

  if not PLATFORM:
    error_exit("Please specify PLATFORM")

  if PLATFORM not in ("coco", "dragon"):
    error_exit("hirestxt-mod is only available for coco/dragon, not", PLATFORM)

  HIRESTXT_LIB = args.file
  if not HIRESTXT_LIB:
    HIRESTXT_LIB = os.getenv("HIRESTXT_LIB")

  env_cache_dir = os.getenv("CACHE_DIR")
  if env_cache_dir:
    CACHE_DIR = env_cache_dir
    HIRESTXT_CACHE_DIR = os.path.join(CACHE_DIR, os.path.basename(HIRESTXT_CACHE_DIR))

  locator = HirestxtLocator(HIRESTXT_LIB)
  locator.printMakeVariables()


if __name__ == "__main__":
  exit(main() or 0)

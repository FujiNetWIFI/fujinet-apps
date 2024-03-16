#!/bin/bash
# Source this file, e.g.
# source ../apple-tools/ac-ver.sh

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

AC_VER="1.9.0"
export AC="java -jar ${SCRIPT_DIR}/AppleCommander-ac-${AC_VER}.jar"
export ACX="java -jar ${SCRIPT_DIR}/AppleCommander-acx-${AC_VER}.jar"

# this also fetches ProDOS. meh.
if [ ! -f "$SCRIPT_DIR}/ProDOS_2_4_2.dsk" ]; then
  curl -s -L -o "$SCRIPT_DIR/ProDOS_2_4_2.dsk" "https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/prodos/ProDOS_2_4_2.dsk"
fi

if [ ! -f "$SCRIPT_DIR/AppleCommander-ac-${AC_VER}.jar" ]; then
  curl -s -L -o "$SCRIPT_DIR/AppleCommander-ac-${AC_VER}.jar" "https://github.com/AppleCommander/AppleCommander/releases/download/${AC_VER}/AppleCommander-ac-${AC_VER}.jar"
  curl -s -L -o "$SCRIPT_DIR/AppleCommander-acx-${AC_VER}.jar" "https://github.com/AppleCommander/AppleCommander/releases/download/${AC_VER}/AppleCommander-acx-${AC_VER}.jar"
fi

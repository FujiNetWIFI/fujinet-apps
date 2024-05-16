#!/bin/bash
# Source this file, e.g.
# source ../dist.apple2/get-binaries.sh

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

OLD_VER="1.8.0"
if [ -f "${SCRIPT_DIR}/AppleCommander-ac-${OLD_VER}.jar" ] ; then
  rm ${SCRIPT_DIR}/AppleCommander-ac-${OLD_VER}.jar
  rm ${SCRIPT_DIR}/AppleCommander-acx-${OLD_VER}.jar
fi

AC_VER="1.9.0"
AC="java -jar ${SCRIPT_DIR}/AppleCommander-ac-${AC_VER}.jar"
ACX="java -jar ${SCRIPT_DIR}/AppleCommander-acx-${AC_VER}.jar"


if [ ! -f ${SCRIPT_DIR}/AppleCommander-ac-${AC_VER}.jar ]; then
  curl -s -L -o ${SCRIPT_DIR}/AppleCommander-ac-${AC_VER}.jar https://github.com/AppleCommander/AppleCommander/releases/download/${AC_VER}/AppleCommander-ac-${AC_VER}.jar
  curl -s -L -o ${SCRIPT_DIR}/AppleCommander-acx-${AC_VER}.jar https://github.com/AppleCommander/AppleCommander/releases/download/${AC_VER}/AppleCommander-acx-${AC_VER}.jar
fi

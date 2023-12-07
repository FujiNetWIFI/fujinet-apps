#!/bin/bash

if [ $# -ne 3 ]; then
  echo "Usage: $(basename $0) DISK.po FILE_TO_ADD NAME"
  echo "  Copies FILE_TO_ADD to DISK.po giving it name NAME"
  exit 1
fi

JAVA_LOCATION=$(which java)
if [ $? -eq 1 ]; then
  echo "Unable to find java on command line. You must have a working java at least version 11 to use this script."
  exit 1
fi

DISKFILE=$1
if [ ! -f $DISKFILE ]; then
  echo "Unable to find target DISK."
  exit 1
fi

INFILE=$2
if [ ! -f $INFILE ]; then
  echo "Unable to find file to put on disk."
  exit 1
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source ${SCRIPT_DIR}/get-binaries.sh

NAME=$3

${AC} -as ${DISKFILE} $NAME < ${INFILE}
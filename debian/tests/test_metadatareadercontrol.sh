#!/bin/sh
# autopkgtest check: Run the unit tests for the AalMetaDataReaderControl class
# to make sure that the control is sane and mediascanner is sane as well.

set -e

TESTFILE="testfile"

WORKDIR=$(mktemp -d)
trap "rm -rf $WORKDIR" 0 INT QUIT ABRT PIPE TERM
cd $WORKDIR

if [ `status mediascanner-2.0` -ne 0 ]; then
    echo "mediascanner-2.0 not running, starting"
    `start mediascanner-2.0`
fi

if [ ! -f "/home/phablet/Videos/$TESTFILE.mp4" ]; then
    echo "/home/phablet/Videos/$TESTFILE.mp4 does not exist"
    exit 1
fi

if [ ! -f "/home/phablet/Music/$TESTFILE.ogg" ]; then
    echo "/home/phablet/Music/$TESTFILE.ogg does not exist"
    exit 1
fi

if [ ! -f "/home/phablet/Pictures/$TESTFILE.jpg" ]; then
    echo "/home/phablet/Pictures/$TESTFILE.jpg does not exist"
    exit 1
fi

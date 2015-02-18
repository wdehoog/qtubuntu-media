#!/bin/sh
# Setup the mediascanner and copy test media into place

set -e

TESTFILE="testfile"
DEST_PREFIX="/home/phablet"

if [ ! -d "$DEST_PREFIX/Videos" ]; then
    echo "Creating directory $DEST_PREFIX/Videos"
    mkdir -p $DEST_PREFIX/Videos
fi

if [ ! -d "$DEST_PREFIX/Music" ]; then
    echo "Creating directory $DEST_PREFIX/Music"
    mkdir -p $DEST_PREFIX/Music
fi

if [ ! -d "$DEST_PREFIX/Pictures" ]; then
    echo "Creating directory $DEST_PREFIX/Pictures"
    mkdir -p $DEST_PREFIX/Pictures
fi

if [ ! -f "$DEST_PREFIX/Videos/$TESTFILE.mp4" ]; then
    echo "Copying video/$TESTFILE.mp4 to $DEST_PREFIX/Videos/$TESTFILE.mp4"
    cp videos/$TESTFILE.mp4 $DEST_PREFIX/Videos/$TESTFILE.mp4
fi

if [ ! -f "$DEST_PREFIX/Music/$TESTFILE.ogg" ]; then
    echo "Copying audio/$TESTFILE.ogg to $DEST_PREFIX/Music/$TESTFILE.ogg"
    cp audio/$TESTFILE.ogg $DEST_PREFIX/Music/$TESTFILE.ogg
fi

if [ ! -f "$DEST_PREFIX/Pictures/$TESTFILE.jpg" ]; then
    echo "Copying images/$TESTFILE.jpg to $DEST_PREFIX/Pictures/$TESTFILE.jpg"
    cp images/$TESTFILE.jpg $DEST_PREFIX/Pictures/$TESTFILE.jpg
fi

status mediascanner-2.0
if [ $? -ne 0 ]; then
    echo "mediascanner-2.0 not running, starting"
    start mediascanner-2.0
else
    echo "restarting mediascanner-2.0"
    restart mediascanner-2.0
fi

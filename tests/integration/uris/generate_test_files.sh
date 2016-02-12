#!/bin/bash

WORKING_DIR=/tmp/qtubuntu-media
BASE_FILE=$1/testdata/Ubuntu.ogg

mkdir $WORKING_DIR

echo "WORKING_DIR: $WORKING_DIR"
echo "BASE_FILE: $BASE_FILE"

# Creates several files with a single character filename that starts
# at ASCII char #32 and ends with #126
for i in {32..127}  # from 'Space' to '~'
do
    # skip '"', '%', '<', '>', '.', 'DEL'
    if [ $i -eq 127 ]
    then
        continue
    else
        chr=$(printf \\$(printf '%03o' $i))

        echo "Creating file $chr.ogg from $BASE_FILE" > "$WORKING_DIR/log.txt"
        cp $BASE_FILE "$WORKING_DIR/track$chr.ogg"
    fi
done

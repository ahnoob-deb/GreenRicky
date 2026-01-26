#!/bin/bash

# EXAMPLE : ./change-color.sh leadingedge font*.png 0019d3 ffffff
# EXAMPLE : ./change-color.sh leadingedge num*.png 4a6cc7 ffffff

if [ "$#" -ne 4 ]; then
    echo "Verwendung: $0 <fontfolder> <filepattern> <hexcol1> <hexcol2>"
    exit 1
fi

fromcolbyte1=${3:0:2}
fromcolbyte2=${3:2:2}
fromcolbyte3=${3:4:2}

tocolbyte1=${4:0:2}
tocolbyte2=${4:2:2}
tocolbyte3=${4:4:2}
echo "CHANGE:"
echo "FROM COLOR : 0x${fromcolbyte1}, 0x${fromcolbyte2}, 0x${fromcolbyte3}"
echo "TO COLOR : 0x${tocolbyte1}, 0x${tocolbyte2}, 0x${tocolbyte3}"

find $1 -name "$2" -print0 | xargs -0 -I {} convert "{}" -fuzz 30% -fill "rgb(0x${tocolbyte1}, 0x${tocolbyte2},0x${tocolbyte3})" -opaque "rgb(0x${fromcolbyte1}, 0x${fromcolbyte2},0x${fromcolbyte3})" "{}"

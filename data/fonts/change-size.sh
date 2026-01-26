#!/bin/bash

# EXAMPLE : ./change-size.sh leadingedge *.png 50%
# EXAMPLE : ./change-size.sh leadingedge num*.png 50%

if [ "$#" -ne 3 ]; then
    echo "Verwendung: $0 <fontfolder> <filepattern> <size>"
    exit 1
fi

echo "CHANGE: $3"
echo "FOLDER: $1"
echo "PATTERN: $2"

find $1 -name "$2" -print0 | xargs -0 -I {} convert "{}" -resize $3 "{}"



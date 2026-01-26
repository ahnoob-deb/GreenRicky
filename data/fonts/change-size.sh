#!/bin/bash

# EXAMPLE : ./change-size.sh leadingedge *.png 50%
# EXAMPLE : ./change-size.sh leadingedge num*.png 50%

if [ "$#" -ne 3 ]; then
    echo "Verwendung: $0 <fontfolder> <filepattern> <size>"
    exit 1
fi

find $1 -name "$2" -print0 | xargs -t -0 -I {} convert "{}" -resize "$3" "{}"



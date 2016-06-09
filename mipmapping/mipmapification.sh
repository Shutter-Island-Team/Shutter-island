#!/bin/sh

# Script to convert a texture
# into a set of textures used for mip mapping.
#
# Parameters :
#     $1 : Filename of the original texture
#     $2 : The number of level (default : 5)


# Testing the command line
if [ "$#" -eq 0 -o "$#" -ge 3 ]; then
    echo "Wrong number of arguments"
    echo "Expected : filename [levels=5]"
    exit 1
fi


# Testing the file name
if [ ! -f "$1" ]; then
    echo "File not found"
    exit 1
fi

# Fetching all needed informations
ABSOLUTE_FILE=$(readlink -f "$1")
ABSOLUTE_PATH=$(dirname "$ABSOLUTE_FILE")
FILE=$(basename -- "$ABSOLUTE_FILE")
EXTENSION=${FILE##*.}
FILENAME=$(basename -- "$FILE" ".$EXTENSION") 

# Checking the level parameter
LEVEL=5
if [ "$#" -eq 2 ]; then
    re='^[0-9]+$'
    if ! [[ "$2" =~ $re ]] ; then
	echo "2° Argument is not an integer"
	exit 1
    fi
    LEVEL="$2"
fi

# Converting

# Special case : renaming the original texture
cp "$ABSOLUTE_FILE" "$ABSOLUTE_PATH"/"$FILENAME"1."$EXTENSION"
PREVIOUS=1
# Loop to convert
for i in `seq 2 "$LEVEL"`
do
    convert "$ABSOLUTE_PATH"/"$FILENAME""$PREVIOUS"."$EXTENSION" \
    	-resize 50% -antialias \
    	"$ABSOLUTE_PATH"/"$FILENAME""$i"."$EXTENSION"
    PREVIOUS="$i"
done
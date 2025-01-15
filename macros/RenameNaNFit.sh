#!/bin/bash

# Check if the directory is provided as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# Directory to check
DIRECTORY="$1"

# Loop through all text files in the directory
for FILE in "$DIRECTORY"/*.fit; do
    # Check if the file contains the string "nan"
    if grep -qi "nan" "$FILE"; then
        # Get the base name and directory of the file
        BASENAME=$(basename "$FILE" .txt)
        DIRNAME=$(dirname "$FILE")
        
        # Rename the file by appending "_nan" to the end of the filename
        mv "$FILE" "$DIRNAME/${BASENAME}_nan.txt"
        echo "Renamed $FILE to ${BASENAME}_nan.txt"
    fi
done
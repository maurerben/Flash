#!/bin/bash

# Check if enough arguments are passed
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <string_to_replace> <new_string> <file>"
    exit 1
fi

# Arguments
STRING_TO_REPLACE=$1
NEW_STRING=$2
FILETOREPLACE=$3

echo "s/${STRING_TO_REPLACE}/${NEW_STRING}/g" ${FILETOREPLACE}

# Replace the string in file.txt
sed -i "s|${STRING_TO_REPLACE}|${NEW_STRING}|g" ${FILETOREPLACE}
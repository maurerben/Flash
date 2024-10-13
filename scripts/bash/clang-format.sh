#!/bin/bash

CONFIG_FILE=$1
SRC_DIR=$2
NUMBER_OF_ARGUMENTS=2

# Check if enough arguments are passed
if [ "$#" -ne ${NUMBER_OF_ARGUMENTS} ]; then
    echo "Usage: $0 <config_file> <src_dir>"
    exit 1
fi

# Check if compile_commands.json exists
if [ ! -f ${COMPILE_COMMANDS} ]; then
    echo "${COMPILE_COMMANDS} not found!"
    exit 1
fi

# Check if compile_commands.json exists
if [ ! -f ${CONFIG_FILE} ]; then
    echo "${CONFIG_FILE} not found!"
    exit 1
fi

# Run clang-format for all cpp files
for file in $(find ${SRC_DIR} -name '*.cpp' -o -name '*.h' -o -name '*.tpp'); do
    echo "Running clang-format on $file "
    clang-format --style=file:${CONFIG_FILE} -i ${file} 

done
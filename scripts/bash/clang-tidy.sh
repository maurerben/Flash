#!/bin/bash

PROJECT_DIR=$1
COMPILE_COMMANDS=$2
CONFIG_FILE=$3
SRC_DIR=$4
CPP_STD=$5
NUMBER_OF_ARGUMENTS=5

# Check if enough arguments are passed
if [ "$#" -ne ${NUMBER_OF_ARGUMENTS} ]; then
    echo "Usage: $0 <project_dir> <compile_commands> <config_file> <src> <cpp_std>"
    exit 1
fi

# Check if compile_commands.json exists
if [ ! -f ${COMPILE_COMMANDS} ]; then
    echo "${COMPILE_COMMANDS} not found!"
    exit 1
fi

# Find all directories named "include" and generate the include string
INCLUDE_PATHS=$(find "$PROJECT_DIR" -type d -name "include" -print | awk '{printf "-I%s ", $0}')

# Run clang-tidy for all cpp files
for file in $(find ${SRC_DIR} -name '*.cpp'); do
    echo "Running clang-tidy on $file "
    clang-tidy ${file} --config-file=${CONFIG_FILE} -- -std=${CPP_STD} ${INCLUDE_PATHS} "-I/usr/include/eigen3"

done

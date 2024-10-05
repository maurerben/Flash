#!/bin/bash



BUILD_DIR=$1
SRC_DIR=$2
CONFIG_FILE=$3
CPP_STD=$4

# Check if enough arguments are passed
NUMBER_OF_ARGUMENTS=5
if [ "$#" -ne ${NUMBER_OF_ARGUMENTS} ]; then
    echo "Usage: $0 <BUILD_DIR> <SRC_DIR> <CONFIG_FILE> <CPP_STD> <INCLUDE_LIBS_EXT>"
    exit 1
fi

# Find compile_commands.json in build dir
COMPILE_COMMANDS_FILE="compile_commands.json"
INCLUDE_PATHS_EXT_FILE="include_paths.txt"
BUILDS=$(find "${BUILD_DIR}" -maxdepth 1 -type d)
COMPILE_COMMANDS=""
INCLUDE_PATHS_EXT=()
for BUILD in ${BUILDS}; do
    if [ -d "${BUILD}" ]; then
        COMPILE_COMMANDS="${BUILD}/${COMPILE_COMMANDS_FILE}"
        mapfile -t INCLUDE_PATHS_EXT < "${BUILD}/${INCLUDE_PATHS_EXT_FILE}"
        echo "Running clang-tidy with ${COMPILE_COMMANDS}..."
    fi
done
if [ -z ${COMPILE_COMMANDS} ]; then\
    echo "Could not find ${COMPILE_COMMANDS_FILE}. Exit..."
    exit 
fi 


# Find all directories named "include" and generate the include string
INCLUDE_PATHS_INT=$(find "$SRC_DIR" -type d -name "include" -print)
INCLUDE_PATHS_LIST=("${INCLUDE_PATHS_EXT[@]}" "${INCLUDE_PATHS_INT[@]}")
INCLUDE_PATHS=""
for LIB in ${INCLUDE_PATHS_LIST[@]}; do 
    INCLUDE_PATHS+="-I${LIB} "
done


# Run clang-tidy for all cpp files
for file in $(find ${SRC_DIR} -name '*.cpp'); do
    echo "Running clang-tidy on $file "
    clang-tidy ${file} --config-file=${CONFIG_FILE} -- -std=${CPP_STD} ${INCLUDE_PATHS} 

done

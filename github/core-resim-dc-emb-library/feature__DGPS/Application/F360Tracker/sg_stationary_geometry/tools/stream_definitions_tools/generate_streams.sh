#!/bin/bash
set -Euo pipefail


################################################################################################
# Input validation
################################################################################################
if [ $# -eq 0 ]; then
    echo "Error: No variant provided"
    echo "Usage: $0 <sg_variant>"
    exit 1
fi
sg_variant="$1"


#################################################################
# Prepare
#################################################################
DIR_ABSOLUTE_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SG_ROOT_PATH=$DIR_ABSOLUTE_PATH/../..
STREAM_DEFINITIONS_DIR=$SG_ROOT_PATH/tools/log_stream_definitions/
COMPARISON_TOOL_DIR=$SG_ROOT_PATH/tools/stream_definitions_tools
GENERATION_SCRIPT_PATH=$COMPARISON_TOOL_DIR/generate_stream_definitions.py

cd $COMPARISON_TOOL_DIR
poetry install


#################################################################
# CMake build & install
#################################################################
CMAKE_SOURCE_PATH=$SG_ROOT_PATH
CMAKE_BINARY_PATH=$DIR_ABSOLUTE_PATH/build_$sg_variant
CMAKE_INSTALL_PATH=$CMAKE_BINARY_PATH/install

echo "Building and installing SG library"
cmake --preset generic_release -S $CMAKE_SOURCE_PATH -B $CMAKE_BINARY_PATH -D SG_VARIANT=$sg_variant -D SG_IFACE_ONLY=ON
cmake --build $CMAKE_BINARY_PATH --parallel $(nproc --all)
cmake --install $CMAKE_BINARY_PATH --prefix $CMAKE_INSTALL_PATH


#################################################################
# Generate
#################################################################
SG_IFACE_PATH=$CMAKE_INSTALL_PATH/sg/iface
GENERATION_OUTPUT_DIR=$DIR_ABSOLUTE_PATH/generated/$sg_variant
mkdir -p $GENERATION_OUTPUT_DIR

poetry run python $GENERATION_SCRIPT_PATH \
 --package_install_dir $CMAKE_INSTALL_PATH \
 --header_file $SG_IFACE_PATH/sg_output.h \
 --struct SG_Output_T \
 --output_file "$GENERATION_OUTPUT_DIR/strdef_src035_str180_verXXX.txt"

poetry run python $GENERATION_SCRIPT_PATH  \
 --package_install_dir $CMAKE_INSTALL_PATH \
 --header_file $SG_IFACE_PATH/sg_timing_dump.h \
 --struct SG_Timing_Dump_T \
 --output_file "$GENERATION_OUTPUT_DIR/strdef_src035_str181_verXXX.txt"

poetry run python $GENERATION_SCRIPT_PATH \
 --package_install_dir $CMAKE_INSTALL_PATH \
 --header_file $SG_IFACE_PATH/sg_internals_dump.h \
 --struct SG_Internals_Dump_T \
 --output_file "$GENERATION_OUTPUT_DIR/strdef_src035_str182_verXXX.txt"

poetry run python $GENERATION_SCRIPT_PATH \
 --package_install_dir $CMAKE_INSTALL_PATH \
 --header_file $SG_IFACE_PATH/sg_reduced_output.h \
 --struct SG_ReducedOutput_T \
 --output_file "$GENERATION_OUTPUT_DIR/strdef_src035_str183_verXXX.txt"

#################################################################
# Clean up
#################################################################
rm -r $CMAKE_BINARY_PATH
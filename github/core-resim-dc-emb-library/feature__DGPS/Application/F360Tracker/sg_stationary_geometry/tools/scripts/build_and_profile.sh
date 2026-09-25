#!/usr/bin/bash

REPO_ROOT="../.."

LOG_FILE="$REPO_ROOT/tests/test_log.keg"

BUILD_PATH="$REPO_ROOT/build/callgrind"
INSTALL_PATH="$REPO_ROOT/build/install_linux"

set -e
cmake -S $REPO_ROOT -B $BUILD_PATH --preset development \
   -DCMAKE_BUILD_TYPE=RelWithDebInfo  \
   -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ \
   #-DF360_TRACKER_VARIANT=K -DRSPP_VARIANT=K
cmake --build $BUILD_PATH -j$(nproc --all)
cmake --install $BUILD_PATH --prefix $INSTALL_PATH
valgrind --tool=callgrind --trace-children=yes --dump-line=yes --instr-atstart=no \
   $INSTALL_PATH/bin/COMPONENT_RESIM_EXECUTABLE \
   -lib_include_folder $INSTALL_PATH/sg/lib/ \
   -input_file $LOG_FILE \
   -logging_folder resim_out \
   -data_injection/fill_missing true \
   -data_injection/remove_excessing true \
   -mode/copy_lib_on_load true \
   -lib_name stationary_geometries_wrapper
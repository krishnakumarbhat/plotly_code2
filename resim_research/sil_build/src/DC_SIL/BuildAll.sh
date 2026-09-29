#!/bin/sh
set -e  # Exit on any error

CMAKE_BUILD_TYPE="Release"

# Step 1: Build srr_dc
echo "=== Building srr_dc ==="
rm -rf build
mkdir -p build
cd build || exit 1

cmake .. -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" -DBUILD_MODE="srr_dc" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated
cmake --build . --config "$CMAKE_BUILD_TYPE"

cd ..
rm -rf build_srr
cp -r build build_srr

# Step 2: Clean and build mrr_dc
echo "=== Building mrr_dc ==="
rm -rf build
mkdir -p build
cd build || exit 1

cmake .. -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" -DBUILD_MODE="mrr_dc" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated
cmake --build . --config "$CMAKE_BUILD_TYPE"
cd ..

cp build_srr/libSRR_DC_SIL_LIB.so build/

rm -rf build_srr


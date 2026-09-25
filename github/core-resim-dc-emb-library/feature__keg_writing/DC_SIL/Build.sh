#!/bin/sh

echo "==== Build Type Selection ===="
echo "1. Release"
echo "2. Debug"
printf "Enter choice (1 or 2) [default: 1]: "
read buildtype

# Set default if empty
[ -z "$buildtype" ] && buildtype=1

# Validate input
if [ "$buildtype" = "1" ]; then
    CMAKE_BUILD_TYPE="Release"
elif [ "$buildtype" = "2" ]; then
    CMAKE_BUILD_TYPE="Debug"
else
    echo "Invalid build type. Exiting."
    exit 1
fi

echo "==== DC to Build ===="
echo "1. srr_dc"
echo "2. mrr_dc"
printf "Enter choice (1 or 2) [default: 1]: "
read buildmode

# Set default if empty
[ -z "$buildmode" ] && buildmode=1

# Validate input
if [ "$buildmode" = "1" ]; then
    CMAKE_BUILD_MODE="srr_dc"
elif [ "$buildmode" = "2" ]; then
    CMAKE_BUILD_MODE="mrr_dc"
else
    echo "Invalid build mode. Exiting."
    exit 1
fi

printf "Do you want to clean the build folder? (y/n): "
read cleanbuild
if [ "$cleanbuild" = "y" ] || [ "$cleanbuild" = "Y" ]; then
    if [ -d build ]; then
        rm -rf build
        echo "Build folder cleaned."
    else
        echo "No build folder found to clean."
    fi
fi

mkdir -p build
cd build || exit 1

cmake .. -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" -DBUILD_MODE="$CMAKE_BUILD_MODE" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    
fi

cmake --build . --config "$CMAKE_BUILD_TYPE"
if [ $? -ne 0 ]; then
    echo "Build failed."
    
fi

echo
echo "Build Successful!"



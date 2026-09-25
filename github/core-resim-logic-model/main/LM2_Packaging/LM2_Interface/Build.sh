#!/usr/bin/sh

#rm -r build 
mkdir build
cd build

echo Select build type 
echo 1.Release 
echo 2.Debug 
read buildtype 

if [ $buildtype -eq '1' ]; then
  build_type="Release"
elif [ $buildtype -eq '2' ]; then
  build_type="Debug"
fi

rm -r $build_type
mkdir $build_type
cd $build_type

cmake -DCMAKE_BUILD_TYPE=$build_type ../.. 
cmake --build ./ --config $build_type

echo '\033[0;33m'[Note]: Deliveries folder should be created in parent directory'\033[0;37m'
#!/bin/bash 

cd ./CMake
rm -rf ./build
mkdir ./build
cd ./build
echo "Welcome to APTIV VV Engine Build Configuration"
echo " "
echo "Please Enter the Configuration you want to build from below list:"
echo "1 - Release_CMake"
echo "2 - Debug_CMake"
echo "3 - Release_Build"
echo "4 - Debug_Build"

read Conf_IN
case $Conf_IN in
"1")
    echo "Starting Release Configuration CMAKE Only Build"
    cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
    break;;
"2")
    echo "Starting Debug Configuration CMAKE Only Build"
    cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..
    break;;
"3")
    echo "Starting Release Configuration Package Build"
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make -j8
    break;;
"4")
    echo "Starting Release Configuration Package Build"
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make -j8
    break;;
* )
    echo "Invalid Entry Quitting build!!"
    exit
    break;;
esac



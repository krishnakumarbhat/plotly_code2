#!/bin/bash 

rm -rf ./build
mkdir ./build
cd ./build
echo "Welcome to APTIV XMLCOMPARATOR Build Configuration"
echo " "
echo "Please Enter the Configuration you want to build from below list:"
echo "1 - Release_Build"
echo "2 - Debug_Build"

read Conf_IN
case $Conf_IN in
"1")
    echo "Starting Release Configuration Package Build"
    cmake -DCMAKE_BUILD_TYPE=RELEASE ..
    make -j8
    break;;
"2")
    echo "Starting Release Configuration Package Build"
    cmake -DCMAKE_BUILD_TYPE=DEBUG ..
    make -j8
    break;;
* )
    echo "Invalid Entry Quitting build!!"
    exit
    break;;
esac



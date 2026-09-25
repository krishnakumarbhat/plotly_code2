#!/bin/bash

file='../../Code/Generic/Version.cpp'  

index=1  

while IFS="=" read -r line val1 val2
do 
#echo $val1
if [ -z "$val1" ]
then  
	i=1 #dummy code 
else     
	val1=`echo $val1 | sed "s/;\r//g"`
	if [ "$index" -eq 1 ] ;then
	major=$val1
	elif  [ "$index" -eq 2 ] ;then
	minor=$val1
	elif  [ "$index" -eq 3 ] ;then
	patch=$val1
	fi   
        index=$((index+1))       
fi
done < $file

version=$major"."$minor"."$patch

#rm -r build 
mkdir build
cd build
Customerstr=""
INPUT_OUTPUT=1

Customer=("CEER")
build_dir=$(pwd)
# Accessing elements
for cust in "${Customer[@]}"; do
	echo "Building for $cust:"
	cd $build_dir
	rm -rf *
	Customerstr=$cust
	INPUT_OUTPUT=5
	if which python > /dev/null 2>&1; then
		#Python is installed
		python_version=`python --version 2>&1 | awk '{print $2}'`
		echo "Python version $python_version is installed."
	else
		#Python is not installed
		echo "No Python executable is found."
		exit
	fi
	python ../UpdateModelDescription.py $INPUT_OUTPUT
		
	buildtype='Release'
	rm -d "Release"
	mkdir "Release"
	cd "Release"
		
	cmake ../.. \
	-DFW_UPDATE_VERSION=$linuxver \
	-DCMAKE_BUILD_TYPE=$build_type \
	-DCUSTOMER=$Customerstr \
	-DNUM_OUTPUTS=$INPUT_OUTPUT \
	-DNUM_INPUTS=$INPUT_OUTPUT

	cmake --build ./ --config $build_type

	echo '\033[0;33m'[Note]: Deliveries folder should be created in parent directory'\033[0;37m'
done


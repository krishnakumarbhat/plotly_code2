#!/usr/bin/sh

echo '\033[0;31m'[INFO]: Please ensure Libs.zip is unzipped to Libs folder else build will fail'\033[0;37m'

echo "==============================================="
echo "	Radar Logic Model Build Script"
echo "==============================================="
echo "This batch file can build: [cmd]: [projectname] "	
echo "1: Interface dll"
echo "2: LogicModel fmu"
echo "3: LogicModelInterface exe"
echo
echo '\033[0;33m'[Note]: build will be in respecting projects build folder'\033[0;37m'
echo 

echo "Please Select one of the projectname [cmd]: "
read project
if [ $project -eq '1' ]; then
	cd ./LM2_Interface/
	sh Build.sh
	cd ../../
elif [ $project -eq '2' ]; then
	cd ./LM2_FMU
	if which python > /dev/null 2>&1; then
	    python --version
	else
	    echo "Python is not found."
	    exit
	fi
	if command -v python3 >/dev/null 2>&1; then
		python3 Build.py
	else
		python Build.py
	fi
	cd ../
elif [ $project -eq '3' ]; then
	echo "not supported"
fi
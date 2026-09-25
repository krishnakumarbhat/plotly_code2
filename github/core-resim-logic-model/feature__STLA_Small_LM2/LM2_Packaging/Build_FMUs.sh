#!/usr/bin/sh

echo '\033[0;31m'[INFO]: Building all customer and variant fmus'\033[0;37m'

cd ./LM2_FMU
	if which python > /dev/null 2>&1; then
	    python --version
	else
	    echo "Python is not found."
	    exit
	fi
	if command -v python3 >/dev/null 2>&1; then
		python3 BuildForAllCustomer.py
	else
		python BuildForAllCustomer.py
	fi
cd ../

	

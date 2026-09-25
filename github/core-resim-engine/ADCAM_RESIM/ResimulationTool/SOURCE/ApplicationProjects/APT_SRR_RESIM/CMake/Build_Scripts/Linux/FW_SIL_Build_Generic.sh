#!/usr/bin/env bash

# Run the script by calling "bash FW_SIL_Build_Generic.sh"
##########################################################################
#  Filename:           FW_SIL_Build_Generic.sh
#  %version:           1 %
#  Copyright 2016 Delphi Technologies, Inc., All Rights Reserved.
##########################################################################
# Description:
#   This is the script Delphi uses to invoke the makefiles on PC.
##########################################################################
#                                          Help : mandeep.singh1@aptiv.com
##########################################################################

#Check if build directory is present, If not create the directory
build_dir="../../build"
if [ ! -d "$build_dir" ]
then
    mkdir "$build_dir"
fi

cd "$build_dir"

choice=""

echo "Choose by entering the *number* before the desired choice."
echo "Config Solution for :"
echo "1) DEBUG"
echo "2) RELEASE"
printf "Enter choice [1-2]: "
read choice

case $choice in
  1|DEBUG|debug)
    choice=DEBUG
    ;;
  2|RELEASE|release)
    choice=RELEASE
    ;;
  *)
    echo "Invalid choice." >&2
    exit 1
    ;;
esac


echo "[FW_Info] **** Removing previously configured solution!!" ****
rm -r *
echo "[FW_Info] **** Configuring solution for $choice moder ****"
cmake -DCMAKE_BUILD_TYPE=$choice -DBUILDCONFIGTYPE=SIL ..

#Call make to compile the solution
echo "[FW_Info] **** Compiling in $choice Linux mode ****"
make


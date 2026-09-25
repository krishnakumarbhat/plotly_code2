#!/usr/bin/env bash

# Run the script by calling "bash FW_SIL_Build.sh"
##########################################################################
#  Filename:           FW_SIL_Build.sh
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
if [ ! -d $build_dir ] 
then
    mkdir $build_dir    
fi

cd $build_dir 

choice='RELEASE'

echo "[FW_Info] **** Removing previously configured solution, if any!!" ****
rm -r *
echo "[FW_Info] **** Configuring solution for $choice mode ****"
cmake -DCMAKE_BUILD_TYPE=$choice -DBUILDCONFIGTYPE=SIL ..

#Call make to compile the solution
echo "[FW_Info] **** Compiling in $choice Linux mode ****"
make

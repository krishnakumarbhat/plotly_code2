#!/usr/bin/env bash

# Run the script by calling "bash FW_VV_SIL_Build_Generic.sh"
##########################################################################
#  Filename:           FW_VV_SIL_Build_Generic.sh
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

buildMode=( 'DEBUG' 'RELEASE' )

echo "Choose by entering the *number* before the desired choice."
echo "Config Solution for :"
select choice in "${buildMode[@]}"; do

  # If an invalid number was chosen, $choice will be empty.
  # Report an error and prompt again.
  [[ -n $choice ]] || { echo "Invalid choice." >&2; continue; }

  case $choice in
    DEBUG)
      
      ;;
    RELEASE)
      
      ;;
  esac
  break

done


echo "[FW_Info] **** Removing previously configured solution!!" ****
rm -r *
echo "[FW_Info] **** Configuring solution for $choice moder ****"
cmake -DCMAKE_BUILD_TYPE=$choice -DBUILD_LIME=ON -DBUILDCONFIGTYPE=SIL ..

#Call make to compile the solution
echo "[FW_Info] **** Compiling in $choice Linux mode ****"
make


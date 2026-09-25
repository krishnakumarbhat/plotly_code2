#!/bin/bash

# script for calculating total library size
#
# usage: ./get_library_size.sh path_to_your_library.lib
#
# NOTE: While using WSL you can access your Windows folders navigating
# to /mnt/ folder where you can find your local drives mounted

REQUIRED_PKG="csvtool"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Please install the package with sudo apt install $REQUIRED_PKG"
  exit 1
fi

if [ $# -eq 0 ]
then
   echo No arguments supplied!
   echo Usage: ./get_library_size.sh path_to_your_library.lib
   exit 1
fi

lib_path=$1

rows_to_drop=$(size $lib_path | csvtool height -t TAB -)
cols_to_drop=$(size $lib_path | csvtool width -t TAB -)

total_text=$(size $lib_path -t | csvtool col 1 -t TAB - | csvtool drop $rows_to_drop -)
total_data=$(size $lib_path -t | csvtool col 2 -t TAB - | csvtool drop $rows_to_drop -)
total_bss=$(size $lib_path -t | csvtool col 3 -t TAB - | csvtool drop $rows_to_drop -)

#convert sizes to MiB
total_text_MiB=$(bc <<< "scale=3; $total_text/(1024*1024)")
total_bss_MiB=$(bc <<< "scale=3; $total_bss/(1024*1024)")
total_data_KiB=$(bc <<< "scale=3; $total_data/1024")

echo Total text size: $total_text_MiB MiB
echo Total bss size: $total_bss_MiB MiB
echo Total data size: $total_data_KiB KiB

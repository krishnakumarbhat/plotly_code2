#!/usr/bin/env bash

# Run the script by calling "bash RECU_RESIM_Build.sh"
##########################################################################
#  Filename:           RECU_RESIM_Build.bat
#  %version:           1 %
#  Copyright 2016 Delphi Technologies, Inc., All Rights Reserved.
##########################################################################
# Description:
#   This is the script Delphi uses to invoke the makefiles on PC.
##########################################################################
#                                          Help : mandeep.singh1@aptiv.com
##########################################################################
#  Call DC_RESIM_BUILD_Generic.sh to execute the batch script

#!/bin/sh
SOURCE_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
BUILD_DIR="$SOURCE_DIR/build"
BUILD_TYPE=""
CUSTOMER=""
TRACKER=""
VERSION_ID=""

# Interactive Menu 
menu_start() {
  clear
  echo ==============================================================================================
  echo     INTERACTIVE CMAKE BUILD HELPER
  echo ============================================================================================== 
}

# --- 1. Choose Build Type ---
choose_build() {
  echo 
  echo [1] Choose a Build Type:
  echo '1. Debug (Used for Debugging, default)'
  echo '2. Release (Optimised)'
  read -p "Enter choice (1-2) [1]:" choice
  if [[ "$choice" == "1" ]]; then BUILD_TYPE="DEBUG"
  else BUILD_TYPE="RELEASE"
  fi
}

# --- 2. Choose Customer option ---
choose_customer() {
  echo
  echo ----------------------------------------------------------------------------------------------
  echo '[2] choose customer default : CEER'
  echo '1. CEER'
  read -p "Enter choice (1) [2]:" choice
  if [ "$choice" == "1" ]; then CUSTOMER="CEER"
  fi
}

# --- 3. Choose Tracker variant option ---
choose_tracker() {
  echo
  echo ----------------------------------------------------------------------------------------------
  echo '[3] choose tracker variant'
  echo 1. SRR
  echo 2. MRR
  read -p "Enter choice (1-2) [3]:" choice
  if [ "$choice" == "1" ]; then TRACKER="SRR"
  else TRACKER="MRR"
  fi
}

# --- 4. Choose clean option ---
choose_clean() {
  echo
  echo ----------------------------------------------------------------------------------------------
  if [ -d "$BUILD_DIR" ]; then 
    echo '( This deletes previous build folder for a fresh build )'
    rm -rf "$BUILD_DIR" 
  fi
  mkdir "$BUILD_DIR"
  cd "$BUILD_DIR"
}

# --- 6. Get OS ID
get_os() {
  VERSION_ID=$(lsb_release -rs)
}

# --- 5. Confirmation Screen ---
build_screen() {
  echo ==============================================================================================
  echo      BUILD SUMMARY
  echo ==============================================================================================
  echo
  echo Your project will be built with the following settings:
  echo
  echo - OS               : "Ubuntu $VERSION_ID"
  echo - BUILD_TYPE       : "$BUILD_TYPE"
  echo - Clean First      : "YES"
  echo - CUSTOMER         : "$CUSTOMER"
  echo - TRACKER          : "$TRACKER"
}

# display values of global variables
debug_values() {
  echo "global variables: "
  echo "SOURCE_DIR  - $SOURCE_DIR"
  echo "BUILD_DIR   - $BUILD_DIR"
  echo "BUILD_TYPE  - $BUILD_TYPE"
  echo "CUSTOMER    - $CUSTOMER"
  echo "TRACKER     - $TRACKER"
} 

main() {
  # calls to functions
  menu_start
  choose_build
  choose_customer
  choose_tracker
  choose_clean
  get_os
  build_screen
# debug_values

  # cmake command
  echo 
  echo CMAKE BUILD COMMAND : cmake -DCUST="$CUSTOMER" -DTRACKER_TYPE="$TRACKER" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
  cmake -DCUST="$CUSTOMER" -DTRACKER_TYPE="$TRACKER" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated ..
  make
}

# driver program
main
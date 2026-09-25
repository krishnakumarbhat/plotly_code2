#!/bin/bash

set -e
set -u
set -C

# global variables
SCRIPT_NAME="$(basename $0)"
SCRIPT_PATH="$(dirname $0)"
LAUTERBACH_VENDOR_ID="${LAUTERBACH_VENDOR_ID:-0897}"
LAUTERBACH_PATH="${LAUTERBACH_PATH:-/mnt/c/T32/bin/pc_linux64/}"
LAUTERBACH_EXE="${LAUTERBACH_PATH}/t32marm"
CONFIG_FILE="${CONFIG_FILE:-${SCRIPT_PATH}/config.t32}"
STARTUP_SCRIPT="${STARTUP_SCRIPT:-${SCRIPT_PATH}/start_powerview.cmm}"
USER_CONFIG="${USER_CONFIG:-${SCRIPT_PATH}/default_user_config.cmm}"
CPU="${CPU:-SAF85xx}"
CORELIST="${CORELIST:-M7}"

script_main() {
  local devlist

  if [ ! -e "${CONFIG_FILE}" ]; then
    >&2 echo "${SCRIPT_NAME}: Error: ${CONFIG_FILE} not found"
    return 1
  fi
  if [ ! -e "${STARTUP_SCRIPT}" ]; then
    >&2 echo "${SCRIPT_NAME}: Error: ${STARTUP_SCRIPT} not found"
    return 1
  fi
  if [ ! -e "${USER_CONFIG}" ]; then
    >&2 echo "${SCRIPT_NAME}: Error: ${USER_CONFIG} not found"
    return 1
  fi

  # if the lauterbach udev rules have been installed, assume they are working
  # otherwise, find the lauterbach device and chmod +rw on it
  if [ ! -e "/etc/udev/rules.d/10-lauterbach.rules" ]; then
    if [ ! -x /usr/bin/lsusb ]; then
      >&2 echo "Please install libusb:"
      >&2 echo "sudo apt install usbutils"
      return 1
    else
      devlist="$(/usr/bin/lsusb -d "${LAUTERBACH_VENDOR_ID}:" | cut -d ' ' -f 2,4)"
      devlist="${devlist//:/}"
      set -- $devlist
      if [ $# -lt 2 ]; then
        >&2 echo "${SCRIPT_NAME}: Error: no Lauterbach USB devices found"
        >&2 echo "WSL instructions: https://learn.microsoft.com/en-us/windows/wsl/connect-usb"
        return 1
      fi

      while [ $# -ge 2 ]; do
        if [ ! -e /dev/bus/usb/$1/$2 ]; then
          >&2 echo "${SCRIPT_NAME}: Warning: USB device file missing: /dev/bus/usb/$1/$2"
          shift 2
          continue
        fi

        if [ ! -r /dev/bus/usb/$1/$2 -o ! -w /dev/bus/usb/$1/$2 ]; then
          >&2 echo "${SCRIPT_NAME}: Preparing USB device $1:$2, enter password if prompted..."
          sudo chmod a+rw /dev/bus/usb/$1/$2
        fi
        shift 2
      done
    fi
  fi

  if [ -z "${LD_LIBRARY_PATH:-}" ]; then
    LD_LIBRARY_PATH="${LAUTERBACH_PATH}/../../demo/arm/hardware/saf85xx/misc/bbe32_libs/linux/"
  else
    LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${LAUTERBACH_PATH}/../../demo/arm/hardware/saf85xx/misc/bbe32_libs/linux/"
  fi
  export LD_LIBRARY_PATH

  # start "" C:\T32\bin\windows64\t32marm -c config.t32 -s start_powerview.cmm CPU=SAF85xx M7 USER_CONFIG=default_user_config.cmm
  "${LAUTERBACH_EXE}" -c "${CONFIG_FILE}" -s "${STARTUP_SCRIPT}" CPU="${CPU}" ${CORELIST} USER_CONFIG="${USER_CONFIG}"
}

script_main "$@"

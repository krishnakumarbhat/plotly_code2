# Lauterbach Readme

Lauterbach is the debugging tool used for Gen 7 Debug activities.
Trace32 is the SW application used to talk with the Lauterbach debugger.

## Installation of Trace32

1. If you have not used Trace32 and the Lauterbach before, download the Trace32 USB Drivers from <https://www.lauterbach.com/frames.html?download_trace32.html>
1. Use install_trace32.bat to download Trace32 from Aptiv's internal artifactory. Python is required for this script to run correctly. This script automatically installs the Trace32 version used by this project to your C:\T32 folder

## Running Trace32

1. Run one of the .bat files in this folder to start Trace32 for this project
    1. _start_powerview_m7.bat - This script is used for multi-core debugging of the m7 (Arm) core
1. The flash Utility that pops up should come preloaded with the basic configuration needed for flashing after building with the build scripts. Therefore, just click "OK" in that window to flash the unit.

## Note

All other files in this directory are used as part of the flashing and debugging process, and general users should not need to modify these files.

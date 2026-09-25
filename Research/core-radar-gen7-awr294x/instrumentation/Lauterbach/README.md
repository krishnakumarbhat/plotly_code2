# Lauterbach Readme

Lauterbach is the debugging tool used for Gen 7 Debug activities.
Trace32 is the SW application used to talk with the Lauterbach debugger.

# Installation of Trace32
1. If you have not used Trace32 and the Lauterbach before, download the Trace32 USB Drivers from https://www.lauterbach.com/frames.html?download_trace32.html
1. Use install_trace32.bat to download Trace32 from Aptiv's internal artifactory. Python is required for this script to run correctly. This script automatically installs the Trace32 version used by this project to your C:\T32 folder

# Running Trace32
1. Run one of the .bat files in this folder to start Trace32 for this project
    1. _start_powerview_r5f_c66.bat - This script is used for multi-core debugging of both the r5f (Arm) and the c66 (Dsp) cores
    1. _start_powerview_r5f - This script can be used if only debugging of the r5f (Arm) core is needed
1. The flash Utility that pops up should come preloaded with the basic configuration needed for flashing after building with the build scripts. Therefore, just click "OK" in that window to flash the unit.
1. After flashing, the unit needs to be reset to load the code from flash into RAM. Therefore, perform a power cycle.
1. In the System.State window, click the "Attach" Radio button to attache the debugger to the micro.
1. Click Break. You should now be stopped inside of the application.
1. To reset the core to the start symbol, set the PC to 0x0 for the r5f (Arm) core and 0x0080 0000 for the c66 (Dsp) core.

# Off-chip Tracing
1. Flash the embedded code
1. Reset the unit to make sure that the new code is used
1. Attach/Run the code. The clock settings which are configured as part of the bootloader are required for Tracing to work
1. Click the Trace button in the Trace32 toolbar to initialize the Trace module
1. Use Trace as described in the Trace32 user manual

# Note
All other files in this directory are used as part of the flashing and debugging process, and general users should not need to modify these files.
# Tools Directory

The tools directory contains many tools and utilities that are needed for compiling, running, verifying, etc. the project. Most of the tools are just stored inside this directory, either as scripts or as binary files.

Some tools are too large to store directly in the repository. These tools are stored using Aptiv's JFrog Artifactory, and can be downloaded using the toolsInit script stored in tools/python/toolsInit. Further details (if needed) for downloading and running these tools can be found in this file.

# Tresos
## Installation
1. Download and install the latest version of the EB Client License Administrator from the "Instrucitons for single users" seciton here: https://www.elektrobit.com/support/licensing/
1. Run the EB Client License Administrator and provide it the license number 8707-5FC5-923F-555D
1. Run install_tresos.bat.
1. A Tresos folder will be added to tools folder (this folder). Tresos can be launched by clicking on tresos\bin\tresos_gui.exe
1. Set the workspace as one of our MCAL workspaces. We have 1 for the mss app and one for the bootloader located at:
    1. software/app/mss/mcal/Workspace
    1. software/boot/mcal/Workspace

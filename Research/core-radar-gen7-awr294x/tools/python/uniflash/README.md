# Advanced Engineering - Reflash over UART
Uniflash is provided by TI as a way to reflash the AWR294x microcontroller over UART.

Aptiv has repurposed the uart_uniflash bootloader from TI to create a bootloader that can both reflash the external flash via UART and load from a flash chip via QSPI. The files in this folder can be used to reflash the external flash via UART.

# Using UniFlash Python scripts
## Prerequisites
1. Python Version 3.8 or greater is required
1. The following python modules are required and should be installed using the pip command below:  pyserial tqdm xmodem
        python -m pip install --trusted-host pypi.org --trusted-host files.pythonhosted.org pyserial tqdm xmodem
1. The scripts were tested with an FTDI USB -> UART cable. Others should work, but have not been tested

## PTP To Binary Conversion
Uniflash requires a Binary file to flash over UART.
Typically, Aptiv has generated PTP or S-Record (s19) files for flashing with our compilers and WaveMaker.
In order to use these files with UniFlash, they must be converted to Binary files.
A python script (convertPtp2Bin.py), located in the ptp folder, can be used to convert from ptp/s-rec to a binary file that can be used with the UniFlash script.
convertPtp2Bin.py is located at tools/ptp/convertPtp2Bin.py in the repository
and ptp/convertPtp2Bin.py in any release pacakge

Example Usage:

        python convertPtp2Bin.py path/to/ptp

The script will output a file with .bin extension to the same folder where the input ptp is located.

## Flashing Steps
1. Power Down the radar
1. Run uart_uniflash.py using the following syntax from the folder containing the script

        python uart_uniflash.py -p COMx --cfg=xxxx_uniflash_config.cfg
    where:
    1. COMx is the COM port your UART adaptor is connected to. This can be found in the Device Manager -> Ports (COM & LPT) on Windows machines.
    1. xxxx_uniflash_config.cfg is the configuration file you wish to use. The configuration file contains the files which need to be flashed to the radar. Configuration files can be changed to meet your needs. The following configuration files are provided:
        1. workspace_uniflash_config.cfg flashes the output files in the current repository.
        1. release_uniflash_config.cfg flashes the output files from a release folder.
        1. cal_uniflash_config.cfg is an example to flash the calibibration files. Since our calibration files
        usually contain a version number in the filename, it is not expected that this file will work without changes.
1. Once the python script has started running, power on the radar.
    *Note: You may see a few UART errors when running, similar to below. This is okay and will not effect flashing.
    The script is smart enough to re-transmit failed packets.*

        end error: expected NAK, CRC, EOT or CAN; got b'\x00'
        send error: expected NAK, CRC, EOT or CAN; got b'\x00'
        send error: expected ACK; got b'\x15' for block xx

1. Wait until all files are flashed and you recieve a success message.
    If you get an error, power cycle and try again.
    A power cycle is also required before running uart_uniflash.py again

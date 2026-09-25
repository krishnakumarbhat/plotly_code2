----------------------------------------------------
CCA-Framework-MDF examples based on CCA-Framework-MDF v4.6.0
----------------------------------------------------
These examples demonstrates how to use the CCA-Framework-MDF in Python with the provided python library.
To run the examples, you need Python 3.4+ and the provided python library needs either to be in your path or in the same directory.


Examples:

cca-device
----------
This program demonstrates how to scan the network for cca devices and show some information about the found devices.

cca-download
------------
This program demonstrates how to download all recorded data from a logger or CopyStation to a local folder.
Usage:
cca-download.py [logger_ip] [local_folder]

cca-readfile
------------
This program demonstrates how to read a file and how to extract information from the read messages.
Usage:
cca-readfile.py <filename>

cca-sorter
----------
This program demonstrates how to create a writer which sorts all messages given to it.
Usage:
cca-sorter.py [inputDirectory] [outputFile]

cca-convert
----------
This program demonstrates how to use the Cca_Convert() and the progress callback function. You can also use CcaDevice_getDataPath() to get the Data path from a logger and convert the available sessions. 
Usage:
cca-convert.py [input_folder1] [input_folder2] [destination_folder]

get-cca-information
----------
This program demonstrates how to connect to a device and read out a lot of different informations about it.
Usage:
get-cca-information.py [device IP or hostname]

ts-monotone
----------
This example demonstrates how to scan a logger/CopyStation or a path for sessions and read the data from it. While reading the data it will check for monotone increasing timestamps per bus ID.
ts-monotone.py [device IP or path]

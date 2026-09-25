----------------------------------------------------
CCA-Framework-MDF examples based on CCA-Framework-MDF v3.0.6
----------------------------------------------------
These examples demonstrates how to use the CCA-Framework-MDF in Python with the provided python library.
To run the examples, you need Python 3.6 and the provided python library needs either to be in your path or in the same directory.


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

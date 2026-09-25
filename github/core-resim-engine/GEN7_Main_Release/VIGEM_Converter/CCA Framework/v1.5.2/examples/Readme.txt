----------------------------------------------------
CCA-Framework examples based on CCA-Framework v1.5.0
----------------------------------------------------
To run the ccasdk examples simple open the "cca-framework-examples-vs2010.sln"-solution-file.


Examples:

cca-device
----------
This program demonstrates how to scan the network for cca devices and how to get a device by ip address.
This program shows also how to start and stop logging. It expects the ip address of the device 
and a command as argument. Possible command values are "start" to start logging and "stop" to stop logging.

Usage:
cca-device.exe 192.168.1.5 start
This will start logging on the logger with ip address 192.168.1.5.


cca-download
------------
Demonstrates the callback process of the Cca_downloadData(...) function. Downloads all data of a CCA (given through IP) 
and prints the download progress on screen.

Usage:
cca-download.exe 192.168.0.188
This will download all data of the logger with ip address 192.168.0.188.


cca-readfile
------------
Demonstrates the use of the CcaSessionReader object to read messages from VPCAP-files.

Usage:
cca-readfile.exe C:/Path/TO/VPCAP-File.vpcap
Reads the messages of the given VPCAP file and prints the ethernet messages on screen.


cca-session
-----------
Demonstrates how a CCA can be scanned for logged sessions and prints session information on screen.
Also downloads or convertes sessions.

Usage:
cca-session.exe 192.168.0.188 C:\Outputdir
Scans the CCA with ip address 192.168.0.188 and downloads sessions to C:\Outputdir.


cca-property
------------
Demonstrates how the properties of a CCA can be manipulated with the CCA-Framework.
In this example the "description" property of the "System"-Webpage of the CCA-Webinterface will be set to a given value.

Usage:
cca-property.exe 192.168.0.188 testDescription
Sets the description of the System-Webpage to "testDescription".


cca-convert
-----------
Demonstrates the usage of Cca_convert() and the progress callback function.
The sessions of a CCA-Device will be merged into one *.vpcap-file.

usage:
cca-convert.exe 192.168.0.188 C:\Outputdir
Sessions from CCA-Device with the ip 192.168.0.188 will be converted into the Output-directory.


cca-interop-file
----------------
Demonstrates interoperability of the CCA-Framework with C#. The general function is equal to the cca-readfile example. 

Usage:
cca-interop-file.exe C:/Path/TO/VPCAP-File.vpcap
Reads the messages of the given VPCAP file and prints the ethernet messages on screen.


cca-state
---------
Demonstrates how the state, different statistics and systeminformation can be retrieved from a CCA-device with the CCA-Framework.
Also demonstrates how the trace-output of the CCA-Framework can be redirect to a logfile.

Usage:
cca-state.exe [ip] [major CAN-Slot] [minor CAN-Slot] [optional name of logfile]
e.g.:
cca-state 192.168.0.188 1 2 logfile.txt
Gets system information of the CCA-Device with the given ip. Also gets the businterfacestatistic of the CAN-Interface in Slot 1.2. 
If errors occur the trace-output of the CCA-Framework is written to logfile.txt. If name of file is not given in commandline, output will be written to "log.txt".


cca-stream
----------
Demonstrates how the CCA-Framework can be used to receive streamed messages from the CCA-Device during recording. CCA-Device has to be configured accordingly.
The TCP Stream has to be active and the interfaces configured so the write their messages to the stream writer.

Usage:
cca-stream.exe [ip]
Receives 500 messages from the device given by ip and prints message class and busid of received messages.
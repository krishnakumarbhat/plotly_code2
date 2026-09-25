----------------------------------------------------
CCA-Framework examples based on CCA-Framework v2.1.13
----------------------------------------------------
To run the ccasdk examples simple use the provided CMakeLists.txt in the examples root.


Examples:

cca-events
----------
This program shows how to use CcaEvents APIs from CCA-Framework. In the example, vpcap file will be generated 
at the given location. with CcaEvent messages of different types. Location is give via command line.
These file is read again and CcaEvent specific parameter values are displayed on the console

Usage.
cca-events.exe C:\Path\to\vpcap\file\

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


cca-getconfiguration
------------
Demonstrates how to download the configuration of a logger using the framework.

Usage:
cca-getconfiguration.exe 192.168.0.188
This will download the syslog of the logger with ip address 192.168.0.188.


cca-getsyslog
------------
Demonstrates how to download the syslog of a logger using the framework.

Usage:
cca-getsyslog.exe 192.168.0.188
This will download the syslog of the logger with ip address 192.168.0.188.


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


cca-interop-devicestate
----------------
Demonstrates interoperability of the CCA-Framework with C#. Demonstrates scanning for devices using the broadcast (Finder)
interface as well as connecting to known devices via an IP address.  Also demonstrates querying device name and state.

Usage:
cca-interop-devicestate.exe IPADDRESS
Shows the name and state of the device at the indicated IP address or, if none is provided, all detected devices on all network interfaces.


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


cca-sorter
----------
Demonstrates how to use the Sorter in order to sort the messages before writing them into Vpcap file.

Usage:
cca-sorter.exe [inputDirectory] [outputFile]
e.g.:
cca-sorter.exe C:/Path/TO/Sessions/Directory/  C:/Path/TO/New/VPCAP-File.vpcap

cca-ipcaminfo
----------
Demonstrates how to use The ipcaminfoMessage setters/creator and the VideoWriter in order to generate a video file from input sessions directory

Usage:
cca-ipcaminfo.exe [session_Path] [output_video_path] [IpcamCcaRtpPort] [IpcamCameraRtpPort] [IpcamCameraI] [IpcamStreamType] [IpcamStreamProp]
e.g.:
cca-ipcaminfo.exe C:/Path/TO/Sessions/Directory/ C:/Path/TO/output/video-File 3490 50000 192.168.0.90 H264 Z0IAKeLDJKLHORFHPOEWFHBNUILJLHOO==


cca-jumpToTimestamp
----------
Example application that reads a file without using the session reader and use CcaFileReader_jumpToTimestamp to start reading from a specific timestamp.

Usage:
cca-jumpToTimestamp.exe [filename] [TimestampInNs]
e.g.:
cca-jumpToTimestamp.exe C:/Path/TO/New/VPCAP-File.vpcap 1596630600168499200


cca-IndexingOldFile
----------
Example application that uses ccalib to generate .index file for an existing .vpcap (which has no indexing file).This uses CcaFileReader_createIndexWhileReading API.

Usage:
cca-IndexingOldFile.exe [filename] [indexing_interval_ns]
e.g.:
cca-IndexingOldFile.exe C:/Path/TO/New/VPCAP-File.vpcap 250000000

cca-indexedFileWriter
----------
Example application that reads from a session using session reader and then write in an indexed File.

Usage:
cca-indexedFileWriter.exe [SessionDirectory] [outputFileName] [indexing_intervalNS]
e.g.:
cca-indexedFileWriter.exe C:/Path/TO/Sessions/Directory/  C:/Path/TO/New/VPCAP-File.vpcap 250000000
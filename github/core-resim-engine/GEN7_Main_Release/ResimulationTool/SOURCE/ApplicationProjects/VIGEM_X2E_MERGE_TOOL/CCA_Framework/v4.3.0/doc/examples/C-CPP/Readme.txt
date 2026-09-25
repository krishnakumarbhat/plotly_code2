----------------------------------------------------
CCA-Framework-MDF examples based on CCA-Framework-MDF v4.3.0
----------------------------------------------------
To run the examples, simply use the provided CMakeLists.txt in this directory.


Examples:

cca-events
----------
This program shows how to use CcaEvents APIs from CCA-Framework. In the example, vpcap file will be generated 
at the given location. with CcaEvent messages of different types. Location is give via command line.
These file is read again and CcaEvent specific parameter values are displayed on the console

Usage.
cca-events.exe [inputFile]
e.g.:
cca-events.exe  C:\Path\to\vpcap\file\VPCAP-FILE.VPCAP

cca-can_message
----------
This program shows how to read from a session and filter out the can message in an MDF file.

Usage:
cca-can_message.exe [inputDirectory] [outputFile]
e.g.:
cca-can_message.exe C:/Path/TO/Sessions/Directory/  C:/Path/TO/New/MDF-File.MF4

cca-device
----------
This program demonstrates how to scan the network for cca devices and how to get a device by ip address.
It shows also some information about the found devices.

Usage:
cca-device.exe


cca-device-c
----------
This program demonstrates how to scan the network for cca devices and how to get a device by ip address.
It shows also some information about the found devices.

Usage:
cca-device.exe


cca-download
------------
Example application that uses CCA-Framework to download all sessions from a CCA-Logger using CcaSessionCollection_downloadData.
It reports the current progress of the download through a callback function prints the download progress on screen.

Usage:
cca-download.exe [ip_address] [destination]
e.g.:
cca-download.exe 192.168.0.188 C:/Path/TO/A/Folder/
This will download all data of the logger with ip address 192.168.0.188 to the given folder path.


cca-session
------------
Example application that uses CCA-Framework to retrieve sessions from a CCA logger.
for each session the handle_files function is called.

Usage:
cca-session.exe [ip_address] [destination]
e.g.:
cca-session.exe 192.168.0.188 C:/Path/TO/A/Folder/


cca-getconfiguration
------------
Demonstrates how to download the configuration (attachments, busspec, ...) of a logger using the framework to a destination folder.

Usage:
cca-getconfiguration.exe [ip_address] [destination]
e.g.:
cca-getconfiguration.exe 192.168.0.188 C:/Path/TO/A/Folder
This will download the configuration of the logger with ip address 192.168.0.188 to the given folder path.


cca-setconfiguration
------------
Example application that uses CCA-Framework to update/upload configuration (.cca file )to the Logger.

Usage:
cca-setconfiguration.exe [ip_address] [configurationFilesource]
e.g.:
cca-setconfiguration.exe 192.168.0.188 C:/Path/TO/A/CcaFile.cca


cca-start_stop_recording
------------
Example application that uses CCA-Framework to start and stop recording and to get the device state after recording.

Usage:
cca-start_stop_recording.exe [ip_address]
e.g.:
cca-start_stop_recording.exe 192.168.0.188


cca-readfile
------------
Demonstrates the use of the CcaSessionReader object to read messages from VPCAP-files.

Usage:
cca-readfile.exe [inputFile] 
e.g:
cca-readfile.exe C:/Path/TO/VPCAP-File.vpcap
Reads the messages of the given VPCAP file and prints the ethernet messages on screen.


cca-property
------------
Demonstrates how the properties of a CCA can be manipulated with the CCA-Framework.
In this example the "description" property of the "System"-Webpage of the CCA-Webinterface will be set to a given value.
It shows also how to get the value of some properties from different CcaClientInstance.

Usage:
cca-property.exe [ip_address] [new_system_description]
e.g.:
cca-property.exe 192.168.0.188 "test Description"
Sets the description of the System-Webpage to "test Description".


cca-convert
-----------
Demonstrates the usage of Cca_convert() and the progress callback function.
The sessions of a CCA-Device will be merged into one *.vpcap file.

usage:
cca-convert.exe [ip_address] [Destination directory]
e.g.:
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
cca-state.exe [ip_address] [major CAN-Slot] [minor CAN-Slot] [optional name of logfile]
e.g.:
cca-state 192.168.0.188 1 2 logfile.txt
Gets system information of the CCA-Device with the given ip. Also gets the businterfacestatistic of the CAN-Interface in Slot 1.2. 
If errors occur the trace-output of the CCA-Framework is written to logfile.txt.
If name of file is not given in commandline, output will be written to "log.txt".


cca-stream
----------
Demonstrates how the CCA-Framework can be used to receive streamed messages from the CCA-Device during recording.
CCA-Device has to be configured accordingly.
The TCP Stream has to be active and the interfaces configured so the write their messages to the stream writer.

Usage:
cca-stream.exe [ip_address]
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
Demonstrates how to use The ipcamInfoMessage setters/creator and the VideoWriter in order to generate a video file from an input sessions directory

Usage:
cca-ipcaminfo.exe [session_Path] [output_video_path] [IpcamCcaRtpPort] [IpcamCameraRtpPort] [IpcamCameraI] [IpcamStreamType] [IpcamStreamProp]
e.g.:
cca-ipcaminfo.exe C:/Path/TO/Sessions/Directory/ C:/Path/TO/output/video-File 3490 50000 192.168.0.90 H264 OJFIROIHLKHLKHWFOEZFOIHFEWHFOIOI==


cca-jumpToTimestamp
----------
Example application that reads a file without using the session reader and use CcaFileReader_jumpToTimestamp to start reading from a specific timestamp.

Usage:
cca-jumpToTimestamp.exe [filename] [TimestampInNs]
e.g.:
cca-jumpToTimestamp.exe C:/Path/TO/New/VPCAP-File.vpcap 1596630600168499200


cca-IndexingOldFile
----------
Example application that uses CCA-Framework to generate .index file for an existing .vpcap (which has no indexing file).
This uses CcaFileReader_createIndexWhileReading API.

Usage:
cca-IndexingOldFile.exe [filename] [indexing_interval_ns] [output_folder(optional)]
e.g.:
cca-IndexingOldFile.exe C:/Path/TO/New/VPCAP-File.vpcap 250000000

cca-indexedFileWriter
----------
Example application that reads from a session using session reader and then write in an indexed File.

Usage:
cca-indexedFileWriter.exe [SessionDirectory] [outputFileName] [indexing_intervalNS]
e.g.:
cca-indexedFileWriter.exe C:/Path/TO/Sessions/Directory/  C:/Path/TO/New/VPCAP-File.vpcap 250000000


cca-camera_frame_indexing
----------
Example application that creates an indexed camera frame vpcap file and reads it after jumping to some frame counters.
It uses CcaFileReader_jumpToFrameCounter API.

Usage:
cca-camera_frame_indexing.exe [outputFileName]
e.g.:
cca-camera_frame_indexing.exe C:/Path/TO/New/VPCAP-File.vpcap


cca-mipi
----------
Example application that reads from a session using session reader and manipulate all Mipi messages before
writing them in an MDF file.

Usage:
cca-mipi.exe [SessionDirectory] [outputFileName] 
e.g.:
cca-mipi.exe C:/Path/TO/Sessions/Directory/  C:/Path/TO/New/MDF-File.MF4


cca-xcpmessage
----------
Demonstrates how to process xcp messages.

Usage:
cca-xcpmessage.exe [vpcap_file] [vxp_file]
e.g.:
cca-xcpmessage.exe C:/Path/TO/New/VPCAP-File.vpcap C:/Path/TO/New/VPCAP-File.vxp


cca-set_current_time
------------
Example application that uses CCA-Framework to set the current Logger time.

Usage:
cca-set_current_time.exe [ip_address] [time_string_ISO]
e.g.:
cca-set_current_time.exe 192.168.0.188 2021-10-05T08:15:30-05:00


cca-datasync
----------
Example application that starts a data synchronisation process on the device

Usage:
cca-datasync.exe cca-datasync.exe [ip_address] [all|filtered]
e.g.:
cca-datasync.exe 192.168.0.188 all

----------------------------------------------------
CCA-Framework-MDF examples based on CCA-Framework-MDF v4.7.6
----------------------------------------------------
These examples demonstrates how to use the CCA-Framework-MDF in C# with C-marshalling.
To run the examples, simply use the provided CMakeLists.txt in this directory.


Examples:

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


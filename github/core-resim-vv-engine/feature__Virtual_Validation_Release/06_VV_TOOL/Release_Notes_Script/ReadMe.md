#Steps for VV Release Notes Generation
# Version 1.0
[Prerequisite]
-> Python version [3.9] and above is required to run the script

[Steps]
-> Update 'VV_Config_File.ini' file with the release details
-> Please update customer specific part in the config file, do not update other customer details
-> Run the 'VV_Release_Notes.py' script
	-> In Windows run cmd 'VV_Release_Notes.py'
	-> In Linux run cmd 'python3 VV_Release_Notes.py'
-> 'VV_Release_Notes.xml' output xml file will be generated which will be machine readable
-> Copy the block and paste in 'Virtual_Validation_Release notes.xml' file outside
-> If the sprint is same replace the block and if the sprint is difference append the whole block above
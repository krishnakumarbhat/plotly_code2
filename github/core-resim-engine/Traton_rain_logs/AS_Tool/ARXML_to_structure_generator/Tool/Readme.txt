* The xml_parsing.exe should be run, after updating the configuration file present in Inputs folder.
* EXE is kept inside the "Tools" folder
* do check the generated debug file, if some errors were found or missing datatypes.
* Allowed types are "PDU", "APPLICATION-RECORD-DATA-TYPE"
*If PDU is given as input and structure is not getting generated, then try to manually search the arxml for the PDU
identify its Application datatype and then rerun the EXE, after updating configuration file. 

For CAN messages or DBC generation give the following
STRUCTURE=""; TYPE="PDU"

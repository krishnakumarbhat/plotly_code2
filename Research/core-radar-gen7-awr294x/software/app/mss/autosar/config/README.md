# Post Generation Scripts

In order to support extended project specific configurations, post-generation scripts are used.

Some scripts modify the file structure of the generated output and are mandatory to run after each successful generation.

Such script is Post_Generation.bat which shows as option **Post_Generation_Mandatory** in the list of External Generation Steps at the bottom of the list.

**It is always selected by default but may be skipped occasionally by the generator, even though it is selected. Please make sure you run it if this is the case. A green checkmark is the indication that the script has run successfully.**


This script is to be run after each successful generation of BSW Stack.

This file depends on the presence of file: Post_Gen_Config.json.

The following modifications are carried out on the indicated generated files to:
- Separate source code files between CAN and SOMEIP to be used in different builds
- Read configuration file Post_Gen_Config.json to:
1- Determine what BSW files to be modified and replace the listed pointers with NULL_PTR
2- Determine and comment out the listed variables in Rte.c file, to save memory on unused constants
3- Move very large array variables from stack allocation to global in SOMEIP RX functions

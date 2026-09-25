"""
This script is to be run after each successful generation of BSW Stack.

This file depends on the presence of file: Post_Gen_Config.json.

The following modifications are carried out on the indicated generated files to:
    Separate source code files between CAN and SOMEIP to be used in different builds
    Read configuration file Post_Gen_Config.json to:
        Determine what BSW files to be modified and replace the listed pointers with NULL_PTR
        Determine and comment out the listed variables in Rte_OsApplication_Core0_ASIL.c file, to save memory on unused constants
        Move huge array variables from stack allocation to global in SOMEIP RX functions
"""
# !/usr/bin/python
import os
import shutil
import json
from os import path
import re

""" ------------------ Function Definitions -------------------"""


def move_and_replace(SourcePath, sourceFiles, CAN_dest_folder, SOMEIP_dest_folder):
    """Clean out destination folders then move source files into them."""
    # move source files to 2 new locations
    for source in sourceFiles:
        if os.path.isfile(os.path.join(SourcePath, source)):
            shutil.copy(os.path.join(SourcePath, source), os.path.join(CAN_dest_folder, source))
            shutil.move(os.path.join(SourcePath, source), os.path.join(SOMEIP_dest_folder, source))


def modify_bsw(mod_data, dest_folder):
    """Replace all patterns read from configuration file with NULL_PTR."""
    # Read files to be modified from the config set
    for _key, value in mod_data["bsw_files"].items():
        file_path = os.path.join(dest_folder, value["name"])
        with open(file_path, "r") as file:
            file_content = file.read()

        # replace all listed expressions
        for rep in value["mod"]:
            file_content = file_content.replace(" " + rep, " NULL_PTR")
        with open(file_path, "w") as file:
            file.write(file_content)
            file.close()


def ExtractConfigData(config_file):
    """Read configuration data for what files and what patterns to be considered for mods."""
    # Read configuration mods out of json file
    with open(config_file) as f:
        cfgData = json.load(f)
        CAN_mods = cfgData["CAN"]
        SOMEIP_mods = cfgData["SOMEIP"]
    return CAN_mods, SOMEIP_mods


def modify_rte(mod_data, dest_folder):
    """Comment out the specific patterns in Rte_OsApplication_Core0_ASIL.c."""
    rte_path = os.path.join(dest_folder, "Rte_OsApplication_Core0_ASIL.c")
    for value in mod_data["rte_mods"]:
        with open(rte_path, "r") as file:
            lines = file.readlines()

        with open(rte_path, "w") as file:
            for line in lines:
                # print(value)
                # Comment out all found instances
                if re.search(re.escape(value), line):
                    file.write("//" + line)
                else:
                    file.write(line)


def modify_SOMEIP_RX(dest_folder, funcPattern, varPattern):
    """Move specifiec variables from function scope to file scope."""
    # Variables to store line numbers
    Rte_Read_Line_Num = None
    Transformer_Buffer_Line_Num = None
    rte_path = os.path.join(dest_folder, "Rte_OsApplication_Core0_ASIL.c")

    with open(rte_path, "r") as file:
        lines = file.readlines()
    # Find the pattern
    for i in range(len(lines)):
        if funcPattern in lines[i]:
            Rte_Read_Line_Num = i
            break

    # Find local stack variable and move it
    if Rte_Read_Line_Num is not None:
        for i in range(Rte_Read_Line_Num, len(lines)):
            if varPattern in lines[i]:
                Transformer_Buffer_Line_Num = i
                break

    if (Rte_Read_Line_Num is not None) and (Transformer_Buffer_Line_Num is not None):
        # Move "transformationBuffer_0" line to the line before "Rte_read_example"
        lines.insert(Rte_Read_Line_Num - 1, (lines.pop(Transformer_Buffer_Line_Num)).strip())

    # Write the modified content back to the file
    with open(rte_path, "w") as file:
        file.writelines(lines)


def modify_rtememcpy(folder, sourceFiles, memcpy_32_variables, vstdlib_variables):
    """Change Rte memcpy to Vstdlib memcpy for CPU throughput."""
    for source in sourceFiles:
        if os.path.isfile(os.path.join(folder, source)):
            file_path = os.path.join(folder, source)
            with open(file_path, "r") as file:
                data = file.read()
                for variablepattern, replacepattern in zip(memcpy_32_variables, vstdlib_variables):
                    data = data.replace(variablepattern, replacepattern)
                data = data.replace(Rte_include_pattern, Rte_replace_include_pattern)
            with open(file_path, "w") as file:
                file.write(data)


""" ---------------- Set global parameters --------------------- """

CUR_PATH = path.dirname(path.abspath(__file__))

config_file = path.abspath(path.join(CUR_PATH, "Post_Gen_Config.json"))

# Destination folders for separation
GenData_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData"))
CAN_dest_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData/VehCom_CAN"))
SOMEIP_dest_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData/VehCom_SOMEIP"))

# Patterns for converting stack variables into global
Rte_stack_to_global_function = (
    "Rte_Read_SWC_RDR_Veh_Com_RP_DownSel_DetectionList_Slave_Event_DownSel_DetectionList_Event"
)
Rte_stack_to_global_variable = "transformationBuffer_0[13365U];"
Rte_memcpyfiles = ["Rte.c", "Rte_OsApplication_Core0_ASIL.c"]
Rte_memcpy_32_variables = ["Rte_MemCpy32(Rte_", "Rte_MemCpy32(&d", "Rte_MemCpy32(Rte_trans"]
Rte_vstdlib_variables = ["VStdLib_MemCpy(Rte_", "VStdLib_MemCpy(&d", "VStdLib_MemCpy(Rte_trans"]
Rte_include_pattern = '#include "Rte_Hook.h"'
Rte_replace_include_pattern = '#include "Rte_Hook.h" \n #include "vstdlib.h"'

# list of source and destination for batch file processing
bsw_files = ["EcuM_Init_Cfg.c", "LdCom_Lcfg.c", "PduR_Lcfg.c", "SoAd_Lcfg.c"]
rte_files = ["Rte_OsApplication_Core0_ASIL.c"]
dest_folders = [CAN_dest_folder, SOMEIP_dest_folder]

""" ------------- Start Execution ---------------"""

""" Before making any modifications, check if all files exist in the GenData
    Meaning that all involved BSW modules + RTE have been newly generated
"""

# Read configuration paramters from json file
lCAN_mods, lSOMEIP_mods = ExtractConfigData(config_file)

# Clean destination folders and move source files
move_and_replace(GenData_folder, bsw_files, CAN_dest_folder, SOMEIP_dest_folder)

# Modify BSW files in their new separate locations
modify_bsw(lCAN_mods, CAN_dest_folder)
modify_bsw(lSOMEIP_mods, SOMEIP_dest_folder)

# Modify Rte memcpy in RTE files
modify_rtememcpy(GenData_folder, Rte_memcpyfiles, Rte_memcpy_32_variables, Rte_vstdlib_variables)

# Modify RTE files in their new separate locations
# Check if RTE Module is newly generated to avoid double modification
if os.path.isfile(os.path.join(GenData_folder, "Rte_OsApplication_Core0_ASIL.c")):
    move_and_replace(GenData_folder, rte_files, CAN_dest_folder, SOMEIP_dest_folder)
    modify_rte(lCAN_mods, CAN_dest_folder)
    modify_rte(lSOMEIP_mods, SOMEIP_dest_folder)

    # Change stack variables into global variables in RTE file
    modify_SOMEIP_RX(
        SOMEIP_dest_folder, Rte_stack_to_global_function, Rte_stack_to_global_variable
    )

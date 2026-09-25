"""
This script is to be run after each successful generation of BSW Stack.

This file depends on the presence of file: Post_Gen_Config.json.

The following modifications are carried out on the indicated generated files to:
    Separate source code files between CAN and SOMEIP to be used in different builds
    Read configuration file Post_Gen_Config.json to:
        Determine what BSW files to be modified and replace the listed pointers with NULL_PTR
"""
# !/usr/bin/python
import os
import shutil
import json
from os import path
import re

""" ------------------ Function Definitions -------------------"""


def move_and_replace(
    SourcePath,
    sourceFiles,
    TRACKER_ENABLE_dest_folder,
    SOMEIP_ENABLE_dest_folder,
    CAN_ENABLE_dest_folder,
):
    """Clean out destination folders then move source files into them."""
    # Ensure destination folders exist
    os.makedirs(TRACKER_ENABLE_dest_folder, exist_ok=True)
    os.makedirs(SOMEIP_ENABLE_dest_folder, exist_ok=True)
    os.makedirs(CAN_ENABLE_dest_folder, exist_ok=True)

    # move source files to 2 new locations
    for source in sourceFiles:
        if os.path.isfile(os.path.join(SourcePath, source)):
            shutil.copy(
                os.path.join(SourcePath, source), os.path.join(TRACKER_ENABLE_dest_folder, source)
            )
            shutil.copy(
                os.path.join(SourcePath, source), os.path.join(SOMEIP_ENABLE_dest_folder, source)
            )
            shutil.move(
                os.path.join(SourcePath, source), os.path.join(CAN_ENABLE_dest_folder, source)
            )


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
        TRACKER_mods = cfgData["VEH_TRACKER"]
        SOMEIP_mods = cfgData["VEH_SOMEIP"]
        CAN_mods = cfgData["VEH_CAN"]
    return TRACKER_mods, SOMEIP_mods, CAN_mods


def modify_rte(mod_data, dest_folder):
    """Comment out the specific patterns in Rte.c."""
    rte_path = os.path.join(dest_folder, "Rte.c")
    for value in mod_data["rte_mods"]:
        with open(rte_path, "r") as file:
            lines = file.readlines()

        with open(rte_path, "w") as file:
            for line in lines:
                # print(value)
                # Comment out all found instances
                if re.search(re.escape(value), line):
                    file.write(
                        "/* Commented out via Post Gen script for (CAN/SomeIP/Tracker)Variant specific changes*/"
                        "\n"
                    )
                    file.write("//" + line)
                else:
                    file.write(line)


def modify_bswm_lcfg(mod_data, dest_folder, flag):
    """Comment out the specific patterns in BswM_Lcfg.c."""
    rte_path = os.path.join(dest_folder, "BswM_Lcfg.c")
    for value in mod_data["BswMm_Lcfg_mods"]:
        with open(rte_path, "r") as file:
            lines = file.readlines()

        with open(rte_path, "w") as file:
            for line in lines:
                # print(value)
                # Comment out all found instances
                if re.search(re.escape(value), line):
                    if flag:
                        if (
                            value
                            == "return Sd_ServerServiceSetState(BswM_GetIdOfSdServerParameters(handleId, partitionIdx), BswM_GetStateOfSdServerParameters(handleId, partitionIdx));"
                            or value
                            == "return Sd_ConsumedEventGroupSetState(BswM_GetIdOfSdConsumedParameters(handleId, partitionIdx), BswM_GetStateOfSdConsumedParameters(handleId, partitionIdx));"
                            or value
                            == "return Sd_ClientServiceSetState(BswM_GetIdOfSdClientParameters(handleId, partitionIdx), BswM_GetStateOfSdClientParameters(handleId, partitionIdx));"
                        ):
                            file.write(
                                "/* Commented out via Post Gen script for (CAN/SomeIP/Tracker)Variant specific changes*/"
                                "\n"
                            )
                            file.write("//" + line + "\n" + "return E_OK;" "\n")
                            # file.write("return E_OK;")
                        else:
                            file.write(
                                "/* Commented out via Post Gen script for (CAN/SomeIP/Tracker)Variant specific changes*/"
                                "\n"
                            )
                            file.write("//" + line)

                    else:
                        file.write(
                            "/* Commented out via Post Gen script for (CAN/SomeIP/Tracker)Variant specific changes*/"
                            "\n"
                        )
                        file.write("//" + line)
                else:
                    file.write(line)


""" ---------------- Set global parameters --------------------- """

CUR_PATH = path.dirname(path.abspath(__file__))

config_file = path.abspath(path.join(CUR_PATH, "Post_Gen_Config.json"))

# Destination folders for separation
GenData_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData"))
TRACKER_ENABLE_dest_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData/VEH_TRACKER"))
SOMEIP_ENABLE_dest_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData/VEH_SOMEIP"))
CAN_ENABLE_dest_folder = path.abspath(path.join(CUR_PATH, "..", "Appl/GenData/VEH_CAN"))

# list of source and destination for batch file processing
bsw_files = ["EcuM_Init_Cfg.c", "PduR_Lcfg.c", "SoAd_Lcfg.c", "LdCom_Lcfg.c"]
rte_files = ["Rte.c"]
bswm_file = ["BswM_Lcfg.c"]
dest_folders = [TRACKER_ENABLE_dest_folder, SOMEIP_ENABLE_dest_folder, CAN_ENABLE_dest_folder]

""" ------------- Start Execution ---------------"""

""" Before making any modifications, check if all files exist in the GenData
    Meaning that all involved BSW modules + RTE have been newly generated
"""

# Read configuration paramters from json file
# print(config_file)
lTRACKER_ENABLE_mods, lSOMEIP_ENABLE_mods, lCAN_ENABLE_mods = ExtractConfigData(config_file)
# print(lCAN_DISABLE_mods)
# lCAN_ENABLE_mods = ExtractConfigData(config_file)
# print(lCAN_ENABLE_mods)

# Clean destination folders and move source files
move_and_replace(
    GenData_folder,
    bsw_files,
    TRACKER_ENABLE_dest_folder,
    SOMEIP_ENABLE_dest_folder,
    CAN_ENABLE_dest_folder,
)

# Modify BSW files in their new separate locations
modify_bsw(lTRACKER_ENABLE_mods, TRACKER_ENABLE_dest_folder)
modify_bsw(lSOMEIP_ENABLE_mods, SOMEIP_ENABLE_dest_folder)
modify_bsw(lCAN_ENABLE_mods, CAN_ENABLE_dest_folder)

# Modify RTE files in their new separate locations
# Check if RTE Module is newly generated to avoid double modification
if os.path.isfile(os.path.join(GenData_folder, "Rte.c")):
    move_and_replace(
        GenData_folder,
        rte_files,
        TRACKER_ENABLE_dest_folder,
        SOMEIP_ENABLE_dest_folder,
        CAN_ENABLE_dest_folder,
    )
    modify_rte(lTRACKER_ENABLE_mods, TRACKER_ENABLE_dest_folder)
    modify_rte(lSOMEIP_ENABLE_mods, SOMEIP_ENABLE_dest_folder)
    modify_rte(lCAN_ENABLE_mods, CAN_ENABLE_dest_folder)


if os.path.isfile(os.path.join(GenData_folder, "BswM_Lcfg.c")):
    move_and_replace(
        GenData_folder,
        bswm_file,
        TRACKER_ENABLE_dest_folder,
        SOMEIP_ENABLE_dest_folder,
        CAN_ENABLE_dest_folder,
    )
    modify_bswm_lcfg(lTRACKER_ENABLE_mods, TRACKER_ENABLE_dest_folder, True)
    modify_bswm_lcfg(lSOMEIP_ENABLE_mods, SOMEIP_ENABLE_dest_folder, False)
    modify_bswm_lcfg(lCAN_ENABLE_mods, CAN_ENABLE_dest_folder, True)

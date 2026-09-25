"""
Parse a map file and generate a summary file.

This Python module performs the following:
 - Parses the given map files for a specific table
 - Prints this information to a memoryStats file
(This script will likely change as new labels get created)
Usage:
  The user will need to create a list of memoryStatsMapFiles, providing map file
  and list of specific memory sections to use, otherwise it will use all.
  Ex:
  memMapFiles = []
  memMapFiles.append(memoryStatsMapFile(buildPaths.abs['OUTPUTS']['mssMap'],[]))
  memMapFiles.append(memoryStatsMapFile(buildPaths.abs['OUTPUTS']['dssMap'],[]))
  Then send this memoryStatsMapFile list to the memoryStats constructor as
  well as the output file for the memory stats.
  memStats = memoryStats([
                    memMapFiles,
                    buildPaths.abs['OUTPUTS']['memStats'])
  After the memStats object is instaniated, the output file can be generated
      Ex. memStats.createMemoryStats()
  Output(s):
  1. the default output file is the aptivMemoryStats.txt
      ex: "************************ Memory Utilization Statistics for AWR294X ************************"
          "5F_VECS:  12.50% (   0.06 KB out of    0.50 KB) (aptivMssApp.map) *"
  2. the second output is only if on linux OS, meant for jenkins builds, to output a csv file for the
     Jenkins plot plugin to graph the memory usage by block.
      ex: "R5F_VECS,R5F_TCMA,R5F_TCMB"
          "12.50,76.32,84.38"
"""
import os.path
import re
import json
import argparse
import csv
import os

from tools.python.dataDogMetrics import dataDogStats

######################################################


class memoryStatsMapFile:
    """
    Define a map file to use during analysis.
    """

    def __init__(self, mapFilePath, sectionsToInclude):
        """
        Add a map file for analysis by the memoryStats class.

        Args:
            mapFilePath: Path to the map file
            sectionsToInclude: When specified, only the provided sections will be included in the analysis. Otherwise, all sections are considered
        """
        assert isinstance(
            sectionsToInclude, list
        )  # Checks that sectionsToInclude is an list, as needed in your current funciton
        self.mapFilePath = mapFilePath
        self.sectionsToInclude = sectionsToInclude


class memoryStats:
    """
    Primary class used to generate the memory stats in a human and machine readable fashion.
    """

    def __init__(
        self,
        map_Files,
        out_file,
        out_file_stats,
        available_kb=4160,
        sendDataDogMetrics=False,
        branch=None,
        variant=None,
        symbolsTable=None,
        dwarfTable=None,
        swVersion=None,
        verbose=False,
    ):
        """
        Initalizaition of the memory stats class takes the following.

        Args:
            map_Files: A colleciton of memoryStatsMapFile class objects to generate memory stats for
            out_file: The path to the output file that is generated
            available_kb: Total available memory for the micro
            dataDog: Flag on wheter to send metrics to dataDog or not
            branch: The branch the stats are generated for. Only used to tag stats for DataDog.
            variant: The variant the stats are generated for. Only used to tag stats for DataDog.
            swVersion: The sw version the stats are generated for. Only used to tag stats for DataDog.
            verbose: Run the script in a more verbose manner
        """
        self.mapFiles = []
        self.format = 1
        self.variant = variant
        self.swVersion = swVersion
        self.symbolsTable = symbolsTable
        self.dwarfTable = dwarfTable
        self.verbose = verbose

        for mapFile in map_Files:
            assert isinstance(mapFile, memoryStatsMapFile)
            if not os.path.exists(mapFile.mapFilePath):
                print("ERROR: {0} does not exist.".format(mapFile.mapFilePath))
            else:
                self.mapFiles.append(mapFile)
                if len(mapFile.sectionsToInclude) > 0:
                    self.format = 0
        self.outFile = out_file
        self.outFile_Stats = out_file_stats

        self.availableKB = available_kb

        self.dataDogMetrics = None
        self.sendDataDogMetrics = sendDataDogMetrics
        if sendDataDogMetrics:
            self.dataDogMetrics = dataDogStats(branch=branch)

    def formatUtilization(self, sec_text, sec_size, total_size, mem_sec="", reuse=""):
        """
        Format section and memory usage for file.
        """
        # Calculate utilization percentage
        util_pct = sec_size * 100 / total_size

        sec_size /= 1024
        sec_size_unit = "KB"
        total_size /= 1024
        total_size_unit = "KB"

        if mem_sec == "":
            newLine = "{0:>25}: {1:>6.2f}% ({2:>7.2f} {3} out of {4:>7.2f} {5})\n".format(
                sec_text, util_pct, sec_size, sec_size_unit, total_size, total_size_unit
            )
        else:
            if reuse == "":
                newLine = (
                    "{0:>25}: {1:>6.2f}% ({2:>7.2f} {3} out of {4:>7.2f} {5}) ({6})\n".format(
                        sec_text,
                        util_pct,
                        sec_size,
                        sec_size_unit,
                        total_size,
                        total_size_unit,
                        mem_sec,
                    )
                )
            else:
                newLine = (
                    "{0:>25}: {1:>6.2f}% ({2:>7.2f} {3} out of {4:>7.2f} {5}) ({6}) {7}\n".format(
                        sec_text,
                        util_pct,
                        sec_size,
                        sec_size_unit,
                        total_size,
                        total_size_unit,
                        mem_sec,
                        reuse,
                    )
                )
        return newLine

    def readMap(self, mapfile):
        """
        Read Map file and populate dictionary with address/size if memory section is found.
        """
        memory_sections = {}
        fileName = os.path.basename(mapfile.mapFilePath)
        # Create a list containing the contents of the map file
        with open(mapfile.mapFilePath, "r") as mFile:
            map_contents = mFile.readlines()
            mFile.close()
        # Find the starting line of the MEMORY usage analyzer section
        line_num = 0
        for line_num in range(0, len(map_contents)):
            if re.search(r"\s*MEMORY usage analyzer", map_contents[line_num]):
                line_num += 4  # Skip forward 4 lines to the start of the table
                bbe_map = 0
                break
            if re.search(r"\s*Memory Configuration", map_contents[line_num]):
                line_num += 3  # Skip forward 4 lines to the start of the table
                bbe_map = 1
                break

        if line_num != 0:
            # Extract each Memory Sections one at a time.
            while line_num < len(map_contents):
                idx = 0
                if bbe_map == 0:
                    match = re.search(
                        r"^(?P<address>\S+)\s+(?P<size>\S+)\s+(?P<used_bytes>\d+)\(\s*(?P<used_percent>\d+)%\)\s+(?P<holes>\S+)\s+(?P<name>\S+)\s+",
                        map_contents[line_num],
                    )
                else:
                    match = re.search(
                        # r"^(?P<name>\S+)\s+((?P<address>0x\w+)\s+)?(?P<size>\S+)\s+(?P<used_bytes>\d+)\s+(?P<holes>0x\w+)\s+(?P<used_percent>\w+)\s+",
                        r"^(?P<name>\S+)\s+(?P<address>\S+)\s+(?P<size>\S+)\s+(?P<used_bytes>\S+)\s+(?P<holes>\S+)\s+",
                        map_contents[line_num],
                    )

                if match:
                    memSec = match.group("name").strip()
                    # only if the memory section is specified in the
                    # customSections list for that file do we add it
                    # or if the list is empty, just include all

                    if bbe_map:
                        used_bytes = int(match.group("used_bytes").strip(), 16)
                    else:
                        used_bytes = int(match.group("used_bytes").strip(), 10)
                    if len(mapfile.sectionsToInclude) == 0 or memSec in mapfile.sectionsToInclude:
                        memory_sections[memSec] = (
                            int(match.group("address").strip(), 16),
                            int(match.group("size").strip(), 16),
                            used_bytes,
                            100,  # Not used
                            int(match.group("size").strip(), 16) - used_bytes,  # unused bytes
                            100,  # Not used
                            int(match.group("holes").strip(), 16),
                            fileName,
                            "",
                        )

                    idx += 1
                elif re.search(r"\s*SEGMENT ALLOCATION MAP", map_contents[line_num]):
                    break
                line_num += 1

            while line_num < len(map_contents):
                if re.search(r"\s*MODULE SUMMARY", map_contents[line_num]):
                    break
                line_num += 1

        return memory_sections

    def buildMemoryTable(self, memSections):
        """
        Read all the memory blocks and label memory sections that are using that memory.

        Leave memory sections unlabled if they are not used.
        Also a reuse "*" is applied if the memory block is used by more than once.
        """
        memoryDict = {}
        csvDict = {}
        for section in memSections:
            for mem in section:
                used = section[mem][2]

                # Add all of the first map file to this list
                # if there is only one file then it will just
                # display that files content.
                if mem not in memoryDict:
                    memoryDict[mem] = []
                    memoryDict[mem].append(list(section[mem]))
                    csvDict[mem] = []
                    csvDict[mem].append(list(section[mem]))
                else:
                    # replace a memory section of zero size with
                    # one that is used
                    if used != 0 and memoryDict[mem][0][2] == 0:
                        del memoryDict[mem][0]
                        memoryDict[mem].append(list(section[mem]))
                        del csvDict[mem][0]
                        csvDict[mem].append(list(section[mem]))
                    # add other memory section that is used
                    # along with any other that is used
                    # "reused"
                    elif used != 0 and memoryDict[mem][0][2] != 0:
                        update = list(section[mem])
                        update[8] = "*"
                        memoryDict[mem].append(update)
                        memoryDict[mem][0][8] = "*"

        return (memoryDict, csvDict)

    def _write_module_line(self, outFile, module, moduleStats, available_kb, csvModules):
        """Helper method to write a single module line to output file."""
        if module == "Uncategorized":
            total_kb = moduleStats[module]["Total"] / 1024
            code_kb = "-"
            data_kb = "-"
            bss_kb = "-"
            other_kb = "-"
            percent = total_kb / available_kb
            if total_kb > 0:
                outFile.write(
                    "{0:<40}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9.2f}  {6:>10.2%}\n".format(
                        module, "-", "-", "-", "-", total_kb, percent
                    )
                )
        elif module != "Total" and module != "Total_WO_Boot_CDC_Log_Ins":
            code_kb = moduleStats[module]["Code"] / 1024
            data_kb = moduleStats[module]["Data"] / 1024
            bss_kb = moduleStats[module]["Bss"] / 1024
            other_kb = moduleStats[module]["Other"] / 1024
            total_kb = code_kb + data_kb + bss_kb + other_kb
            percent = total_kb / available_kb
            outFile.write(
                "{0:<40}  {1:>9.2f}  {2:>9.2f}  {3:>9.2f}  {4:>9.2f}  {5:>9.2f}  {6:>10.2%}\n".format(
                    module, code_kb, data_kb, bss_kb, other_kb, total_kb, percent
                )
            )
            csvModules[module] = {
                "total_kb": total_kb,
                "code_kb": code_kb,
                "data_kb": data_kb,
                "bss_kb": bss_kb,
                "other_kb": other_kb,
                "percent": percent,
            }

    def _update_running_totals(
        self,
        module,
        moduleStats,
        runningTotal_kb,
        runningCode_kb,
        runningData_kb,
        runningBss_kb,
        runningOther_kb,
        runningTotal_wo_boot_kb,
        runningCode_wo_boot_kb,
        runningData_wo_boot_kb,
        runningBss_wo_boot_kb,
        runningOther_wo_boot_kb,
    ):
        """Helper method to update running totals (passed by reference won't work, returning tuple instead)."""
        if module not in ["Total", "Total_WO_Boot_CDC_Log_Ins", "Uncategorized"]:
            code_kb = moduleStats[module]["Code"] / 1024
            data_kb = moduleStats[module]["Data"] / 1024
            bss_kb = moduleStats[module]["Bss"] / 1024
            other_kb = moduleStats[module]["Other"] / 1024
            total_kb = code_kb + data_kb + bss_kb + other_kb

            runningTotal_kb += total_kb
            runningCode_kb += code_kb
            runningData_kb += data_kb
            runningBss_kb += bss_kb
            runningOther_kb += other_kb

            if module not in ["Bootloader", "CDC", "Logging", "Instrumentation"]:
                runningTotal_wo_boot_kb += total_kb
                runningCode_wo_boot_kb += code_kb
                runningData_wo_boot_kb += data_kb
                runningBss_wo_boot_kb += bss_kb
                runningOther_wo_boot_kb += other_kb
        elif module == "Uncategorized":
            total_kb = moduleStats[module]["Total"] / 1024
            runningTotal_kb += total_kb

        return (
            runningTotal_kb,
            runningCode_kb,
            runningData_kb,
            runningBss_kb,
            runningOther_kb,
            runningTotal_wo_boot_kb,
            runningCode_wo_boot_kb,
            runningData_wo_boot_kb,
            runningBss_wo_boot_kb,
            runningOther_wo_boot_kb,
        )

    def writeOutput(
        self, out, tables, moduleStats, individualStats, fileFormat=0, csvSections=None
    ):
        """
        Write sections and memory usage to file.
        """
        if csvSections is None:
            csvSections = []

        outFile = open(out, "w")
        outFile.write(
            "************************ Memory Utilization Statistics for SAF85xx ************************\n\n"
        )

        for table in tables:
            if fileFormat == 0:
                outFile.write("\n")
                for mem in table:
                    outFile.write(
                        self.formatUtilization(mem, table[mem][2], table[mem][1], table[mem][5])
                    )
            else:
                for sec in tables[table]:
                    if sec[2] == 0:  # dont display the map file
                        outFile.write(self.formatUtilization(table, sec[2], sec[1]))
                    else:
                        outFile.write(
                            self.formatUtilization(table, sec[2], sec[1], sec[7], sec[8])
                        )

        available_kb = self.availableKB
        outFile.write("\n\n")
        outFile.write("=" * 120 + "\n")
        outFile.write("Overall Module Memory Statistics\n")
        outFile.write("=" * 120 + "\n\n")
        outFile.write(
            "{0:<40}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9}  {6:>10}\n".format(
                "Module",
                "Code(KB)",
                "Data(KB)",
                "Bss(KB)",
                "Other(KB)",
                "Total(KB)",
                "% of {}MB".format(round(available_kb / 1024, 3)),
            )
        )
        csvModules = {}
        runningTotal_kb = 0
        runningCode_kb = 0
        runningData_kb = 0
        runningBss_kb = 0
        runningOther_kb = 0
        runningTotal_wo_boot_kb = 0
        runningCode_wo_boot_kb = 0
        runningData_wo_boot_kb = 0
        runningBss_wo_boot_kb = 0
        runningOther_wo_boot_kb = 0

        # Define the module groupings (same as in group_autosar_modules)
        module_group_mappings = {
            "MCAL": lambda m: m.startswith("MCAL_"),
            "AUTOSAR_SWC_PLT": lambda m: m.startswith("AUTOSAR_SWC_PLT_"),
            "AUTOSAR_SWC_PRJ": lambda m: m.startswith("AUTOSAR_SWC_PRJ_"),
            "AUTOSAR_SWC_TKEY": lambda m: m.startswith("AUTOSAR_SWC_TKEY_"),
            "AUTOSAR_SIP_SystemStack": [
                "AUTOSAR_SIP_BswM",
                "AUTOSAR_SIP_EcuM",
                "AUTOSAR_SIP_StbM",
                "AUTOSAR_SIP_Det",
                "AUTOSAR_SIP_WdgIf",
                "AUTOSAR_SIP_WdgM",
            ],
            "AUTOSAR_SIP_DebugAndMeasurement": [
                "AUTOSAR_SIP_Rtm",
                "AUTOSAR_SIP_Dlt",
                "AUTOSAR_SIP_Xcp",
            ],
            "AUTOSAR_SIP_CAN_COM": [
                "AUTOSAR_SIP_CanIf",
                "AUTOSAR_SIP_CanNm",
                "AUTOSAR_SIP_CanSM",
                "AUTOSAR_SIP_CanTSyn",
                "AUTOSAR_SIP_CanTp",
                "AUTOSAR_SIP_CanTrcv_30_Tja1043",
                "AUTOSAR_SIP_CanXcp",
                "AUTOSAR_SIP_Can_30_Flexcan4",
                "AUTOSAR_SIP_Com",
                "AUTOSAR_SIP_ComM",
                "AUTOSAR_SIP_LdCom",
                "AUTOSAR_SIP_SecOC",
                "AUTOSAR_SIP_IpduM",
                "AUTOSAR_SIP_PduR",
                "AUTOSAR_SIP_Nm",
            ],
            "AUTOSAR_SIP_Security": [
                "AUTOSAR_SIP_CryIf",
                "AUTOSAR_SIP_Crypto_30_vHsm",
                "AUTOSAR_SIP_Csm",
                "AUTOSAR_SIP_FvM",
                "AUTOSAR_SIP_IdsM",
                "AUTOSAR_SIP_KeyM",
                "AUTOSAR_SIP_Mka",
            ],
            "AUTOSAR_SIP_DIAG": ["AUTOSAR_SIP_Dcm", "AUTOSAR_SIP_Dem"],
            "AUTOSAR_SIP_E2EandCRC": ["AUTOSAR_SIP_E2E", "AUTOSAR_SIP_E2EXf", "AUTOSAR_SIP_Crc"],
            "AUTOSAR_SIP_Ethernet": [
                "AUTOSAR_SIP_EthFw",
                "AUTOSAR_SIP_EthIf",
                "AUTOSAR_SIP_EthSM",
                "AUTOSAR_SIP_EthTSyn",
                "AUTOSAR_SIP_EthTrcv_30_Rtl9010",
                "AUTOSAR_SIP_Eth_30_Wrapper",
                "AUTOSAR_SIP_Sd",
                "AUTOSAR_SIP_SoAd",
                "AUTOSAR_SIP_SomeIpTp",
                "AUTOSAR_SIP_TcpIp",
                "AUTOSAR_SIP_DoIPInt",
                "AUTOSAR_SIP_IpBase",
                "AUTOSAR_SIP_UdpNm",
            ],
            "AUTOSAR_SIP_MEM": [
                "AUTOSAR_SIP_Fee",
                "AUTOSAR_SIP_Fee_30_FlexNor",
                "AUTOSAR_SIP_MemAcc",
                "AUTOSAR_SIP_MemIf",
                "AUTOSAR_SIP_If_vMemAccM_MemAccAdapter",
                "AUTOSAR_SIP_NvM",
            ],
            "AUTOSAR_SIP_others": [
                "AUTOSAR_SIP_VStdLib",
                "AUTOSAR_SIP_DemoMcpVttOnly",
                "AUTOSAR_SIP_vBaseEnv",
                "AUTOSAR_SIP_vFsmLib",
            ],
            "AUTOSAR_SIP_FOTA": [
                "AUTOSAR_SIP_vFotaH",
                "AUTOSAR_SIP_vStreamProc",
                "AUTOSAR_SIP_vStreamProcCmprLz77",
                "AUTOSAR_SIP_vSwUpdM",
            ],
        }

        # Categorize modules by core based on where they appear in the JSON
        m7_modules = []
        bbe32_modules = []
        a53_modules = []
        shared_modules = []

        if hasattr(self, "module_core_info"):
            # Get the original module to core mapping from JSON
            for module in moduleStats:
                if module in ["Total", "Total_WO_Boot_CDC_Log_Ins"]:
                    continue

                in_m7 = False
                in_bbe32 = False
                in_a53 = False

                # Get the list of sub-modules for this grouped module
                sub_modules = []
                if module in module_group_mappings:
                    mapping = module_group_mappings[module]
                    if callable(mapping):
                        # For prefix-based groupings (MCAL, SWC_PLT, etc.)
                        # Check all modules in M7 core only (where these modules exist)
                        if "m7App.elf" in self.module_core_info:
                            sub_modules.extend(
                                [
                                    m
                                    for m in self.module_core_info["m7App.elf"].keys()
                                    if mapping(m)
                                ]
                            )
                    else:
                        # For list-based groupings (AUTOSAR_SIP groups)
                        sub_modules = mapping
                else:
                    # For non-grouped modules, just check the module itself
                    sub_modules = [module]

                # Check which cores have any of these sub-modules
                if "m7App.elf" in self.module_core_info:
                    for orig_module in self.module_core_info["m7App.elf"].keys():
                        if orig_module in sub_modules:
                            in_m7 = True
                            break

                if "bbe32App.elf" in self.module_core_info:
                    for orig_module in self.module_core_info["bbe32App.elf"].keys():
                        if orig_module in sub_modules:
                            in_bbe32 = True
                            break

                if "a53App.elf" in self.module_core_info:
                    for orig_module in self.module_core_info["a53App.elf"].keys():
                        if orig_module in sub_modules:
                            in_a53 = True
                            break

                # Categorize module based on which cores it appears in
                if in_m7 and not in_bbe32 and not in_a53:
                    m7_modules.append(module)
                elif in_bbe32 and not in_m7 and not in_a53:
                    bbe32_modules.append(module)
                elif in_a53 and not in_m7 and not in_bbe32:
                    a53_modules.append(module)
                else:
                    shared_modules.append(module)
        else:
            # Fallback: put all in shared if we don't have core info
            shared_modules = [
                m for m in moduleStats if m not in ["Total", "Total_WO_Boot_CDC_Log_Ins"]
            ]

        # Write M7 modules
        if m7_modules:
            outFile.write("\n--- M7 Core Modules ---\n")
            # Sort M7 modules by total memory consumption (descending)
            # Calculate total as sum of Code + Data + Bss + Other
            m7_modules_sorted = sorted(
                m7_modules,
                key=lambda m: (
                    moduleStats[m].get("Code", 0)
                    + moduleStats[m].get("Data", 0)
                    + moduleStats[m].get("Bss", 0)
                    + moduleStats[m].get("Other", 0)
                ),
                reverse=True,
            )
            for module in m7_modules_sorted:
                self._write_module_line(outFile, module, moduleStats, available_kb, csvModules)
                (
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                ) = self._update_running_totals(
                    module,
                    moduleStats,
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                )

        # Write BBE32 modules
        if bbe32_modules:
            outFile.write("\n--- BBE32 Core Modules ---\n")
            # Sort BBE32 modules by total memory consumption (descending)
            # Calculate total as sum of Code + Data + Bss + Other
            bbe32_modules_sorted = sorted(
                bbe32_modules,
                key=lambda m: (
                    moduleStats[m].get("Code", 0)
                    + moduleStats[m].get("Data", 0)
                    + moduleStats[m].get("Bss", 0)
                    + moduleStats[m].get("Other", 0)
                ),
                reverse=True,
            )
            for module in bbe32_modules_sorted:
                self._write_module_line(outFile, module, moduleStats, available_kb, csvModules)
                (
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                ) = self._update_running_totals(
                    module,
                    moduleStats,
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                )

        # Write A53 modules
        if a53_modules:
            outFile.write("\n--- A53 Core Modules ---\n")
            # Sort A53 modules by total memory consumption (descending)
            # Calculate total as sum of Code + Data + Bss + Other
            a53_modules_sorted = sorted(
                a53_modules,
                key=lambda m: (
                    moduleStats[m].get("Code", 0)
                    + moduleStats[m].get("Data", 0)
                    + moduleStats[m].get("Bss", 0)
                    + moduleStats[m].get("Other", 0)
                ),
                reverse=True,
            )
            for module in a53_modules_sorted:
                self._write_module_line(outFile, module, moduleStats, available_kb, csvModules)
                (
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                ) = self._update_running_totals(
                    module,
                    moduleStats,
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                )

        # Write shared/common modules
        if shared_modules:
            outFile.write("\n--- Shared/Common Modules ---\n")
            # Sort shared modules by total memory consumption (descending)
            # Calculate total as sum of Code + Data + Bss + Other
            shared_modules_sorted = sorted(
                shared_modules,
                key=lambda m: (
                    moduleStats[m].get("Code", 0)
                    + moduleStats[m].get("Data", 0)
                    + moduleStats[m].get("Bss", 0)
                    + moduleStats[m].get("Other", 0)
                ),
                reverse=True,
            )
            for module in shared_modules_sorted:
                self._write_module_line(outFile, module, moduleStats, available_kb, csvModules)
                (
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                ) = self._update_running_totals(
                    module,
                    moduleStats,
                    runningTotal_kb,
                    runningCode_kb,
                    runningData_kb,
                    runningBss_kb,
                    runningOther_kb,
                    runningTotal_wo_boot_kb,
                    runningCode_wo_boot_kb,
                    runningData_wo_boot_kb,
                    runningBss_wo_boot_kb,
                    runningOther_wo_boot_kb,
                )

        # Write summary totals
        outFile.write("\n" + "-" * 120 + "\n")

        percent = runningTotal_kb / available_kb
        outFile.write(
            "{0:<40}  {1:>9.2f}  {2:>9.2f}  {3:>9.2f}  {4:>9.2f}  {5:>9.2f}  {6:>10.2%}\n".format(
                "Total",
                runningCode_kb,
                runningData_kb,
                runningBss_kb,
                runningOther_kb,
                runningTotal_kb,
                percent,
            )
        )
        csvModules["Total"] = {
            "total_kb": runningTotal_kb,
            "code_kb": runningCode_kb,
            "data_kb": runningData_kb,
            "bss_kb": runningBss_kb,
            "other_kb": runningOther_kb,
            "percent": percent,
        }
        unused_kb = available_kb - runningTotal_kb
        percent = unused_kb / available_kb
        csvModules["Unused"] = {
            "total_kb": unused_kb,
            "code_kb": unused_kb,
            "data_kb": unused_kb,
            "bss_kb": unused_kb,
            "other_kb": unused_kb,
            "percent": percent,
        }
        outFile.write(
            "{0:<40}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9.2f}  {6:>10.2%}\n".format(
                "Unused", "-", "-", "-", "-", unused_kb, percent
            )
        )
        percent = runningTotal_wo_boot_kb / available_kb
        outFile.write(
            "{0:<40}  {1:>9.2f}  {2:>9.2f}  {3:>9.2f}  {4:>9.2f}  {5:>9.2f}  {6:>10.2%}\n".format(
                "Total_WO_Boot_CDC_Log_Ins",
                runningCode_wo_boot_kb,
                runningData_wo_boot_kb,
                runningBss_wo_boot_kb,
                runningOther_wo_boot_kb,
                runningTotal_wo_boot_kb,
                percent,
            )
        )
        csvModules["Total_WO_Boot_CDC_Log_Ins"] = {
            "total_kb": runningTotal_wo_boot_kb,
            "code_kb": runningCode_wo_boot_kb,
            "data_kb": runningData_wo_boot_kb,
            "bss_kb": runningBss_wo_boot_kb,
            "other_kb": runningOther_wo_boot_kb,
            "percent": percent,
        }
        unused_kb = available_kb - runningTotal_wo_boot_kb
        percent = unused_kb / available_kb
        csvModules["Unused_WO_Boot_CDC_Log_Ins"] = {
            "total_kb": unused_kb,
            "code_kb": unused_kb,
            "data_kb": unused_kb,
            "bss_kb": unused_kb,
            "other_kb": unused_kb,
            "percent": percent,
        }
        outFile.write(
            "{0:<40}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9.2f}  {6:>10.2%}\n".format(
                "Unused_WO_Boot_CDC_Log_Ins", "-", "-", "-", "-", unused_kb, percent
            )
        )

        # Write individual module details at the end
        if individualStats:
            outFile.write("\n\n")
            outFile.write("=" * 120 + "\n")
            outFile.write("Individual Module Details (Grouped modules breakdown)\n")
            outFile.write("=" * 120 + "\n\n")

            # Define groupings for organizing output
            group_prefixes = {
                "MCAL": "MCAL_",
                "AUTOSAR_SWC_PLT": "AUTOSAR_SWC_PLT_",
                "AUTOSAR_SWC_PRJ": "AUTOSAR_SWC_PRJ_",
                "AUTOSAR_SWC_TKEY": "AUTOSAR_SWC_TKEY_",
                "AUTOSAR_SIP_SystemStack": [
                    "AUTOSAR_SIP_BswM",
                    "AUTOSAR_SIP_EcuM",
                    "AUTOSAR_SIP_StbM",
                    "AUTOSAR_SIP_Det",
                    "AUTOSAR_SIP_WdgIf",
                    "AUTOSAR_SIP_WdgM",
                ],
                "AUTOSAR_SIP_DebugAndMeasurement": [
                    "AUTOSAR_SIP_Rtm",
                    "AUTOSAR_SIP_Dlt",
                    "AUTOSAR_SIP_Xcp",
                ],
                "AUTOSAR_SIP_CAN_COM": [
                    "AUTOSAR_SIP_CanIf",
                    "AUTOSAR_SIP_CanNm",
                    "AUTOSAR_SIP_CanSM",
                    "AUTOSAR_SIP_CanTSyn",
                    "AUTOSAR_SIP_CanTp",
                    "AUTOSAR_SIP_CanTrcv_30_Tja1043",
                    "AUTOSAR_SIP_CanXcp",
                    "AUTOSAR_SIP_Can_30_Flexcan4",
                    "AUTOSAR_SIP_Com",
                    "AUTOSAR_SIP_ComM",
                    "AUTOSAR_SIP_LdCom",
                    "AUTOSAR_SIP_SecOC",
                    "AUTOSAR_SIP_IpduM",
                    "AUTOSAR_SIP_PduR",
                    "AUTOSAR_SIP_Nm",
                ],
                "AUTOSAR_SIP_Security": [
                    "AUTOSAR_SIP_CryIf",
                    "AUTOSAR_SIP_Crypto_30_vHsm",
                    "AUTOSAR_SIP_Csm",
                    "AUTOSAR_SIP_FvM",
                    "AUTOSAR_SIP_IdsM",
                    "AUTOSAR_SIP_KeyM",
                    "AUTOSAR_SIP_Mka",
                ],
                "AUTOSAR_SIP_DIAG": ["AUTOSAR_SIP_Dcm", "AUTOSAR_SIP_Dem"],
                "AUTOSAR_SIP_E2EandCRC": [
                    "AUTOSAR_SIP_E2E",
                    "AUTOSAR_SIP_E2EXf",
                    "AUTOSAR_SIP_Crc",
                ],
                "AUTOSAR_SIP_Ethernet": [
                    "AUTOSAR_SIP_EthFw",
                    "AUTOSAR_SIP_EthIf",
                    "AUTOSAR_SIP_EthSM",
                    "AUTOSAR_SIP_EthTSyn",
                    "AUTOSAR_SIP_EthTrcv_30_Rtl9010",
                    "AUTOSAR_SIP_Eth_30_Wrapper",
                    "AUTOSAR_SIP_Sd",
                    "AUTOSAR_SIP_SoAd",
                    "AUTOSAR_SIP_SomeIpTp",
                    "AUTOSAR_SIP_TcpIp",
                    "AUTOSAR_SIP_DoIPInt",
                    "AUTOSAR_SIP_IpBase",
                    "AUTOSAR_SIP_UdpNm",
                ],
                "AUTOSAR_SIP_MEM": [
                    "AUTOSAR_SIP_Fee",
                    "AUTOSAR_SIP_Fee_30_FlexNor",
                    "AUTOSAR_SIP_MemAcc",
                    "AUTOSAR_SIP_MemIf",
                    "AUTOSAR_SIP_If_vMemAccM_MemAccAdapter",
                    "AUTOSAR_SIP_NvM",
                ],
                "AUTOSAR_SIP_others": [
                    "AUTOSAR_SIP_VStdLib",
                    "AUTOSAR_SIP_DemoMcpVttOnly",
                    "AUTOSAR_SIP_vBaseEnv",
                    "AUTOSAR_SIP_vFsmLib",
                ],
                "AUTOSAR_SIP_FOTA": [
                    "AUTOSAR_SIP_vFotaH",
                    "AUTOSAR_SIP_vStreamProc",
                    "AUTOSAR_SIP_vStreamProcCmprLz77",
                    "AUTOSAR_SIP_vSwUpdM",
                ],
            }

            # Track which modules have been written
            written_modules = set()

            # Write each group with its heading
            for group_name, group_filter in group_prefixes.items():
                # Find modules belonging to this group
                group_modules = []
                if isinstance(group_filter, str):
                    # Prefix-based grouping (MCAL, SWC_PLT, etc.)
                    group_modules = [
                        m for m in individualStats.keys() if m.startswith(group_filter)
                    ]
                else:
                    # List-based grouping (AUTOSAR_SIP groups)
                    group_modules = [m for m in group_filter if m in individualStats]

                if group_modules:
                    # Sort modules by total memory consumption (descending)
                    group_modules_sorted = sorted(
                        group_modules,
                        key=lambda m: (
                            individualStats[m].get("Code", 0)
                            + individualStats[m].get("Data", 0)
                            + individualStats[m].get("Bss", 0)
                            + individualStats[m].get("Other", 0)
                        ),
                        reverse=True,
                    )

                    # Write group heading
                    outFile.write("\n" + "-" * 120 + "\n")
                    outFile.write(f"{group_name}\n")
                    outFile.write("-" * 120 + "\n")
                    outFile.write(
                        "{0:<40}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9}  {6:>10}\n".format(
                            "Module",
                            "Code(KB)",
                            "Data(KB)",
                            "Bss(KB)",
                            "Other(KB)",
                            "Total(KB)",
                            "% of {}MB".format(round(available_kb / 1024, 3)),
                        )
                    )

                    # Write modules in this group
                    for module in group_modules_sorted:
                        stats = individualStats[module]
                        code_kb = stats["Code"] / 1024
                        data_kb = stats["Data"] / 1024
                        bss_kb = stats["Bss"] / 1024
                        other_kb = stats["Other"] / 1024
                        total_kb = code_kb + data_kb + bss_kb + other_kb
                        percent = total_kb / available_kb

                        outFile.write(
                            "{0:<40}  {1:>9.2f}  {2:>9.2f}  {3:>9.2f}  {4:>9.2f}  {5:>9.2f}  {6:>10.2%}\n".format(
                                module, code_kb, data_kb, bss_kb, other_kb, total_kb, percent
                            )
                        )
                        written_modules.add(module)

        outFile.close()
        if self.verbose:
            print("INFO: Memory Stats File Created\n")

        if self.sendDataDogMetrics:
            if len(csvSections) > 0:
                for sec in csvSections:
                    tags = ["gen7_v2_env:test", "Section:" + sec, "Variant:" + self.variant]
                    if self.swVersion:
                        tags.append("SW_Version:" + self.swVersion)
                    self.dataDogMetrics.sendGaugeMetric(
                        "advradar.gen7_v2.memoryStat.sections",
                        csvSections[sec][0][2] * 100 / csvSections[sec][0][1],
                        tags,
                    )

            if len(csvModules) > 0:
                for module in csvModules:
                    total_kb_tags = [
                        "gen7_v2_env:test",
                        "Module:" + module,
                        "Type:total_kb",
                        "Variant:" + self.variant,
                    ]
                    code_kb_tags = [
                        "gen7_v2_env:test",
                        "Module:" + module,
                        "Type:code_kb",
                        "Variant:" + self.variant,
                    ]
                    data_kb_tags = [
                        "gen7_v2_env:test",
                        "Module:" + module,
                        "Type:data_kb",
                        "Variant:" + self.variant,
                    ]
                    if self.swVersion:
                        total_kb_tags.append("SW_Version:" + self.swVersion)
                        code_kb_tags.append("SW_Version:" + self.swVersion)
                        data_kb_tags.append("SW_Version:" + self.swVersion)

                    self.dataDogMetrics.sendGaugeMetric(
                        "advradar.gen7_v2.memoryStat.modules",
                        csvModules[module]["total_kb"],
                        total_kb_tags,
                    )
                    self.dataDogMetrics.sendGaugeMetric(
                        "advradar.gen7_v2.memoryStat.modules",
                        csvModules[module]["code_kb"],
                        code_kb_tags,
                    )
                    self.dataDogMetrics.sendGaugeMetric(
                        "advradar.gen7_v2.memoryStat.modules",
                        csvModules[module]["data_kb"],
                        data_kb_tags,
                    )

    def parseLlvmFile(self, file):
        """
        Parse the LLVM file.
        """
        # Initialize the dictionary to store the data
        llvm_dict = {}

        # Open the input file for reading
        with open(file, "r") as input_file:
            current_key = None
            current_value = []

            for line in input_file:
                # Check if the line contains ".elf:"
                if ".elf:" in line:
                    # If there's a current key, store the accumulated lines in the dictionary
                    if current_key is not None:
                        llvm_dict[current_key] = current_value

                    # Update the current key to the new .elf marker
                    current_key = line.strip().split()[0].split("/")[-1].split(".")[0]
                    current_value = []
                else:
                    # Add the line to the current value list (excluding lines with the .elf marker)
                    current_value.append(line.strip())

            # Add the last section to the dictionary
            if current_key is not None:
                llvm_dict[current_key] = current_value

        # The dictionary elf_dict now contains the .elf markers as keys and corresponding lines as list values
        return llvm_dict

    def create_dwarf_symbol_map(self, lines):
        """
        Create two mappings:.

        1. symbol_map: symbol name -> file name.
        2. file_map:   file name  -> trimmed file path (starting from external/ or software/).
        """
        symbol_map = {}
        file_map = {}

        prefixes = ["external/", "external\\", "software/", "software\\"]

        for i, line in enumerate(lines):
            if "DW_AT_name" in line:
                key = line.split('"')[1]  # symbol name
                value = "Unknown"
                trimmed_path = None

                # look ahead for DW_AT_decl_file
                for j in range(i + 1, min(i + 6, len(lines))):
                    if "DW_AT_decl_file" in lines[j]:
                        full_path = lines[j].split('"')[1]

                        # extract only filename
                        value = full_path.rsplit("\\", 1)[-1].rsplit("/", 1)[-1]

                        # find first matching prefix in full_path
                        for prefix in prefixes:
                            idx = full_path.find(prefix)
                            if idx != -1:
                                trimmed_path = full_path[idx:]
                                break
                        break

                if value != "Unknown":
                    symbol_map[key] = value

                    # only add if prefix match was found
                    if trimmed_path and value not in file_map:
                        file_map[value] = trimmed_path

        return symbol_map, file_map

    def read_nmfile_to_list(self, nmlines_list, dwarf_symbol_map):
        """
        Read NM file to list.
        """
        # Stripping newline characters from each line and splitting by spaces
        lines = [
            line.strip().split()
            for line in nmlines_list
            if len(line.strip().split()) >= 4 and line.strip().split()[1] != "00000000"
        ]  # Filter out lines with sublist size < 5 and second element not equals '00000000'

        for line in lines:
            if len(line) == 4:
                line.append("ld-temp.o")
            # Convert second element to integer format and then back to string
            line[1] = "{:06}".format(int(line[1], 16))
            line[4] = line[4].rsplit("\\", 1)[-1].rsplit("/", 1)[-1].split(":")[0]
            if line[4] == "ld-temp.o":
                symbol = line[3].split(".")[0] if "." in line[3] else line[3]
                line[4] = dwarf_symbol_map.get(symbol, "Linker-Optimised Unknown")

        return lines

    def generate_FileSize_table(self, lines_list):
        """
        Generate file size table.
        """
        try:
            output_dict = {}
            for line in lines_list:
                key = line[4]
                section_type = line[2].lower()
                size_type = (
                    "Code"
                    if section_type in ["t", "r"]
                    else "Data"
                    if section_type in ["d"]
                    else "Bss"
                    if section_type in ["b"]
                    else "Other"
                )
                output_dict.setdefault(
                    key, {"Code": [], "Rodata": [], "Data": [], "Bss": [], "Other": []}
                )[size_type].append(line[1])
            return output_dict
        except Exception as e:
            print(f"Error occurred while generating output table: {e}")
            return None

    def generate_FileSize_table_merged(self, output_dict):
        """
        Generate Merged file size table.
        """
        try:
            merged_output_dict = {}
            for key, values_dict in output_dict.items():
                merged_output_dict[key] = {
                    k: sum(int(x) for x in v) for k, v in values_dict.items()
                }
            return merged_output_dict
        except Exception as e:
            print(f"Error occurred while generating merged output table: {e}")
            return None

    def process_file_sizes(self, module_files_dict, file_sizes_dict):
        """
        Process the file sizes.
        """
        output_dict = {}
        not_found_files = []

        # Iterate through the module files dictionary
        for file, file_info in file_sizes_dict.items():
            # Check which module the file belongs to
            if file in module_files_dict["Ignored"]:
                continue  # Skip the rest of the loop body
            found = False
            for module, files in module_files_dict.items():
                if file in files:
                    # If the module is found, update its info with file sizes
                    module_info = output_dict.setdefault(
                        module, {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0}
                    )
                    for key in module_info:
                        module_info[key] += file_info.get(key, 0)
                    found = True
                    break

            # If the file is not found in any module, add its size to not_found_info
            if not found:
                not_found_files.append(file)
                uncategorized = output_dict.setdefault(
                    "Uncategorized", {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0}
                )
                for key in uncategorized:
                    uncategorized[key] += file_info.get(key, 0)

        total = {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0}
        for values in output_dict.values():
            for key in total:
                total[key] += values[key]
        output_dict["Total"] = total

        for inner_dict in output_dict.values():
            inner_dict["Total"] = round(sum(inner_dict.values()), 2)
            for key in inner_dict:
                inner_dict[key] = round(inner_dict[key], 2)

        return output_dict, not_found_files

    def build_file_to_module_map(self, apps_dict):
        """
        apps_dict: { "<elf>": { "<module>": [files...] } }.

        Returns: { "<filename>": "<module>" }.
        """
        result = {}
        for _, modules in apps_dict.items():
            for module, files in modules.items():
                for fname in files:
                    if fname and fname not in result:
                        result[fname] = module
        return result

    def _suggest_module_mappings(self, missing_files_dict):
        """
        Suggest which module each missing file should belong to based on filename patterns.

        Args:
            missing_files_dict: Dictionary with cores as keys and list of missing filenames as values

        Returns:
            Dictionary mapping core -> filename -> suggested_module
        """
        suggestions = {}

        if not any(missing_files_dict.values()):
            return suggestions

        print(
            "\n\033[93m{}\033[0m".format(
                "SUGGESTIONS: Based on filename patterns, consider adding:"
            )
        )

        # Define patterns to match files to modules
        module_patterns = {
            # MCAL modules
            "MCAL_Adc": ["Adc_", "Adc.c"],
            "MCAL_Clock": ["Clock_Ip"],
            "MCAL_Crypto": ["Crypto_43_HSE"],
            "MCAL_Ctu": ["Ctu_Ip"],
            "MCAL_Dio": ["Dio_", "Dio.c"],
            "MCAL_Dma": ["Dma_Ip", "Dma_Mux"],
            "MCAL_Eth": ["Eth_43_GMAC"],
            "MCAL_Ftm": ["Ftm_"],
            "MCAL_Gmac": ["Gmac_Ip"],
            "MCAL_Gpt": ["Gpt_", "Gpt.c"],
            "MCAL_Hse": ["Hse_Ip", "HSE_"],
            "MCAL_I2c": ["I2c_Ip", "CDD_I2c"],
            "MCAL_IntCtrl": ["IntCtrl_Ip"],
            "MCAL_MACsec": ["MACsec_Ip", "Macsec_"],
            "MCAL_Mcl": ["CDD_Mcl", "Mcl_"],
            "MCAL_Mcu": ["Mcu_", "Mcu.c"],
            "MCAL_Mem": ["Mem_43_EXFLS"],
            "MCAL_Mpu": ["Mpu_"],
            "MCAL_Mscm": ["Mscm_Ip"],
            "MCAL_Ocotp": ["Ocotp_Ip", "CDD_Ocotp"],
            "MCAL_OsIf": ["OsIf_"],
            "MCAL_Pit": ["Pit_Ip"],
            "MCAL_Platform": ["Platform_Ipw"],
            "MCAL_Port": ["Port_", "Siul2_Port"],
            "MCAL_Power": ["Power_Ip"],
            "MCAL_Qspi": ["Qspi_Ip"],
            "MCAL_Ram": ["Ram_Ip"],
            "MCAL_Rm": ["CDD_Rm", "Rm_"],
            "MCAL_Serdes": ["CDD_Serdes"],
            "MCAL_Serdes1": ["Serdes1_Ip"],
            "MCAL_Siul2": ["Siul2_"],
            "MCAL_Stm": ["Stm_Ip"],
            "MCAL_Swt": ["Swt_Ip"],
            "MCAL_Wdg": ["Wdg_"],
            "MCAL_Xbic": ["Xbic_Ip"],
            "MCAL_Xrdc": ["Xrdc_Ip"],
            "MCAL_SchM": ["SchM_"],
            # AUTOSAR SIP modules
            "AUTOSAR_SIP_Os": ["Os_"],
            "AUTOSAR_SIP_Com": ["Com_"],
            "AUTOSAR_SIP_Dcm": ["Dcm_"],
            "AUTOSAR_SIP_Dem": ["Dem_"],
            "AUTOSAR_SIP_EthIf": ["EthIf_"],
            "AUTOSAR_SIP_TcpIp": ["TcpIp_"],
            "AUTOSAR_SIP_PduR": ["PduR_"],
            # Add more patterns as needed
        }

        for core, files in missing_files_dict.items():
            if not files:
                continue

            suggestions[core] = {}
            print(f"\n\033[93m  {core}.elf:\033[0m")

            for filename in sorted(files):
                suggested_module = "Other"  # Default

                # Try to match against patterns
                for module, patterns in module_patterns.items():
                    if any(pattern in filename for pattern in patterns):
                        suggested_module = module
                        break

                suggestions[core][filename] = suggested_module
                print(
                    f"    \033[93m'{filename}'\033[0m -> Suggested module: \033[92m{suggested_module}\033[0m"
                )

        print(
            "\n\033[93m{}\033[0m\n".format(
                "Please update module_filename_map.json manually with the above suggestions."
            )
        )
        return suggestions

    def _update_json_with_suggestions_REMOVED(self, missing_files_dict, suggestions):
        """
        Update module_filename_map.json with suggested module mappings.

        Args:
            missing_files_dict: Dictionary with cores as keys and list of missing filenames
            suggestions: Dictionary mapping core -> filename -> suggested_module

        Returns:
            bool: True if update successful, False otherwise
        """
        try:
            module_map_path = os.path.join(os.path.dirname(__file__), "module_filename_map.json")

            # Read current JSON
            with open(module_map_path, "r") as f:
                module_map = json.load(f)

            # Track what was added
            updates_made = []

            # Process each core
            for core, files in missing_files_dict.items():
                if not files:
                    continue

                core_key = core + ".elf"
                if core_key not in module_map:
                    print(f"\033[91mWarning: {core_key} not found in JSON\033[0m")
                    continue

                # Add each file to its suggested module
                for filename in files:
                    suggested_module = suggestions.get(core, {}).get(filename, "Other")

                    # Create module entry if it doesn't exist
                    if suggested_module not in module_map[core_key]:
                        module_map[core_key][suggested_module] = []

                    # Add file if not already present
                    if filename not in module_map[core_key][suggested_module]:
                        module_map[core_key][suggested_module].append(filename)
                        # Keep files sorted
                        module_map[core_key][suggested_module].sort()
                        updates_made.append(f"  {core_key}: '{filename}' -> {suggested_module}")

            if not updates_made:
                print("\033[93mNo updates to make\033[0m")
                return False

            # Write updated JSON back with proper formatting
            with open(module_map_path, "w") as f:
                json.dump(module_map, f, indent=2)

            print("\033[92m\nUpdated module_filename_map.json with:\033[0m")
            for update in updates_made:
                print(f"\033[92m{update}\033[0m")

            return True

        except Exception as e:
            print(f"\033[91mError updating JSON: {e}\033[0m")
            import traceback

            traceback.print_exc()
            return False

    def createModuleStats(self, outfile, llvm_nm_input_dict, llvm_dwarf_input_dict):
        """
        Create Module stats for file.
        """
        cores = ["m7App", "bbe32App", "a53App"]
        header = ["Core", "File", "Code", "Rodata", "Data", "Bss", "Other"]
        Not_found_files_Dict = {"m7App": [], "bbe32App": [], "a53App": []}

        # Read module files dictionary from JSON file
        module_map = os.path.join(os.path.dirname(__file__), "module_filename_map.json")
        with open(module_map, "r") as module_files_file:
            module_files_dict_all = json.load(module_files_file)

        # Dynamically build module list from all cores in the JSON file
        all_modules = set()
        for _core_name, modules_dict in module_files_dict_all.items():
            all_modules.update(modules_dict.keys())

        # Add standard modules that should always be present
        all_modules.update(["Bootloader", "Uncategorized", "Total", "Total_WO_Boot_CDC_Log_Ins"])

        # Create output dictionary with all discovered modules
        output_module_size_allcores = {
            module: dict.fromkeys(["Code", "Rodata", "Data", "Bss", "Other", "Total"], 0)
            for module in sorted(all_modules)
        }

        # Initialize bootloader to 128 KB (64KB code and 64Kb Data), since we dont provide bootloader elf file for stats
        output_module_size_allcores["Bootloader"]["Code"] = 0
        output_module_size_allcores["Bootloader"]["Bss"] = 0
        if not os.path.exists(outfile):
            with open(outfile, mode="w", newline="") as f:
                writer = csv.writer(f)
                writer.writerow(header)

        files2module_map = self.build_file_to_module_map(module_files_dict_all)
        for core in cores:
            if core in llvm_dwarf_input_dict.keys():
                dwarf_symbol_map, filepath_map = self.create_dwarf_symbol_map(
                    llvm_dwarf_input_dict[core]
                )
                lines_list = self.read_nmfile_to_list(llvm_nm_input_dict[core], dwarf_symbol_map)

                if lines_list:
                    FileSize_table_dict = self.generate_FileSize_table(lines_list)

                    if FileSize_table_dict:
                        Merged_FileSize_table_dict = self.generate_FileSize_table_merged(
                            FileSize_table_dict
                        )

                # with open(outfile, "a") as f:
                # for key, value in Merged_FileSize_table_dict.items():
                # f.write(f"{key}: {value}\n")

                with open(outfile, mode="a", newline="") as f:
                    writer = csv.writer(f)
                    for filename, sizes in Merged_FileSize_table_dict.items():
                        writer.writerow(
                            [
                                core,
                                filename,
                                sizes.get("Code", 0),
                                sizes.get("Rodata", 0),
                                sizes.get("Data", 0),
                                sizes.get("Bss", 0),
                                sizes.get("Other", 0),
                                filepath_map.get(filename, filename),
                                files2module_map.get(filename, "NONE"),
                            ]
                        )

                output_module_size, not_found_files = self.process_file_sizes(
                    module_files_dict_all[core + ".elf"], Merged_FileSize_table_dict
                )

                Not_found_files_Dict[core] = not_found_files

                for module, module_info in output_module_size.items():
                    for section, value in module_info.items():
                        output_module_size_allcores[module][section] += value

        # Group AUTOSAR_SIP modules and MCAL modules into logical groups
        output_module_size_allcores, individual_module_details = self.group_autosar_modules(
            output_module_size_allcores
        )

        # Store the core information in the module stats for later use
        self.module_core_info = module_files_dict_all

        return output_module_size_allcores, individual_module_details, Not_found_files_Dict

    def group_autosar_modules(self, module_stats):
        """
        Group AUTOSAR_SIP modules, AUTOSAR_SWC modules, and MCAL modules into logical groups.

        Returns tuple: (grouped_stats, individual_module_details)
        """
        # Define module groupings
        module_groups = {
            "AUTOSAR_SIP_SystemStack": [
                "AUTOSAR_SIP_BswM",
                "AUTOSAR_SIP_EcuM",
                "AUTOSAR_SIP_StbM",
                "AUTOSAR_SIP_Det",
                "AUTOSAR_SIP_WdgIf",
                "AUTOSAR_SIP_WdgM",
            ],
            "AUTOSAR_SIP_DebugAndMeasurement": [
                "AUTOSAR_SIP_Rtm",
                "AUTOSAR_SIP_Dlt",
                "AUTOSAR_SIP_Xcp",
            ],
            "AUTOSAR_SIP_CAN_COM": [
                "AUTOSAR_SIP_CanIf",
                "AUTOSAR_SIP_CanNm",
                "AUTOSAR_SIP_CanSM",
                "AUTOSAR_SIP_CanTSyn",
                "AUTOSAR_SIP_CanTp",
                "AUTOSAR_SIP_CanTrcv_30_Tja1043",
                "AUTOSAR_SIP_CanXcp",
                "AUTOSAR_SIP_Can_30_Flexcan4",
                "AUTOSAR_SIP_Com",
                "AUTOSAR_SIP_ComM",
                "AUTOSAR_SIP_LdCom",
                "AUTOSAR_SIP_SecOC",
                "AUTOSAR_SIP_IpduM",
                "AUTOSAR_SIP_PduR",
                "AUTOSAR_SIP_Nm",
            ],
            "AUTOSAR_SIP_Security": [
                "AUTOSAR_SIP_CryIf",
                "AUTOSAR_SIP_Crypto_30_vHsm",
                "AUTOSAR_SIP_Csm",
                "AUTOSAR_SIP_FvM",
                "AUTOSAR_SIP_IdsM",
                "AUTOSAR_SIP_KeyM",
                "AUTOSAR_SIP_Mka",
            ],
            "AUTOSAR_SIP_DIAG": ["AUTOSAR_SIP_Dcm", "AUTOSAR_SIP_Dem"],
            "AUTOSAR_SIP_E2EandCRC": ["AUTOSAR_SIP_E2E", "AUTOSAR_SIP_E2EXf", "AUTOSAR_SIP_Crc"],
            "AUTOSAR_SIP_Ethernet": [
                "AUTOSAR_SIP_EthFw",
                "AUTOSAR_SIP_EthIf",
                "AUTOSAR_SIP_EthSM",
                "AUTOSAR_SIP_EthTSyn",
                "AUTOSAR_SIP_EthTrcv_30_Rtl9010",
                "AUTOSAR_SIP_Eth_30_Wrapper",
                "AUTOSAR_SIP_Sd",
                "AUTOSAR_SIP_SoAd",
                "AUTOSAR_SIP_SomeIpTp",
                "AUTOSAR_SIP_TcpIp",
                "AUTOSAR_SIP_DoIPInt",
                "AUTOSAR_SIP_IpBase",
                "AUTOSAR_SIP_UdpNm",
            ],
            "AUTOSAR_SIP_MEM": [
                "AUTOSAR_SIP_Fee",
                "AUTOSAR_SIP_Fee_30_FlexNor",
                "AUTOSAR_SIP_MemAcc",
                "AUTOSAR_SIP_MemIf",
                "AUTOSAR_SIP_If_vMemAccM_MemAccAdapter",
                "AUTOSAR_SIP_NvM",
            ],
            "AUTOSAR_SIP_others": [
                "AUTOSAR_SIP_VStdLib",
                "AUTOSAR_SIP_DemoMcpVttOnly",
                "AUTOSAR_SIP_vBaseEnv",
                "AUTOSAR_SIP_vFsmLib",
            ],
            "AUTOSAR_SIP_FOTA": [
                "AUTOSAR_SIP_vFotaH",
                "AUTOSAR_SIP_vStreamProc",
                "AUTOSAR_SIP_vStreamProcCmprLz77",
                "AUTOSAR_SIP_vSwUpdM",
            ],
        }

        # Create new grouped module stats
        grouped_stats = {}
        individual_details = {}

        # Collect all MCAL modules
        mcal_modules = []
        mcal_total = {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0, "Total": 0}

        for module, stats in module_stats.items():
            if module.startswith("MCAL_"):
                mcal_modules.append(module)
                for key in mcal_total.keys():
                    mcal_total[key] += stats.get(key, 0)
                # Store individual MCAL module details
                individual_details[module] = stats.copy()

        # Add grouped MCAL to grouped_stats
        if mcal_total["Total"] > 0:
            grouped_stats["MCAL"] = mcal_total

        # Collect all AUTOSAR_SWC_PLT modules
        swc_plt_total = {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0, "Total": 0}
        for module, stats in module_stats.items():
            if module.startswith("AUTOSAR_SWC_PLT_"):
                for key in swc_plt_total.keys():
                    swc_plt_total[key] += stats.get(key, 0)
                individual_details[module] = stats.copy()

        if swc_plt_total["Total"] > 0:
            grouped_stats["AUTOSAR_SWC_PLT"] = swc_plt_total

        # Collect all AUTOSAR_SWC_PRJ modules
        swc_prj_total = {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0, "Total": 0}
        for module, stats in module_stats.items():
            if module.startswith("AUTOSAR_SWC_PRJ_"):
                for key in swc_prj_total.keys():
                    swc_prj_total[key] += stats.get(key, 0)
                individual_details[module] = stats.copy()

        if swc_prj_total["Total"] > 0:
            grouped_stats["AUTOSAR_SWC_PRJ"] = swc_prj_total

        # Collect all AUTOSAR_SWC_TKEY modules
        swc_tkey_total = {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0, "Total": 0}
        for module, stats in module_stats.items():
            if module.startswith("AUTOSAR_SWC_TKEY_"):
                for key in swc_tkey_total.keys():
                    swc_tkey_total[key] += stats.get(key, 0)
                individual_details[module] = stats.copy()

        if swc_tkey_total["Total"] > 0:
            grouped_stats["AUTOSAR_SWC_TKEY"] = swc_tkey_total

        # First, add all non-grouped modules
        for module, stats in module_stats.items():
            # Skip MCAL modules (already grouped)
            if module.startswith("MCAL_"):
                continue

            # Skip AUTOSAR_SWC_PLT, PRJ, TKEY modules (already grouped)
            if (
                module.startswith("AUTOSAR_SWC_PLT_")
                or module.startswith("AUTOSAR_SWC_PRJ_")
                or module.startswith("AUTOSAR_SWC_TKEY_")
            ):
                continue

            # Check if this module should be grouped
            is_grouped = False
            for _group_name, group_modules in module_groups.items():
                if module in group_modules:
                    is_grouped = True
                    # Store individual module details
                    individual_details[module] = stats.copy()
                    break

            # If not grouped, add as-is
            if not is_grouped:
                grouped_stats[module] = stats.copy()

        # Now create grouped modules by aggregating their stats
        for group_name, group_modules in module_groups.items():
            group_total = {"Code": 0, "Rodata": 0, "Data": 0, "Bss": 0, "Other": 0, "Total": 0}

            for module in group_modules:
                if module in module_stats:
                    for key in group_total.keys():
                        group_total[key] += module_stats[module].get(key, 0)

            # Only add group if it has non-zero values
            if group_total["Total"] > 0:
                grouped_stats[group_name] = group_total

        # Add AUTOSAR_SIP_Os separately (unchanged)
        if "AUTOSAR_SIP_Os" in module_stats:
            grouped_stats["AUTOSAR_SIP_Os"] = module_stats["AUTOSAR_SIP_Os"].copy()

        return grouped_stats, individual_details

    def createMemoryStats(self):
        """
        Call this to read all map files then create the memory stats output file.
        """
        memSections = []
        for mapFile in self.mapFiles:
            memory_sections = self.readMap(mapFile)
            memSections.append(memory_sections)

        if self.format == 1:
            (memSections, csvSections) = self.buildMemoryTable(memSections)

        symbolStats = self.parseLlvmFile(self.symbolsTable)
        dwarfStats = self.parseLlvmFile(self.dwarfTable)

        moduleStats, individualStats, Not_found_files_Dict = self.createModuleStats(
            self.outFile_Stats, symbolStats, dwarfStats
        )

        if (
            len(Not_found_files_Dict["m7App"])
            or len(Not_found_files_Dict["bbe32App"])
            or len(Not_found_files_Dict["a53App"])
        ):
            print(
                "\033[91m{}\033[0m".format(
                    "ERROR: Below files are not grouped to module for memory scripts. Group files in \\tools\\python\\memoryStats\\module_filename_map.json"
                )
            )
            print("\033[91m{}\033[0m".format(Not_found_files_Dict))

            # Provide suggestions only (do not auto-update)
            self._suggest_module_mappings(Not_found_files_Dict)

            # Fail the build
            raise SystemExit(1)

        if len(memSections) > 0:
            self.writeOutput(
                self.outFile, memSections, moduleStats, individualStats, self.format, csvSections
            )
        else:
            print("INFO: No map file available to create memory stats file.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Provide inputs to the script on what map files to include in the memory stats analysis. \n \
        Also provide an output file to write the stats to. At least one map file and the output file are required."
    )
    parser.add_argument(
        "input_maps", type=str, nargs="+", help="The map files to calculate the memory stats for"
    )
    parser.add_argument("output_file", type=str, nargs=1, help="The path to the output file")
    parser.add_argument(
        "output_file_stats",
        type=str,
        nargs=1,
        help="The path to the output file with individual file stats",
    )
    parser.add_argument(
        "--symbols-txt",
        dest="symbolsTable",
        type=str,
        action="store",
        default=None,
        help="A sections JSON file generated with llvm-readelf",
    )
    parser.add_argument(
        "--dwarf-txt",
        dest="dwarfTable",
        type=str,
        action="store",
        default=None,
        help="A sections JSON file generated with llvm-readelf",
    )
    parser.add_argument(
        "--branch",
        dest="branch",
        type=str,
        action="store",
        default=None,
        help="A branch to tag the DataDog metrics with",
    )
    parser.add_argument(
        "--variant",
        dest="variant",
        type=str,
        action="store",
        default=None,
        help="A variant to tag the DataDog metrics with",
    )
    parser.add_argument(
        "--dataDog",
        dest="sendDataDogMetrics",
        action="store_true",
        default=False,
        help="A flag to trigger sending metrics to dataDog",
    )
    parser.add_argument(
        "--swVersion",
        dest="swVersion",
        type=str,
        action="store",
        default=None,
        help="A SW version to tag the DataDog metrics with",
    )
    parser.add_argument(
        "--verbose",
        dest="verbose",
        action="store_true",
        default=False,
        help="Run this script in verbose mode",
    )
    args = parser.parse_args()

    # -------------------------------------------------------------------
    # Create Memory Stats File - Basic example with no inclusions/exclusions
    # -------------------------------------------------------------------
    memMapFiles = []

    for fi in args.input_maps:
        if ".map" in fi:
            memMapFiles.append(
                memoryStatsMapFile(
                    fi, []  # add memory sections to include, otherwise it will include all
                )
            )

    memStats = memoryStats(
        memMapFiles,
        args.output_file[0],
        args.output_file_stats[0],
        sendDataDogMetrics=args.sendDataDogMetrics,
        branch=args.branch,
        variant=args.variant,
        swVersion=args.swVersion,
        symbolsTable=args.symbolsTable,
        dwarfTable=args.dwarfTable,
        verbose=args.verbose,
    )

    memStats.createMemoryStats()

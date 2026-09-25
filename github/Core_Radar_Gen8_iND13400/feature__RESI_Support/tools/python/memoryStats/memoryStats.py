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

# from tools.python.dataDogMetrics import dataDogStats TODO Sandeep

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
        available_kb=4096,
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

        self.availableKB = available_kb

        self.dataDogMetrics = None
        self.sendDataDogMetrics = sendDataDogMetrics
        # if sendDataDogMetrics:
        # self.dataDogMetrics = dataDogStats(branch=branch) TODO Sandeep

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

    def writeOutput(self, out, tables, moduleStats, fileFormat=0, csvSections=None):
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
        outFile.write(
            "{0:>25}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9}  {6:>10}\n".format(
                "Module",
                "Code(KB)",
                "Data(KB)",
                "Bss(KB)",
                "Other(KB)",
                "Total(KB)",
                "% of {}MB".format(round(available_kb / 1024, 1)),
            )
        )
        csvModules = {}
        runningTotal_kb = 0
        runningCode_kb = 0
        runningData_kb = 0
        runningBss_kb = 0
        runningOther_kb = 0
        for module in moduleStats:
            if module == "Uncategorized":
                total_kb = moduleStats[module]["Total"] / 1024
                runningTotal_kb = runningTotal_kb + total_kb
                code_kb = "-"
                data_kb = "-"
                percent = total_kb / available_kb
                if total_kb > 0:
                    outFile.write(
                        "{0:>25}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9.2f}  {6:>10.2%}\n".format(
                            module, "-", "-", "-", "-", total_kb, percent
                        )
                    )
            elif module != "Total":
                code_kb = moduleStats[module]["Code"] / 1024
                data_kb = moduleStats[module]["Data"] / 1024
                bss_kb = moduleStats[module]["Bss"] / 1024
                other_kb = moduleStats[module]["Other"] / 1024
                total_kb = code_kb + data_kb + bss_kb + other_kb
                runningTotal_kb = runningTotal_kb + total_kb
                runningCode_kb = runningCode_kb + code_kb
                runningData_kb = runningData_kb + data_kb
                runningBss_kb = runningBss_kb + bss_kb
                runningOther_kb = runningOther_kb + other_kb
                percent = total_kb / available_kb
                outFile.write(
                    "{0:>25}  {1:>9.2f}  {2:>9.2f}  {3:>9.2f}  {4:>9.2f}  {5:>9.2f}  {6:>10.2%}\n".format(
                        module, code_kb, data_kb, bss_kb, other_kb, total_kb, percent
                    )
                )
            else:
                continue

            csvModules[module] = {
                "total_kb": total_kb,
                "code_kb": code_kb,
                "data_kb": data_kb,
                "bss_kb": bss_kb,
                "other_kb": other_kb,
                "percent": percent,
            }

        percent = runningTotal_kb / available_kb
        outFile.write(
            "{0:>25}  {1:>9.2f}  {2:>9.2f}  {3:>9.2f}  {4:>9.2f}  {5:>9.2f}  {6:>10.2%}\n".format(
                "Total",
                runningCode_kb,
                runningData_kb,
                runningBss_kb,
                runningOther_kb,
                runningTotal_kb,
                percent,
            )
        )

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
            "{0:>25}  {1:>9}  {2:>9}  {3:>9}  {4:>9}  {5:>9.2f}  {6:>10.2%}\n".format(
                "Unused", "-", "-", "-", "-", unused_kb, percent
            )
        )

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
        Create the dwarf symbol map.
        """
        result_dict = {}

        for i, line in enumerate(lines):
            if "DW_AT_name" in line:
                key = line.split('"')[1]
                value = "Unknown"
                for j in range(i + 1, min(i + 6, len(lines))):
                    if "DW_AT_decl_file" in lines[j]:
                        value = lines[j].split('"')[1].rsplit("\\", 1)[-1].rsplit("/", 1)[-1]
                        break

                if value != "Unknown":
                    result_dict[key] = value

        return result_dict

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

    def createModuleStats(self, llvm_nm_input_dict, llvm_dwarf_input_dict):
        """
        Create Module stats for file.
        """
        cores = ["r52App", "bbe32App"]
        Not_found_files_Dict = {"r52App": [], "bbe32App": []}
        output_module_size_allcores = {
            module: {k: 0 for k in ["Code", "Rodata", "Data", "Bss", "Other", "Total"]}
            for module in [
                "MCAL",
                "MMIC",
                "AUTOSAR",
                "APP",
                "Signal_Processing",
                "Xtensa",
                "Logging",
                "CALS",
                "IPC",
                "Angle_Finding",
                "Other",
                "Linker Optimised",
                "Uncategorized",
                "Total",
            ]
        }

        # Read module files dictionary from JSON file
        module_map = os.path.join(os.path.dirname(__file__), "module_filename_map.json")
        with open(module_map, "r") as module_files_file:
            module_files_dict_all = json.load(module_files_file)
        for core in cores:
            if core in llvm_dwarf_input_dict.keys():
                dwarf_symbol_map = self.create_dwarf_symbol_map(llvm_dwarf_input_dict[core])
                lines_list = self.read_nmfile_to_list(llvm_nm_input_dict[core], dwarf_symbol_map)

                if lines_list:
                    FileSize_table_dict = self.generate_FileSize_table(lines_list)

                    if FileSize_table_dict:
                        Merged_FileSize_table_dict = self.generate_FileSize_table_merged(
                            FileSize_table_dict
                        )

                output_module_size, not_found_files = self.process_file_sizes(
                    module_files_dict_all[core + ".elf"], Merged_FileSize_table_dict
                )

                Not_found_files_Dict[core] = not_found_files

                for module, module_info in output_module_size.items():
                    for section, value in module_info.items():
                        output_module_size_allcores[module][section] += value

        return output_module_size_allcores, Not_found_files_Dict

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

        moduleStats, Not_found_files_Dict = self.createModuleStats(symbolStats, dwarfStats)

        if len(Not_found_files_Dict["r52App"]) or len(Not_found_files_Dict["bbe32App"]):
            print(
                "\033[93m{}\033[0m".format(
                    "WARNING: Below files are not grouped to module for memory scripts. Group files in \\tools\\python\\memoryStats\\module_filename_map.json"
                )
            )
            print("\033[93m{}\033[0m".format(Not_found_files_Dict))

        if len(memSections) > 0:
            self.writeOutput(self.outFile, memSections, moduleStats, self.format, csvSections)
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
        sendDataDogMetrics=args.sendDataDogMetrics,
        branch=args.branch,
        variant=args.variant,
        swVersion=args.swVersion,
        symbolsTable=args.symbolsTable,
        dwarfTable=args.dwarfTable,
        verbose=args.verbose,
    )

    memStats.createMemoryStats()

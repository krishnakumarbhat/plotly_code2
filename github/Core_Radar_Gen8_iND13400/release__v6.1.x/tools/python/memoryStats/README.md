# MemoryStats README
**Memory stats outputs are generated with the help of the following three files:**

### Memory_stats_wrapper.bzl
This rule generates memory statistics for the provided map files and ELF files. It uses these files to run a Python script that formats and outputs a text file with a detailed memory breakdown. This script utilizes LLVM utilities `llvm-nm` and `llvm-dwarfdump` to provide module statistics. Additionally, it can upload this breakdown to DataDog if additional parameters are specified.

### MemoryStats.py
This Python script parses the given map/ELF files for a specific table and outputs this information to a `memoryStats` file. Users need to create a list of `memoryStatsMapFiles`, providing the map file and a list of specific memory sections to use; otherwise, it will use all sections.

### module_filename_map.json
This JSON file maps source files to their respective modules. Each time a developer adds a new file to the project, this file must be updated to reflect the appropriate module mapping.

## <u>Important Note for Developers</u>

1. Every time a developer adds a new file to the project, they must map it to the appropriate module in `module_filename_map.json`. Otherwise, below warning will appear during the build, and the file will be categorized under the Uncategorized section in the memory stats output:

    ```
    Warning : Files listed below are not grouped into modules for memory scripts. Group these files in module_filename_map.json.
    ```

2. If you need to add a new module to the memory stats, ensure you update the `output_module_size_allcores` dictionary in the `memoryStats.py` file to include the new module, as shown below:

    ```
    output_module_size_allcores = {
    module: {k: 0 for k in ["Code", "Rodata", "Data", "Bss", "Other", "Total"]}
    for module in [
        "MCAL",
        "MMIC",
        "BSW",
        "APP",
        "Signal_Processing",
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
    ```

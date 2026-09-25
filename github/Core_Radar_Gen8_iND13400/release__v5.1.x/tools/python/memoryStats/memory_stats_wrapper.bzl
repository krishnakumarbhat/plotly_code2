"""This rule generates the memory statistics for given map files

Using provided map files, it runs a python script which formats and outputs
a text file with a memory breakdown.
It can also upload this breakdown to DataDog, if additional parameters are provided
"""

load("@bazel_skylib//rules:common_settings.bzl", "BuildSettingInfo")

def _generate_memory_stats_impl(ctx):
    map_depsets = []
    map_files = []
    elf_depsets = []
    elf_files = []
    for binary in ctx.attr.cc_windriver_binaries:
        for fi in binary.files.to_list():
            if fi.extension == "map":
                map_depset = depset(direct = [fi])
                map_depsets.append(map_depset)

                map_file = map_depset.to_list()
                map_files = map_files + map_file
            elif fi.extension == "elf":
                elf_depset = depset(direct = [fi])
                elf_depsets.append(elf_depset)

                elf_file = elf_depset.to_list()
                elf_files = elf_files + elf_file

    # Run llvm-readelf
    llvm_nm_out = ctx.actions.declare_file("temp/" + ctx.label.name + "-symbols.txt")
    llvm_dwarf_out = ctx.actions.declare_file("temp/" + ctx.label.name + "-dwarfdump.txt")

    nm_args_list = [
        "-l",
        "-S",
    ] + [elf_file.path for elf_file in elf_files] + [">", "{}".format(llvm_nm_out.path)]

    dwarf_args_list = [
        "--debug-info",
    ] + [elf_file.path for elf_file in elf_files] + [">", "{}".format(llvm_dwarf_out.path)]

    if ctx.attr.is_windows:
        bat1 = ctx.actions.declare_file("temp\\" + ctx.label.name + "-nm-cmd.bat")
        ctx.actions.write(
            output = bat1,
            content = " ".join([ctx.executable.llvmNmLabel.path.replace("/", "\\")] + nm_args_list),
            is_executable = True,
        )

        ctx.actions.run(
            tools = [ctx.executable.llvmNmLabel, bat1],
            executable = bat1,
            outputs = [llvm_nm_out],
            inputs = elf_files,
            progress_message = "Generating Memory symbol table text file...",
        )

        bat2 = ctx.actions.declare_file("temp\\" + ctx.label.name + "-dwarf-cmd.bat")
        ctx.actions.write(
            output = bat2,
            content = " ".join([ctx.executable.llvmDwarfLabel.path.replace("/", "\\")] + dwarf_args_list),
            is_executable = True,
        )

        ctx.actions.run(
            tools = [ctx.executable.llvmDwarfLabel, bat2],
            executable = bat2,
            outputs = [llvm_dwarf_out],
            inputs = elf_files,
            progress_message = "Generating dwarf symbol text file...",
        )
    else:
        ctx.actions.run_shell(
            tools = [ctx.executable.llvmNmLabel],
            command = " ".join([ctx.executable.llvmNmLabel.path] + nm_args_list),
            outputs = [llvm_nm_out],
            inputs = elf_files,
            progress_message = "Generating Memory symbol table text file...",
        )

        ctx.actions.run_shell(
            tools = [ctx.executable.llvmDwarfLabel],
            command = " ".join([ctx.executable.llvmDwarfLabel.path] + dwarf_args_list),
            outputs = [llvm_dwarf_out],
            inputs = elf_files,
            progress_message = "Generating dwarf symbol text file...",
        )

    memorystats_args = ctx.actions.args()

    # Run memoryStats.py
    if ctx.attr.uploadStatsToDataDog or ctx.attr.swVersion[BuildSettingInfo].value:
        memorystats_args.add("--dataDog")
        memorystats_args.add_all([
            "--variant",
            ctx.attr.variant,
        ])
        if ctx.attr.branch[BuildSettingInfo].value:
            memorystats_args.add_all([
                "--branch",
                ctx.attr.branch[BuildSettingInfo].value,
            ])
        else:
            fail(
                "\n\nBranch is required when trying to upload metrics to dataDog. " +
                "When not provided, metrics cannot be easily fitlered, thus creating unusable data.\n" +
                "Please provide a branch by passing a string_flag into the the branch attribute of this rule.\n\n",
            )
        if ctx.attr.swVersion[BuildSettingInfo].value:
            memorystats_args.add_all([
                "--swVersion",
                ctx.attr.swVersion[BuildSettingInfo].value,
            ])
    if ctx.attr.verbosePython:
        memorystats_args.add("--verbose")

    outfile = ctx.actions.declare_file("temp/memoryStats.txt")
    outfile_file_stats = ctx.actions.declare_file("temp/memoryStats_files.xlsx")
    outfile_symbol_stats = ctx.actions.declare_file("temp/memoryStats_symbols.xlsx")

    memorystats_args.add_all(map_files)
    memorystats_args.add(outfile)
    memorystats_args.add(outfile_file_stats)
    memorystats_args.add(outfile_symbol_stats)
    memorystats_args.add("--module-map")
    memorystats_args.add(ctx.file.module_map)
    memorystats_args.add("--symbols-txt")
    memorystats_args.add(llvm_nm_out)
    memorystats_args.add("--dwarf-txt")
    memorystats_args.add(llvm_dwarf_out)

    ctx.actions.run(
        executable = ctx.executable.memStatsLabel,
        arguments = [memorystats_args],
        outputs = [outfile] + [outfile_file_stats] + [outfile_symbol_stats],
        inputs = map_files + [ctx.file.module_map] + [llvm_nm_out] + [llvm_dwarf_out],
        progress_message = "Generating Memory Stats...",
    )

    # Run stackAnalysis.py
    # These arguments may need to be made a bit more configurable in the
    # future. For now, just iterate through the list of targets and derive
    # corresponding input and output file names from the target name.
    stackanalysis_outfiles = []
    if ctx.executable.stackAnalysisLabel:
        # target like "bbe32App.elf", "m7App.map"
        for target in ctx.attr.stack_analysis_targets:
            # target_base becomes "bbe32App", "m7App"
            target_base = target.rpartition(".")[0]

            # tmpout becomes "temp/bbe32App_stack_report.txt"
            tmpout = ctx.actions.declare_file("temp/" + target_base + "_stack_report.txt")
            stackanalysis_outfiles.append(tmpout)
            stackanalysis_args = ctx.actions.args()

            # config file basename like "bbe32App_config.ini"
            stackanalysis_args.add(target_base + "_config.ini")

            # find corresponding input elf/map file in list
            for file in elf_files + map_files:
                if file.basename == target:
                    stackanalysis_args.add(file.path)
                    break

            # generage output report
            stackanalysis_args.add_all(["-o", tmpout])

            # specify exec root location (happens to be PWD)
            stackanalysis_args.add_all(["-r", "."])

            # enable extra verbosity if desired
            if ctx.attr.verbosePython:
                stackanalysis_args.add("--verbose")
            ctx.actions.run(
                executable = ctx.executable.stackAnalysisLabel,
                arguments = [stackanalysis_args],
                outputs = [tmpout],
                inputs = elf_files,
                progress_message = "Generating Stack Usage Report for {}...".format(target),
            )

    return [DefaultInfo(files = depset([
        outfile,
        outfile_file_stats,
        llvm_nm_out,
        # llvm_dwarf_out,
        outfile_symbol_stats,
    ] + stackanalysis_outfiles))]

_generate_memory_stats = rule(
    implementation = _generate_memory_stats_impl,
    attrs = {
        "is_windows": attr.bool(mandatory = True),
        "llvmNmLabel": attr.label(mandatory = True, executable = True, cfg = "exec", allow_single_file = True, doc = "Label of the llvm-nm executable"),
        "llvmDwarfLabel": attr.label(mandatory = True, executable = True, cfg = "exec", allow_single_file = True, doc = "Label of the llvm-Dwarfdump executable"),
        "memStatsLabel": attr.label(mandatory = True, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the memoryStats script"),
        "stackAnalysisLabel": attr.label(mandatory = False, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the stackAnalysis script"),
        "stack_analysis_targets": attr.string_list(mandatory = False, doc = "List of elf/map files to perform stack analysis"),
        "cc_windriver_binaries": attr.label_list(mandatory = True, doc = "List of the binaries to include in the memory stats metrics"),
        "module_map": attr.label(mandatory = False, allow_single_file = True, doc = "JSON file map of file names to module"),
        "uploadStatsToDataDog": attr.bool(default = False, mandatory = False, doc = "Memory Stats can be uploaded to DataDog. Set to True to upload them."),
        "variant": attr.string(mandatory = True, doc = "Variant for which the memory stats are generated."),
        "branch": attr.label(default = ":gitBranch", mandatory = False, doc = "Branch this was built on. Used to tag metrics for dataDog."),
        "swVersion": attr.label(default = ":swVersion", mandatory = False, doc = "Sw Version release tag. Used to tag metrics for dataDog."),
        "verbosePython": attr.bool(default = False, mandatory = False, doc = "Run the memory stats script in verbose mode"),
    },
)

def generate_memory_stats(name, cc_windriver_binaries, module_map = None, stack_analysis_targets = None, uploadStatsToDataDog = None, branch = None, swVersion = None, **kwargs):
    """Generate a memory statistics breakdown file.

    This rule takes in map files, and converts them to a human readable memory statistics breakdown.
    It can also upload the breakdown to DataDog, if the uploadStatsToDataDog is set to True.

    Args:
        name: name of the target to generate
        branch: string_flag used to tag metrics when uploading them to DataDog
        cc_windriver_binaries: targets that contain the map files to generate the memory stats for
        module_map: JSON file map of file names to module
        stack_analysis_targets: list of targets (.elf or .map files) that will be analyzed by the stackAnalysis.py script
        uploadStatsToDataDog: Optional - Set to true to force the metrics to be uploaded to DataDog.
            Otherwise, it will only upload when it is a jenkins build on the dev branch
        swVersion: Optional - Sw Version release tag. Used to tag metrics for dataDog
        **kwargs: further keyword arguments, e.g. `visibility`
    """
    generate_memory_stats_impl = _generate_memory_stats

    upload_to_datadog = False

    # if uploadStatsToDataDog == None:
    # pload_to_datadog = select({
    # //tools/python/memoryStats:jenkinsDevBranch": True,
    # //conditions:default": False,
    # })

    if branch == None:
        branch = "//tools/python/memoryStats:gitBranch"

    if swVersion == None:
        swVersion = "//tools/python/memoryStats:swVersion"

    if module_map == None:
        module_map = Label("//software:module_filename_map.json")

    generate_memory_stats_impl(
        name = name,
        llvmNmLabel = select({
            "@platforms//os:windows": "@clang_windows//:bin/llvm-nm.exe",
            "//conditions:default": "@clang_linux//:bin/llvm-nm",
        }),
        llvmDwarfLabel = select({
            "@platforms//os:windows": "@clang_windows//:bin/llvm-dwarfdump.exe",
            "//conditions:default": "@clang_linux//:bin/llvm-dwarfdump",
        }),
        branch = branch,
        cc_windriver_binaries = cc_windriver_binaries,
        module_map = module_map,
        is_windows = select({
            "@platforms//os:windows": True,
            "//conditions:default": False,
        }),
        memStatsLabel = "//tools/python/memoryStats",
        stackAnalysisLabel = "//tools/python/stackAnalysis",
        stack_analysis_targets = stack_analysis_targets,
        uploadStatsToDataDog = upload_to_datadog,
        variant = select({
            "@build_config//:flr8": "flr8",
            "@build_config//:srr8p": "srr8p",
        }),
        swVersion = swVersion,
        **kwargs
    )

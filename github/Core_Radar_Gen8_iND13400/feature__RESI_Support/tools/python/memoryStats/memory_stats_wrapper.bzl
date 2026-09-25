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

    memorystats_args.add_all(map_files)
    memorystats_args.add(outfile)
    memorystats_args.add("--symbols-txt")
    memorystats_args.add(llvm_nm_out)
    memorystats_args.add("--dwarf-txt")
    memorystats_args.add(llvm_dwarf_out)

    ctx.actions.run(
        executable = ctx.executable.memStatsLabel,
        arguments = [memorystats_args],
        outputs = [outfile],
        inputs = map_files + [llvm_nm_out] + [llvm_dwarf_out],
        progress_message = "Generating Memory Stats...",
    )

    return [DefaultInfo(files = depset([
        outfile,
    ]))]

_generate_memory_stats = rule(
    implementation = _generate_memory_stats_impl,
    attrs = {
        "is_windows": attr.bool(mandatory = True),
        "llvmNmLabel": attr.label(mandatory = True, executable = True, cfg = "exec", allow_single_file = True, doc = "Label of the llvm-nm executable"),
        "llvmDwarfLabel": attr.label(mandatory = True, executable = True, cfg = "exec", allow_single_file = True, doc = "Label of the llvm-Dwarfdump executable"),
        "memStatsLabel": attr.label(mandatory = True, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the memoryStats script"),
        "cc_windriver_binaries": attr.label_list(mandatory = True, doc = "List of the binaries to include in the memory stats metrics"),
        "uploadStatsToDataDog": attr.bool(default = False, mandatory = False, doc = "Memory Stats can be uploaded to DataDog. Set to True to upload them."),
        "variant": attr.string(mandatory = True, doc = "Variant for which the memory stats are generated."),
        "branch": attr.label(default = ":gitBranch", mandatory = False, doc = "Branch this was built on. Used to tag metrics for dataDog."),
        "swVersion": attr.label(default = ":swVersion", mandatory = False, doc = "Sw Version release tag. Used to tag metrics for dataDog."),
        "verbosePython": attr.bool(default = False, mandatory = False, doc = "Run the memory stats script in verbose mode"),
    },
)

def generate_memory_stats(name, cc_windriver_binaries, uploadStatsToDataDog = None, branch = None, swVersion = None, **kwargs):
    """Generate a memory statistics breakdown file.

    This rule takes in map files, and converts them to a human readable memory statistics breakdown.
    It can also upload the breakdown to DataDog, if the uploadStatsToDataDog is set to True.

    Args:
        name: name of the target to generate
        branch: string_flag used to tag metrics when uploading them to DataDog
        cc_windriver_binaries: targets that contain the map files to generate the memory stats for
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

    generate_memory_stats_impl(
        name = name,
        llvmNmLabel = select({
            "@bazel_tools//src/conditions:host_windows": "@llvm-nm-win//file",
            "//conditions:default": "@llvm-nm-linux//:bin/llvm-nm",
        }),
        llvmDwarfLabel = select({
            "@bazel_tools//src/conditions:host_windows": "@llvm-dwarfdump-win//file",
            "//conditions:default": "@llvm-dwarfdump-linux//file",
        }),
        branch = branch,
        cc_windriver_binaries = cc_windriver_binaries,
        is_windows = select({
            "@bazel_tools//src/conditions:host_windows": True,
            "//conditions:default": False,
        }),
        memStatsLabel = "//tools/python/memoryStats",
        uploadStatsToDataDog = upload_to_datadog,
        variant = select({
            "@build_config//:flr8": "flr8",
        }),
        swVersion = swVersion,
        **kwargs
    )

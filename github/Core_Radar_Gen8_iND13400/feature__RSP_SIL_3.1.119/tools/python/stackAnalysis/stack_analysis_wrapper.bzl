"""This rule generates the stack analysis for given .elf files

Using provided .elf files, it runs a python script which formats and outputs
a text file with a stack analysis report.
"""

def _stack_analysis_impl(ctx):
    # Run stackAnalysis.py
    # These arguments may need to be made a bit more configurable in the
    # future. For now, just iterate through the list of targets and derive
    # corresponding input and output file names from the target name.
    map_files = []
    elf_files = []
    for fi in ctx.attr.stack_analysis_target.files.to_list():
        if fi.extension == "map":
            map_files += depset(direct = [fi]).to_list()
        elif fi.extension == "elf":
            elf_files += depset(direct = [fi]).to_list()
    if not elf_files and not map_files:
        fail("No .map or .elf files found in stack_analysis_target: {}".format(ctx.attr.stack_analysis_target))

    stackanalysis_outfiles = []

    # tmpout becomes "temp/bbe32App_stack_report.txt"
    tmpout = ctx.actions.declare_file("temp/" + ctx.label.name + "_report.txt")
    stackanalysis_outfiles.append(tmpout)

    stackanalysis_args = ctx.actions.args()

    # config file basename like "bbe32App_config.ini"
    stackanalysis_args.add(ctx.label.name + "_config.ini")

    # There should only be one input file, so use the first one we found.
    for file in elf_files + map_files:
        stackanalysis_args.add(file.path)
        break

    # generate output report
    stackanalysis_args.add_all(["-o", tmpout])

    # specify exec root location (happens to be PWD)
    stackanalysis_args.add_all(["-r", "."])

    # enable extra verbosity if desired
    if ctx.attr.verbosePython:
        stackanalysis_args.add("--verbose")

    # If a tool path was provided, add its exec-file path and include it in inputs
    extra_inputs = []
    if ctx.attr.tool_path:
        # ctx.file.data is the File object for a single-file exec cfg label
        stackanalysis_args.add_all(["--tool_path", ctx.file.tool_path.path])
        extra_inputs = [ctx.file.tool_path]
    if ctx.attr.system_path:
        # ctx.file.data is the File object for a single-file exec cfg label
        stackanalysis_args.add_all(["--system_path", ctx.file.system_path.path])
        extra_inputs = [ctx.file.system_path]

    ctx.actions.run(
        executable = ctx.executable.stackAnalysisLabel,
        arguments = [stackanalysis_args],
        outputs = [tmpout],
        inputs = elf_files + extra_inputs,
        progress_message = "Generating Stack Usage Report for {}...".format(ctx.attr.stack_analysis_target),
    )

    return [DefaultInfo(files = depset(stackanalysis_outfiles))]

stack_analysis = rule(
    implementation = _stack_analysis_impl,
    attrs = {
        "stackAnalysisLabel": attr.label(default = "//tools/python/stackAnalysis", mandatory = False, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the stackAnalysis script"),
        "stack_analysis_target": attr.label(mandatory = True, doc = "A bbe32_cc_binary() target to perform stack analysis"),
        "tool_path": attr.label(mandatory = False, allow_single_file = True, cfg = "exec", doc = "Path to the xt-stack-usage tool to use"),
        "system_path": attr.label(mandatory = False, allow_single_file = True, cfg = "exec", doc = "Path to the xtensa_system folders"),
        "verbosePython": attr.bool(default = False, mandatory = False, doc = "Run the memory stats script in verbose mode"),
    },
)

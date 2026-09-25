"""
Generate output file with .csv extention by taking the inputs(no_of_bytes)

from the streamdef text files

"""

def _bandwidth_impl(ctx):
    output_file_name = ctx.attr.variant_name + "_" + "Stream_Bandwidth_Details.csv"

    source_files = ctx.actions.declare_file("stream_bandwidth_source_file.txt")

    filtered_files_list = []
    filtered_name = source_files.dirname + "/outputs/" + ctx.attr.variant_name + "/streamdefs/streamdef_src" + ctx.attr.variant_id

    for file in ctx.files.input_files:
        if file.path.startswith(filtered_name) and file.path.endswith(".txt") and not file.path.startswith(filtered_name + "_str014_ver"):
            filtered_files_list.append(file.path)

    ctx.actions.write(
        output = source_files,
        content = "\n".join(filtered_files_list),
        is_executable = False,
    )

    # print("\nfiltered files: ", filtered_files_list)

    output_file_list = ctx.actions.declare_file(output_file_name)

    # print ("file:", output_file_list)

    args = ctx.actions.args()
    args.add_all([
        "--output_file_path",
        output_file_list.path,
        "--input_files",
        source_files.path,
    ])

    ctx.actions.run(
        inputs = ctx.files.input_files + [source_files],
        outputs = [output_file_list],
        arguments = [args],
        executable = ctx.executable.py_script,
        progress_message = "Generating stream bandwidth report...",
    )

    return DefaultInfo(files = depset([output_file_list]))

_bandwidth = rule(
    implementation = _bandwidth_impl,
    attrs = {
        "input_files": attr.label_list(mandatory = True, allow_files = True),
        "variant_name": attr.string(mandatory = False, values = [], default = "", doc = None),
        "variant_id": attr.string(mandatory = False, values = [], default = "", doc = None),
        "py_script": attr.label(mandatory = True, executable = True, cfg = "exec"),
    },
)

def Stream_Bandwidth_Info(name, input_files, variant_name, variant_id, **kwargs):
    """
    Generate stream bandwidth details for a provided input files.

    read the input file and get first line as stream size and calculate the bandwidth in Mbps.

    Args:
        name: Name of the rule.
        input_files: The input stream file which provide the stream's size.
        variant_name: variant name
        variant_id: variant Id
        **kwargs: Standard Bazel parameters for rules
    """
    stream_bw_imp = _bandwidth

    stream_bw_imp(
        name = name,
        input_files = input_files,
        variant_name = variant_name,
        variant_id = variant_id,
        py_script = "//tools/python/streamBandwidth:stream_bandwidth",
        **kwargs
    )

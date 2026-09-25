"""
Generate stream definition files for a provided logging file

generate_stream_files runs the stream def tool on a given stream logging file.
"""

def _getfilefromlist(files, filename):
    for file in files:
        if filename in file.basename:
            return file
    return None

def _CreateStreamDefs(ctx):
    output_folder = "generated_streamdefs"
    output_folder = ctx.attr.variant_name + "_" + output_folder
    supported_folder = "supported_files"

    if ctx.attr.is_windows:
        stream_def_tool = _getfilefromlist(ctx.files.windows_stream_def_tool_target, "STREAM_GENERATOR")
    else:
        stream_def_tool = _getfilefromlist(ctx.files.linux_stream_def_tool_target, "STREAM_GENERATOR")
    print("stream_def_tool_path:", stream_def_tool, ctx.attr.is_windows)

    config_file = ctx.actions.declare_file("config_file.txt")
    ctx.actions.write(
        output = config_file,
        content = config_file.dirname + "/" + output_folder + "/" + supported_folder + "/stream_header.h" + "\n" + config_file.dirname + "/" + output_folder + "/" + supported_folder + "/radar_sw_config.h" + "\n",
        is_executable = False,
    )

    filtered_files_list = []
    supported_file_list = []
    for file in ctx.files.logging_files:
        if file.path.endswith("_stream.h") and file.basename not in ctx.attr.exclude_files:
            filtered_files_list.append(file.path)
        else:
            supported_file_list.append(file.path)

    stream_files = ctx.actions.declare_file("logging_file.txt")
    ctx.actions.write(
        output = stream_files,
        content = "\n".join(filtered_files_list),
        is_executable = False,
    )

    supported_files = ctx.actions.declare_file("supported_logging_file.txt")
    ctx.actions.write(
        output = supported_files,
        content = "\n".join(supported_file_list),
        is_executable = False,
    )

    output_file_list = []
    for file in ctx.files.logging_files:
        if file.path.endswith("_stream.h") and file.basename not in ctx.attr.exclude_files:
            filename = file.basename.replace("_stream.h", "", 1)
            print(filename, file.basename)
            output_file_list.extend([
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream.c"),
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream.h"),
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream.xml"),
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream_Error_Check.c"),
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream_Error_Check.h"),
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream_mf.cpp"),
                ctx.actions.declare_file(output_folder + "/" + filename + "_stream_mf.h"),
            ])
    args = ctx.actions.args()
    args.add_all([
        "--stream_gen_executable",
        stream_def_tool.path,
        "--input_logging_files",
        stream_files.path,
        "--config_file",
        config_file.path,
        "--output_folder",
        config_file.dirname + "/" + output_folder,
        "--supported_folder",
        config_file.dirname + "/" + output_folder + "/" + supported_folder,
        "--input_supported_files",
        supported_files.path,
        "--resim_stream_def_zip_path",
        config_file.dirname + "/" + "outputs" + "/" + ctx.attr.variant_name + "/" + output_folder,
        "--is_windows",
        ctx.attr.is_windows,
    ])
    ctx.actions.run(
        inputs = ctx.files.logging_files + [config_file, stream_files, supported_files],
        outputs = output_file_list,
        arguments = [args],
        progress_message = "Creating Stream File: ",
        executable = ctx.executable.py_script,
    )

    return [DefaultInfo(files = depset(output_file_list))]

_generate_stream_files = rule(
    implementation = _CreateStreamDefs,
    attrs = {
        "logging_files": attr.label_list(mandatory = True, allow_files = True),
        "exclude_files": attr.string_list(mandatory = True, allow_empty = True, default = [], doc = None),
        "variant_name": attr.string(mandatory = False, values = [], default = "", doc = None),
        "windows_stream_def_tool_target": attr.label(mandatory = True),
        "linux_stream_def_tool_target": attr.label(mandatory = True),
        "is_windows": attr.int(mandatory = True),
        "py_script": attr.label(mandatory = True, executable = True, cfg = "exec"),
    },
)

def generate_stream_files(name, logging_files, exclude_files, variant_name, **kwargs):
    """
    Generate stream definition files for a provided logging file

    generate_stream_files runs the stream def tool on a given stream logging file.

    It selects the appropriate windows/linux script and passes all needed parameters.

    File names for the stream defs are formatted here.

    It is expected that the necessary logging files and its includes are copied
    into a common directory before the directory is passed in to logging_dir.

    Args:
        name: Name of the rule.
        logging_files: The base logging file to convert to a streamdef file
        exclude_files:
        variant_name:
        **kwargs: Standard Bazel parameters for rules
    """
    generate_stream_files_imp = _generate_stream_files

    generate_stream_files_imp(
        name = name,
        is_windows = select({
            "@bazel_tools//src/conditions:host_windows": 1,
            "//conditions:default": 0,
        }),
        logging_files = logging_files,
        exclude_files = exclude_files,
        variant_name = variant_name,
        py_script = "//tools/python/streamGenerator:streamgenerator",
        windows_stream_def_tool_target = "@StreamHeader_Gen//:StreamGeneratorWindows",
        linux_stream_def_tool_target = "@StreamHeader_Gen//:StreamGeneratorLinux",
        **kwargs
    )

"""
Generate stream definition files for a provided logging file

generate_stream_def runs the stream def tool on a given stream logging file.

It selects the appropriate windows/linux script and passes all needed parameters.

File names for the stream defs are formatted here.

It is expected that the necessary logging files and its includes are copied
into a common directory before the directory is passed in to logging_dir.
"""

def _CreateStreamDefs(ctx):
    stream_def_tool = depset(direct = ctx.attr.stream_def_tool_target.files.to_list()).to_list()[0]
    stream_def_script = depset(direct = ctx.attr.stream_def_script.files.to_list())
    logging_dir = ctx.attr.logging_dir.files.to_list()
    logging_file = ctx.attr.logging_file
    section_compatability = str(ctx.attr.section_compatability)
    generate_yml = ctx.attr.generate_yml

    # DvrlStreamTool supports user-defined preprocessing macros via the `-d` option.
    # Our wrapper scripts historically hard-coded a few, but some headers require
    # byte-order macros to be defined (LITTLE_ENDIAN_STRUCTURE/BIG_ENDIAN_STRUCTURE).
    #
    # Note: Bazel --conlyopt/--cxxopt are not visible from Starlark. To make a
    # macro available here, it must be provided via Bazel `--define`, e.g.:
    #   build --define=LITTLE_ENDIAN_STRUCTURE=1
    macro_labels = "PCRESIM=true,PC_RESIM=true,DVTOOL_MODIFICATION=true"
    little = ctx.var.get("LITTLE_ENDIAN_STRUCTURE")
    big = ctx.var.get("BIG_ENDIAN_STRUCTURE")
    if little != None:
        macro_labels = macro_labels + ",LITTLE_ENDIAN_STRUCTURE=true"
    elif big != None:
        macro_labels = macro_labels + ",BIG_ENDIAN_STRUCTURE=true"

    output_list = []

    for _file in logging_dir:
        if (_file.basename == logging_file):
            logging_file = _file

    stream_struct = ctx.attr.stream_struct
    repeating_struct = ctx.attr.repeating_struct
    stream_num = str(ctx.attr.stream_num)
    stream_ver = str(ctx.attr.stream_ver)
    source_num = str(ctx.attr.source_num)

    # pad with zeros to get 3 digits
    if (len(stream_num) < 2):
        stream_num = "00" + stream_num
    elif (len(stream_num) == 2):
        stream_num = "0" + stream_num
    if (len(stream_ver) < 2):
        stream_ver = "00" + stream_ver
    elif (len(stream_ver) == 2):
        stream_ver = "0" + stream_ver
    if (len(source_num) < 2):
        source_num = "00" + source_num
    elif (len(source_num) == 2):
        source_num = "0" + source_num

    if (stream_num == "014"):
        # Stream 14 has sub streams for usc/smc so they are duplicated to append the section compatability version to the file name.
        postfix = logging_file.basename.replace(".h", "").replace("_cal", "")
        stream_out_txt = ctx.actions.declare_file("streamdef_src%s_str%s_ver%s.txt" % (source_num, stream_num, stream_ver))
        output_list.append(stream_out_txt)
        if (generate_yml):
            stream_out_yml = ctx.actions.declare_file("streamdef_src%s_str%s_ver%s.yml" % (source_num, stream_num, stream_ver))
            output_list.append(stream_out_yml)
        if (section_compatability):
            if (len(section_compatability) < 2):
                section_compatability = "00" + section_compatability
            elif (len(section_compatability) == 2):
                section_compatability = "0" + section_compatability

            cal_out_txt = ctx.actions.declare_file("streamdef_src%s_str%s_%s_ver%s.txt" % (source_num, stream_num, postfix, section_compatability))
            output_list.append(cal_out_txt)
    else:
        stream_out_txt = ctx.actions.declare_file("streamdef_src%s_str%s_ver%s.txt" % (source_num, stream_num, stream_ver))
        output_list.append(stream_out_txt)
        if (generate_yml):
            stream_out_yml = ctx.actions.declare_file("streamdef_src%s_str%s_ver%s.yml" % (source_num, stream_num, stream_ver))
            output_list.append(stream_out_yml)

    for outFile in output_list:
        args = ctx.actions.args()
        args.add_all([
            "-STREAM_DEF_TOOL",
            stream_def_tool.path,  # Stream def tool exe
            "-LOGGING_FILE",
            logging_file.path,  # Logging structure file
            "-SOURCE_NUM",
            source_num,  # Variant Source Number
            "-STREAM_NUM",
            ctx.attr.stream_num,  # Stream Number
            "-STREAM_VER",
            ctx.attr.stream_ver,  # Stream Version
            "-STREAM_STRUCT",
            stream_struct,  # Stream Structure
            "-MACRO_LABELS",
            macro_labels,
            "-OUT_DIR",
            outFile.dirname,
            "-OUT_FILE",
            outFile.basename,
        ])

        if (repeating_struct):
            args.add_all([
                "-REPEATING_STRUCT",
                repeating_struct,
            ])

        if (stream_num == "014" and section_compatability):
            # pass in the section compatability version for stream 14
            args.add_all([
                "-SECTION_COMPATABILITY",
                ctx.attr.section_compatability,
            ])

        if ctx.attr.is_windows:
            win_args = ctx.actions.args()
            win_args.add_all([
                stream_def_script.to_list()[0].path,
            ])
            ctx.actions.run(
                inputs = [logging_file] + logging_dir,
                outputs = [outFile],
                arguments = [win_args, args],
                progress_message = "Creating Stream Def File: " + outFile.basename,
                executable = ctx.files.powershell[0],
                tools = [stream_def_script, stream_def_tool],
            )
        else:
            ctx.actions.run(
                inputs = [logging_file] + logging_dir,
                outputs = [outFile],
                arguments = [args],
                progress_message = "Creating Stream Def File: " + outFile.basename,
                executable = stream_def_script.to_list()[0].path,
                tools = [stream_def_script, stream_def_tool],
            )

    return [DefaultInfo(files = depset(output_list))]

_generate_stream_def = rule(
    implementation = _CreateStreamDefs,
    attrs = {
        "logging_dir": attr.label(mandatory = True, allow_files = True),
        "logging_file": attr.string(mandatory = True),
        "powershell": attr.label(mandatory = False, cfg = "exec", allow_files = True),
        "source_num": attr.int(mandatory = True),
        "stream_num": attr.int(mandatory = True),
        "stream_ver": attr.int(mandatory = True),
        "stream_struct": attr.string(mandatory = True),
        "stream_def_tool_target": attr.label(mandatory = True),
        "stream_def_script": attr.label(mandatory = True, cfg = "exec", allow_files = True),
        "repeating_struct": attr.string(mandatory = False),
        "section_compatability": attr.int(mandatory = False),
        "generate_yml": attr.bool(mandatory = False, default = False),
        "is_windows": attr.bool(mandatory = True),
    },
)

def generate_stream_def(name, logging_dir, logging_file, stream_num, stream_struct, stream_ver, repeating_struct = None, section_compatability = None, generate_yml = False, visibility = None):
    """
    Generate stream definition files for a provided logging file

    generate_stream_def runs the stream def tool on a given stream logging file.

    It selects the appropriate windows/linux script and passes all needed parameters.

    File names for the stream defs are formatted here.

    It is expected that the necessary logging files and its includes are copied
    into a common directory before the directory is passed in to logging_dir.

    Args:
        name: Name of the rule.
        logging_dir: A target that contains a folder with all the necessary header files for the provided logging_file
        logging_file: The base logging file to convert to a streamdef file
        stream_num: The stream number for this logging file
        stream_struct: The main structure to use for generating the streamdef
        stream_ver: The stream version to use for this logging file
        repeating_struct: Optional - used to specify the final, repeating sub-structure element for variable-size-streams
        section_compatability: Optional - Used for stream 14 to determine which cal stream it iss
        generate_yml: Optional - Set to True to generate both .txt and .yml outputs
        visibility: Standard Bazel visibility rules apply
    """
    generate_stream_def_imp = _generate_stream_def

    generate_stream_def_imp(
        name = name,
        is_windows = select({
            "@bazel_tools//src/conditions:host_windows": True,
            "//conditions:default": False,
        }),
        logging_dir = logging_dir,
        logging_file = logging_file,
        powershell = select({
            "@bazel_tools//src/conditions:host_windows": "@build-tools//windows:powershell",
            "//conditions:default": None,
        }),
        section_compatability = section_compatability,
        generate_yml = generate_yml,
        source_num = select({
            "@build_config//:flr8": 82,
            "@build_config//:srr8p": 83,
        }),
        stream_def_script = select({
            "@bazel_tools//src/conditions:host_windows": "@StreamHeader_Gen//:create_stream_def_ps",
            "//conditions:default": "@StreamHeader_Gen//:create_stream_def_sh",
        }),
        stream_def_tool_target = "@StreamHeader_Gen//:streamDefTool",
        stream_num = stream_num,
        stream_struct = stream_struct,
        repeating_struct = repeating_struct,
        stream_ver = stream_ver,
        visibility = visibility,
    )

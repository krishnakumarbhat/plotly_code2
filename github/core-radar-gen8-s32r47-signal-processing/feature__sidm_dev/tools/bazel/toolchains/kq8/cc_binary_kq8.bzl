load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain")
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")

def _impl(ctx):
    elf_file = ctx.actions.declare_file(ctx.label.name + ".elf")
    map_file = ctx.actions.declare_file(ctx.label.name + ".elf.map")  # Mapfile automatcially generated with the elf name + .map
    srecord_file = ctx.actions.declare_file(ctx.label.name + ".s19")

    cc_toolchain = find_cpp_toolchain(ctx)
    feature_configuration = cc_common.configure_features(
        ctx = ctx,
        cc_toolchain = cc_toolchain,
        requested_features = ctx.features,
        unsupported_features = ctx.disabled_features,
    )

    # Compile any files provded in srcs
    (c_compilation_context, c_output) = cc_common.compile(
        cc_toolchain = cc_toolchain,
        feature_configuration = feature_configuration,
        actions = ctx.actions,
        srcs = [f for f in ctx.files.srcs if f.extension in ["cc", "cpp", "cxx", "c++", "C", "cu", "cl", "c", "s", "S", "asm"]],
        public_hdrs = [f for f in ctx.files.srcs if f.extension not in ["cc", "cpp", "cxx", "c++", "C", "cu", "cl", "c", "s", "S", "asm"]],
        compilation_contexts = [dep[CcInfo].compilation_context for dep in ctx.attr.deps if CcInfo in dep],
        name = ctx.label.name,
        user_compile_flags = ctx.fragments.cpp.copts + ctx.fragments.cpp.conlyopts + ctx.attr.copts + ["-D" + define for define in ctx.attr.defines],
        disallow_pic_outputs = True,
    )

    # Create a linking context based on the above compiled files
    linking_context, _ = cc_common.create_linking_context_from_compilation_outputs(
        name = ctx.label.name,
        actions = ctx.actions,
        feature_configuration = feature_configuration,
        disallow_dynamic_library = True,
        disallow_static_libraries = True,
        cc_toolchain = cc_toolchain,
        linking_contexts = [dep[CcInfo].linking_context for dep in ctx.attr.deps if CcInfo in dep],
        compilation_outputs = c_output,
    )

    # Combine compile context and linking context into a CcInfo, which can be returned
    complete_cc_info = CcInfo(compilation_context = c_compilation_context, linking_context = linking_context)

    # Gather all the input libraries (.o and .a) needed to link the output binary into a single list
    lib_inputs = [] + c_output.objects
    for linker_input in complete_cc_info.linking_context.linker_inputs.to_list():
        for library_to_link in linker_input.libraries:
            if hasattr(library_to_link, "static_library"):
                # prefer static library
                lib_inputs.append(library_to_link.static_library)
            elif hasattr(library_to_link, "objects") and len(library_to_link.objects) > 0:
                # if no static library found, use object files
                lib_inputs.extend(library_to_link.objects)
            else:
                fail("Found a dependency which has neither .o or .a files: {}".format(library_to_link))

    # Combine linkopts from the linkopts attribute and the --linkopts command line option
    all_linkopts_raw = ctx.attr.linkopts + ctx.fragments.cpp.linkopts

    # Iterate through all the linkopts, and use pseudo make replacement to replace a target linker script with its actual path.
    # This mimics behavior provided by the native cc_binary rule.
    # Add all linkopts to a "processed" list
    all_linkopts = []
    ld_scripts = []
    for opt in all_linkopts_raw:
        make_expanision_start_idx = opt.find("$(")
        if make_expanision_start_idx >= 0:
            if opt.count("(") == 1 and opt.count(")") == 1 and opt.count("execpath"):
                if "@" in opt:
                    startidx = opt.find("@") + 1
                elif "//" in opt:
                    startidx = opt.find("//")
                else:
                    fail("Could not find start of target for Make expansion.")

                endidx = opt.find(")")
                label = Label("@" + opt[startidx:endidx])

                script_found = False
                for dep in ctx.attr.deps:
                    if dep.label == label:
                        script_found = True
                        ld_script_file = dep.files.to_list()[0]
                        ld_scripts.append(ld_script_file)
                        all_linkopts.append(opt[0:make_expanision_start_idx] + ld_script_file.path)
                        break

                if not script_found:
                    fail("in linkopts attribute of cc_binary rule {}: label '{}' in $(execpath) expression is not a declared dep of this rule".format(ctx.label, label))
            else:
                fail("Could not expand the Make substitution. Only a single execpath per linker option is currently supported.")
        else:
            all_linkopts.append(opt)

    # Create a link variables map to help generate the command line syntax
    link_variables = cc_common.create_link_variables(
        cc_toolchain = cc_toolchain,
        feature_configuration = feature_configuration,
        output_file = elf_file.path,
        is_using_linker = True,
        user_link_flags = all_linkopts,
        is_linking_dynamic_library = False,
    )

    # Get the command line syntax for linking
    command_line = cc_common.get_memory_inefficient_command_line(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.cpp_link_executable,
        variables = link_variables,
    )

    # Use a param file for linking
    args = ctx.actions.args()
    args.set_param_file_format("multiline")
    args.use_param_file(param_file_arg = "@%s", use_always = True)

    # Add all the required arguments to the param file
    args.add_all([x for x in command_line])
    args.add_all(["-Wl,-Map,{}".format(map_file.path)])
    args.add_all([f.path for f in lib_inputs])

    # Get the toolchain defined environment for linking
    env = cc_common.get_environment_variables(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.cpp_link_executable,
        variables = link_variables,
    )

    # Get the toolchain defined path to the linker
    linker_path = cc_common.get_tool_for_action(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.cpp_link_executable,
    )

    # Link the binary and generate the map file
    ctx.actions.run(
        executable = linker_path,
        arguments = [args],
        env = env,
        progress_message = "Linking {}".format(ctx.label.name),
        mnemonic = "CcLink",
        inputs = depset(
            direct = lib_inputs,
            transitive = [cc_toolchain.all_files],
        ),
        outputs = [map_file, elf_file],
    )

    # now generate S19 file from elf
    # Get the toolchain defined environment for stripping
    env = cc_common.get_environment_variables(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.cpp_link_executable,
        variables = link_variables,  # required, but does not effect, so reuse link_variables.
    )

    strip_tool_path = cc_common.get_tool_for_action(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.strip,
    )
    args = ctx.actions.args()
    args.add_all(["-Osrec", "-s", "-D"])
    args.add("-o")
    args.add(srecord_file.path)
    args.add(elf_file.path)

    ctx.actions.run(
        executable = strip_tool_path,
        env = env,
        arguments = [args],
        inputs = [elf_file],
        outputs = [srecord_file],
        mnemonic = "generateS19",
        progress_message = "generating S19 file",
        tools = [cc_toolchain.all_files],
    )

    return [
        complete_cc_info,
        DefaultInfo(files = depset([map_file, elf_file, srecord_file])),
    ]

cc_binary_kq8 = rule(
    implementation = _impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True, doc = "Additional *.c or *cpp files, which shall be compiled and linked to the software"),
        "copts": attr.string_list(doc = "Additional compiler flags/options, which should be applied to the compiltation process.  Strings should start with `-` or `--` depending on compiler flag/option definition"),
        "defines": attr.string_list(allow_empty = True, doc = "defines flags will be added to this target and all it's dependencies"),
        "deps": attr.label_list(allow_files = True, doc = "The list of other libraries to be linked into the binary target. These can be `cc_library` or `cc_import`"),
        "linkopts": attr.string_list(doc = "Additional linker flags/options, which should be only applied to this linking process. Strings should start with `-` or `--` depending on compiler flag/option definition"),
    },
    fragments = ["cpp"],
    toolchains = ["@bazel_tools//tools/cpp:toolchain_type"],
)

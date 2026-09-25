"""NXP ARM GCC Toolchain Configuration for S32R47 M7"""

load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
)
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(":action_config.bzl", "declare_actions")

all_compile_actions = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_codegen,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.linkstamp_compile,
]

all_assemble_actions = [
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
]

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _impl(ctx):
    # Resolve external toolchain include paths from runfiles
    gcc_include_dirs = []
    if ctx.attr.additional_include_paths:
        # These are labels, we need to resolve them to actual file paths
        for label_str in ctx.attr.additional_include_paths:
            # Parse labels into path components
            if label_str.startswith("@"):
                # Extract the path part after "::"
                parts = label_str.split("::")
                if len(parts) == 2:
                    path_part = parts[1]

                    # Add as absolute path in the external runfiles directory
                    gcc_include_dirs.append(path_part)

    include_paths_feature = feature(
        name = "include_paths",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [
                    flag_group(flags = ["-I%{quote_include_paths}"], iterate_over = "quote_include_paths", expand_if_available = "quote_include_paths"),
                    flag_group(flags = ["-I%{include_paths}"], iterate_over = "include_paths", expand_if_available = "include_paths"),
                    flag_group(flags = ["-isystem%{system_include_paths}"], iterate_over = "system_include_paths", expand_if_available = "system_include_paths"),
                ],
            ),
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [
                    flag_group(flags = ["-isystem" + path for path in gcc_include_dirs]),
                ] if gcc_include_dirs else [],
            ),
        ],
    )

    random_seed_feature = feature(name = "random_seed", enabled = False)

    archiver_flags_feature = feature(
        name = "archiver_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [ACTION_NAMES.cpp_link_static_library],
                flag_groups = [
                    # Set the archiver flags
                    flag_group(
                        flags = [
                            "-qc",
                            "%{output_execpath}",
                        ],
                    ),
                    # Specify the compiled files to link into the library, using Bazel's built-in "libraries_to_link"
                    flag_group(
                        expand_if_available = "libraries_to_link",
                        iterate_over = "libraries_to_link",
                        flag_groups = [flag_group(flags = ["%{libraries_to_link.name}"])],
                    ),
                ],
            ),
        ],
    )

    features = [
        random_seed_feature,
        archiver_flags_feature,
        include_paths_feature,
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        toolchain_identifier = "nxp-arm-gcc",
        host_system_name = "local",
        target_system_name = "arm-none-eabi",
        target_cpu = "arm",
        target_libc = "newlib",
        compiler = "gcc",
        abi_version = "eabi",
        abi_libc_version = "newlib",
        tool_paths = [],
        action_configs = declare_actions(ctx),
        cxx_builtin_include_directories = gcc_include_dirs,
    )

design_studio_gcc_m7_cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {
        "additional_include_paths": attr.string_list(default = [], doc = "Additional include paths to add as -isystem flags."),
        "archiver_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to arm-none-eabi-ar"),
        "c_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to arm-none-eabi-gcc"),
        "cpp_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to arm-none-eabi-g++"),
        "cpp_preprocessor_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to arm-none-eabi-cpp"),
        "link_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to arm-none-eabi-g++ for linking"),
        "strip_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to arm-none-eabi-strip"),
    },
    provides = [CcToolchainConfigInfo],
)

"""Toolchain configuration for windriver diab compiler"""

load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl", "env_entry", "env_set", "feature", "flag_group", "flag_set")
load(":action_config.bzl", "declare_actions")

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

all_assemble_actions = [
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
]

all_compile_actions = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.clif_match,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_codegen,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.lto_backend,
]

all_actions = all_compile_actions + all_link_actions + all_assemble_actions

def _impl_cc_toolchain_config(ctx):
    dependency_file_feature = feature(
        name = "dependency_file",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + [ACTION_NAMES.preprocess_assemble],
                flag_groups = [
                    flag_group(flags = ["-MD"]),
                    flag_group(
                        flags = ["-MF%{dependency_file}"],
                        expand_if_available = "dependency_file",
                    ),
                ],
            ),
        ],
    )

    include_paths_feature = feature(
        name = "include_paths",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [
                    #   flag_group(flags = ["-iquote%{quote_include_paths}"], iterate_over = "quote_include_paths", expand_if_available = "quote_include_paths"),  # noqa: E126 Buildifier and Flake8 discrepency
                    flag_group(flags = ["-I%{quote_include_paths}"], iterate_over = "quote_include_paths", expand_if_available = "quote_include_paths"),  # noqa: E126 Buildifier and Flake8 discrepency
                    flag_group(flags = ["-I%{include_paths}"], iterate_over = "include_paths", expand_if_available = "include_paths"),
                    flag_group(flags = ["-isystem%{system_include_paths}"], iterate_over = "system_include_paths", expand_if_available = "system_include_paths"),
                ],  # noqa: E126 Buildifier and Flake8 discrepency
            ),
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups =
                    [
                        flag_group(flags = ["-isystem" + compiler_inc_path.path for compiler_inc_path in ctx.files.compiler_inc_paths]),  # noqa: E126 Buildifier and Flake8 discrepency
                    ] if ctx.files.compiler_inc_paths else [],  # noqa: E126 Buildifier and Flake8 discrepency
            ),
        ],
    )

    default_compiler_flags = feature(
        name = "default_compiler_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_assemble_actions,
                flag_groups = [
                    flag_group(
                        iterate_over = "preprocessor_defines",
                        expand_if_available = "preprocessor_defines",
                        flags = ["-D%{preprocessor_defines}"],
                    ),
                ],
            ),
            flag_set(
                actions = all_assemble_actions,
                flag_groups = [flag_group(flags = ctx.attr.asmopts)] if ctx.attr.asmopts else [],
            ),
            flag_set(
                actions = [
                    ACTION_NAMES.cpp_compile,
                    ACTION_NAMES.cpp_module_codegen,
                    ACTION_NAMES.cpp_module_compile,
                ],
                flag_groups = [flag_group(flags = ctx.attr.cxxopts)] if ctx.attr.cxxopts else [],
            ),
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.clif_match,
                    ACTION_NAMES.cpp_compile,
                    ACTION_NAMES.cpp_header_parsing,
                    ACTION_NAMES.cpp_module_codegen,
                    ACTION_NAMES.cpp_module_compile,
                ],
                flag_groups = [flag_group(flags = ctx.attr.copts)] if ctx.attr.copts else [],
            ),
        ],
    )
    default_linker_flags = feature(
        name = "default_linker_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_link_actions,
                flag_groups = [
                    flag_group(
                        flags = ctx.attr.linkopts,
                    ),
                    flag_group(
                        expand_if_available = "libraries_to_link",
                        iterate_over = "libraries_to_link",
                        flag_groups = [
                            flag_group(
                                flags = ["%{libraries_to_link.name}"],
                            ),
                        ],
                    ),
                ],
            ),
        ],
    )
    random_seed_feature = feature(name = "random_seed", enabled = False)

    strip_debug_symbol_feature = feature(name = "strip_debug_symbols", enabled = False)

    archiver_flags_feature = feature(
        name = "archiver_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [ACTION_NAMES.cpp_link_static_library],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-qc",
                            "%{output_execpath}",
                        ],
                    ),
                    flag_group(
                        expand_if_available = "libraries_to_link",
                        iterate_over = "libraries_to_link",
                        flag_groups = [flag_group(flags = ["%{libraries_to_link.name}"])],
                    ),
                ],
            ),
        ],
    )

    xtensa_core_and_system_path_feature = feature(
        name = "xtensa_core_and_system_path",
        enabled = True,
        env_sets = [
            env_set(
                actions = all_compile_actions + all_assemble_actions + all_link_actions + [ACTION_NAMES.cpp_link_static_library, ACTION_NAMES.strip],
                env_entries = [
                                  env_entry(key = "XTENSA_CORE", value = ctx.attr.xtensa_core),
                              ] +
                              [
                                  env_entry(key = "XTENSA_SYSTEM", value = ctx.files.xtensa_system_path[0].path),
                              ] if ctx.files.xtensa_system_path else [],
            ),
        ],
    )

    preprocessor_defines_feature = feature(
        name = "preprocessor_defines",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [
                    flag_group(
                        flags = ["-D%{preprocessor_defines}"],
                        expand_if_available = "preprocessor_defines",
                        iterate_over = "preprocessor_defines",
                    ),
                ],
            ),
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [flag_group(flags = ["-D" + define for define in ctx.attr.defines])] if ctx.attr.defines else [],
            ),
        ],
    )

    compiler_input_flags_feature = feature(
        name = "compiler_input_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [flag_group(flags = ["-c", "%{source_file}"], expand_if_available = "source_file")],
            ),
        ],
    )
    compiler_output_flags_feature = feature(
        name = "compiler_output_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [flag_group(flags = ["-o", "%{output_file}"], expand_if_available = "output_file")],
            ),
        ],
    )
    output_execpath_flags_feature = feature(
        name = "output_execpath_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_link_actions,
                flag_groups = [
                    flag_group(
                        flags = [
                            "-o",
                            "%{output_execpath}",
                        ],
                        expand_if_available = "output_execpath",
                    ),
                ],
            ),
        ],
    )

    license_server_env = feature(
        name = "license_server_env",
        enabled = True,
        env_sets = [
            env_set(
                actions = all_actions,
                env_entries = [
                    env_entry(key = "XTENSAD_LICENSE_FILE", value = ctx.attr.license_server),
                ],
            ),
        ],
    )

    user_link_flags_feature = feature(
        name = "user_link_flags",
        enabled = True,
        flag_sets = [
                        flag_set(
                            actions = all_link_actions,
                            flag_groups = [
                                flag_group(
                                    expand_if_available = "user_link_flags",
                                    iterate_over = "user_link_flags",
                                    flags = ["%{user_link_flags}"],
                                ),
                            ],
                        ),
                    ] +
                    [
                        flag_set(
                            actions = [ACTION_NAMES.cpp_link_executable],
                            flag_groups = [
                                flag_group(
                                    flags = ["-mlsp={}".format(ctx.files.linker_support_package[0].path)],
                                ),
                            ],
                        ),
                    ] if ctx.files.linker_support_package else [],
    )

    user_compile_flags_feature = feature(
        name = "user_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions + all_assemble_actions,
                flag_groups = [
                    flag_group(
                        expand_if_available = "user_compile_flags",
                        iterate_over = "user_compile_flags",
                        flags = ["%{user_compile_flags}"],
                    ),
                ],
            ),
        ],
    )

    lto_feature = feature(
        name = "lto",
        enabled = ctx.attr.enable_lto,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                    ACTION_NAMES.preprocess_assemble,
                    ACTION_NAMES.cpp_link_executable,
                ],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-flto",
                        ],
                    ),
                ],
            ),
        ],
    )

    link_only_feature = feature(
        name = "link_only_feature",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.cpp_link_executable,
                ],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-Wl,--orphan-handling=place",
                            "-Wl,-v",
                        ],
                    ),
                ],
            ),
        ],
    )

    features = [
        license_server_env,
        feature(name = "no_legacy_features"),  # remove all legacy features.
        preprocessor_defines_feature,
        user_compile_flags_feature,
        user_link_flags_feature,
        xtensa_core_and_system_path_feature,
        default_compiler_flags,
        default_linker_flags,
        lto_feature,
        link_only_feature,
        strip_debug_symbol_feature,  # To remove -S flag from linker
        archiver_flags_feature,
        random_seed_feature,
        include_paths_feature,
        dependency_file_feature,
        output_execpath_flags_feature,
        compiler_output_flags_feature,
        compiler_input_flags_feature,
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        toolchain_identifier = "bbe32",
        host_system_name = "local",
        target_system_name = "racerunner",
        target_libc = "unknown",
        target_cpu = "arm",
        compiler = "bbe32",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        cxx_builtin_include_directories = [],
        tool_paths = [],  # tools,
        action_configs = declare_actions(ctx),
    )

cc_toolchain_config = rule(
    implementation = _impl_cc_toolchain_config,
    attrs = {
        "archiver_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to Diab Archiver (dar)"),
        "assembler_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to Diab Assembler (das)"),
        "asmopts": attr.string_list(default = [], doc = "flags to set while compiling asm file"),
        "c_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to Diab C Compiler Wrapper (dcc)"),
        "compiler_inc_paths": attr.label_list(allow_files = True, doc = "Paths to add to -I for compiler standard includes. Typically, a directory is provided via filegroup."),
        "conlyopts": attr.string_list(default = [], doc = "flags to set while compiling only c source file"),
        "copts": attr.string_list(default = [], doc = "flags to set while compiling c/c++ source file"),
        "cpp_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to Diab C++ Compiler Wrapper (dplus)"),
        "cxxopts": attr.string_list(default = [], doc = "flags to set while compiling only c++ source file"),
        "defines": attr.string_list(default = [], doc = "common defines to add while compiling"),
        "link_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to Diab Linker (dld)"),
        "linker_support_package": attr.label(allow_single_file = True, doc = "Path to the Linker Support Package (-mlsp=[provided path]). Should be a filegroup to a single directory"),
        "enable_lto": attr.bool(default = False, doc = "Enable LLVM Link Time Optimizations by default."),
        "linkopts": attr.string_list(default = [], doc = "flags to set while linking objects file"),
        "license_server": attr.string(mandatory = False, default = "27001@nlnhsrk-is22.aptiv.com", doc = "License address"),
        "strip_executable": attr.label(mandatory = True, allow_single_file = True, executable = True, cfg = "exec", doc = "Path to Clang Strip Executable. This is different for 32 and 64 bit targes."),
        "xtensa_core": attr.string(mandatory = True, doc = "The name of the core for which the toolchain is configured. Only one is allowed per toolchain instance."),
        "xtensa_system_path": attr.label(allow_files = True, doc = "Directory path to add to xtensa-system that are required by the compiler. Typically, a directory is provided via filegroup. *NOTE: if an absolute path is provided, your build will not be hermetic!*"),
    },
    provides = [CcToolchainConfigInfo],
)

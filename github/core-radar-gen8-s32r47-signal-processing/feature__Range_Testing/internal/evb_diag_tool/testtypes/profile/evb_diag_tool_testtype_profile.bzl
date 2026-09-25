"""
Macro to build a complete EVB Diagnostics Tool test suite for profiling code.

This macro generates the build targets for the tested cores, and the M7 code that controls the test,
including compilation, linking, image generation, and size analysis.
"""

load("//internal/evb_diag_tool/base:evb_diag_tool_test.bzl", "evb_diag_tool_test")

def evb_diag_tool_testtype_profile(
        name,
        m7_profile_defs_h,  # list of header paths
        m7_copts = [],
        m7_linkopts = [],
        spt_0_srcs = [],  # The SPT source(s) that contain the kernel labels to be tested
        spt_0_deps = [],
        spt_0_linkopts = [],
        spt_1_srcs = [],  # The SPT source(s) that contain the kernel labels to be tested
        spt_1_deps = [],
        spt_1_linkopts = [],
        bbe_0_srcs = [],  # The BBE0 source(s) that contain the code to be tested
        bbe_0_deps = [],
        bbe_0_copts = [],
        bbe_0_defines = [],
        bbe_0_linkopts = [],
        bbe_0_hdrs = [],
        bbe_1_srcs = [],  # The BBE1 source(s) that contain the code to be tested
        bbe_1_deps = [],
        bbe_1_copts = [],
        bbe_1_defines = [],
        bbe_1_linkopts = [],
        bbe_1_hdrs = []):
    """
    Build a complete EVB Diagnostics Tool test suite for profiling code.

    Args:
        name: The base name for generated targets
        m7_profile_defs_h: The headers that defines the test structs and information, must all be in the same directory.
        m7_copts: (Optional) Additional compiler options for M7 core. Defaults to [].
        m7_linkopts: (Optional) Additional linker options for M7 core. Defaults to [].
        spt_0_srcs: (Optional) The SPT source(s) that contain the kernel labels to be tested
        spt_0_deps: (Optional) Additional cc_library dependencies for SPT core 0. Defaults to [].
        spt_0_linkopts: (Optional) Additional linker options for SPT core 0. Defaults to [].
        spt_1_srcs: (Optional) The SPT source(s) that contain the kernel labels to be tested
        spt_1_deps: (Optional) Additional cc_library dependencies for SPT core 1. Defaults to [].
        spt_1_linkopts: (Optional) Additional linker options for SPT core 1. Defaults to [].
        bbe_0_srcs: (Optional) The BBE source(s) that contain the code to be tested
        bbe_0_deps: (Optional) The BBE deps(s) for the code to be tested
        bbe_0_copts: (Optional) Additional compiler options for BBE core 0. Defaults to [].
        bbe_0_defines: (Optional) Preprocessor defines for BBE core 0. Defaults to [].
        bbe_0_linkopts: (Optional) Linker options for BBE core 0 binary. Defaults to [].
        bbe_0_hdrs: (Optional) Header files for BBE core 0 to expose via `hdrs` on the generated cc_library. Defaults to [].
        bbe_1_srcs: (Optional) The BBE source(s) that contain the code to be tested
        bbe_1_deps: (Optional) The BBE deps(s) for the code to be tested
        bbe_1_copts: (Optional) Additional compiler options for BBE core 1. Defaults to [].
        bbe_1_defines: (Optional) Preprocessor defines for BBE core 1. Defaults to [].
        bbe_1_linkopts: (Optional) Linker options for BBE core 1 binary. Defaults to [].
        bbe_1_hdrs: (Optional) Header files for BBE core 1 to expose via `hdrs` on the generated cc_library. Defaults to [].
    """
    # ====================================================================
    # Argument Preparation
    # ====================================================================

    # Assign defaults if no BBE sources are specified
    bbe_0_sources = bbe_0_srcs if bbe_0_srcs else ["//internal/evb_diag_tool/testtypes/profile:src/bbe_default_scenario.c"]
    bbe_1_sources = bbe_1_srcs if bbe_1_srcs else ["//internal/evb_diag_tool/testtypes/profile:src/bbe_default_scenario.c"]

    # ====================================================================
    # Code
    # ====================================================================

    # Build user header file into a library so it can be included in the build deps to provide the proper include path.
    # Strip any directory prefix so callers can #include it directly.
    # Expect a list of header strings.
    m7_profile_defs_list = m7_profile_defs_h

    if len(m7_profile_defs_list) > 0 and "/" in m7_profile_defs_list[0]:
        strip_prefix = m7_profile_defs_list[0][:m7_profile_defs_list[0].rfind("/")]
    else:
        strip_prefix = ""

    native.cc_library(
        name = name + "_m7_profile_defs_h_lib",
        hdrs = m7_profile_defs_list,
        strip_include_prefix = strip_prefix,
        include_prefix = "",
        visibility = ["//visibility:private"],
    )

    # Call the base macro
    evb_diag_tool_test(
        name = name,
        m7_hdrs = [
            "//internal/evb_diag_tool/testtypes/profile:m7_testtype_profile_info_h",
        ],
        m7_test_src = "//internal/evb_diag_tool/testtypes/profile:m7_testtype_profile_c",
        m7_deps = [
            "//internal/evb_diag_tool/testtypes/profile:m7_testtype_profile_info_h",
            ":" + name + "_m7_profile_defs_h_lib",
        ],
        m7_copts = m7_copts,
        m7_linkopts = m7_linkopts,
        spt_0_srcs = spt_0_srcs,
        spt_0_deps = spt_0_deps,
        spt_0_linkopts = spt_0_linkopts,
        spt_1_srcs = spt_1_srcs,
        spt_1_deps = spt_1_deps,
        spt_1_linkopts = spt_1_linkopts,
        bbe_0_src = "//internal/evb_diag_tool/testtypes/profile:src/main_bbe_wrapper.c",
        bbe_0_srcs = bbe_0_sources,
        bbe_0_deps = bbe_0_deps + ["//modules/helpers/imp:profiling_helpers_lib"],
        bbe_0_copts = bbe_0_copts,
        bbe_0_defines = bbe_0_defines,
        bbe_0_linkopts = bbe_0_linkopts,
        bbe_0_hdrs = bbe_0_hdrs,
        bbe_1_src = "//internal/evb_diag_tool/testtypes/profile:src/main_bbe_wrapper.c",
        bbe_1_srcs = bbe_1_sources,
        bbe_1_deps = bbe_1_deps + ["//modules/helpers/imp:profiling_helpers_lib"],
        bbe_1_copts = bbe_1_copts,
        bbe_1_defines = bbe_1_defines,
        bbe_1_linkopts = bbe_1_linkopts,
        bbe_1_hdrs = bbe_1_hdrs,
    )

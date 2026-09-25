"""
Macro to build a complete EVB Diagnostics Tool test suite for profiling SPT code.

This macro generates the build targets for SPT code, and M7 code,
including compilation, linking, image generation, and size analysis.
"""

load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("//internal/evb_diag_tool/base:evb_diag_tool_test.bzl", "evb_diag_tool_test")

def evb_diag_tool_testtype_profile_spt(
        name,
        m7_profile_spt_defs_h,
        m7_test_src = "",
        m7_copts = [],
        m7_deps_h = [],
        m7_srcs = [],
        spt_0_srcs = [],
        spt_1_srcs = [],
        spt_0_deps = [],
        spt_1_deps = []):
    """
    Build a complete EVB Diagnostics Tool test suite for profiling SPT kernels.

    Args:
        name: The base name for generated targets
        m7_profile_spt_defs_h: The header that defines the SPT kernel labels to be profiled and their test names
        m7_test_src: (Optional) M7 test source file. Defaults to //internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_c
        m7_copts: (Optional) Additional compiler options for M7 core. Defaults to [].
        spt_0_srcs: The SPT source(s) that contain the kernel labels to be tested
        spt_0_deps: (Optional) Additional dependencies for SPT core 0 assembly/include paths. Defaults to [].
        spt_1_deps: (Optional) Additional dependencies for SPT core 1 assembly/include paths. Defaults to [].
    """
    # ====================================================================
    # Code
    # ====================================================================

    # Build user header file into a library so it can be included in the build deps to provide the proper include path.
    # Strip any directory prefix so callers can #include it directly.
    if "/" in m7_profile_spt_defs_h:
        strip_prefix = m7_profile_spt_defs_h[:m7_profile_spt_defs_h.rfind("/")]
    else:
        strip_prefix = ""
    cc_library(
        name = name + "_m7_profile_spt_defs_h_lib",
        hdrs = [m7_profile_spt_defs_h],
        strip_include_prefix = strip_prefix,
        include_prefix = "",
        visibility = ["//visibility:private"],
    )

    # Normalize optional extra deps (e.g. memory definition helpers) to a list
    # so callers can pass either a single label string or a list of labels.
    if type(m7_deps_h) == "string":
        m7_deps_h = [m7_deps_h] if m7_deps_h else []

    # Use the package default test loop source if caller does not provide one.
    if not m7_test_src:
        m7_test_src = "//internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_c"

    evb_diag_tool_test(
        name = name,
        m7_hdrs = [
            "//internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_info_h",
            m7_profile_spt_defs_h,
        ],
        m7_test_src = m7_test_src,  # The actual test file that loops through the tests
        m7_copts = m7_copts,
        m7_srcs = m7_srcs,
        m7_deps = [
            "//internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_info_h",
            ":" + name + "_m7_profile_spt_defs_h_lib",
        ] + m7_deps_h,
        spt_0_srcs = spt_0_srcs,
        spt_0_deps = spt_0_deps,
        spt_1_srcs = spt_1_srcs,
        spt_1_deps = spt_1_deps,
    )

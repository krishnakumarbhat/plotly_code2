"""
Macro to build a complete EVB Diagnostics Tool test suite for profiling SPT code.

This macro generates the build targets for SPT code, and M7 code,
including compilation, linking, image generation, and size analysis.
"""

load("//internal/evb_diag_tool/base:evb_diag_tool_test.bzl", "evb_diag_tool_test")

def evb_diag_tool_testtype_profile_spt(
        name,
        m7_profile_spt_defs_h,
        spt_0_srcs = []):
    """
    Build a complete EVB Diagnostics Tool test suite for profiling SPT kernels.

    Args:
        name: The base name for generated targets
        m7_profile_spt_defs_h: The header that defines the SPT kernel labels to be profiled and their test names
        spt_0_srcs: The SPT source(s) that contain the kernel labels to be tested
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

    native.cc_library(
        name = name + "_m7_profile_spt_defs_h_lib",
        hdrs = [m7_profile_spt_defs_h],
        strip_include_prefix = strip_prefix,
        include_prefix = "",
        visibility = ["//visibility:private"],
    )

    evb_diag_tool_test(
        name = name,
        m7_hdrs = [
            "//internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_info_h",
            m7_profile_spt_defs_h,
        ],
        m7_test_src = "//internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_c",  # The actual test file that loops through the tests
        m7_deps = [
            "//internal/evb_diag_tool/testtypes/profile_spt:m7_testtype_profile_spt_info_h",
            ":" + name + "_m7_profile_spt_defs_h_lib",
        ],
        spt_0_srcs = spt_0_srcs,
    )

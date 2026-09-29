package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Customer_Adapter",
    srcs = [
        ":base",
        "//TA/Customer_Adapter/BMW_SP25",
    ],
)

cc_library(
    includes = ["."],
    name = "base",
    srcs = [
        "ta_post_run.h",
        "ta_pre_run.h",
    ],
    hdrs = [
        "ta_post_run.h",
        "ta_pre_run.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//Mock_Files/SIL_Library",
        "//TA:core",
        "//TA/Calibration:TA_Core",
        "//TA/Customer_Adapter/BMW_SP25:input_output",
    ],
)

cc_library(
    includes = ["."],
    name = "boundary_checks",
    srcs = [
        "ta_input_boundary_check.h",
        "ta_output_boundary_check.h",
    ],
    hdrs = [
        "ta_input_boundary_check.h",
        "ta_output_boundary_check.h",
    ],
    strip_include_prefix = "",
    deps = [
    ],
)

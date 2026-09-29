package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Customer_Adapter",
    srcs = [
        ":base",
        "//LTB/Customer_Adapter/BMW_SP25",
    ],
)

cc_library(
    includes = ["."],
    name = "base",
    srcs = [
        "ltb_post_run.h",
        "ltb_pre_run.h",
    ],
    hdrs = [
        "ltb_post_run.h",
        "ltb_pre_run.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//LTB:core",
        "//LTB/Calibration:LTB_Core",
        "//LTB/Customer_Adapter/BMW_SP25:input_output",
    ],
)

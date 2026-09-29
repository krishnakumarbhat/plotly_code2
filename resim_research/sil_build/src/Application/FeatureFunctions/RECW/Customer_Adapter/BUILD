package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Customer_Adapter",
    srcs = [
        ":base",
        "//RECW/Customer_Adapter/BMW_SP25",
    ],
)

cc_library(
    includes = ["."],
    name = "base",
    srcs = [
        "recw_post_run.h",
        "recw_pre_run.h",
    ],
    hdrs = [
        "recw_post_run.h",
        "recw_pre_run.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//RECW:main",
        "//RECW/Calibration:RECW_Core",
        "//RECW/Customer_Adapter/BMW_SP25:input_output",
    ],
)

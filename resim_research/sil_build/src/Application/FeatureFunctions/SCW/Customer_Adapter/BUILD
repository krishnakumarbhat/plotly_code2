package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Customer_Adapter",
    srcs = [
        ":base",
        "//SCW/Customer_Adapter/BMW_SP25",
    ],
)

cc_library(
    includes = ["."],
    name = "base",
    srcs = [
        "scw_post_run.h",
        "scw_pre_run.h",
    ],
    hdrs = [
        "scw_post_run.h",
        "scw_pre_run.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//SCW:core",
        "//SCW/Calibration:SCW_Core",
        "//SCW/Customer_Adapter/BMW_SP25:input",
		"//SCW/Customer_Adapter/BMW_SP25:output",
    ],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "fast_math_h",
    srcs = [],
    hdrs = [
        "Math_Selector.h",
        "fastMath.h",
        "fastMathTables.h",
    ],
    defines = [],
    strip_include_prefix = "//emb_tracker/FastMath",
    visibility = ["//visibility:public"],
    deps = [
        "@reuse",
    ],
)

cc_library(
    name = "fast_math_lib",
    srcs = [
        "fastMath.c",
        "fastMathTables.c",
    ],
    hdrs = [],
    defines = [],
    visibility = ["//visibility:public"],
    deps = [
        ":fast_math_h",
        "@reuse",
    ],
)

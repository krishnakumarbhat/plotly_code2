load("@rules_cc//cc:defs.bzl", "cc_library")
cc_library(
    name = "OLP_Core_Lib",
    srcs = [
        "olp.cpp",
        "olp_compute_cog.cpp",
        "olp_compute_curvi_data.cpp",
        "olp_distance_based_curvature.cpp",
        "olp_iface.cpp",
        "olp_ml_math.cpp",
        "olp_snail_trail_computation.cpp",
        "olp_calibration.cpp",
    ],
    hdrs = [],
    defines = ["OLP_NUMBER_OF_OBJECTS=96"],
    deps = [
            "//OLP_Core/includes:OLP_Core_h",
            "//ML_MathLibrary/ml_core:ml_core",
           ],
    visibility = ["//visibility:public"],
)
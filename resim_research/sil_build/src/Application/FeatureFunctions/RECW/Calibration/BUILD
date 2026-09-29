load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Calibration",
    srcs = [
        ":BMW_SP25",
        ":RECW_Core",
        ":Generic"
    ],
)

cc_library(
    includes = ["."],
    name = "BMW_SP25",
    srcs = [
        "BMW_SP25/recw_calibration_t.c",
    ],
    hdrs = [
        "BMW_SP25/recw_calibration_t.c",
    ],
    strip_include_prefix = "BMW_SP25",
    deps = [
        "//RECW/Calibration:RECW_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "RECW_Core",
    srcs = [
        "RECW_Core/recw_calibration_t.h",
        "RECW_Core/recw_calibration_t_print_functions.c",
    ],
    hdrs = [
        "RECW_Core/recw_calibration_t.h",
    ],
    strip_include_prefix = "RECW_Core",
    deps = [
        "//Calibration_Tool:c_src",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
    ],
)

cc_library(
    includes = ["."],
    name = "Generic",
    srcs = [
        "Generic/recw_calibration_t.c",
    ],
    hdrs = [
        "Generic/recw_calibration_t.c",
    ],
    strip_include_prefix = "Generic",
    deps = [
        "//RECW/Calibration:RECW_Core",
    ],
)

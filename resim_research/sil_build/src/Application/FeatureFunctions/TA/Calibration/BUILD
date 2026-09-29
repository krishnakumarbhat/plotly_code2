load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Calibration",
    srcs = [
        ":BMW_SP25",
        ":TA_Core",
        ":Generic"
    ],
)

cc_library(
    includes = ["."],
    name = "BMW_SP25",
    srcs = [
        "BMW_SP25/ta_calibration_t.c",
    ],
    hdrs = [
        "BMW_SP25/ta_calibration_t.c",
    ],
    strip_include_prefix = "BMW_SP25",
    deps = [
        "//TA/Calibration:TA_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "TA_Core",
    srcs = [
        "TA_Core/ta_calibration_boundary_check.c",
        "TA_Core/ta_calibration_boundary_check.h",
        "TA_Core/ta_calibration_t.h",
        "TA_Core/ta_calibration_t_print_functions.c",
    ],
    hdrs = [
        "TA_Core/ta_calibration_boundary_check.c",
        "TA_Core/ta_calibration_boundary_check.h",
        "TA_Core/ta_calibration_t.h",
        "TA_Core/ta_calibration_t_print_functions.c",
    ],
    strip_include_prefix = "TA_Core",
    deps = [
        "//Calibration_Tool:c_src",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
    ],
)

cc_library(
    includes = ["."],
    name = "Generic",
    srcs = [
        "Generic/ta_calibration_t.c",
    ],
    hdrs = [
        "Generic/ta_calibration_t.c",
    ],
    strip_include_prefix = "Generic",
    deps = [
        "//TA/Calibration:TA_Core",
    ],
)

load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Calibration",
    srcs = [
        ":BMW_SP25",
        ":SCW_Core",
        ":Generic"
    ],
)

cc_library(
    includes = ["."],
    name = "BMW_SP25",
    srcs = [
        "BMW_SP25/scw_calibration_t.c",
    ],
    hdrs = [
        "BMW_SP25/scw_calibration_t.c",
    ],
    strip_include_prefix = "BMW_SP25",
    deps = [
        "//SCW/Calibration:SCW_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "SCW_Core",
    srcs = [
        "SCW_Core/scw_calibration_t.h",
        "SCW_Core/scw_calibration_t_print_functions.c",
    ],
    hdrs = [
        "SCW_Core/scw_calibration_t.h",
        "SCW_Core/scw_calibration_t_print_functions.c",
    ],
    strip_include_prefix = "SCW_Core",
    deps = [
        "//Calibration_Tool:c_src",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
    ],
)

cc_library(
    includes = ["."],
    name = "Generic",
    srcs = [
        "Generic/scw_calibration_t.c",
    ],
    hdrs = [
        "Generic/scw_calibration_t.c",
    ],
    strip_include_prefix = "Generic",
    deps = [
        "//SCW/Calibration:SCW_Core",
    ],
)

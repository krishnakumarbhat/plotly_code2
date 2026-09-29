load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "Calibration",
    srcs = [
        ":BMW_SP25",
        ":LTB_Core",
        ":Generic"
    ],
)

cc_library(
    includes = ["."],
    name = "BMW_SP25",
    srcs = [
        "BMW_SP25/ltb_calibration_t.c",
    ],
    hdrs = [
        "BMW_SP25/ltb_calibration_t.c",
    ],
    strip_include_prefix = "BMW_SP25",
    deps = [
        "//LTB/Calibration:LTB_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "LTB_Core",
    srcs = [
        "LTB_Core/ltb_calibration_t.h",
        "LTB_Core/ltb_calibration_t_print_functions.c",
    ],
    hdrs = [
        "LTB_Core/ltb_calibration_t.h",
    ],
    strip_include_prefix = "LTB_Core",
    deps = [
        "//Calibration_Tool:c_src",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
    ],
)

cc_library(
    includes = ["."],
    name = "Generic",
    srcs = [
        "Generic/ltb_calibration_t.c",
    ],
    hdrs = [
        "Generic/ltb_calibration_t.c",
    ],
    strip_include_prefix = "Generic",
    deps = [
        "//LTB/Calibration:LTB_Core",
    ],
)

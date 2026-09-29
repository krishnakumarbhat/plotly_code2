package(default_visibility = ["//visibility:public"])

genrule(
    name = "RECW",
    srcs = ["//:merge_libs.sh",
			":RECW_group",
			],
    outs = ["libRECW.a"],
	cmd = "$(location //:merge_libs.sh) '$(locations :RECW_group)' $(location libRECW.a)",
)

filegroup(
    name = "RECW_group",
    srcs = [
        ":main",
        ":core",
        ":iface",
        ":types",
        "//RECW/Customer_Adapter",
        "//RECW/Calibration"
    ],
)

cc_library(
    includes = ["."],
    name = "main",
    srcs = [
        "Source/recw.c",
        "Source/recw.h",
        "Source/recw_car_wash_detection.c",
        "Source/recw_car_wash_detection.h",
        "Source/recw_crash_prob_steer_brake.c",
        "Source/recw_crash_prob_steer_brake.h",
        "Source/recw_debug_interface.c",
        "Source/recw_debug_interface.h",
        "Source/recw_debug_writer.c",
        "Source/recw_debug_writer.h",
        "Source/recw_object_validator.c",
        "Source/recw_object_validator.h",
        "Source/recw_output_debouncer.c",
        "Source/recw_output_debouncer.h",
        "Source/recw_persistent_t.h",
    ],
    hdrs = [
        "Source/recw.c",
        "Source/recw.h",
        "Source/recw_car_wash_detection.c",
        "Source/recw_car_wash_detection.h",
        "Source/recw_crash_prob_steer_brake.c",
        "Source/recw_crash_prob_steer_brake.h",
        "Source/recw_debug_interface.h",
        "Source/recw_debug_writer.h",
        "Source/recw_object_validator.c",
        "Source/recw_object_validator.h",
        "Source/recw_output_debouncer.c",
        "Source/recw_output_debouncer.h",
        "Source/recw_persistent_t.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":core",
        ":types",
        "//RECW/Calibration:BMW_SP25",
        "//RECW/Calibration:RECW_Core",
        "//RECW/Customer_Adapter/BMW_SP25:input_output",
        "//Feature_Building_Kit/Interface:fbk_iface",
        "//Feature_Building_Kit/Shared_Feature_Functions:reference_point",
        "//Feature_Building_Kit/Shared_Feature_Functions:validation",
        "//Feature_Building_Kit/Shared_Feature_Functions:zone",
    ],
)

cc_library(
    includes = ["."],
    name = "core",
    srcs = [
        "Source/recw_core_input_t.h",
        "Source/recw_core_output_t.h",
    ],
    hdrs = [
        "Source/recw_core_input_t.h",
        "Source/recw_core_output_t.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":types",
    ],
)

cc_library(
    includes = ["."],
    name = "iface",
    srcs = [
        "Source/recw_iface.c",
        "Source/recw_iface.h",
    ],
    hdrs = [
        "Source/recw_iface.c",
        "Source/recw_iface.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":main",
        # "//CED/Calibration:BMW_SP25",
        "//RECW/Calibration:RECW_Core",
        "//RECW/Customer_Adapter/BMW_SP25:pre_post",
    ],
)

cc_library(
    includes = ["."],
    name = "types",
    srcs = [
        "Source/recw_types.h",
    ],
    hdrs = [
        "Source/recw_types.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
    ],
)

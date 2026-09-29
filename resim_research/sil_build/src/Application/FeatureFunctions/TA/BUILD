package(default_visibility = ["//visibility:public"])

genrule(
    name = "TA",
    srcs = ["//:merge_libs.sh",
			":TA_group",
			],
    outs = ["libTA.a"],
	cmd = "$(location //:merge_libs.sh) '$(locations :TA_group)' $(location libTA.a)",
)

filegroup(
    name = "TA_group",
    srcs = [
        ":core",
        ":debug",
        ":iface",
        ":main",
        ":side",
        ":types",
        "//TA/Customer_Adapter",
        "//TA/Calibration"
    ],
)

cc_library(
    includes = ["."],
    name = "main",
    srcs = [
        "Source/ta.c",
        "Source/ta.h",
    ],
    hdrs = [
        "Source/ta.c",
        "Source/ta.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":core",
        ":debug",
        ":side",
        "//TA/Calibration:TA_Core",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
		"//Feature_Building_Kit/Shared_Feature_Functions:trajectory_prediction",
    ],
)

cc_library(
    includes = ["."],
    name = "side",
    srcs = [
        "Source/ta_collision_filter.c",
        "Source/ta_collision_filter.h",
        "Source/ta_common_functions.c",
        "Source/ta_common_functions.h",
        "Source/ta_constants.c",
        "Source/ta_constants.h",
        "Source/ta_factory.c",
        "Source/ta_factory.h",
        "Source/ta_intersection_analyzer.c",
        "Source/ta_intersection_analyzer.h",
        "Source/ta_object_filter.c",
        "Source/ta_object_filter.h",
        "Source/ta_persistent_t.h",
        "Source/ta_warn_logic.c",
        "Source/ta_warn_logic.h",
    ],
    hdrs = [
        "Source/ta_collision_filter.c",
        "Source/ta_collision_filter.h",
        "Source/ta_common_functions.c",
        "Source/ta_common_functions.h",
        "Source/ta_constants.h",
        "Source/ta_factory.c",
        "Source/ta_factory.h",
        "Source/ta_intersection_analyzer.c",
        "Source/ta_intersection_analyzer.h",
        "Source/ta_object_filter.c",
        "Source/ta_object_filter.h",
        "Source/ta_persistent_t.h",
        "Source/ta_warn_logic.c",
        "Source/ta_warn_logic.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":core",
        ":types",
        "//TA/Calibration:TA_Core",
        "//TA/Customer_Adapter:base",
        "//Feature_Building_Kit/Interface:fbk_iface",
    ],
)

cc_library(
    includes = ["."],
    name = "core",
    srcs = [
        "Source/ta_core_input_t.h",
        "Source/ta_core_output_t.h",
    ],
    hdrs = [
        "Source/ta_core_input_t.h",
        "Source/ta_core_output_t.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":types",
        "//Feature_Building_Kit/Shared_Feature_Functions:zone",
    ],
)

cc_library(
    includes = ["."],
    name = "debug",
    srcs = [
        "Source/ta_debug_interface.c",
        "Source/ta_debug_interface.h",
        "Source/ta_debug_writer.c",
        "Source/ta_debug_writer.h",
    ],
    hdrs = [
        "Source/ta_debug_interface.h",
        "Source/ta_debug_writer.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":core",
        ":side",
        ":types",
        "//TA/Calibration:TA_Core",
        "//TA/Customer_Adapter/BMW_SP25:input_output",
    ],
)

cc_library(
    includes = ["."],
    name = "iface",
    srcs = [
        "Source/ta_iface.c",
        "Source/ta_iface.h",
    ],
    hdrs = [
        "Source/ta_iface.c",
        "Source/ta_iface.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":main",
        "//TA/Calibration:TA_Core",
        "//TA/Customer_Adapter/BMW_SP25:pre_post",
    ],
)

cc_library(
    includes = ["."],
    name = "types",
    srcs = [
        "Source/ta_types.h",
    ],
    hdrs = [
        "Source/ta_types.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        "//Feature_Building_Kit/Shared_Feature_Functions:reference_point",
        "//Feature_Building_Kit/Shared_Feature_Functions:validation",
		"//Feature_Building_Kit/Shared_Feature_Functions:trajectory_prediction",
    ],
)

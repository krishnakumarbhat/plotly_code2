package(default_visibility = ["//visibility:public"])

filegroup(
    name = "BMW_SP25",
    srcs = [
        ":debug",
        ":input_output",
        ":main",
        ":pre_post",
        ":types",
        ":state_machine",
    ],
)

cc_library(
    includes = ["."],
    name = "main",
    srcs = [
        "ta_bmw_diagnostic.c",
        "ta_bmw_diagnostic.h",
    ],
    hdrs = [
        "ta_bmw_diagnostic.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":input_output",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
        "//TA/Calibration:TA_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "boardnet",
    srcs = [
        "ta_bmw_boardnet_t.h",
    ],
    hdrs = [
        "ta_bmw_boardnet_t.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":enums",
        # "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        # "//Feature_Building_Kit/Shared_Feature_Functions:main",
        # "//TA/Calibration:TA_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "types",
    srcs = [
        "ta_bmw_sp25_types.h",
    ],
    hdrs = [
        "ta_bmw_sp25_types.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//TA:types",
    ]
)

cc_library(
    includes = ["."],
    name = "enums",
    srcs = [
        "ta_bmw_enums.h",
    ],
    hdrs = [
        "ta_bmw_enums.h",
    ],
    strip_include_prefix = "",
)

cc_library(
    includes = ["."],
    name = "state_machine",
    srcs = [
        "ta_state_machine.c",
    ],
    hdrs = [
        "ta_state_machine.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":types",
        ":input_output"
    ]
)

cc_library(
    includes = ["."],
    name = "debug",
    srcs = [
        "ta_bmw_sp25_debug_interface.c",
        "ta_bmw_sp25_debug_interface.h",
        "ta_bmw_sp25_debug_writer.c",
        "ta_bmw_sp25_debug_writer.h",
    ],
    hdrs = [
        "ta_bmw_sp25_debug_interface.h",
        "ta_bmw_sp25_debug_writer.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//TA:debug",
        "//TA:types",
    ]
)

cc_library(
    includes = ["."],
    name = "input_output",
    srcs = [
        "ta_input_t.h",
        "ta_input_boundary_check.c",
        "ta_output_t.h",
        "ta_output_boundary_check.c",
    ],
    hdrs = [
        "ta_input_t.h",
        "ta_output_t.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":boardnet",
        ":types",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
        "//TA/Customer_Adapter:boundary_checks",
    ],
)

cc_library(
    includes = ["."],
    name = "pre_post",
    srcs = [
        "ta_post_run.c",
        "ta_pre_run.c",
    ],
    strip_include_prefix = "",
    deps = [
        ":main",
        ":debug",
        "//TA:types",
        ":state_machine",
        "//TA/Customer_Adapter:base",
        "//Feature_Building_Kit/Platform_Abstraction_Layer:pa_selector",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
        "//Feature_Building_Kit/Shared_Feature_Functions:validation",
    ],
)

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "BMW_SP25",
    srcs = [
        ":input_output",
        ":main",
        ":pre_post",
        ":types",
    ],
)


cc_library(
    includes = ["."],
    name = "main",
    srcs = [
		"ltb_state_machine.c",
		"ltb_state_machine.h",
    ],
    hdrs = [
		"ltb_state_machine.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":input_output",
        ":types",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
        "//Feature_Building_Kit/Shared_Feature_Functions:validation",
        "//Feature_Building_Kit/Shared_Feature_Functions:trajectory_prediction",
		"//LTB/Calibration:LTB_Core",
    ],
)

cc_library(
    includes = ["."],
    name = "types",
    srcs = [
        "ltb_bmw_sp25_types.h",
    ],
    hdrs = [
        "ltb_bmw_sp25_types.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//LTB:core",
    ]
)

cc_library(
    includes = ["."],
    name = "input_output",
    srcs = [
        "ltb_input_t.h",
        "ltb_output_t.h",
    ],
    hdrs = [
        "ltb_input_t.h",
        "ltb_output_t.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":types",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
    ],
)

cc_library(
    includes = ["."],
    name = "pre_post",
    srcs = [
        "ltb_post_run.c",
        "ltb_pre_run.c",
    ],
    strip_include_prefix = "",
    deps = [
        ":types",
        ":main",
        "//LTB:core",
        "//LTB/Customer_Adapter:base",
        "//Feature_Building_Kit/Platform_Abstraction_Layer:pa_selector"
    ],
)

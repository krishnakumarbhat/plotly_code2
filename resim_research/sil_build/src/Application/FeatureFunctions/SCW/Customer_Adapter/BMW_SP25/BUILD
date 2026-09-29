package(default_visibility = ["//visibility:public"])

filegroup(
    name = "BMW_SP25",
    srcs = [
        ":input",
		":output",
        ":pre_post",
		":state_machine",
    ],
)

cc_library(
    includes = ["."],
    name = "state_machine",
    srcs = [
		"scw_state_machine.c",
		"scw_state_machine.h",
    ],
    hdrs = [
		"scw_state_machine.h",
    ],
    strip_include_prefix = "",
    deps = [
		":input",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
    ],
)

cc_library(
    includes = ["."],
    name = "input",
    srcs = [
        "scw_input_t.h",
    ],
    hdrs = [
        "scw_input_t.h",
    ],
    strip_include_prefix = "",
	deps = [
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
    ],
)

cc_library(
    includes = ["."],
    name = "output",
    srcs = [
        "scw_output_t.h",
    ],
    hdrs = [
        "scw_output_t.h",
    ],
    strip_include_prefix = "",
	deps = [
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
		":state_machine",
    ],
)

cc_library(
    includes = ["."],
    name = "pre_post",
    srcs = [
        "scw_post_run.c",
        "scw_pre_run.c",
    ],
    strip_include_prefix = "",
    deps = [
        "//SCW/Customer_Adapter:base",
		"//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Platform_Abstraction_Layer:pa_selector",
    ],
)

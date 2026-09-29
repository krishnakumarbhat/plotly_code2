package(default_visibility = ["//visibility:public"])

filegroup(
    name = "BMW_SP25",
    srcs = [
        ":input_output",
        ":pre_post",
        ":types",
        ":state_machine",
    ],
)

cc_library(
    includes = ["."],
    name = "state_machine",
    srcs = [
        "recw_state_machine.c",
        "recw_state_machine.h",
    ],
    hdrs = [
        "recw_state_machine.h",
    ],
    strip_include_prefix = "",
    deps = [
        ":types",
        "//RECW:core",
        ":input_output",
    ],
)

cc_library(
    includes = ["."],
    name = "types",
    srcs = [
        "recw_bmw_sp25_types.h",
    ],
    hdrs = [
        "recw_bmw_sp25_types.h",
    ],
    strip_include_prefix = "",
)

cc_library(
    includes = ["."],
    name = "input_output",
    srcs = [
        "recw_input_t.h",
        "recw_output_t.h",
    ],
    hdrs = [
        "recw_input_t.h",
        "recw_output_t.h",
    ],
    strip_include_prefix = "",
    deps = [
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic",
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        ":types",
    ],
)

cc_library(
    includes = ["."],
    name = "pre_post",
    srcs = [
        "recw_post_run.c",
        "recw_pre_run.c",
    ],
    strip_include_prefix = "",
    deps = [
        ":types",
        "//RECW/Customer_Adapter:base",
        "//Feature_Building_Kit/Platform_Abstraction_Layer:pa_selector",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
        ":state_machine",
    ],
)

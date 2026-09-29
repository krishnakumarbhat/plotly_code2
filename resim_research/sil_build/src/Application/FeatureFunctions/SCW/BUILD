package(default_visibility = ["//visibility:public"])

genrule(
    name = "SCW",
    srcs = ["//:merge_libs.sh",
			":SCW_group",
			],
    outs = ["libSCW.a"],
	cmd = "$(location //:merge_libs.sh) '$(locations :SCW_group)' $(location libSCW.a)",
)

filegroup(
    name = "SCW_group",
    srcs = [
        ":core",
        ":iface",
        ":main",
        ":types",
        "//SCW/Customer_Adapter",
        "//SCW/Calibration"
    ],
)

cc_library(
    includes = ["."],
    name = "main",
    srcs = [
        "Source/scw.c",
        "Source/scw.h",
        "Source/scw_debug_interface.c",
        "Source/scw_debug_interface.h",
        "Source/scw_debug_writer.c",
        "Source/scw_debug_writer.h",
        "Source/scw_helper_functions.c",
        "Source/scw_helper_functions.h",
        "Source/scw_persistent_t.h",
    ],
    hdrs = [
        "Source/scw.c",
        "Source/scw.h",
        "Source/scw_debug_interface.c",
        "Source/scw_debug_interface.h",
        "Source/scw_debug_writer.c",
        "Source/scw_debug_writer.h",
        "Source/scw_helper_functions.c",
        "Source/scw_helper_functions.h",
        "Source/scw_persistent_t.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":core",
        ":types",
        "//SCW/Calibration:SCW_Core",
        "//SCW/Customer_Adapter:base",
        "//Feature_Building_Kit/Interface:fbk_iface",
    ],
)

cc_library(
    includes = ["."],
    name = "core",
    srcs = [
        "Source/scw_core_input_t.h",
        "Source/scw_core_output_t.h",
    ],
    hdrs = [
        "Source/scw_core_input_t.h",
        "Source/scw_core_output_t.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":types",
        "//Feature_Building_Kit/Shared_Feature_Functions:zone",
    ],
)

cc_library(
    includes = ["."],
    name = "iface",
    srcs = [
        "Source/scw_iface.c",
        "Source/scw_iface.h",
    ],
    hdrs = [
        "Source/scw_iface.c",
        "Source/scw_iface.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        ":main",
        "//SCW/Customer_Adapter/BMW_SP25:pre_post",
    ],
)

cc_library(
    includes = ["."],
    name = "types",
    srcs = [
        "Source/scw_types.h",
    ],
    hdrs = [
        "Source/scw_types.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        "//Feature_Building_Kit/Shared_Feature_Functions:reference_point",
        "//Feature_Building_Kit/Shared_Feature_Functions:validation",
    ],
)

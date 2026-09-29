package(default_visibility = ["//visibility:public"])

genrule(
    name = "LTB",
    srcs = ["//:merge_libs.sh",
			":LTB_group",
			],
    outs = ["libLTB.a"],
	cmd = "$(location //:merge_libs.sh) '$(locations :LTB_group)' $(location libLTB.a)",
)

filegroup(
    name = "LTB_group",
    srcs = [
        ":core",
        ":main",
        "//LTB/Customer_Adapter",
        "//LTB/Calibration"
        
    ],
)

cc_library(
    includes = ["."],
    name = "main",
    srcs = glob(["Source/*.c"]),
    hdrs = glob(["Source/*.h"]),
    strip_include_prefix = "Source",
    deps = [
        ":core",
        "//LTB/Calibration:BMW_SP25",
        "//LTB/Calibration:LTB_Core",
        "//LTB/Customer_Adapter:base",
        "//LTB/Customer_Adapter/BMW_SP25:main",
        "//Feature_Building_Kit/Interface:fbk_iface",
        "//Feature_Building_Kit/Shared_Feature_Functions:interpolation",
        "//Feature_Building_Kit/Shared_Feature_Functions:validation",
        "//Feature_Building_Kit/Shared_Feature_Functions:zone",
    ],
)

cc_library(
    includes = ["."],
    name = "core",
    srcs = [
        "Source/ltb_core_input_t.h",
        "Source/ltb_core_output_t.h",
        "Source/ltb_types.h",
    ],
    hdrs = [
        "Source/ltb_core_input_t.h",
        "Source/ltb_core_output_t.h",
        "Source/ltb_types.h",
    ],
    strip_include_prefix = "Source",
    deps = [
        "//Feature_Building_Kit/Shared_Feature_Functions:zone",
        "//Feature_Building_Kit/Shared_Feature_Functions:main",
        "//Feature_Building_Kit/Shared_Feature_Functions:reference_point",
    ],
)
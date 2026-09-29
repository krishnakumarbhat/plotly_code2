package(default_visibility = ["//visibility:public"])

cc_test(
    name = "SCW_UT",
    srcs = [
        "Source/scw_calibration_test.cpp",
        "Source/scw_helper_functions_test.cpp",
        "Source/scw_iface_test.cpp",
        "Source/scw_test.cpp",
        "main.cpp",
    ],
    deps = [
        ":Helper_Functions",
        ":Test_Classes",
        "//SCW:iface",
        "//SCW:main",
        "//SCW/Calibration:BMW_SP25",
        "//SCW/Calibration:SCW_Core",
        "//SCW/Customer_Adapter:base",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    includes = ["."],
    name = "Helper_Functions",
    srcs = [
        "Helper_Functions/scw_input_generator.c",
        "Helper_Functions/scw_input_generator.h",
    ],
    hdrs = [
        "Helper_Functions/scw_input_generator.h",
    ],
    strip_include_prefix = "Helper_Functions",
    deps = [
        "//Feature_Building_Kit/Platform_Abstraction_Layer/Generic:data_ports",
        "//SCW:types",
    ]
)

cc_library(
    includes = ["."],
    name = "Test_Classes",
    srcs = [
        "Test_Classes/scw_calibration_test.hpp",
        "Test_Classes/scw_helper_functions_test.hpp",
        "Test_Classes/scw_iface_test.hpp",
        "Test_Classes/scw_test.hpp",
    ],
    hdrs = [
        "Test_Classes/scw_calibration_test.hpp",
        "Test_Classes/scw_helper_functions_test.hpp",
        "Test_Classes/scw_iface_test.hpp",
        "Test_Classes/scw_test.hpp",
    ],
    strip_include_prefix = "Test_Classes",
)

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "TA_UT",
    srcs = [
        "Source/ta_calibration_boundary_check_test.cpp",
        "Source/ta_calibration_test.cpp",
        "Source/ta_collision_filter_test.cpp",
        "Source/ta_common_functions_test.cpp",
        "Source/ta_constants_test.cpp",
        "Source/ta_factory_test.cpp",
        "Source/ta_iface_test.cpp",
        "Source/ta_intersection_analyzer_test.cpp",
        "Source/ta_object_filter_test.cpp",
        "Source/ta_test.cpp",
        "Source/ta_warn_logic_test.cpp",
        "main.cpp",
    ],
    deps = [
        ":Test_Classes",
        "//TA:iface",
        "//TA:side",
        "//TA:main",
        "//TA/Calibration:BMW_SP25",
        "//TA/Calibration:TA_Core",
        "@com_google_googletest//:gtest",
    ],
)


cc_library(
    includes = ["."],
    name = "Test_Classes",
    srcs = [
        "Test_Classes/ta_calibration_boundary_check_test.hpp",
        "Test_Classes/ta_calibration_test.hpp",
        "Test_Classes/ta_collision_filter_test.hpp",
        "Test_Classes/ta_common_functions_test.hpp",
        "Test_Classes/ta_constants_test.hpp",
        "Test_Classes/ta_factory_test.hpp",
        "Test_Classes/ta_iface_test.hpp",
        "Test_Classes/ta_intersection_analyzer_test.hpp",
        "Test_Classes/ta_object_filter_test.hpp",
        "Test_Classes/ta_test.hpp",
        "Test_Classes/ta_warn_logic_test.hpp",
    ],
    hdrs = [
        "Test_Classes/ta_calibration_boundary_check_test.hpp",
        "Test_Classes/ta_calibration_test.hpp",
        "Test_Classes/ta_collision_filter_test.hpp",
        "Test_Classes/ta_common_functions_test.hpp",
        "Test_Classes/ta_constants_test.hpp",
        "Test_Classes/ta_factory_test.hpp",
        "Test_Classes/ta_iface_test.hpp",
        "Test_Classes/ta_intersection_analyzer_test.hpp",
        "Test_Classes/ta_object_filter_test.hpp",
        "Test_Classes/ta_test.hpp",
        "Test_Classes/ta_warn_logic_test.hpp",
    ],
    strip_include_prefix = "Test_Classes",
    deps = [
        "//TA:side",
        "//TA/Calibration:BMW_SP25",
        "//TA/Calibration:TA_Core",
        "//TA/Customer_Adapter/BMW_SP25:input_output",
    ]
)

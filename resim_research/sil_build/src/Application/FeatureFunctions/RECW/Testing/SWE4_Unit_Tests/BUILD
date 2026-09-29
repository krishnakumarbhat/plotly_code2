package(default_visibility = ["//visibility:public"])

cc_test(
    name = "RECW_UT",
    srcs = [
        "Source/recw_calibration_test.cpp",
        "Source/recw_car_wash_detection_test.cpp",
        "Source/recw_crash_prob_steer_brake_test.cpp",
        "Source/recw_iface_test.cpp",
        "Source/recw_object_validator_test.cpp",
        "Source/recw_output_debouncer_test.cpp",
        "Source/recw_test.cpp",
        "main.cpp",
    ],
    deps = [
        ":Test_Classes",
        "//RECW:iface",
        "//RECW:main",
        "//RECW/Calibration:BMW_SP25",
        "//RECW/Calibration:RECW_Core",
        "//RECW/Customer_Adapter:base",
        "@com_google_googletest//:gtest",
    ],
)


cc_library(
    includes = ["."],
    name = "Test_Classes",
    srcs = [
        "Test_Classes/recw_calibration_test.hpp",
        "Test_Classes/recw_car_wash_detection_test.hpp",
        "Test_Classes/recw_crash_prob_steer_brake_test.hpp",
        "Test_Classes/recw_iface_test.hpp",
        "Test_Classes/recw_object_validator_test.hpp",
        "Test_Classes/recw_output_debouncer_test.hpp",
        "Test_Classes/recw_test.hpp",
    ],
    hdrs = [
        "Test_Classes/recw_calibration_test.hpp",
        "Test_Classes/recw_car_wash_detection_test.hpp",
        "Test_Classes/recw_crash_prob_steer_brake_test.hpp",
        "Test_Classes/recw_iface_test.hpp",
        "Test_Classes/recw_object_validator_test.hpp",
        "Test_Classes/recw_output_debouncer_test.hpp",
        "Test_Classes/recw_test.hpp",
    ],
    strip_include_prefix = "Test_Classes",
)

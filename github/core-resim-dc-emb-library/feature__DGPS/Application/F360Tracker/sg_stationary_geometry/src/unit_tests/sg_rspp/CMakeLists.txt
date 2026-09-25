cmake_minimum_required(VERSION 3.16)

add_executable(sg_rspp_ut)

target_sources(
    sg_rspp_ut
      PRIVATE
        ut_rspp_calibrations.cpp
        ut_rspp_coordinate_transformation.cpp
        ut_rspp_get_version.cpp
        ut_rspp_input_validation.cpp
        ut_rspp_mark_detection_with_low_detection_conf.cpp
        ut_rspp_math_func.cpp
        ut_rspp_motion_status_classification.cpp
        ut_rspp_norm_heading_angle.cpp
        ut_rspp_range_rate_compensation.cpp
        ut_rspp_sensor_capability.cpp
        ut_rspp_state.cpp
        ut_rspp_uncertainty_calculation.cpp
        ut_rspp_vcs_long_sorted_dets_support_functions.cpp
        ut_rspp.cpp
)

target_link_libraries(
    sg_rspp_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Rspp
)

include(GoogleTest)
gtest_discover_tests(sg_rspp_ut)

cmake_minimum_required(VERSION 3.16)

add_executable(underdrivability_test)

target_sources(
    underdrivability_test
        PRIVATE
            ocg_assign_detections_to_underdrivability_zones_helpers_unittest.cpp
            ocg_assign_detections_to_underdrivability_zones_unittest.cpp
            ocg_assign_underdrivability_status_to_zones_unittest.cpp
            ocg_calc_underdrivability_circular_zone_idx_unittest.cpp
            ocg_calc_underdrivability_probabilities_helpers_unittest.cpp
            ocg_calc_underdrivability_probabilities_unittest.cpp
            ocg_initialize_underdrivability_unittest.cpp
            ocg_shift_circular_zones_unittest.cpp
            ocg_test_highly_critical_zones_unittest.cpp
            ocg_test_low_critical_zones_unittest.cpp
            ocg_test_med_critical_zones_unittest.cpp
            ocg_underdrivability_unittest.cpp
)

target_link_libraries(
    underdrivability_test
        PRIVATE occupancy_grid_iface
        PRIVATE occupancy_grid
        PRIVATE ocg_underdrivability
        PRIVATE ocg_common
        PRIVATE GTest::gtest_main
)

# Backdoor access to linked library private includes
# Only public iface includes are inherited through target_link_libraries(),
# but UTs require access to both private and public includes
get_target_property(OCG_UNDERDRIVABILITY_INCLUDES ocg_underdrivability INCLUDE_DIRECTORIES)

target_include_directories(
   underdrivability_test
      PRIVATE ${OCG_UNDERDRIVABILITY_INCLUDES}
)

include(GoogleTest)

gtest_discover_tests(underdrivability_test
    XML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/gtest_results/underdrivability_test
)

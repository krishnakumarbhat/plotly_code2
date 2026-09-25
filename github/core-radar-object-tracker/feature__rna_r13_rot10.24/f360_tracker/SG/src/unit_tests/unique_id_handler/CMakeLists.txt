cmake_minimum_required(VERSION 3.16)

add_executable(unique_id_handler_ut)

target_sources(
   unique_id_handler_ut
      PRIVATE
         ut_unique_id_handler.cpp
         ut_unique_id_handler_incremental.cpp
)

target_link_libraries(
   unique_id_handler_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
)

include(GoogleTest)
gtest_discover_tests(unique_id_handler_ut)

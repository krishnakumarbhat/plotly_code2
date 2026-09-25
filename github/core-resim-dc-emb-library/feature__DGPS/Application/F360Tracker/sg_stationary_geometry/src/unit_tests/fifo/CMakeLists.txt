cmake_minimum_required(VERSION 3.16)

add_executable(fifo_ut)

target_sources(
   fifo_ut
      PRIVATE
         ut_fifo.cpp
)

target_link_libraries(
  fifo_ut
    PRIVATE sg_ut_config
    PRIVATE SG-Common
    PRIVATE stationary_geometries_iface
)

include(GoogleTest)
gtest_discover_tests(fifo_ut)
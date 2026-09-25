cmake_minimum_required(VERSION 3.16)

add_executable(memory_pool_ut)

target_sources(
   memory_pool_ut
      PRIVATE
         ut_memory_pool.cpp
         ut_memory_pool_static.cpp
)

target_link_libraries(
   memory_pool_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
)

include(GoogleTest)
gtest_discover_tests(memory_pool_ut)

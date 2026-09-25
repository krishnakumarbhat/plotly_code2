cmake_minimum_required(VERSION 3.16)

add_executable(sg_stationary_geometries_ut)

target_sources(
   sg_stationary_geometries_ut
      PRIVATE
         ut_stationary_geometries.cpp
)

target_link_libraries(
  sg_stationary_geometries_ut
    PRIVATE sg_ut_config
    PRIVATE sg_ut_shared
    PRIVATE stationary_geometries
    PRIVATE "$<$<CXX_COMPILER_ID:Clang,GNU>:pthread>"
)

target_include_directories(
   sg_stationary_geometries_ut
      PRIVATE "$<TARGET_PROPERTY:stationary_geometries,INCLUDE_DIRECTORIES>"
)

include(GoogleTest)
gtest_discover_tests(sg_stationary_geometries_ut)
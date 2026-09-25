cmake_minimum_required(VERSION 3.16)

add_executable(sg_core_ut)

target_sources(
   sg_core_ut
      PRIVATE
         ut_sg_core.cpp
)

target_link_libraries(
  sg_core_ut
    PRIVATE sg_ut_config
    PRIVATE stationary_geometries
    PRIVATE "$<$<CXX_COMPILER_ID:Clang,GNU>:pthread>"
)

target_include_directories(
   sg_core_ut
      PRIVATE "$<TARGET_PROPERTY:stationary_geometries,INCLUDE_DIRECTORIES>"
)

include(GoogleTest)
gtest_discover_tests(sg_core_ut)

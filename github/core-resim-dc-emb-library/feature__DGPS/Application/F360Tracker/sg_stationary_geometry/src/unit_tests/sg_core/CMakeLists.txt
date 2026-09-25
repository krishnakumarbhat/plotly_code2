cmake_minimum_required(VERSION 3.16)

add_executable(sg_core_ut)

target_sources(
   sg_core_ut
      PRIVATE
         ut_sg_core.cpp
         ut_measurement_timestamp.cpp
         ut_sg_dump_output.cpp
         ut_sg_dump_reduced_output.cpp
         ut_sg_dump_internals.cpp
         ut_sg_input_diagnostics.cpp
         ut_sg_output_diagnostics.cpp

         $<$<BOOL:${SG_DC_ENABLED}>: ut_dc_dump_internals.cpp>
         $<$<BOOL:${SG_DC_ENABLED}>: ut_sg_core_with_dc.cpp>
)

target_link_libraries(
  sg_core_ut
    PRIVATE sg_ut_config
    PRIVATE stationary_geometries
    PRIVATE SG-Common
    PRIVATE "$<$<CXX_COMPILER_ID:Clang,GNU>:pthread>"
    PRIVATE $<$<BOOL:${SG_DC_ENABLED}>: DC-Drivability-Classification>
)

target_include_directories(
   sg_core_ut
      PRIVATE "$<TARGET_PROPERTY:stationary_geometries,INCLUDE_DIRECTORIES>"
)

include(GoogleTest)
gtest_discover_tests(sg_core_ut)
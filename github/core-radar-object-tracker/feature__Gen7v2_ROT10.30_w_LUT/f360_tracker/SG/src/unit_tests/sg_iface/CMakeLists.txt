cmake_minimum_required(VERSION 3.16)

add_executable(sg_iface_ut)

target_sources(
   sg_iface_ut
      PRIVATE
         ut_sg_iface.cpp
         ut_sg_dumping.cpp
)

target_link_libraries(
  sg_iface_ut
    PRIVATE sg_ut_config
    PRIVATE stationary_geometries
    PRIVATE stationary_geometries_iface
    PRIVATE DC-Drivability-Classification
    PRIVATE SG-Math
    PRIVATE timer_resim
)

include(GoogleTest)
gtest_discover_tests(sg_iface_ut)

target_include_directories(
   sg_iface_ut
      PRIVATE "$<TARGET_PROPERTY:stationary_geometries,INCLUDE_DIRECTORIES>"
)

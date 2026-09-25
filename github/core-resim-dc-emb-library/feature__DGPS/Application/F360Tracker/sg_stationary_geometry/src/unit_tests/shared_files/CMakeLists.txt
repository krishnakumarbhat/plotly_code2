cmake_minimum_required(VERSION 3.16)

add_library(sg_ut_shared INTERFACE)

target_sources(
   sg_ut_shared
      PRIVATE
         ut_shared.h
)

target_include_directories(
    sg_ut_shared INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
   sg_ut_shared INTERFACE stationary_geometries_iface
)
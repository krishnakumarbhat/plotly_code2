cmake_minimum_required(VERSION 3.16)

add_library(SG-Safety OBJECT)

target_include_directories(
   SG-Safety
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Safety
      PRIVATE
         iface/sg_safety.h
         
         src/sg_safety.cpp
)

target_link_libraries(
   SG-Safety
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
)

get_target_property(src_files SG-Safety SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
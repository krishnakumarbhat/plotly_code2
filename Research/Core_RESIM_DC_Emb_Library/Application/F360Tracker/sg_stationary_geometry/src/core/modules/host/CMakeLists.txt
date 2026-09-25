cmake_minimum_required(VERSION 3.16)

add_library(SG-Host OBJECT)

target_include_directories(
    SG-Host
        PUBLIC iface
)

target_sources(
    SG-Host
        PRIVATE
            iface/sg_host_props.h
            src/sg_host_props.cpp
)

target_link_libraries(
      SG-Host
         PRIVATE sg_compiler_config
         PRIVATE SG-Math
         PRIVATE rspp
)

get_target_property(src_files SG-Host SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
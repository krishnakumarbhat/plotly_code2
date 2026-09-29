cmake_minimum_required(VERSION 3.16)

add_library(DC-Dummy-Generator OBJECT)

target_include_directories(
   DC-Dummy-Generator
      PUBLIC iface
)

target_sources(
   DC-Dummy-Generator
      PRIVATE
         iface/dc_dummy_generator.h
         src/dc_dummy_generator.cpp
)

set(msvc_cxx_compiler "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(msvc_cxx_compiler_flags "")
list(APPEND msvc_cxx_compiler_flags "/wd4820")
list(APPEND msvc_cxx_compiler_flags "/wd4710")
list(APPEND msvc_cxx_compiler_flags "/wd4868")

target_compile_options(
   DC-Dummy-Generator
      PRIVATE
         "$<${msvc_cxx_compiler}:${msvc_cxx_compiler_flags}>"
)

target_link_libraries(
   DC-Dummy-Generator
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE DC-Drivability-Classification
)

target_include_directories(
   DC-Dummy-Generator
      PRIVATE $<TARGET_PROPERTY:DC-Drivability-Classification,INCLUDE_DIRECTORIES>
)

get_target_property(src_files DC-Dummy-Generator SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
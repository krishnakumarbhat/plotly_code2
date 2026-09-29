cmake_minimum_required(VERSION 3.16)

add_library(SG-Dummy-Generator OBJECT)

target_include_directories(
   SG-Dummy-Generator
      PUBLIC iface
)

target_sources(
   SG-Dummy-Generator
      PRIVATE
         iface/sg_dummy_generator.h
         src/sg_dummy_generator.cpp
)

set(msvc_cxx_compiler "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(msvc_cxx_compiler_flags "")
list(APPEND msvc_cxx_compiler_flags "/wd4820")
list(APPEND msvc_cxx_compiler_flags "/wd4710")
list(APPEND msvc_cxx_compiler_flags "/wd4868")

target_compile_options(
   SG-Dummy-Generator
      PRIVATE
         "$<${msvc_cxx_compiler}:${msvc_cxx_compiler_flags}>"
)

target_link_libraries(
   SG-Dummy-Generator
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Detection-Processing
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Dummy-Generator SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
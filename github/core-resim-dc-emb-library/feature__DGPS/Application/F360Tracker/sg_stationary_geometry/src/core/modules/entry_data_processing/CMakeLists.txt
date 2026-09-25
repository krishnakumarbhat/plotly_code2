cmake_minimum_required(VERSION 3.16)

add_library(SG-Entry-Data_Processing OBJECT)

target_include_directories(
   SG-Entry-Data_Processing
      PUBLIC iface
      PRIVATE src
)

target_sources(
   SG-Entry-Data_Processing
      PRIVATE
         iface/sg_entry_data_processing.h
         src/sg_entry_data_processing.cpp
)

target_link_libraries(
   SG-Entry-Data_Processing
      PRIVATE sg_compiler_config
      PRIVATE SG-Rspp
)

get_target_property(src_files SG-Entry-Data_Processing SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
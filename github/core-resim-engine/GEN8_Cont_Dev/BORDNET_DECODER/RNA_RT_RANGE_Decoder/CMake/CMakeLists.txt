##########################
### Project definition ###
##########################
####RNA_RL#####
cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "RNA_RT_RANGE_Decoder")
project(${PROJECTNAME})


set(RNA_RT_RANGE_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/RNA_RT_RANGE_Decoder.h")

list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/RNA_RT_RANGE_Common.h")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/RNARTRANGEDecoderBase.h")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/IRNA_RT_RANGE_Decoder.h")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/IRNA_RT_RANGE_DecoderAPI.h")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/ReleaseNotes_RNA_RT_RANGE.txt")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/RNA_RT_RANGE_Decoder.cpp")
list(APPEND SOURCE_ALL "${RNA_RT_RANGE_DECODER_ROOT_PATH}/RNA_RT_RANGE_DecoderAPI.cpp")



set(target_name ${PROJECTNAME})
add_library(${target_name} SHARED "")


target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCE_ALL})

target_link_libraries(${PROJECTNAME} PRIVATE Auto_Gen_Files)

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}")												 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)

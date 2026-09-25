##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(project_name  "SCAN_Decoder")
project(${project_name})

set(SCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

##include custom specific
set(SOURCES "${SCAN_PROJECT_ROOT_PATH}/CanDecoderBase.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/ISCANDecoder.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/ISCANDecoderAPI.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/SCAN_Decoder_A310.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/SCAN_Decoder_A310.cpp")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/SCAN_Decoder_A480.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/SCAN_Decoder_A480.cpp")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/SCANDecoderAPI.cpp")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/CommonHeaders.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/Version.h")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/Resource.rc")
list(APPEND SOURCES "${SCAN_PROJECT_ROOT_PATH}/ReleaseNotes_SCAN.txt")

set(target_name ${project_name})
add_library(${target_name} SHARED "")

target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCES})

set_target_properties(${target_name} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _WIN_PLATFORM) 
endif(MSVC)

##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "BMWL2_IPNEXT_DLT_Decoder")
project(${project_name})



set(SOMIP_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

#include custom specific
set(SOURCES "${SOMIP_PROJECT_ROOT_PATH}/BMWL2_IPNEXT_DLT_Decoder.cpp")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/BMWL2_IPNEXT_DLT_Decoder.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/DLTVersionFactoryBmwIPNEXT.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/IBMWL2_IPNEXT_DLT_DecoderAPI.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/BMWL2_IPNEXT_DLT_Base.h")

list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/BMWL2_IPNEXT_DLT_DecoderAPI.cpp")

list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/BMWL2_IPNEXT_DLT_BaseI380.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/BMWL2_IPNEXT_DLT_BaseI380.cpp")

list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/Resource.rc")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/ReleaseNotes_BMWL2_IPNEXT_DLT.txt")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/Version.h")


set(target_name ${project_name})
add_library(${target_name} SHARED "")

target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCES})
if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)




if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)


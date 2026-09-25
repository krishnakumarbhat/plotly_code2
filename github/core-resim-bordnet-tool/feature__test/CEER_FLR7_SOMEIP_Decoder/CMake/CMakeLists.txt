##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "CEER_FLR7_SOMEIP_Decoder")
project(${project_name})



set(SOMIP_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

#include custom specific
set(SOURCES "${SOMIP_PROJECT_ROOT_PATH}/CEER_FLR7_SOMEIP_Decoder.cpp")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/CEER_FLR7_SOMEIP_Decoder.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/SOMEIPVersionFactoryCEERFlr7.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/ICEER_FLR7_SOMEIP_DecoderAPI.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/CEER_FLR7_SOMEIP_Base.h")

list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/CEER_FLR7_SOMEIP_DecoderAPI.cpp")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/CEER_FLR7_SOMEIP_V2.h")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/CEER_FLR7_SOMEIP_V2.cpp")

list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/Resource.rc")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/ReleaseNotes_CEER_FLR7_SOMEIP.txt")
list(APPEND SOURCES "${SOMIP_PROJECT_ROOT_PATH}/Version.h")

#list(APPEND SOURCES "${UTILITY_PROJ}/Utility.h")
#list(APPEND SOURCES "${UTILITY_PROJ}/Utility.cpp")


set(target_name ${project_name})
add_library(${target_name} SHARED "")

target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})
target_include_directories(${target_name} PRIVATE ${UTILITY_PROJ})

target_sources(${target_name} PRIVATE ${SOURCES})
if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)




if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)


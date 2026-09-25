##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "CrossPlatform")
project(${PROJECTNAME})

set(CROSS_PLATFORM_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

list(APPEND SOURCE_FILES ${CROSS_PLATFORM_PATH}/genutil.cpp)
list(APPEND SOURCE_FILES ${CROSS_PLATFORM_PATH}/genutil.h)
list(APPEND SOURCE_FILES ${CROSS_PLATFORM_PATH}/commonUtils.cpp)
if(UNIX)
list(APPEND SOURCE_FILES ${CROSS_PLATFORM_PATH}/linuxUtils.cpp)
else()
list(APPEND SOURCE_FILES ${CROSS_PLATFORM_PATH}/windowsUtils.cpp)
endif(UNIX)


set(target_name ${PROJECTNAME})

add_library(${target_name} STATIC "")

target_sources(${target_name} PRIVATE ${SOURCE_FILES})

target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/udp_headers)
target_include_directories(${target_name} PUBLIC ${CROSS_PLATFORM_PATH})


set_target_properties( ${target_name}    PROPERTIES
									ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									)


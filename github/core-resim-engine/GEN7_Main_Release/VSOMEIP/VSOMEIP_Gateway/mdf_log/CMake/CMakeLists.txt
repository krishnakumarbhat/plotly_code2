##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "MdfLog")
project (${PROJECTNAME})
set(target_name ${PROJECTNAME})

#Adding required source and header files

list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/../src/apt_mdf_log.cpp")
list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/../inc/apt_mdf_log.h")
list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/../inc/vector_mdf_licence.h")

#Creating Static Library
add_library(${target_name} STATIC "")

#link include directories
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/MDF_Include)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/MDF_Include/Interface)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Cross_Platform)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../inc)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/udp_headers)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/dvlMessage/inc)

#link required sources
target_sources(${target_name} PRIVATE ${SOURCE_FILES})

target_link_libraries(${target_name} PUBLIC CrossPlatform)


set_target_properties( ${target_name}    PROPERTIES
									ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/.. FILES ${SOURCE_FILES})
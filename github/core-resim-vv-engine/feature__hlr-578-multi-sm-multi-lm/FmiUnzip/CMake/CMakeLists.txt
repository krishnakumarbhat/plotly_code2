##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "FmuUnzip")
project(${PROJECTNAME})

set(SOURCES "${CMAKE_CURRENT_LIST_DIR}/../fmuUnzip.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/../fmuUnzip.h")

set(MINIUZ_PATH "${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/Minizip/minizip")

add_library(${PROJECTNAME} STATIC "")

target_compile_definitions(${PROJECTNAME} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)

target_sources(${PROJECTNAME} PRIVATE ${SOURCES})

target_include_directories(${PROJECTNAME} PUBLIC ${MINIUZ_PATH})

target_link_libraries(${PROJECTNAME} Common_Includes_Interface)
target_link_libraries(${PROJECTNAME} minizip)
IF(OUT_DIR_LIB)
		set_target_properties(${PROJECTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR_LIB}"
														LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR_LIB}")
ENDIF(OUT_DIR_LIB)
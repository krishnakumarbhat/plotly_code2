##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(project_name  "Common_Structures")
project(${project_name})

set(COMMON_STRUCTURE_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCES "${COMMON_STRUCTURE_PROJECT_ROOT_PATH}/Common_Structures.h")
add_library(${project_name} STATIC "")
set_target_properties(${project_name} PROPERTIES LINKER_LANGUAGE CXX)
target_sources(${project_name} PUBLIC ${SOURCES})




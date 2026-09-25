##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME  "Common_Includes_Interface")
project(${PROJECTNAME})
add_library(${PROJECTNAME} INTERFACE)


target_include_directories(${PROJECTNAME} INTERFACE ${COMMON_INCLUDES_FILE_PATH}/include)
target_include_directories(${PROJECTNAME} INTERFACE ${COMMON_INCLUDES_FILE_PATH}/helper)
target_include_directories(${PROJECTNAME} INTERFACE ${COMMON_INCLUDES_FILE_PATH}/MDF_Include/Interface)
target_include_directories(${PROJECTNAME} INTERFACE ${COMMON_INCLUDES_FILE_PATH}/MDF_Include)

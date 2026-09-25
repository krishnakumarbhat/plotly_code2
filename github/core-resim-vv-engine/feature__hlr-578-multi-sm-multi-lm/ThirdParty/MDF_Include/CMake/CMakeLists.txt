##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "Mdf_Include")
project(${PROJECTNAME})
add_library(${PROJECTNAME} INTERFACE)

target_include_directories(${PROJECTNAME} INTERFACE ${MDF_INCLUDE_PATH}/Interface)
target_include_directories(${PROJECTNAME} INTERFACE ${MDF_INCLUDE_PATH})

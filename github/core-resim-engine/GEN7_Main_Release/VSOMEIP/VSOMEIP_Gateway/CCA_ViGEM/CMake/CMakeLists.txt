#start CMake for CCA ViGEM
#Build Steps
#Complier MSVC

#Debug Mode:
#	Step1 cmake -G "Visual Studio 11 Win64" ..\CMake
#	Step2 cmake --build . --target ALL_BUILD --config Debug

#Compiler GNU CXX
#	Step1 cmake ../CMake
#	Step2 make

##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "CCA_ViGEM")
project(${PROJECTNAME})

set(target_name ${PROJECTNAME})

#Linking directories
link_directories("${CMAKE_CURRENT_LIST_DIR}/../../Output")
link_directories("${CMAKE_CURRENT_LIST_DIR}/../../CCA_ViGEM/bin/lib")

#Adding required source and header files

list(APPEND SOURCE_ALL "${CMAKE_CURRENT_LIST_DIR}/../src/vgm_cca_log.cpp")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_LIST_DIR}/../inc/vgm_cca_log.h")


#Creating Static Library
add_library(${target_name} STATIC "")

#link required sources
target_sources(${target_name} PRIVATE ${SOURCE_ALL})

set(CCA_LIB_VERSION "v2.1.6")

set(CCA_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../../CCA_ViGEM/bin/lib")
find_library(cca_lib_2-1-6 ccalib_mdf_x64_2-1-6 HINTS "${CCA_LIB_PATH}")
#target_link_libraries(${PROJECTNAME} ${cca_lib_2-1-6})


set(CCA_VIGEM_INC_PATH "${CMAKE_CURRENT_LIST_DIR}/../CCA Framework MDF/${CCA_LIB_VERSION}/include")

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../inc)
target_include_directories(${target_name} PUBLIC ${CCA_VIGEM_INC_PATH})
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/udp_headers)
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Common_Files/dvlMessage/inc)

set_target_properties( ${target_name}    PROPERTIES
									ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../Output"
									)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/.. FILES ${SOURCE_FILES})
target_link_libraries(${target_name} PUBLIC CrossPlatform)



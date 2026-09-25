cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)
set (CMAKE_CXX_STANDARD 11)

set(TEST_PROJECT  "ASDSPACE")
project(${TEST_PROJECT})

set(InterfaceMain_Proj  "${CMAKE_CURRENT_SOURCE_DIR}/../../Code")

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TO_UPPER)

add_subdirectory("${InterfaceMain_Proj}" "${CMAKE_CURRENT_BINARY_DIR}/InterfaceMain")
add_executable (${TEST_PROJECT} src/InterfaceIface.cpp
								inc/fixmac.h
								inc/customer_ID_config.h
								inc/LM2_Interface.h)
						
if(UNIX)
target_link_libraries(${TEST_PROJECT} ${CMAKE_DL_LIBS})
endif(UNIX)

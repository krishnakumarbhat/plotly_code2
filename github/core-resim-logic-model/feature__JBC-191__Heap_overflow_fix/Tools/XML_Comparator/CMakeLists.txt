cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)
set (CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TO_UPPER)

if(CMAKE_BUILD_TYPE_TO_UPPER STREQUAL "DEBUG")
    set(BUILD_FOLDER "Debug")
else()
    set(BUILD_FOLDER "Release")
endif()

set(XML_COMPARE_PROJECT  "XML_Compare")
project(${XML_COMPARE_PROJECT})

if(MSVC)
	set(pugi_xml "${CMAKE_CURRENT_SOURCE_DIR}/libs/${BUILD_FOLDER}/pugixml.lib")
else()
	set(pugi_xml "${CMAKE_CURRENT_SOURCE_DIR}/libs/${BUILD_FOLDER}/libpugixml.a")
endif()

add_executable (${XML_COMPARE_PROJECT} src/XmlCompareReport.cpp
										inc/XmlCompareReport.h)
		
target_link_libraries(${XML_COMPARE_PROJECT} ${pugi_xml})
		
if(UNIX)
target_link_libraries(${XML_COMPARE_PROJECT} ${CMAKE_DL_LIBS})
endif(UNIX)

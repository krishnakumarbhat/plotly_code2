
#set minimum cmake version required
cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(CheckTypeSize)
check_type_size("void*"  SIZEOF_DATA_PTR)

if(${SIZEOF_DATA_PTR} EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
	message("Building for 64 bit.....")
elseif(${SIZEOF_DATA_PTR} EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x86")
	message("Building for 32 bit.....")
endif() 

set(OUT_DIR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../output/${CONF_DIR}/")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

project (vsomeipGateway)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(APTIV_GATEWAY_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../")

set(VSOMEIP_NAME "vsomeip3")
set(VSOMEIP_LIBRARIES vsomeip3)
set(VSOMEIP_LIBRARIES_DIR "${CMAKE_CURRENT_LIST_DIR}/../Output")
set(VSOMEIP_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/../Interface")

if(UNIX)
	add_compile_options("-fPIC")
	string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TO_UPPER)
	if(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
		add_compile_options("-g")
	endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
	SET(CMAKE_CXX_FLAGS -pthread)
endif(UNIX)

link_directories(${VSOMEIP_LIBRARIES_DIR})
include_directories(${VSOMEIP_INCLUDE_DIRS})
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../Cross_Platform/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Cross_Platform")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../mdf_log/CMake" "${CMAKE_CURRENT_BINARY_DIR}/mdf_log")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../CCA_ViGEM/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CCA_ViGEM")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../Application/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Application")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../Gateway/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Gateway")
#add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../TestReceiver/CMake" "${CMAKE_CURRENT_BINARY_DIR}/TestReceiver")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../GatewayReceiver/CMake" "${CMAKE_CURRENT_BINARY_DIR}/GatewayReceiver")


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

cmake_minimum_required(VERSION 4.0.3)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

#Set project name
set(PROJECTNAME "SRR_HiL_Resim")
project(${PROJECTNAME})

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 11)
if(MSVC)
else()
set(CMAKE_C_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "-fPIC")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

endif()

set(buildConfig ${BUILDCONFIGTYPE})
string(TOUPPER "${buildConfig}" buildConfigToUpper)
message("Build Config Type is : ${buildConfigToUpper}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

include(CheckTypeSize)
check_type_size("void*"     SIZEOF_DATA_PTR)

# Multi processor compilation
SET(MSVC_MULTITHREADED_BUILD ON CACHE BOOL "Multi processor compilation build with as many threads as possible when building with microsoft visual studio")
if(MSVC_MULTITHREADED_BUILD)
if(MSVC)
add_definitions(/MP)
set(CMAKE_CXX_STACK_SIZE ${CMAKE_CXX_STACK_SIZE} 1320000)
endif()
endif()

if(${SIZEOF_DATA_PTR} EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
elseif(${SIZEOF_DATA_PTR} EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x86")
endif() 			

# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/../../..")

# set path for Common includes
set(COMMON_INC "${SOURCE_PROJECT}/CommonFiles")

# set path for Application
set(APP_PROJECT "${SOURCE_PROJECT}/ApplicationProjects")

# set path for core library projects
set(CORE_LIB_PROJECT "${SOURCE_PROJECT}/CoreLibraryProjects")

if(MSVC)
set(OUTPUT_PATH "${APP_PROJECT}/output/${CMAKE_VS_PLATFORM_NAME}") 
else()
set(OUTPUT_PATH "${APP_PROJECT}/SRR3_Resimulator/output")
endif()

#Set Project Names
set(INICONFIG_PROJ "${APP_PROJECT}/SRR3_Resimulator/IniConfig")
set(SRR3COMM_PROJ "${APP_PROJECT}/SRR3_Resimulator/SRR3_Comm")

set(SRR3HIL_EXE_PROJ "${APP_PROJECT}/SRR3_Resimulator/SRR3_HiL_Exec")
set(SRR3HIL_RESIM_PROJ "${APP_PROJECT}/SRR3_Resimulator/SRR3_HiL_Resim")
set(MUDP_LOG_PROJ "${CORE_LIB_PROJECT}/mudp_log")
set(MUDP_DECODER_PROJ "${CORE_LIB_PROJECT}/mudp_decoder")
set(CROSSPLATFORM_PROJ "${CORE_LIB_PROJECT}/CrossPlatform")
set(DPH_RR_ADAS_LOGGER_PROJ "${APP_PROJECT}/DPH_RR_ADAS_LOGGER")
#set(FFMPEG_PROJ "${CORE_LIB_PROJECT}/ffmpeg")
set(ETHERNETGRABBER_PROJ "${APP_PROJECT}/SRR3_Resimulator/EthernetGrabber")
set(MDFLOG_PROJ "${CORE_LIB_PROJECT}/mdf_log")

set(LIBFORT_PROJ "${COMMON_INC}/libfort")


if(MSVC)
set(FDX_PROJ "${APP_PROJECT}/SRR3_Resimulator/FDX_Protocol")
#set(POCO_NET "${APP_PROJECT}/SRR3_Resimulator/Poco_Net")
else()
endif()

# Disable building tests and examples in libfort project (Table creation)
set(FORT_ENABLE_TESTING OFF CACHE INTERNAL "")

#add_subdirectory("${FFMPEG_PROJ}/aptiv_mdf4" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/aptiv_mdf4")

#add subdirectories
add_subdirectory("${INICONFIG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/IniConfig")
add_subdirectory("${SRR3COMM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/SRR3_Comm")

add_subdirectory("${SRR3HIL_EXE_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/SRR3_Hil_Exe")
add_subdirectory("${SRR3HIL_RESIM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/SRR3_Resim")
add_subdirectory("${MUDP_LOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Mudp_log")
add_subdirectory("${MUDP_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Mudp_decoder")
add_subdirectory("${ETHERNETGRABBER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/EthernetGrabber")
add_subdirectory("${DPH_RR_ADAS_LOGGER_PROJ}/CMake" "${DPH_RR_ADAS_LOGGER_PROJ}/Dph_logger")
add_subdirectory("${CROSSPLATFORM_PROJ}/CMake" "${CROSSPLATFORM_PROJ}/CrossPlatform")
add_subdirectory("${MDFLOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MDFLog")

add_subdirectory("${LIBFORT_PROJ}/" "${CMAKE_CURRENT_BINARY_DIR}/libfort")


if(MSVC)
add_subdirectory("${FDX_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/FDX_Protocol")
#add_subdirectory("${POCO_NET}/" "${CMAKE_CURRENT_BINARY_DIR}/Poco_Net")
else()
endif()

#Set start up project
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECTNAME}) 

SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

enable_language(C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project (SRR_HiL_Resim)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif()

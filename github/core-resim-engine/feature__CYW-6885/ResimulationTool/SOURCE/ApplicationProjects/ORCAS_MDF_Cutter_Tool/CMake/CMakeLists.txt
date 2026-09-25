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

cmake_minimum_required(VERSION 4.0)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)
set(buildConfig ${BUILDCONFIGTYPE})
string(TOUPPER "${buildConfig}" buildConfigToUpper)
message("Build Config Type is : ${buildConfigToUpper}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/../../../../SOURCE")

# set path for Common includes
set(COMMON_INC "${SOURCE_PROJECT}/CommonFiles")

# set path for Application
set(APP_PROJECT "${SOURCE_PROJECT}/ApplicationProjects")

# set path for core library projects
set(CORE_LIB_PROJECT "${SOURCE_PROJECT}/CoreLibraryProjects")
# OutputPath
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x32")
endif() 
if(MSVC)
set(OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../ApplicationProjects/ORCAS_MDF_Cutter_Tool/output/${CMAKE_VS_PLATFORM_NAME}")
else()
set(OUTPUT_PATH "${APP_PROJECT}/ORCAS_MDF_Cutter_Tool/output")
endif()
# Set Project Names

#set(STREAM_DECODER_PROJ "${CORE_LIB_PROJECT}/radar_stream_decoder")
set(MDFLOG_PROJ "${CORE_LIB_PROJECT}/mdf_log")
set(DPH_RR_ADAS_LOGGER_PROJ "${APP_PROJECT}/DPH_RR_ADAS_LOGGER")
set(CROSSPLATFORM_PROJ "${CORE_LIB_PROJECT}/CrossPlatform")
#set(MUDP_DECODER_PROJ "${CORE_LIB_PROJECT}/mudp_decoder")
#set(MUDP_LOG_PROJ "${CORE_LIB_PROJECT}/mudp_log")
set(ORCAS_MDF_Cutter_Tool_PROJ "${APP_PROJECT}/ORCAS_MDF_Cutter_Tool/ORCAS_MDF_Cutter_Tool")


add_subdirectory("${CROSSPLATFORM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CrossPlatform")
add_subdirectory("${DPH_RR_ADAS_LOGGER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_LOGGER")
#add_subdirectory("${STREAM_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/radar_stream_decoder")
add_subdirectory("${MDFLOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/mdf_log")
#add_subdirectory("${MUDP_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/mudp_decoder")
#add_subdirectory("${MUDP_LOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/mudp_log")
add_subdirectory("${ORCAS_MDF_Cutter_Tool_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/ORCAS_MDF_Cutter_Tool/ORCAS_MDF_Cutter_Tool")

add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "ORCAS_MDF_Cutter_Tool" ) 
project (ORCAS_MDF_Cutter_Tool)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif()


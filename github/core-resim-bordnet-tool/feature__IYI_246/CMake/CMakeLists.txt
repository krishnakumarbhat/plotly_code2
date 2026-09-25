#Build Steps
#mkdir Build

#Debug Mode:
#Complier MSVC
#	Step1 cmake -G "Visual Studio 11 Win64" ..
#	Step2 cmake --build . --target ALL_BUILD --config Debug

#Compiler GNU CXX
#	Step1 cmake ..
#	Step2 make

##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 4.0)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

set(CMAKE_CXX_STANDARD 11)
set(buildtype ${CMAKE_BUILD_TYPE})
string(TOUPPER "${buildtype}" buildtypeToUpper)
message("Build Type is : ${buildtypeToUpper}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type can be Debug or Release")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/..")

# set path for Common includes
set(COMMON_INC "${SOURCE_PROJECT}/Common_Headers")
set(COMMON_STRUCTURES "${SOURCE_PROJECT}/Common_Structures")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
else()
	if(WIN32)
		add_compile_options(/bigobj)
		add_compile_options(/FS)
		set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "Embedded")
	endif()
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

#output path
set(OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../output/")

# Set Project Names
set(BORDNET_DECODER_PROJ "${SOURCE_PROJECT}/BordNetDecoder")
set(AUTO_GEN_FILES_PROJ "${SOURCE_PROJECT}/Auto_Gen_Files")

set(CROSS_PLATFORM_PROJ "${SOURCE_PROJECT}/Cross_Platform")
set(UTILITY_PROJ "${SOURCE_PROJECT}/Utility")
set(MDF_LOG_PROJ "${SOURCE_PROJECT}/mdf_log")


#CEER
set(CEER_PCAN_EXTRACTOR_PROJ "${SOURCE_PROJECT}/CEER_PCAN_Extractor")
set(CEER_PCAN_RL_DECODER_PROJ "${SOURCE_PROJECT}/CEER_PCAN_RL_Decoder")
set(CEER_PCAN_RR_DECODER_PROJ "${SOURCE_PROJECT}/CEER_PCAN_RR_Decoder")
set(CEER_PCAN_FR_DECODER_PROJ "${SOURCE_PROJECT}/CEER_PCAN_FR_Decoder")
set(CEER_PCAN_FL_DECODER_PROJ "${SOURCE_PROJECT}/CEER_PCAN_FL_Decoder")
set(CEER_PCAN_FLR_DECODER_PROJ "${SOURCE_PROJECT}/CEER_PCAN_FLR_Decoder")
#CEER_SOMEIP
set(CEER_SOMEIP_EXTRACTOR_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_Extractor")
set(CEER_SOMEIP_FL_DECODER_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_FL_Decoder")
set(CEER_SOMEIP_FR_DECODER_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_FR_Decoder")
set(CEER_SOMEIP_RR_DECODER_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_RR_Decoder")
set(CEER_SOMEIP_RL_DECODER_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_RL_Decoder")
set(CEER_SOMEIP_FLR_DECODER_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_FLR_Decoder")
#MCIP
set(MCIP_PCAN_EXTRACTOR_PROJ "${SOURCE_PROJECT}/MCIP_PCAN_Extractor")
set(MCIP_PCAN_RL_DECODER_PROJ "${SOURCE_PROJECT}/MCIP_PCAN_RL_Decoder")
set(MCIP_PCAN_RR_DECODER_PROJ "${SOURCE_PROJECT}/MCIP_PCAN_RR_Decoder")
set(MCIP_PCAN_FR_DECODER_PROJ "${SOURCE_PROJECT}/MCIP_PCAN_FR_Decoder")
set(MCIP_PCAN_FL_DECODER_PROJ "${SOURCE_PROJECT}/MCIP_PCAN_FL_Decoder")
set(MCIP_PCAN_FLR_DECODER_PROJ "${SOURCE_PROJECT}/MCIP_PCAN_FLR_Decoder")
#CEER_SRR7P
set(CEER_SRR7P_SOMEIP_Decoder_PROJ "${SOURCE_PROJECT}/CEER_SRR7P_SOMEIP_Decoder")
set(CEER_SOMEIP_Decoder_Wrapper_PROJ "${SOURCE_PROJECT}/CEER_SOMEIP_Decoder_Wrapper")
set(CEER_FLR7_SOMEIP_Decoder_PROJ "${SOURCE_PROJECT}/CEER_FLR7_SOMEIP_Decoder")



set(COMMON_STRUCTURES_PROJ "${SOURCE_PROJECT}/Common_Structures")




add_subdirectory("${AUTO_GEN_FILES_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Auto_Gen_Files")

add_subdirectory("${CROSS_PLATFORM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Cross_Platform")
add_subdirectory("${UTILITY_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Utility")
add_subdirectory("${MDF_LOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/mdf_log")

#CEER
add_subdirectory("${CEER_PCAN_EXTRACTOR_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_PCAN_Extractor")
add_subdirectory("${CEER_PCAN_FL_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_PCAN_FL_Decoder")
add_subdirectory("${CEER_PCAN_FR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_PCAN_FR_Decoder")
add_subdirectory("${CEER_PCAN_RL_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_PCAN_RL_Decoder")
add_subdirectory("${CEER_PCAN_RR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_PCAN_RR_Decoder")
add_subdirectory("${CEER_PCAN_FLR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_PCAN_FLR_Decoder")

#CEER_SOMEIP
add_subdirectory("${CEER_SOMEIP_EXTRACTOR_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_Extractor")
add_subdirectory("${CEER_SOMEIP_FL_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_FL_Decoder")
add_subdirectory("${CEER_SOMEIP_FR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_FR_Decoder")
add_subdirectory("${CEER_SOMEIP_RR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_RR_Decoder")
add_subdirectory("${CEER_SOMEIP_RL_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_RL_Decoder")
add_subdirectory("${CEER_SOMEIP_FLR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_FLR_Decoder")

#MCIP
add_subdirectory("${MCIP_PCAN_EXTRACTOR_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MCIP_PCAN_Extractor")
add_subdirectory("${MCIP_PCAN_FL_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MCIP_PCAN_FL_Decoder")
add_subdirectory("${MCIP_PCAN_FR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MCIP_PCAN_FR_Decoder")
add_subdirectory("${MCIP_PCAN_RL_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MCIP_PCAN_RL_Decoder")
add_subdirectory("${MCIP_PCAN_RR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MCIP_PCAN_RR_Decoder")
add_subdirectory("${MCIP_PCAN_FLR_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MCIP_PCAN_FLR_Decoder")

#CEER_SRR7P
add_subdirectory("${CEER_SRR7P_SOMEIP_Decoder_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SRR7P_SOMEIP_Decoder")
add_subdirectory("${CEER_SOMEIP_Decoder_Wrapper_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_SOMEIP_Decoder_Wrapper")
add_subdirectory("${CEER_FLR7_SOMEIP_Decoder_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CEER_FLR7_SOMEIP_Decoder")


add_subdirectory("${COMMON_STRUCTURES_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Common_Structures")
add_subdirectory("${BORDNET_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/BordNetDecoder")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "BordNetDecoder" ) 
project (BordNetDecoder)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

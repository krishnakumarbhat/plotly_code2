#Build Steps
#Complier MSVC

#Debug Mode:
#	Step1 cmake -DBUILDCONFIGTYPE=SIL -G "Visual Studio 11 Win64" -..\CMake
#	Step2 cmake --build . --target ALL_BUILD --config Debug

#Compiler GNU CXX
#	Step1 cmake -DBUILDCONFIGTYPE=SIL ../CMake
#	Step2 make

##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 4.0)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

set(PROJECTNAME  "APT_SRR_RESIM")
project(${PROJECTNAME})

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 11)
set(buildConfig ${BUILDCONFIGTYPE})
string(TOUPPER "${buildConfig}" buildConfigToUpper)
message("Build Config Type is : ${buildConfigToUpper}")
option(BUILD_LIME "build logger static library" OFF)

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
set(COMMON "${SOURCE_PROJECT}/Common")
# OutputPath

if(UNIX)
	set(CMAKE_INSTALL_RPATH "$ORIGIN")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	#add_compile_options("-fpack-struct=2")
	add_compile_options("-fgnu89-inline")
	add_compile_options("-fPIC")
	add_compile_options("$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

	option(ENABLE_SANITIZERS "Enable AddressSanitizer and LeakSanitizer (Debug builds only)" OFF)
	# if(ENABLE_SANITIZERS)
	# 	message(STATUS "Sanitizers enabled: AddressSanitizer + LeakSanitizer")
	# 	add_compile_options("$<$<CONFIG:Debug>:-O1>")
	# 	add_compile_options("$<$<CONFIG:Debug>:-fsanitize=address,leak>")
	# 	add_compile_options("$<$<CONFIG:Debug>:-fno-omit-frame-pointer>")
	# 	add_link_options("$<$<CONFIG:Debug>:-fsanitize=address,leak>")
	# endif()

	if(BUILD_FOR_32_BIT)
		set(CMAKE_CXX_FLAGS "-m32")
		set(CMAKE_C_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
endif(UNIX)

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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	#set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Multi processor compilation
SET(MSVC_MULTITHREADED_BUILD ON CACHE BOOL "Multi processor compilation build with as many threads as possible when building with microsoft visual studio")
if(MSVC_MULTITHREADED_BUILD)
if(MSVC)
add_definitions(/MP)
set(CMAKE_CXX_STACK_SIZE ${CMAKE_CXX_STACK_SIZE} 2000000)
endif()
endif()

if(MSVC)
	set(OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../ApplicationProjects/APT_SRR_RESIM/output/${CMAKE_VS_PLATFORM_NAME}")
else()
	if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM/output/Linux/Debug")
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM/output/Linux/Debug")
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM/output/Linux/Release")
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM/output/Linux/Release")
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM/output/Linux/RelWithDebInfo")
	endif()
endif()
# Set Project Names
set(ADAS_CONFIG_PROJ "${APP_PROJECT}/DPH_RR_ADAS_Config")
#set(STREAM_DECODER_PROJ "${CORE_LIB_PROJECT}/radar_stream_decoder")
set(VEHICLE_CONFIG_PROJ "${CORE_LIB_PROJECT}/Vehicle_Config")
set(RADAR_CONFIG_PROJ "${CORE_LIB_PROJECT}/radar_config")
set(CAF_CONFIG_PROJ "${CORE_LIB_PROJECT}/CAF_Config")
set(MUDP_LOG_PROJ "${CORE_LIB_PROJECT}/mudp_log")
set(MUDP_DATA_RECEIVE_PROJ "${CORE_LIB_PROJECT}/mudp_receiver")
set(OBJTODET_PROJ "${CORE_LIB_PROJECT}/ObjToDet")
set(MUDP_DECODER_CALIB_PROJ "${CORE_LIB_PROJECT}/mudp_decoder_calib")
set(DVLFILE_PROJ "${CORE_LIB_PROJECT}/dvlFile")
set(MDFLOG_PROJ "${CORE_LIB_PROJECT}/mdf_log")
set(CCA_ViGEM_PROJ "${CORE_LIB_PROJECT}/CCA_ViGEM")
set(ECU_SYNC_PROJ "${CORE_LIB_PROJECT}/ecu_sync")
set(UDP_TRANSMIT_PROJ "${CORE_LIB_PROJECT}/UDP_Transmitter")
set(CCA_VPCAP_PROJ "${CORE_LIB_PROJECT}/CCA_Vpcap")
set(MUDP_DECODER_PROJ "${CORE_LIB_PROJECT}/mudp_decoder")
set(DPH_RR_ADAS_LOGGER_PROJ "${APP_PROJECT}/DPH_RR_ADAS_LOGGER")
set(DPH_RR_ADAS_SIL_PROJ "${APP_PROJECT}/DPH_RR_ADAS_SIL")
set(CROSSPLATFORM_PROJ "${CORE_LIB_PROJECT}/CrossPlatform")
set(APT_SRR_RESIM_PROJ "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM")
set(dvlFile_PROJ "${CORE_LIB_PROJECT}/dvlFile")
set(TIMING_PROFILE_PROJ "${APP_PROJECT}/Timinng_Profile")
#set(BLF_PROJ "${CORE_LIB_PROJECT}/BLF_Project")
set(Stream_Plugin_PROJ "${CORE_LIB_PROJECT}/Stream_Plugin")
#set(FFMPEG_PROJ "${CORE_LIB_PROJECT}/ffmpeg/")
#set(FFMPEG_AVUTIL "${CORE_LIB_PROJECT}/avutil")

#add_subdirectory("${FFMPEG_PROJ}/readFF_version" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/readFF_version")
#add_subdirectory("${FFMPEG_PROJ}/aptiv_signals" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/aptiv_signals")
#add_subdirectory("${FFMPEG_PROJ}/aptiv_avi" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/aptiv_avi")

#add_subdirectory("${FFMPEG_PROJ}/readFF" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/readFF")
#add_subdirectory("${FFMPEG_PROJ}/aptiv_mdf4" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/aptiv_mdf4")

#add_subdirectory("${FFMPEG_PROJ}/rtp_streamer" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/rtp_streamer")

#add_subdirectory("${FFMPEG_PROJ}/src/libavutil/CMake" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/libavutil")
#add_subdirectory("${FFMPEG_PROJ}/src/libavcodec/CMake" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/libavcodec")
#add_subdirectory("${FFMPEG_PROJ}/src/libavformat/CMake" "${CMAKE_CURRENT_BINARY_DIR}/ffmpeg/libavformat")
#add_subdirectory("${BLF_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/BLF_Project")
add_subdirectory("${CROSSPLATFORM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CrossPlatform")
add_subdirectory("${DPH_RR_ADAS_LOGGER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_LOGGER")

add_subdirectory("${TIMING_PROFILE_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Timing_Profile")
#add_subdirectory("${STREAM_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/radar_stream_decoder")
add_subdirectory("${ADAS_CONFIG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/config")
add_subdirectory("${MUDP_LOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Mudp_log")
add_subdirectory("${MUDP_DATA_RECEIVE_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Mudp_Receiver")
add_subdirectory("${MUDP_DECODER_CALIB_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Mudp_decoder_calib")
add_subdirectory("${MDFLOG_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MDFLog")
add_subdirectory("${CCA_ViGEM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CCA_ViGEM")
add_subdirectory("${UDP_TRANSMIT_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/UDP_Transmitter")
add_subdirectory("${CCA_VPCAP_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CCA_Vpcap")
add_subdirectory("${MUDP_DECODER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Mudp_decoder")
add_subdirectory("${DPH_RR_ADAS_SIL_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_SIL")
add_subdirectory("${APT_SRR_RESIM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/ADAS_RESIM")
add_subdirectory("${dvlFile_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/dvlFile")
add_subdirectory("${Stream_Plugin_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Stream_Plugin")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "APT_SRR_RESIM" ) 
project (APT_SRR_RESIM)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif()

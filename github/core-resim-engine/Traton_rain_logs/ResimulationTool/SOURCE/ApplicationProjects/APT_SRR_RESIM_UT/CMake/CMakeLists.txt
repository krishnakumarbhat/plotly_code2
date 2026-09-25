# Main CMake for "APT_SRR_RESIM_UT" 

# Compiler GNU CXX
#	Step1 cmake -DBUILDCONFIGTYPE=SIL ../CMake
#	Step2 make

##########################
### Project definition ###
##########################


cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 11)

message(STATUS "*********************APT_SRR_RESIM_UT****************************")

# Set the CMake MSVC Runtime Library to MultiThreaded Debug (/MTd) 
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") 
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")

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
	#add_compile_options("-fpack-struct=2")
	add_compile_options("-fgnu89-inline")
	add_compile_options("-fPIC")
	add_compile_options("$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
		
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
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Multi processor compilation
SET(MSVC_MULTITHREADED_BUILD ON CACHE BOOL "Multi processor compilation build with as many threads as possible when building with microsoft visual studio")
if(MSVC_MULTITHREADED_BUILD)
if(MSVC)
add_definitions(/MP)
set(CMAKE_CXX_STACK_SIZE ${CMAKE_CXX_STACK_SIZE} 1320000)
endif()
endif()

if(MSVC)#MODIFIED PATH
	set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/output_UT/${CMAKE_VS_PLATFORM_NAME}")
else()
	if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/output_UT/Linux/Debug")
	elseif (${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
		set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/output_UT/Linux/Release")
	endif()
endif()
# Set Project Names
set(ADAS_CONFIG_PROJ "${APP_PROJECT}/DPH_RR_ADAS_Config")
# set(STREAM_DECODER_PROJ "${CORE_LIB_PROJECT}/radar_stream_decoder")
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
set(APT_SRR_RESIM_UT_PROJ "${APP_PROJECT}/APT_SRR_RESIM_UT")
set(DPH_RR_ADAS_SIL_PROJ "${APP_PROJECT}/DPH_RR_ADAS_SIL")
set(CROSSPLATFORM_PROJ "${CORE_LIB_PROJECT}/CrossPlatform")
set(APT_SRR_RESIM_PROJ "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM")
set(dvlFile_PROJ "${CORE_LIB_PROJECT}/dvlFile")
set(TIMING_PROFILE_PROJ "${APP_PROJECT}/Timinng_Profile")


add_subdirectory("${CROSSPLATFORM_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/CrossPlatform")
add_subdirectory("${DPH_RR_ADAS_LOGGER_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_LOGGER")
#add_subdirectory("${APT_SRR_RESIM_UT_PROJ}/CMake" "${APP_PROJECT}/APT_SRR_RESIM_UT")
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

#setting my each UT Module for my UT folder
#set(APT_SRR_RESIM_UT_PROJ "${APP_PROJECT}/APT_SRR_RESIM_UT")
set(DPH_RR_ADAS_LOGGER_UT_PROJ "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT")
set(DPH_RR_ADAS_RESIM_UT_PROJ "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT")
set(DPH_RR_ADAS_SIL_UT_PROJ "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT")
set(DPH_RR_ADAS_Config_UT_PROJ "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT")

#adding Module as sub_directory to my UT folder
#add_subdirectory("${APT_SRR_RESIM_UT_PROJ}/CMake" "${APP_PROJECT}/APT_SRR_RESIM_UT")
add_subdirectory("${DPH_RR_ADAS_LOGGER_UT_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_LOGGER_UT")
add_subdirectory("${DPH_RR_ADAS_RESIM_UT_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_RESIM_UT")
add_subdirectory("${DPH_RR_ADAS_SIL_UT_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_SIL_UT")

#suspect 1
add_subdirectory("${DPH_RR_ADAS_Config_UT_PROJ}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/DPH_RR_ADAS_Config_UT")

#set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "APT_SRR_RESIM" ) 
set(PROJECTNAME  "APT_SRR_RESIM_UT")
project (${PROJECTNAME})
add_executable(${PROJECTNAME} ${CMAKE_CURRENT_LIST_FILE})


#Adding required common header files
# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/../../../../SOURCE")
set(COMMON_INC "${SOURCE_PROJECT}/CommonFiles")
set(APP_PROJECT "${SOURCE_PROJECT}/ApplicationProjects")
set(CORE_LIB_PROJECT "${SOURCE_PROJECT}/CoreLibraryProjects")
set(COMMON "${SOURCE_PROJECT}/Common")
set(COMMONINC "${COMMON_INC}/inc/")
set(COMMONSYM "${COMMON_INC}/sym/")
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders/")
set(COMMONHELPER "${COMMON_INC}/helper/")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers/")
set(COMMONPLUGIN "${COMMON_INC}/plugin/")
set(COMMONUTIL "${COMMON_INC}/Utility/")
set(BOOST_INC "${APP_PROJECT}/BOOST/Include/")
#set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/output_UT/${CMAKE_VS_PLATFORM_NAME}")
set(CUSTOM_CONFIG "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/")
set(UT_INC "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/inc/")
set(LOGGER_INC "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/inc/")

list(APPEND HEADERS "${COMMONINC}/guard.h")
list(APPEND HEADERS "${COMMONUTIL}/commonUtils.h")
list(APPEND HEADERS "${COMMONHEADERS}/types.h")
list(APPEND HEADERS "${COMMONHEADERS}/tObjects.h")
#ADDED 1/6/2024
list(APPEND HEADERS "${COMMONHELPER}/genutil.h")
#list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/bmw_output_trace.c")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/bmw_output_trace.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/src/bmw_output_trace_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/inc/bmw_output_trace_UT.hpp")

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/Buspecidlookuptable.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/Buspecidlookuptable.h")

list(APPEND HEADERS "${COMMONHEADERS}/dph_rr_adas_logger.h")
list(APPEND HEADERS "${COMMONHEADERS}/dph_rr_adas_logger_lib_loader.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_LOGGER/dph_rr_adas_logger.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_LOGGER/dph_rr_adas_logger_lib_loader.cpp")

list(APPEND SOURCE_ALL "${CORE_LIB_PROJECT}/CrossPlatform/commonUtils.cpp")



#list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/matData_org.cpp")
list(APPEND SOURCE_ALL "${COMMONHEADERS}/tObjects.h")


#added 1/6/2024

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/matData.cpp")
#list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.h")


list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/Error_Logging_Status.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM/DPH_RR_ADAS_RESIM/Error_Logging_Status.h")

list(APPEND HEADERS "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.h")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/src/SRR3_Internal_Data_Logger_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/inc/SRR3_Internal_Data_Logger_UT.hpp")


#pluginLoader

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/src/PluginLoader_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/inc/PluginLoader_UT.hpp")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.h")

#VerboseLogUtil

list(APPEND SOURCE_ALL "${COMMONHELPER}/VerboseLogUtil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/VerboseLogUtil.h")
#list(APPEND SOURCE_ALL "${COMMON}/IRadarStream.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/src/VerboseLogUtil_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/inc/VerboseLogUtil_UT.hpp")

#DPH_RR_ADAS_Config
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/dph_rr_adas_config.cpp")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/CommonFiles/CommonHeaders/dph_rr_adas_config.h")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/CommonFiles/inc/pugiconfig.hpp")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/CommonFiles/inc/pugixml.hpp")

list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/AUDI/dph_rr_adas_audi_config.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/AUDI/dph_rr_adas_audi_config.h")


list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/BMW/dph_rr_adas_bmw_config.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/BMW/dph_rr_adas_bmw_config.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/src/dph_rr_adas_config_mock_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/inc/dph_rr_adas_config_UT.hpp")

#MUDP_DECODER

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/src/dllmain_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/inc/dph_rr_adas_sil_UT.hpp")
list(APPEND SOURCE_ALL "${MUDP_DECODER_PROJ}/dllmain.cpp")



# RESIM
file(GLOB DPH_RR_ADAS_RESIM_UT_TEST_SOURCES ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_RESIM_UT/src/*.cpp)
file(GLOB DPH_RR_ADAS_LOGGER_UT_TEST_SOURCES ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/src/*.cpp)
file(GLOB DPH_RR_ADAS_Config_UT_TEST_SOURCES ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/src/*.cpp)
file(GLOB DPH_RR_ADAS_SIL_UT_TEST_SOURCES ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/src/*.cpp)
file(GLOB DPH_RR_ADAS_RESIM_UT_TEST_HEADERS ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_RESIM_UT/inc/*.hpp)
file(GLOB DPH_RR_ADAS_LOGGER_UT_TEST_HEADERS ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/inc/*.hpp)
file(GLOB DPH_RR_ADAS_Config_UT_TEST_HEADERS ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/inc/*.hpp)
file(GLOB DPH_RR_ADAS_SIL_UT_TEST_HEADERS ${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_SIL_UT/inc/*.hpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECTNAME} gtest_main gmock_main)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif()

#RESIM

set(TEST_SOURCES     
	${DPH_RR_ADAS_RESIM_UT_TEST_SOURCES}
	${DPH_RR_ADAS_LOGGER_UT_TEST_SOURCES}
	${DPH_RR_ADAS_SIL_UT_TEST_SOURCES}
	${DPH_RR_ADAS_Config_UT_TEST_SOURCES}
	${DPH_RR_ADAS_RESIM_UT_TEST_HEADERS}
	${DPH_RR_ADAS_LOGGER_UT_TEST_HEADERS}
	${DPH_RR_ADAS_SIL_UT_TEST_HEADERS}
	${DPH_RR_ADAS_Config_UT_TEST_HEADERS}	
)

#target_sources(${PROJECTNAME} PRIVATE ${TEST_SOURCES})
target_include_directories(${PROJECTNAME} PRIVATE ${HEADERS} ${DPH_RR_ADAS_LOGGER_UT_TEST_HEADERS} ${DPH_RR_ADAS_RESIM_UT_TEST_HEADERS} ${DPH_RR_ADAS_Config_UT_TEST_HEADERS} ${DPH_RR_ADAS_SIL_UT_TEST_HEADERS} ${COMMONINC} ${AUDI_INC} ${COMMONHEADERS}) 
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${TEST_SOURCES} ${AUDI_FILES} ${BMW_FILES})
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
						${CMAKE_CURRENT_SOURCE_DIR}
						${COMMONHEADERS} 
						${COMMONHELPER} 
						${COMMONUDPHEADER} 
						${COMMONPLUGIN}
						${COMMONUTIL}
						${COMMOM_INC}
						${COMMOMINC}
						${BOOST_INC}
						${HEADERS}
						${UT_INC}
						${SOURCE_PROJECT}
						${COMMON}
						${LOGGER_INC}
						
						#${CORE_LIB_PROJECT}# for future usage
						)
set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)


target_link_libraries(${PROJECTNAME} ${pugixml_debug} gtest_main gmock_main)

add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
option(COVERAGE "Option description" OFF) 
message("coverage Config Type is : ${COVERAGE}")
if(COVERAGE)
    execute_process(COMMAND OpenCppCoverage --modules=APT_SRR_RESIM -- ./output_UT/x64/Debug/APT_SRR_RESIM_UT.exe --export_type=html)
endif(COVERAGE)


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	#target_link_libraries (${PROJECTNAME} ${SOURCES} )

	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 

#	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MD /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")
    target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MDd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")	
	
endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN
	 												WIN32_EXTRA_LEAN)
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)




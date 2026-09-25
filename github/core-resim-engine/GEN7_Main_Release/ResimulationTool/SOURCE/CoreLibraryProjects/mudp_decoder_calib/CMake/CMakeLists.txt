#start Cmake for Mudp_decoder_calib
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

set(PROJECTNAME  "mudp_decoder_calib")
project(${PROJECTNAME})
add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)

#Adding required common header files
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONUTILITY "${COMMON_INC}/Utility")
set(COMMONUDPHEADERS "${COMMON_INC}/udp_headers")
set(AUDI_INC "${COMMON_INC}/inc/AUDI")
set(COMMONPLUGIN "${COMMON_INC}/plugin")

#Linking directories
link_directories(${OUTPUT_PATH})

#Adding required source and header files
set(SorrcesFiles "${MUDP_DECODER_CALIB_PROJ}/")
list(APPEND SOURCE_ALL "${SorrcesFiles}/CalibDecoder.cpp")
list(APPEND SOURCE_ALL "${SorrcesFiles}/TrackerIntenalDecoder.cpp")
list(APPEND Header "${SorrcesFiles}/CalibDecoder.h")
list(APPEND Header "${SorrcesFiles}/TrackerIntenalDecoder.h")
#list(APPEND Header "${STREAM_DECODER_PROJ}/Common/IRadarStream.h")
#list(APPEND Header "${STREAM_DECODER_PROJ}/Common/Z_Logging.h")
list(APPEND Header "${COMMONHEADERS}/dph_rr_adas_shared_types.h" 
					"${COMMONHEADERS}/dph_rr_adas_logger.h" )
list(APPEND Header "${COMMONINC}/common.h" 
					#"${COMMONINC}/IRadarStream.h" 
					#"${COMMONINC}/Z_Logging.h"
					"${COMMONINC}/usc_cal.h"
					"${COMMONINC}/Resim_Config.h")
					
list(APPEND Header "${COMMONUDPHEADERS}/rr_cal_log.h")
list(APPEND Header "${AUDI_INC}/sil_srr3_debug_types.h")
list(APPEND Header "${COMMONUTILITY}/commonUtils.h")
list(APPEND Header "${COMMONPLUGIN}/PluginLoader.h")


source_group(\\HeaderFiles FILES "${Header}CalibDecoder.h")

#Creating Static Library
add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONHEADERS} ${COMMONINC} ${COMMONUTILITY} ${COMMONUDPHEADERS} ${AUDI_INC} ${COMMONPLUGIN})


#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

target_link_libraries (${PROJECTNAME}  DPH_RR_ADAS_LOGGER)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH) 

 if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)

	target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)

#End Cmake for  Mudp_decoder_calib
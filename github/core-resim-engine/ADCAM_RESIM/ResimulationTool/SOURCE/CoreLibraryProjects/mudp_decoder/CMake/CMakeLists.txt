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

if("${buildConfigToUpper}" STREQUAL "SIL")	
	set(PROJECTNAME  "srr3_decoder_dph")
else()
	set(PROJECTNAME  "srr_decoder_dph")
endif()
project(${PROJECTNAME})

link_directories(${OUTPUT_PATH})

#Adding required common header files
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers")


#Adding required source and header files
set(SourceFiles "${MUDP_DECODER_PROJ}/")
set(SOURCE_ALL "${SourceFiles}radar_udp_plugin.cpp")
if(MSVC)
list(APPEND SOURCE_ALL "${SourceFiles}dllmain.cpp")
endif()
list(APPEND SOURCE_ALL "${SourceFiles}radar_udp_logger_latch.c")
#list(APPEND SOURCE_ALL "${SourceFiles}gen5_radar_udp_logger_latch.c")
#list(APPEND SOURCE_ALL "${SourceFiles}gen7_radar_udp_logger_latch.c")
list(APPEND SOURCE_ALL "${SourceFiles}adcam_camera_udp_logger_latch.c")
list(APPEND SOURCE_ALL "${COMMONHELPER}/VerboseLogUtil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")
#list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")

set(BOOST_INC "${APP_PROJECT}/BOOST/Include")

list(APPEND HEADERS "${SourceFiles}radar_udp_logger_latch.h")
#list(APPEND HEADERS "${SourceFiles}gen5_radar_udp_logger_latch.h")
#list(APPEND HEADERS "${SourceFiles}gen7_radar_udp_logger_latch.h")
list(APPEND HEADERS "${SourceFiles}adcam_camera_udp_logger_latch.h")
list(APPEND HEADERS "${SourceFiles}radar_udp_plugin.h")
list(APPEND HEADERS "${SourceFiles}resource.h")
#list(APPEND HEADERS "${STREAM_DECODER_PROJ}/Common/IRadarStream.h")
#list(APPEND HEADERS "${STREAM_DECODER_PROJ}/Common/Z_Logging.h")

#list(APPEND CDCSOURCES "${SourceFiles}/CDC/cdc_decompress.h")
#list(APPEND CDCSOURCES "${SourceFiles}/CDC/cdc_shared.h")
#list(APPEND CDCSOURCES "${SourceFiles}/CDC/cdc_decompress.cpp")
list(APPEND Header "${COMMONINC}/common.h" 
				 #  "${COMMONINC}/IRadarStream.h" 
				#	"${COMMONINC}/Z_Logging.h" 
					"${COMMONINC}/calib_logging_structure.h")
					
list(APPEND Header "${COMMONUDPHEADER}/udp_sources.h" )


#Adding Definitions
#target_compile_definitions(${PROJECTNAME} PRIVATE UDP_DECODER_PLUGIN_EXPORTS ENABLE_CUSTHDR)
#Creating Shared Library
add_library(${PROJECTNAME} SHARED "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} ${BOOST_INC} ${COMMONHEADERS} ${COMMONHELPER} ${COMMONPLUGIN} ${COMMONUDPHEADER})

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS} ${Header})

#link required definition
target_compile_definitions(${PROJECTNAME} PRIVATE 
							UDP_DECODER_PLUGIN_EXPORTS 
							ENABLE_CUSTHDR
							)

#set posix thread libs
if(UNIX)
	SET(CMAKE_CXX_FLAGS -pthread)
	SET(CMAKE_C_FLAGS -pthread)
	target_link_libraries(${PROJECTNAME} ${CMAKE_THREAD_LIBS_INIT}  DPH_RR_ADAS_LOGGER)
else()
	target_link_libraries(${PROJECTNAME}  DPH_RR_ADAS_LOGGER)	
endif(UNIX)

#Check if Multi configuration generator
		get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
	if(MSVC)
		if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		    target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	    else()
		    target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	    endif()
		    target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
	endif(MSVC)

if(UNIX)
	set_target_properties(${PROJECTNAME} PROPERTIES
		BUILD_WITH_INSTALL_RPATH TRUE
		INSTALL_RPATH "$ORIGIN")
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)
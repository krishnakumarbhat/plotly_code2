#start Cmake for MDF_LOG
##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "Mdf_Log")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#include directories
set(MDFINCLUDE "${COMMON_INC}/MDF_Include")

#Adding required source and header files

list(APPEND Sources "${MDF_LOG_PROJ}/src/apt_mdf_log.cpp")
list(APPEND Header "${MDF_LOG_PROJ}/inc/apt_mdf_log.h")
list(APPEND Header "${MDF_LOG_PROJ}/inc/vector_mdf_licence.h")
list(APPEND Header "${CROSS_PLATFORM_PROJ}/genutil.h") 
source_group(\\HeaderFiles FILES "${Header}inc/apt_mdf_log.h")

#Creating Static Library
add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${MDFINCLUDE})

target_include_directories(${PROJECTNAME} PUBLIC ${MDF_LOG_PROJ}/inc)

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${Sources} ${Header})

target_link_libraries(${PROJECTNAME} PUBLIC CrossPlatform)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)

# Structure Alignment
	if(MSVC)
		target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM)
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)
#End Cmake for MDF_LOG
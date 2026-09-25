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

set(PROJECTNAME  "CrossPlatform")
project(${PROJECTNAME})

set(COMMONUTILITY "${COMMON_INC}/Utility")
#Linking directories
link_directories(${OUTPUT_PATH})

#include custom specific
set(SOURCE_ALL "${COMMONUTILITY}/commonUtils.h")
list(APPEND SOURCE_ALL "${CROSSPLATFORM_PROJ}/commonUtils.cpp")
if(MSVC)
	list(APPEND SOURCE_ALL "${CROSSPLATFORM_PROJ}/windowsUtils.cpp")
else()
	list(APPEND SOURCE_ALL "${CROSSPLATFORM_PROJ}/linuxUtils.cpp")
endif()

add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONUTILITY})

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
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
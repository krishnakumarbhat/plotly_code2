#start CMake for CCA ViGEM
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

set(PROJECTNAME  "CCA_ViGEM")
project(${PROJECTNAME})
add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)
#Linking directories
link_directories(${OUTPUT_PATH})
link_directories("${CORE_LIB_PROJECT}/CCA_ViGEM/bin/lib")


#include directories
set(DVLMESSAGE "${CORE_LIB_PROJECT}/dvlMessage/inc")
set(MDFINCLUDE "${COMMON_INC}/MDF_Include")

#Adding required source and header files
set(SourceFiles "${CCA_ViGEM_PROJ}/")
list(APPEND SOURCE_ALL "${SourceFiles}/src/vgm_cca_log.cpp")

list(APPEND Header "${SourceFiles}/inc/vgm_cca_log.h")

source_group(\\HeaderFiles FILES "${Header}inc/vgm_cca_log.h")

add_definitions(-D_WIN32_LEAN_AND_MEAN)
list(APPEND SOURCE_ALL "${COMMON_INC}/cca_vigem_inc/vgm_cca.h")
list(APPEND SOURCE_ALL "${COMMON_INC}/cca_vigem_inc/vgm_common.h")
list(APPEND SOURCE_ALL "${COMMON_INC}/cca_vigem_inc/vgm_errors.h")

#Creating Static Library
add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE  ${DVLMESSAGE} ${MDFINCLUDE})

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

#link library
target_link_libraries(${PROJECTNAME} CrossPlatform)

set(CCA_LIB_PATH "${CORE_LIB_PROJECT}/CCA_ViGEM/bin/lib")
find_library(cca_lib_4-3-0 ccalib_mdf_x64_4-3-0 HINTS "${CCA_LIB_PATH}")
target_link_libraries(${PROJECTNAME} ${cca_lib_4-3-0})

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												) 				
endif(OUTPUT_PATH)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM)
endif(MSVC)

if(UNIX)

	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
	find_package (Threads)
	target_link_libraries (${PROJECTNAME} ${CMAKE_THREAD_LIBS_INIT})
endif(UNIX)
#End CMake for CCA ViGEM

#start Cmake for DVL_FILE
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

set(PROJECTNAME  "dvlFile")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#Adding required source and header files
set(SorrcesFiles "${DVLFILE_PROJ}/")
list(APPEND SOURCE_ALL "${SorrcesFiles}/src/dvl_class.cpp")
list(APPEND SOURCE_ALL "${SorrcesFiles}/src/dvl_file.cpp")
list(APPEND SOURCE_ALL "${SorrcesFiles}/src/dvl_file_flx.cpp")
list(APPEND SOURCE_ALL "${SorrcesFiles}/src/dvl_file_xtd.cpp")
list(APPEND SOURCE_ALL "${SorrcesFiles}/src/dvl_record.c")
list(APPEND Header "${SorrcesFiles}/inc/new_dvl_file.h")
list(APPEND Header "${SorrcesFiles}/inc/dvl_file.h")
source_group(\\HeaderFiles FILES "${Header}inc/dvl_file.h")

#Creating Static Library
add_library(${PROJECTNAME} STATIC "")

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

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
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)
#End Cmake for DVL_FILE
#start Cmake for MDF_LOG
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

cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "Mdf_Log_Convertor")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#include directories
set(DVLFILE "${CORE_LIB_PROJECT}/dvlFile/inc")
set(DVLMESSAGE "${CORE_LIB_PROJECT}/dvlMessage/inc")
set(MDFINCLUDE "${COMMON_INC}/MDF_Include")

#Adding required source and header files
set(SourceFiles "${MDFLOG_PROJ}/")
list(APPEND SOURCE_ALL "${SourceFiles}/src/apt_mdf_log.cpp")
list(APPEND Header "${SourceFiles}/inc/apt_mdf_log.h")
list(APPEND Header "${SourceFiles}/inc/vector_mdf_licence.h")
source_group(\\HeaderFiles FILES "${Header}inc/apt_mdf_log.h")

#Creating Static Library
add_library(${PROJECTNAME} STATIC "")
add_definitions(-D_CONVERTER_TOOL)
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${DVLFILE} ${DVLMESSAGE} ${MDFINCLUDE})

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

#link library
if(MSVC)
target_link_libraries(${PROJECTNAME} CrossPlatform)
endif(MSVC)

if(UNIX)
target_link_libraries(${PROJECTNAME} LINK_PUBLIC dl pthread CrossPlatform)
endif(UNIX) 

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
	if(MSVC)
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
		target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM)
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
	target_compile_definitions(${PROJECTNAME} PRIVATE PC_RESIM _XKEYCHECK_H -DLINUX)
	target_link_options(${PROJECTNAME} PRIVATE "-Wl,--no-as-needed,--enable-new-dtags")
 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

endif(UNIX) 
 

#End Cmake for MDF_LOG
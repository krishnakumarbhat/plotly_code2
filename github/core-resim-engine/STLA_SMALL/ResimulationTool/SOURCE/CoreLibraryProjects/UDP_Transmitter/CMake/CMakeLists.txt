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

#Set Project Name
set(PROJECTNAME  "UDP_Transmitter")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})
link_directories("${CORE_LIB_PROJECT}/UDP_Transmitter/bin/lib")

#including sources and headers
set(SourceFiles "${UDP_TRANSMIT_PROJ}/")
#list(APPEND HEADERS "${COMMONINC}/DSPACE_Input_Structure.h")
list(APPEND SOURCE_ALL "${SourceFiles}/src/UDP_transmission.cpp")
list(APPEND HEADERS "${SourceFiles}/inc/UDP_transmission.h")
#source_group(\\HeaderFiles FILES "${Header}inc/UDP_transmission.h")

#Creating static lib
add_library(${PROJECTNAME} STATIC "")

#Link source and headers to target

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})
#target_link_libraries(${PROJECTNAME} CrossPlatform)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)							

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													${COMMON_THREADS})

if(MSVC)	
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS __WIN_PLATFORM) 
	
	# /Zi - Create debugging information
	# /Od - Disable optimization
	# /MTd - Statically link the multithreaded debug version of the CRT
	# /MDd - Dynamically link the multithreaded debug version of the CRT
	# /RTC1 - Run time checks
	# /DNDEBUG - Disables asserts
	# /MT - Statically link the multithreaded release version of the CRT
	# /MD - Dynamically link the multithreaded release version of the CRT
	# /O2 - Optimize for speed
	# /Oy - Enable frame pointer omission (FPO) (otherwise CMake will automatically turn it off)
	# /GL - Link time code generation (whole program optimization)
	# /Gy - Function-level linking
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MT /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")

	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MTd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")

	
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		set_target_properties(${PROJECTNAME} PROPERTIES  COMPILE_FLAGS "/Zp2" )
	endif()
endif(MSVC)

if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



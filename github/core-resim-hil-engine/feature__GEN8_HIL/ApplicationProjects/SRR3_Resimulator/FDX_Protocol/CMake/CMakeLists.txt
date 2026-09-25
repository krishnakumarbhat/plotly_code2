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

#Set Project Name
set(PROJECTNAME  "FDX_Protocol")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#including sources and headers
set(SOURCES "${${FDX_PROJ}}")
list(APPEND HEADERS "${FDX_PROJ}/CANoeFDX.h")
list(APPEND HEADERS "${FDX_PROJ}/FDXDatagram.h")
list(APPEND HEADERS "${FDX_PROJ}/FDXDispatcher.h")
list(APPEND HEADERS "${FDX_PROJ}/FDXSocket.h")

list(APPEND SOURCE_ALL "${FDX_PROJ}/FDXDatagram.cpp")
list(APPEND SOURCE_ALL "${FDX_PROJ}/FDXDispatcher.cpp")
list(APPEND SOURCE_ALL "${FDX_PROJ}/FDXSocket.cpp")

#Creating static lib
add_library(${PROJECTNAME} STATIC "")

#Link source and headers to target

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

target_compile_definitions(${PROJECTNAME} PRIVATE SRR3_HIL_EXPORTS
													PC_RESIM
													HIL_MODE
													ENABLE_CUSTHDR
													WIN32_LEAN_AND_MEAN
													WIN32_EXTRA_LEAN)

target_link_libraries(${PROJECTNAME} Ws2_32.lib)													

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													${COMMON_THREADS})

if(MSVC)	
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
	
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

	set_target_properties(${PROJECTNAME} PROPERTIES
                                        COMPILE_FLAGS "/Zp2"
                                        )
endif(MSVC)
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
endif(MSVC)
if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



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

#Set Project Name
set(PROJECTNAME  "srr_comm")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#Setting common include headers
set(COMMON_THREADS "${COMMON_INC}/thread")
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers")
set(COMMONPLUGIN "${COMMON_INC}/plugin")

message("common thread ${COMMON_INC}")

#including sources and headers
set(SOURCES "${SRR3COMM_PROJ}")
list(APPEND SOURCE_ALL "${SOURCES}/srr3_comm.cpp")
list(APPEND SOURCE_ALL "${COMMON_THREADS}/ThBase.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/ThTimeOut.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn.cpp")

list(APPEND SOURCE_ALL "${SOURCES}/resource.h")
list(APPEND SOURCE_ALL "${SOURCES}/srr3_comm.h")
list(APPEND SOURCE_ALL "${SOURCES}/ThTimeOut.h")
list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn.h")
if(MSVC)
	list(APPEND SOURCE_ALL "${SOURCES}/dllmain.cpp")
endif()
#Creating static lib
add_library(${PROJECTNAME} SHARED "")

#Link source and headers to target
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

target_compile_definitions(${PROJECTNAME} PRIVATE SRR3_COMM_EXPORTS)

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
	endif()
endif(MSVC)
if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



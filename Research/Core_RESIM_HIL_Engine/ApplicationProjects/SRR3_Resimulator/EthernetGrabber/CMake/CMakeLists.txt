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
set(PROJECTNAME  "EthernetGrabber")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#Setting common include headers
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONHEADER "${COMMON_INC}/CommonHeaders")
set(COMMONPLUGIN "${COMMON_INC}/plugin")
set(COMMON_THREADS "${COMMON_INC}/thread")
set(LIBPATH "${APP_PROJECT}/SRR3_Resimulator/lib/x64")

#including sources and headers
set(SOURCES "${ETHERNETGRABBER_PROJ}")
if(MSVC)
list(APPEND SOURCE_ALL "${APP_PROJECT}/SRR3_Resimulator/Include/pcap/pcap.h")
list(APPEND SOURCE_ALL "${APP_PROJECT}/SRR3_Resimulator/Include/pcap/bpf.h")
endif()
list(APPEND SOURCE_ALL "${SOURCES}/PacketGrabber.h")
list(APPEND SOURCE_ALL "${SOURCES}/PacketGrabber.cpp")

#Creating static lib
add_library(${PROJECTNAME} STATIC "")

#Link source and headers to target

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

#link include directories
#target_include_directories(${PROJECTNAME} PRIVATE ${LIBPATH})
if(MSVC)
target_link_libraries(${PROJECTNAME} PUBLIC ${LIBPATH}/wpcap.lib)
target_link_libraries(${PROJECTNAME} PUBLIC ${LIBPATH}/Packet.lib)
target_link_libraries(${PROJECTNAME} PUBLIC ${LIBPATH}/MultiCanCardD.lib)
target_link_libraries(${PROJECTNAME} PUBLIC ${LIBPATH}/MultiCanCard.lib)
target_link_libraries(${PROJECTNAME} PUBLIC ${LIBPATH}/dvlFile.lib)
else()
target_link_libraries(${PROJECTNAME} PUBLIC -lpcap )
endif()
set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)	
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS
														_LIB
														_WIN_PLATFORM) 
	
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

	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MT /DNDEBUG /O2 /Oy /GL /GF /Gy ${CRT_FLAG_RELEASE}>")

	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MTd /Zi /Od /GF /Gy ${CRT_FLAG_DEBUG}>")

endif(MSVC)
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)
if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



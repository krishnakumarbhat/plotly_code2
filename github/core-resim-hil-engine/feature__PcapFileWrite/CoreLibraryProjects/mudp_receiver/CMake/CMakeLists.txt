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
set(PROJECTNAME  "SRR_MUDP_Receive")

project(${PROJECTNAME})


#Linking directories
link_directories(${OUTPUT_PATH})

set(SRC_FILE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

#Adding required source and header files
list(APPEND SOURCE_ALL ${SRC_FILE_PATH}/mudp_receive_data.cpp)
list(APPEND SOURCE_ALL ${SRC_FILE_PATH}/mudp_receive_data.h)

add_library (${PROJECTNAME} "")

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR)
target_include_directories(${PROJECTNAME} PRIVATE ${SRC_FILE_PATH})

if(MSVC)
target_link_libraries(${PROJECTNAME} Timing_Profile)
else()
	set(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")
	target_link_libraries (${PROJECTNAME} Timing_Profile  ${CMAKE_DL_LIBS}) #${CMAKE_DL_LIBS} for dlopen and dlclose lib for Linux build
endif(MSVC)


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
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)
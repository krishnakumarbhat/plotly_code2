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
set(proj_name "Timing_Profile")

PROJECT (${proj_name})

#include files
set(SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Timing_Profile.cpp")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Timing_Profile_Lib_Loader.cpp")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Timing_Profile.h")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Timing_Profile_Lib_Loader.h")

# adding library
if(BUILD_LIME)
message("Building static logger library")
add_library(${proj_name} STATIC "")
else()
message("Building shared logger library")
add_library(${proj_name} SHARED "")
endif(BUILD_LIME)

#add_library(${proj_name} SHARED "")

target_sources(${proj_name} PRIVATE ${SOURCE_ALL})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	SET(CMAKE_CXX_FLAGS -pthread)
endif()

if(OUTPUT_PATH)
set_target_properties(${proj_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
											    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
											    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)


if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${proj_name} PRIVATE  "WINDOWS_IGNORE_PACKING_MISMATCH") 
	else()
		target_compile_options(${proj_name} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${proj_name} PRIVATE  _WIN_PLATFORM) 

		target_compile_options(${proj_name} PRIVATE "$<$<CONFIG:Release>:/MD /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")
		 target_compile_options(${proj_name} PRIVATE "$<$<CONFIG:Debug>:/MDd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")	
	
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
	
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${proj_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${proj_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)

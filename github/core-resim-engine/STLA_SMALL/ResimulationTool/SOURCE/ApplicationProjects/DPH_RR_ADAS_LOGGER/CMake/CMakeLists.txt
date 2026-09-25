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

set(PROJECTNAME  "DPH_RR_ADAS_LOGGER")
project(${PROJECTNAME})

#include directories
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")

#Linking directories
link_directories(${OUTPUT_PATH})

#include sources
set(SourceFiles "${DPH_RR_ADAS_LOGGER_PROJ}/")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_logger_lib_loader.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_logger.cpp")
list(APPEND SOURCE_ALL "${COMMONHEADERS}/dph_rr_adas_logger.h")
list(APPEND SOURCE_ALL "${COMMONHEADERS}/dph_rr_adas_logger_lib_loader.h")

# adding library
if(BUILD_LIME)
message("Building static logger library")
add_library(${PROJECTNAME} STATIC "")
else()
message("Building shared logger library")
add_library(${PROJECTNAME} SHARED "")
endif(BUILD_LIME)
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONHEADERS})

#link source files
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

#link library
target_link_libraries(${PROJECTNAME} CrossPlatform)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	SET(CMAKE_CXX_FLAGS -pthread)
endif()

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 

	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MD /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")
    target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MDd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")	
	
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
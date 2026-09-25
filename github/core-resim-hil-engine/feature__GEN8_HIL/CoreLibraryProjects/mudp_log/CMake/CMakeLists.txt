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

if("${buildConfigToUpper}" STREQUAL "SIL")	
	set(PROJECTNAME  "SRR3_MUDP_Log")
else()
	set(PROJECTNAME  "SRR_MUDP_Log")
endif()
project(${PROJECTNAME})


#Linking directories
link_directories(${OUTPUT_PATH})

#Adding required source and header files
set(SourceFiles "${MUDP_LOG_PROJ}/")
list(APPEND SOURCE_ALL ${SourceFiles}/mudp_log.cpp )
list(APPEND SOURCE_ALL ${SourceFiles}/MudpRecord/mudpLog.cpp)
list(APPEND SOURCE_ALL ${SourceFiles}/MudpRecord/RawLog.cpp)
list(APPEND SOURCE_ALL ${SourceFiles}/sectionedlog.cpp)
list(APPEND SOURCE_ALL ${SourceFiles}/DvsuRecord/SRR3UDPLog.cpp )
list(APPEND SOURCE_ALL ${SourceFiles}/mudp_log.h )
list(APPEND SOURCE_ALL ${SourceFiles}/MudpRecord/mudpLog.cpp)
list(APPEND SOURCE_ALL ${SourceFiles}/resource.h )
list(APPEND SOURCE_ALL ${SourceFiles}/sectionedlog.h )
list(APPEND SOURCE_ALL ${SourceFiles}/DvsuRecord/SRR3UDPLog.h )
list(APPEND SOURCE_ALL ${SourceFiles}/DvsuRecord/SRR3UDPRec.h )

if(UNIX)
add_definitions(${CMAKE_CXX_FLAGS} "-fPIC")
endif()

add_library (${PROJECTNAME} SHARED "")

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR MUDP_LOG_EXPORTS)

target_link_libraries (${PROJECTNAME} CrossPlatform)

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
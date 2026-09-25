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

cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME  "ecu_sync")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})
link_directories("${CORE_LIB_PROJECT}/ecu_sync/bin/lib")


#include directories
set(DVLFILE "${CORE_LIB_PROJECT}/dvlFile/inc")
set(DVLMESSAGE "${CORE_LIB_PROJECT}/dvlMessage/inc")
set(MDFINCLUDE "${COMMON_INC}/MDF_Include")

#Adding required source and header files
set(SourceFiles "${ECU_SYNC_PROJ}/")
list(APPEND SOURCE_ALL "${SourceFiles}/src/ecu_sync.cpp")
list(APPEND Header "${SourceFiles}/inc/ecu_sync.h")
#list(APPEND Header "${SourceFiles}/inc/vgm_cca_log.h")
#list(APPEND Header "${SourceFiles}/inc/vgm_common.h")
#list(APPEND Header "${SourceFiles}/inc/vgm_errors.h")
source_group(\\HeaderFiles FILES "${Header}inc/ecu_sync.h")


#Creating Static Library
add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PUBLIC ${DVLFILE} 
												${DVLMESSAGE} 
												${MDFINCLUDE} 
												${ECU_SYNC_PROJ}/inc
												)

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

# #set(LIBPATH  "${SourceFiles}/")
# link_directories(${SourceFiles})

#add_library( mylib SHARED IMPORTED )
#set_target_properties( mylib PROPERTIES IMPORTED_LOCATION /bin/lib/ccalib_mdf_x64_2-1-3.lib)

#link library
target_link_libraries(${PROJECTNAME} DC_RESIM_IFACE)


if(MSVC)
#TARGET_LINK_LIBRARIES(${PROJECTNAME} mylib)
#target_link_libraries(${PROJECTNAME} /bin/lib/ccalib_mdf_x64_2-1-3)
endif(MSVC)
if(UNIX)
#target_link_libraries(${PROJECTNAME} ccalib_mdf_x64_2-1-3)
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

	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
	TARGET_LINK_LIBRARIES( ${PROJECTNAME} -pthread)
	add_compile_options(-lpthread)
endif(UNIX)

#End Cmake for MDF_LOG
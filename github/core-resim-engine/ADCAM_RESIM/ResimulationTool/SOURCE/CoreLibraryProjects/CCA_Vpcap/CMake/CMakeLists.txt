#start Cmake for CCA VPCAP
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

set(PROJECTNAME  "CCA_Vpcap")
project(${PROJECTNAME})


#Linking directories
link_directories(${OUTPUT_PATH})

#Adding required source and header files
set(SourceFiles "${CCA_VPCAP_PROJ}/")
list(APPEND SOURCE_ALL "${SourceFiles}/src/vgm_cca_vpcap.cpp")

list(APPEND Header "${SourceFiles}/inc/vgm_cca_vpcap.h")
source_group(\\HeaderFiles FILES "${Header}inc/vgm_cca_vpcap.h")

list(APPEND SOURCE_ALL "${COMMON_INC}/cca_vigem_inc/vgm_cca.h")
list(APPEND SOURCE_ALL "${COMMON_INC}/cca_vigem_inc/vgm_common.h")
list(APPEND SOURCE_ALL "${COMMON_INC}/cca_vigem_inc/vgm_errors.h")

link_directories(${SourceFiles}/bin)

#Creating Static Library
add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${DVLFILE} ${DVLMESSAGE} ${MDFINCLUDE})
target_include_directories(${PROJECTNAME} PRIVATE ${SourceFiles}/inc)

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

#link library
target_link_libraries(${PROJECTNAME} CrossPlatform)

set(CCA_LIB_PATH "${CORE_LIB_PROJECT}/CCA_ViGEM/bin/lib")
find_library(cca_lib_4-7-6 ccalib_mdf_x64_4-7-6 HINTS "${CCA_LIB_PATH}")
#target_link_libraries(${PROJECTNAME} ${cca_lib_2-1-6})

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
endif(UNIX)
#End Cmake for CCA VPCAP
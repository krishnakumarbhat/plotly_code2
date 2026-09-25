#start CMake for CCA ViGEM
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

set(PROJECTNAME  "Video_Decoder")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

set(SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Video_Decoder.cpp")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Video_Decoder_Lib_Loader.cpp")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Video_Decoder.h")
list(APPEND SOURCE_ALL "${CMAKE_CURRENT_SOURCE_DIR}/../Video_Decoder_Lib_Loader.h")

#Adding required source and header files
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/ApplicationProjects/MDF4_Decoder/CCA Framework/v4.7.6/include/vgm_cca.h")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/ApplicationProjects/MDF4_Decoder/CCA Framework/v4.7.6/include/vgm_common.h")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/ApplicationProjects/MDF4_Decoder/CCA Framework/v4.7.6/include/vgm_errors.h")

#Creating Static Library
add_library(${PROJECTNAME} SHARED "")

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${Header})

#set(CCA_LIB_PATH "${CORE_LIB_PROJECT}/CCA_ViGEM/bin/lib")
find_library(ccalib_mdf_x64_4-7-6 ccalib_mdf_x64_4-7-6 HINTS "${SOURCE_PROJECT}/ApplicationProjects/MDF4_Decoder/CCA Framework/v4.7.6/lib")
target_link_libraries(${PROJECTNAME} ccalib_mdf_x64_4-7-6)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
	if(MSVC)
		message(${PROJECTNAME})
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
		target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM WINDOWS_IGNORE_PACKING_MISMATCH)
	endif(MSVC)

#End CMake for CCA ViGEM
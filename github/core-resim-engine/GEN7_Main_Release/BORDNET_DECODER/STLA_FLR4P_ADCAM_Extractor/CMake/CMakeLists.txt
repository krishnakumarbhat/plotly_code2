cmake_minimum_required(VERSION 4.0)

set(project_name "STLA_FLR4P_ADCAM_Extractor")
project(${project_name})

set(target_name ${project_name})

add_library(${target_name} STATIC "")


set(PRJ_ROOT_PATH "${STLA_FLR4P_ADCAM_EXTRACTOR_PROJ}")
set(SOURCE_PATH "${PRJ_ROOT_PATH}/Source")
set(INCLUDE_PATH "${PRJ_ROOT_PATH}/Include")
set(INCLUDE_INTERFACE_PATH "${PRJ_ROOT_PATH}/../STLA_FLR4P_ADCAM_Decoder")
set(COMMON_STRUCTURES_PATH "${PRJ_ROOT_PATH}/../Common_Structures")

list(APPEND SOURCE_FILES "${SOURCE_PATH}/STLA_FLR4P_ADCAM_Decoder.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/STLA_FLR4P_ADCAM_Base.cpp")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/STLA_FLR4P_ADCAM_Decoder.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/STLA_FLR4P_ADCAMM_Base.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/STLA_FLR4P_ADCAM_Base.h")


list(APPEND SOURCE_FILES "${SOURCE_PATH}/STLA_FLR4P_ADCAM_Decoder_UPCluster.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/STLA_FLR4P_ADCAM_Base_UPCluster.cpp")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/STLA_FLR4P_ADCAM_Decoder_UPCluster.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/STLA_FLR4P_ADCAMM_Base_UPCluster.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/STLA_FLR4P_ADCAM_Base_UPCLuster.h")

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

target_sources(${target_name} PRIVATE ${SOURCE_FILES})
target_include_directories(${target_name} PRIVATE ${INCLUDE_PATH})
target_include_directories(${target_name} PRIVATE ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${INCLUDE_INTERFACE_PATH})
target_include_directories(${target_name} PUBLIC ${COMMON_STRUCTURES_PATH})

target_link_libraries(${target_name} CrossPlatform)
if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${project_name} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)
if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
	
	

	find_package (Threads)
	target_link_libraries (${target_name} ${CMAKE_THREAD_LIBS_INIT})

endif(UNIX)


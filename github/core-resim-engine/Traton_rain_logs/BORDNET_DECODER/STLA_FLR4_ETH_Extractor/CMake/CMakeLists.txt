##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "STLA_FLR4_ETH_Extractor")
project(${project_name})

set(STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(STLA_FLR4_ETHERNET_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(STLA_FLR4_ETHERNET_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_FLR4_ETH_Decoder")

set(SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/Include/STLA_FLR4_EthDecoder.h")
list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/Source/STLA_FLR4_EthDecoder.cpp")


list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Source/STLA_FLR4_ETHBase.cpp")
list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Include/STLA_FLR4_ETHBase.h")
list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Source/STLA_FLR4_ETHBase_V60.cpp")
list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Include/STLA_FLR4_ETHBase_V60.h")
list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Include/STLA_FLR4_Base_V60.h")
list(APPEND SOURCES "${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Include/STLA_FLR4_Base.h")

source_group(TREE ${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/STLA_FLR4_ETHERNET_FC/Include)


target_include_directories(${target_name} PUBLIC ${STLA_FLR4_ETHERNET_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${STLA_FLR4_ETHERNET_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${STLA_FLR4_ETHERNET_INTERFACEFILES})


target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

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


# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)


if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)
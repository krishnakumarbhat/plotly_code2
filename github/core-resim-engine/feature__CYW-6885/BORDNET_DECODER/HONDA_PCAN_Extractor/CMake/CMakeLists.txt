##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "HONDA_PCAN_Extractor")
project(${project_name})

set(HONDA_PCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(HONDA_PCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HONDA_PCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_PCAN_RL_Decoder")
set(HONDA_PCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_PCAN_RR_Decoder")
set(HONDA_PCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_PCAN_FR_Decoder")
set(HONDA_PCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_PCAN_FL_Decoder")

set(SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Include/HondaPcanDecoderRL.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Source/HondaPcanDecoderRL.cpp")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Include/HondaPcanDecoderRR.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Source/HondaPcanDecoderRR.cpp")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Include/HondaPcanDecoderFR.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Source/HondaPcanDecoderFR.cpp")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Include/HondaPcanDecoderFL.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/Source/HondaPcanDecoderFL.cpp")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RL/Source/HondePcanRLBase.cpp")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RL/Include/HondaPcanBaseRL.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RL/Include/HondePcanRLBase.h")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RR/Source/HondePcanRRBase.cpp")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RR/Include/HondaPcanBaseRR.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RR/Include/HondePcanRRBase.h")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FR/Source/HondePcanFRBase.cpp")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FR/Include/HondaPcanBaseFR.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FR/Include/HondePcanFRBase.h")

list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FL/Source/HondePcanFLBase.cpp")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FL/Include/HondaPcanBaseFL.h")
list(APPEND SOURCES "${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FL/Include/HondePcanFLBase.h")

source_group(TREE ${HONDA_PCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_PROJECT_ROOT_PATH}/HONDA_PCAN_FL/Include)


target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${HONDA_PCAN_FL_INTERFACEFILES})


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
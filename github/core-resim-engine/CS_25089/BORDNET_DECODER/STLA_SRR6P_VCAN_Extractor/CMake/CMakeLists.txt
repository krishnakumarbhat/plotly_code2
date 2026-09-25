##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(project_name  "STLA_SRR6P_VCAN_Extractor")
project(${project_name})

set(STLA_SRR6P_VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(STLA_SRR6P_VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(STLA_SRR6P_VCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SRR6P_VCAN_RL_Decoder")
set(STLA_SRR6P_VCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SRR6P_VCAN_RR_Decoder")
set(STLA_SRR6P_VCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SRR6P_VCAN_FR_Decoder")
set(STLA_SRR6P_VCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SRR6P_VCAN_FL_Decoder")

set(SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SRR6P_VcanDecoderRL.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SRR6P_VcanDecoderRL.cpp")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SRR6P_VcanDecoderRR.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SRR6P_VcanDecoderRR.cpp")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SRR6P_VcanDecoderFR.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SRR6P_VcanDecoderFR.cpp")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SRR6P_VcanDecoderFL.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SRR6P_VcanDecoderFL.cpp")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RL/Source/STLA_SRR6P_VcanRLBase.cpp")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RL/Include/STLA_SRR6P_VcanBaseRL.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RL/Include/STLA_SRR6P_VcanRLBase.h")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RR/Source/STLA_SRR6P_VcanRRBase.cpp")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RR/Include/STLA_SRR6P_VcanBaseRR.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RR/Include/STLA_SRR6P_VcanRRBase.h")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FR/Source/STLA_SRR6P_VcanFRBase.cpp")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FR/Include/STLA_SRR6P_VcanBaseFR.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FR/Include/STLA_SRR6P_VcanFRBase.h")

list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FL/Source/STLA_SRR6P_VcanFLBase.cpp")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FL/Include/STLA_SRR6P_VcanBaseFL.h")
list(APPEND SOURCES "${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FL/Include/STLA_SRR6P_VcanFLBase.h")

source_group(TREE ${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/STLA_SRR6P_VCAN_FL/Include)


target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${STLA_SRR6P_VCAN_FL_INTERFACEFILES})


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
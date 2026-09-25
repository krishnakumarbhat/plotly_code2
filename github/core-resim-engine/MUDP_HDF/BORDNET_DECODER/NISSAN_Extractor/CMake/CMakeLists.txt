##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "NISSAN_Extractor")
project(${project_name})

set(NISSAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(NISSAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(NISSAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../NISSAN_FL_Decoder")
set(NISSAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../NISSAN_FR_Decoder")
set(NISSAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../NISSAN_RL_Decoder")
set(NISSAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../NISSAN_RR_Decoder")

set(SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Include/NissanDecoderFL.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Source/NissanDecoderFL.cpp")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Include/NissanDecoderFR.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Source/NissanDecoderFR.cpp")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Include/NissanDecoderRL.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Source/NissanDecoderRL.cpp")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Include/NissanDecoderRR.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/Source/NissanDecoderRR.cpp")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FL/Source/NissanBaseFL.cpp")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FL/Include/NissanBaseFrontLeft.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FL/Include/NissanBaseFL.h")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FR/Source/NissanBaseFR.cpp")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FR/Include/NissanBaseFrontRight.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FR/Include/NissanBaseFR.h")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RL/Source/NissanBaseRL.cpp")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RL/Include/NissanBaseRearLeft.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RL/Include/NissanBaseRL.h")

list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RR/Source/NissanBaseRR.cpp")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RR/Include/NissanBaseRearRight.h")
list(APPEND SOURCES "${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RR/Include/NissanBaseRR.h")

source_group(TREE ${NISSAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FL/Include)
target_include_directories(${target_name} PUBLIC ${NISSAN_PROJECT_ROOT_PATH}/NISSAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${NISSAN_PROJECT_ROOT_PATH}/NISSAN_RR/Include)


target_include_directories(${target_name} PUBLIC ${NISSAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${NISSAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${NISSAN_FL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${NISSAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${NISSAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${NISSAN_RR_INTERFACEFILES})


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
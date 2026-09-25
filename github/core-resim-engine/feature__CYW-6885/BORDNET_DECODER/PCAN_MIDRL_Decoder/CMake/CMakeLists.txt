##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "PCAN_MIDRL_Decoder")
project(${project_name})

set(PCAN_MID_RL_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

##include custom specific
set(SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/CanDecoderBase.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/IPCAN_MIDRL_DecoderAPI.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A310.cpp")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A310.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A390.cpp")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A390.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A410.cpp")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A410.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A480.cpp")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A480.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A430_SOP3.cpp")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCAN_Decoder_A430_SOP3.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCANDecoderAPI.cpp")
#list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/PCANDecoderBase.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/Version.h")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/Resource.rc")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/ReleaseNotes_PCANMIDRL.txt")
list(APPEND SOURCES "${PCAN_MID_RL_PROJECT_ROOT_PATH}/CommonHeaders.h")

set(target_name ${project_name})
add_library(${target_name} SHARED "")

target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCES})

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _WIN_PLATFORM) 
endif(MSVC)

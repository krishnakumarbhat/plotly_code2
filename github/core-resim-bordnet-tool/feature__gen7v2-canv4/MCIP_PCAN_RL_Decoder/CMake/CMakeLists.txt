##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "MCIP_PCAN_RL_Decoder")
project(${PROJECTNAME})


set(MCIP_PCAN_RL_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL"${MCIP_PCAN_RL_DECODER_ROOT_PATH}/IMCIP_PCAN_RL_DecoderAPI.h")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/MCIP_PCAN_RL_DecoderAPI.cpp")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/MCIP_PCAN_RL_Decoder_V2.h")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/MCIP_PCAN_RL_Common_V2.h")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/MCIPPCANRLDecoderBase_V2.h")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/IMCIP_PCAN_RL_Decoder_V2.h")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/MCIP_PCAN_RL_Decoder_V2.cpp")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/ReleaseNotes_MCIP_PCAN_RL.txt")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${MCIP_PCAN_RL_DECODER_ROOT_PATH}/Version.h")



set(target_name ${PROJECTNAME})
add_library(${target_name} SHARED "")


target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCE_ALL})

target_link_libraries(${PROJECTNAME} PRIVATE Auto_Gen_Files)

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}")												 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)

##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "STLA_SRR6P_VCAN_RR_Decoder")
project(${PROJECTNAME})


set(STLA_VCAN_RR_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/STLA_SRR6P_VCAN_RR_Decoder.h")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/STLA_SRR6P_VCANRRDecoderBase.h")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/ISTLA_SRR6P_VCAN_RR_Decoder.h")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/ISTLA_SRR6P_VCAN_RR_DecoderAPI.h")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/ReleaseNotes_STLA_SRR6P_VCAN_RR.txt")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/STLA_SRR6P_VCAN_RR_Decoder.cpp")
list(APPEND SOURCE_ALL "${STLA_VCAN_RR_DECODER_ROOT_PATH}/STLA_SRR6P_VCAN_RR_DecoderAPI.cpp")



set(target_name ${PROJECTNAME})
add_library(${target_name} SHARED "")


target_include_directories(${target_name} PRIVATE ${COMMON_STRUCTURES})

target_sources(${target_name} PRIVATE ${SOURCE_ALL})

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}")												 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)

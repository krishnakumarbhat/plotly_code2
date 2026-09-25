##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "AL_VCAN_FLR_Decoder")
project(${PROJECTNAME})


set(AL_VCAN_FLR_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
list(APPEND SOURCE_ALL "${CEER_VCAN_FLR_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${CEER_VCAN_FLR_DECODER_ROOT_PATH}/Version.h")

#v1_1
set(SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/AL_VCAN_FLR_Decoder_V1_1.h")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/AL_VCAN_FLR_Decoder_V1_1.h")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/AL_VCAN_FLR_Common_V1_1.h")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/ALVCANFLRDecoderBase_V1_1.h")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/IAL_VCAN_FLR_Decoder_V1_1.h")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/AL_VCAN_FLR_Decoder_V1_1.cpp")

list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/AL_VCAN_FLR_DecoderAPI.cpp")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/IAL_VCAN_FLR_DecoderAPI.h")
list(APPEND SOURCE_ALL "${AL_VCAN_FLR_DECODER_ROOT_PATH}/ReleaseNotes_AL_VCAN_FLR.txt")

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

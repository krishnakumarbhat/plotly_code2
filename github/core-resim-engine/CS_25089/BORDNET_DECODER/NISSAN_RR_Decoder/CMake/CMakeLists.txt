##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "NISSAN_RR_Decoder")
project(${PROJECTNAME})


set(NISSAN_RR_DECODER_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/NISSAN_RR_Decoder.h")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/NISSAN_RR_Common.h")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/INISSANRRDecoder.h")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/NISSANDecoderRRBase.h")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/INISSANRRDecoderAPI.h")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/Resource.rc")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/Version.h")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/ReleaseNotes_NISSAN_RR.txt")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/NISSAN_RR_Decoder.cpp")
list(APPEND SOURCE_ALL "${NISSAN_RR_DECODER_ROOT_PATH}/NISSANRR_DecoderAPI.cpp")



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

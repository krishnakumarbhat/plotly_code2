##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "Auto_Gen_Files")
project(${project_name})

set(Auto_Gen_Files_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(Auto_Gen_Files_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(Auto_Gen_Files_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../Auto_Gen_Files")

#CEER_V1
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_FR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_RL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V1/autogen_CEER_PCAN_RR_Decoder_DBC_definition.cpp")

#CEER V2
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Front_V2_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_SRR_Rear_V2_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2/autogen_CEER_PCAN_FLR_V2_Decoder_DBC_definition.h")

#MCIP V2
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Front_V2_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_SRR_Rear_V2_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V2/autogen_MCIP_PCAN_FLR_V2_Decoder_DBC_definition.h")

#MCIP V4
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Front_V4_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_SRR_Rear_V4_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4/autogen_MCIP_PCAN_FLR_V4_Decoder_DBC_definition.h")

#CEER V3
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Front_V3_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_SRR_Rear_V3_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3/autogen_CEER_PCAN_FLR_V3_Decoder_DBC_definition.h")

#CEER V4
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Front_V4_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_SRR_Rear_V4_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4/autogen_CEER_PCAN_FLR_V4_Decoder_DBC_definition.h")

source_group(TREE ${Auto_Gen_Files_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V2)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V3)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/CEER_V4)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/MCIP_V4)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include)


target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_INTERFACEFILES})


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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
	target_compile_options(${target_name} PRIVATE -fPIC)

endif(UNIX)
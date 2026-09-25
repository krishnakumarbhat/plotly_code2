##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "Auto_Gen_Files")
project(${project_name})

set(Auto_Gen_Files_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(Auto_Gen_Files_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(Auto_Gen_Files_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../Auto_Gen_Files")
#set(Auto_Gen_Files_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../../Auto_Gen_Files")

set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_definition.h")
set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_definition.h")

set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FL_Decoder_DBC_definition.h")

set(SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RL_Decoder_DBC_definition.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_definition.cpp")


list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_definition.cpp")


# RNA V1
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_FL_V1_Decoder_DBC_message_decoding.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_FR_V1_Decoder_DBC_message_decoding.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_RL_V1_Decoder_DBC_message_decoding.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_V1/autogen_RNA_VCAN_RR_V1_Decoder_DBC_message_decoding.cpp")

               #============================HONDA_V12=====================================

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FL_Decoder_DBC_V12_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_FR_Decoder_DBC_V12_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RL_Decoder_DBC_V12_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12/autogen_HONDA_VCAN_RR_Decoder_DBC_V12_signal_macros.h")

               #============================HONDA_V12========================================

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_TRATON_VCAN_RL_Decoder_DBC_definition.cpp")
		#======================================C1_3===========================

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_signal_macros.h")
		
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_3/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_3_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_3/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_3_encode_messages.cpp")

		#======================================C1_2===========================

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_encode_messages.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_FC_Decoder_DBC_C1_2_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RL_Decoder_DBC_C1_2_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2/autogen_TRATON_VCAN_RR_Decoder_DBC_C1_2_signal_macros.h")

		#======================================C1_2===========================

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_RL_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_FL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_RR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_FL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_FR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_RL_Decoder_DBC_message_decoding.cpp")
# list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_RL_Decoder_DBC_xml_print_functions.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RL_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_RR_Decoder_DBC_message_decoding.cpp")
#list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_RR_Decoder_DBC_xml_print_functions.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_RR_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_FL_Decoder_DBC_message_decoding.cpp")
#list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_FL_Decoder_DBC_xml_print_functions.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FL_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_FR_Decoder_DBC_message_decoding.cpp")
#list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_HONDA_VCAN_FR_Decoder_DBC_xml_print_functions.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_HONDA_VCAN_FR_Decoder_DBC_signal_macros.h")


#================================================== Tarton ==================

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_TRATON_VCAN_RR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_FC_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_TRATON_VCAN_FC_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_TRATON_VCAN_RL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RL_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_TRATON_VCAN_RR_Decoder_DBC_message_decoding.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_RR_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_TRATON_VCAN_FC_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_FC_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_FC_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_TRATON_VCAN_FC_Decoder_DBC_signal_macros.h")

#RNA


list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_FR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_RL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_RR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_FL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_FL_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FL_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_FR_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_FR_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_FR_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_RL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_RL_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RL_Decoder_DBC_signal_macros.h")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_RR_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_RNA_VCAN_RR_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_RNA_VCAN_RR_Decoder_DBC_signal_macros.h")

#CEER

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_FL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_FL_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FL_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_FL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_FR_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_FR_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FR_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_FR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_FR_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_RL_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_RL_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RL_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RL_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RL_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RL_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_RL_Decoder_DBC_definition.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_RR_Decoder_DBC_message_decoding.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_RR_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RR_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RR_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RR_Decoder_DBC_signal_macros.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/autogen_CEER_PCAN_RR_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/autogen_CEER_PCAN_RR_Decoder_DBC_definition.cpp")

#RNA RT RANGE
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_definition.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_encode_messages.cpp")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Source/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_message_decoding.cpp")

list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_definition.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_message_decode_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_message_encoded_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_RT_RANGE_D1_0/autogen_RNA_RT_Range_Decoder_DBC_signal_macros.h")


list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/lib_types.h")
list(APPEND SOURCES "${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/types.h")



source_group(TREE ${Auto_Gen_Files_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_RT_RANGE_D1_0)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_3)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/C1_2)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/HONDA_V12)
target_include_directories(${target_name} PUBLIC ${Auto_Gen_Files_PROJECT_ROOT_PATH}/Include/RNA_V1)
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
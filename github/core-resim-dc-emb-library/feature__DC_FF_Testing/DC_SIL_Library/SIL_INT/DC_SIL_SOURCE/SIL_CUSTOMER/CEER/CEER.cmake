##########################################################################################################
#	This .cmake will handlebelow cases
# 			1. setting path of Customer dependent directories
#			2. Upconversion handling
#				1. CAF_Version_Handling
#				2. Tracker_internal_Handling
# Created by : d1cse7 (Mandeep.singh1@aptiv.com)
##########################################################################################################
cmake_minimum_required(VERSION 3.10)

message("[INFO] : SIL_SOURCE_CUSTOMER linking started for ${CUST}")

# setting path to all needed SIL_CUSTOMER Directories
set(RECU_SIL_CUST_SIL_XML_TRACE     	"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SIL_XML_Trace")
set(RECU_SIL_CUST_SIL_CSV_TRACE     	"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SIL_CSV_Trace")
set(RECU_SIL_CUST_SIL_INCLUDE     		"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SIL/Include")
set(RECU_SIL_CUST_SIL_SOURCE     		"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SIL/Source")
set(RECU_SOMEIP_PATH					"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SOME_IP")
set(RECU_SIL_SOURCE_SOMEIP_PLP			"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SOMEIP_Sync")
set(RECU_SIL_SOURCE_CUST_PCAN 			"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/PCAN")
set(RECU_SIL_SOURCE_CUST_PCANSYNC 		"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/PCANSync")
set(RECU_SIL_SOURCE_VCAN				"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/VCAN")
set(RECU_SIL_SOURCE_UPCONVERSION		"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/Upconversion")
set(RECU_SIL_SOURCE_UPCONVERSION_SOMEIP	"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/Upconversion/SOMIP_UpConversion")
set(RECU_SIL_CUST_HDF_TRACE				"${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}/SIL_HDF_Trace")

# append needed .cpp and .h files to SOURCE_ALL
list(APPEND SOURCE_ALL 			"${RECU_SIL_CUST_SIL_INCLUDE}/SIL_Customer_Cal_Updates.h")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CUST_SIL_INCLUDE}/SIL_Fill_ECU_Input.h")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CUST_SIL_SOURCE}/SIL_Customer_Cal_Updates.cpp")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CUST_SIL_SOURCE}/SIL_Fill_ECU_Input.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_SIL_XML_TRACE}/Event_Logger.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_SIL_XML_TRACE}/SIL_Statistics_Node.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_SIL_CSV_TRACE}/UDP_trace.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_SIL_CSV_TRACE}/DFT_trace.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_SIL_XML_TRACE}/Valid_Data_to_XML.cpp")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/e2e_some_ip.c")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/e2e_some_ip.h")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/some_ip_output.c")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/some_ip_input.c")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/some_ip_input.h")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/some_ip_output.h")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/qualifier.h")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/qualifiers.c")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/someip_EthHdrUpdate.c")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/someip_EthHdrUpdate.h")
list(APPEND SOURCE_ALL			"${RECU_SOMEIP_PATH}/DFT_Sfl.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCAN}/SIL_iface_pcan_read.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCAN}/SIL_iface_pcan_read.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCANSYNC}/PCAN_sync.c")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCANSYNC}/PCAN_sync.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCANSYNC}/recu_diag_print.c")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCANSYNC}/recu_diag_print.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCANSYNC}/recu_diag_print_c.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_CUST_PCANSYNC}/recu_diag_print_cpp.h")
list(APPEND SOURCE_ALL          "${RECU_SIL_SOURCE_UPCONVERSION_SOMEIP}/Upconversion.cpp")
list(APPEND SOURCE_ALL          "${RECU_SIL_SOURCE_UPCONVERSION_SOMEIP}/Upconversion.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_VCAN}/vcan_output.c")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_SOMEIP_PLP}/SOMEIP_PLP_sync.c")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_SOMEIP_PLP}/SOMEIP_PLP_sync.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_HDF_TRACE}/HDF_Trace.h")
list(APPEND SOURCE_ALL			"${RECU_SIL_CUST_HDF_TRACE}/HDF_Trace.cpp")
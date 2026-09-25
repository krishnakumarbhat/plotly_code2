##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 3.10)

message("[INFO] : SIL_SOURCE_CORE linking started for ${CUST}")

link_directories(${OUTPUT_PATH})
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../HDF/lib")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../HDF/bin")



set(PROJECTNAME  "${CUST}_DC_RESIM")
project(${PROJECTNAME})
set(target_name ${PROJECTNAME})
set(ROOT_PATH 							"${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
set(RECU_SIL_CORE_SIL_INCLUDE			"${CMAKE_CURRENT_SOURCE_DIR}/../SIL/Include")
set(RECU_SIL_CORE_SIL_SOURCE			"${CMAKE_CURRENT_SOURCE_DIR}/../SIL/Source")
set(RECU_SIL_DEPENDENCIES_INC_PATH		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/Include")
set(RECU_ADCAM_PATH						"${CMAKE_CURRENT_SOURCE_DIR}/../ADCAM")

set(RECU_SIL_SOURCE_CORE					"${ROOT_PATH}/DC_SIL_Library/SIL_INT/DC_SIL_SOURCE/SIL_CORE")
set(RECU_SIL_SOURCE_CORE_INCLUDE 			"${RECU_SIL_SOURCE_CORE}/SIL/Include")
set(RECU_SIL_SOURCE_CORE_DEPENDENCIES_INC 	"${RECU_SIL_SOURCE_CORE}/Dependencies/Include")
set(RECU_SIL_SOURCE_CORE_COMMON				"${RECU_SIL_SOURCE_CORE}/Common")
set(RECU_SIL_SOURCE_WRAPPER_PLP				"${RECU_SIL_SOURCE_CORE}/PLP")
set(RECU_SIL_SOURCE_ERROR_CODE				"${RECU_SIL_SOURCE_CORE}/Error_Codes")
set(RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET	"${RECU_SIL_SOURCE_CORE}/DGPS_Decoding/Ethernet")
set(RECU_SIL_SOURCE_DGPS_DECODING_CAN		"${RECU_SIL_SOURCE_CORE}/DGPS_Decoding/CAN")
set(RECU_SIL_SOURCE_DGPS_DECODING_COMMON	"${RECU_SIL_SOURCE_CORE}/DGPS_Decoding/common_iface")


set(RECU_SIL_CUSTOMER     "${CMAKE_CURRENT_SOURCE_DIR}/../../SIL_CUSTOMER/${CUST}")

if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	set(BUILD_FOLDER_TYPE "Release")
	set(protolibname "libprotobuf.a")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	set(BUILD_FOLDER_TYPE "Debug")
	set(protolibname "libprotobufd.a")
endif()

if(MSVC)
	set(OS_FOLDER_TYPE "Windows")
else()
	set(OS_FOLDER_TYPE "Linux")
	set(CMAKE_CXX_FLAGS "-fPIC")
endif()

#Set path to Interface Folder
SET(SIL_SMIFACE_INCLUDE "${RECU_SIL_SOURCE_CORE}/SMIface/inc")
SET(SIL_SMIFACE_SOURCE "${RECU_SIL_SOURCE_CORE}/SMIface/src")

#Set Libs path
set(LIBS_DIR "${RECU_SIL_SOURCE_CORE}/SMIface/Libs")
# set Protobuf and OSI library versions
set(ProtoLibraryVersion "protobuf-3.6.1")
set(OSILibraryVersion "open-simulation-interface-3.5.0_w_Proto_3.6.1")
# set Protobuf and OSI library Includes
set(OSILibraryIncludes "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/include/osi3")
set(ProtoLibraryIncludes "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	add_definitions(${CMAKE_CXX_FLAGS} "-rdynamic")
	add_definitions(${CMAKE_CFLAGS} "-rdynamic")
endif()

if(${CUST} STREQUAL "CEER")
	set(UDP_LOGGING_STREAM					"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator")
	set(UDP_LOGGING_STREAM_C0				"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core0")
	set(UDP_LOGGING_STREAM_C1				"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core1")
	set(UDP_LOGGING_STREAM_C1INTERNAL		"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core1_internal")
	set(UDP_LOGGING_STREAM_C2				"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core2")
	set(UDP_LOGGING_STREAM_C2VRU			"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core2_VRU")
	set(UDP_LOGGING_STREAM_C3				"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core3")
	set(UDP_LOGGING_STREAM_C4CALIB			"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Core4_Calibration")
	set(UDP_LOGGING_STREAM_OSI_LIME         "${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/OSI_LIME")
	
	set(UDP_LOGGING_CORE_PATH			"${ROOT_PATH}/Application/UDP_Logging/Core")
	set(UDP_LOGGING_CORE_INC_PATH		"${UDP_LOGGING_CORE_PATH}/inc")
	set(UDP_LOGGING_CORE_SRC_PATH		"${UDP_LOGGING_CORE_PATH}/src")
	
	set(UDP_LOGGING_CUSTOMER_PATH		"${ROOT_PATH}/Application/UDP_Logging/Customer/BMW")
	set(UDP_LOGGING_CUSTOMER_INC_PATH	"${UDP_LOGGING_CUSTOMER_PATH}/inc")
	set(UDP_LOGGING_CUSTOMER_SRC_PATH	"${UDP_LOGGING_CUSTOMER_PATH}/src")
	set(UDP_LOGGING_STREAM_DSPACE           "${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/Dspace")
endif()

# A5 streams for GEN7
set(G7_UDP_ROOT_PATH			"${CMAKE_CURRENT_SOURCE_DIR}/../Component_Streams/A5")

SET(pugixml_PROJ 				"${CMAKE_CURRENT_SOURCE_DIR}/../pugixml-1.10")
SET(hdf_PROJ 					"${CMAKE_CURRENT_SOURCE_DIR}/../HDF")
SET(HDF5_INC 					"${CMAKE_CURRENT_SOURCE_DIR}/../HDF/include")
SET(RECU_SFL_COMMON_INC         "${Source_path}/Application/FeatureFunctions/Interface/Include")
#Add files to project
set(SOURCE_ALL 					"${RECU_SIL_CORE_SIL_INCLUDE}/${CUST}_SIL_Wrapper.h")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CORE_SIL_INCLUDE}/udp_bmw_cals.h")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CORE_SIL_INCLUDE}/DC_validator.h")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CORE_SIL_INCLUDE}/SIL_Radar_cals.h")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CORE_SIL_SOURCE}/${CUST}_SIL_Wrapper.cpp")
list(APPEND SOURCE_ALL 			"${RECU_SIL_CORE_SIL_SOURCE}/DC_validator.cpp")
#Add DGPS files to project
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/datatypes.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/GenericSMIface.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/DGPSSMIface.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/smiface_common_macros.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/GenericSensorDLL.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/RadarSensorDLL.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/SensorModelIface.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/SM2Customer.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/SM2SensorType.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/SM2SensorVariant.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/Z7BLogSMIface.h")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/GenericSMIface.cpp")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/DGPSSMIface.cpp")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/GenericSensorDLL.cpp")
	list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/RadarSensorDLL.cpp")
	##########################################################################################################

	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/lib_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/types_.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/DGPS_Msg_Data.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/Endianness_conversion.h")
	
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/src/Endianness_conversion.cpp")
		
	# source file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/src/DGPS_Eth_decoding.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/src/DGPS_Eth_signal_debug_print.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/src/Eth_header_decoder.cpp")
	# header file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc/Eth_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc/DGPS_Eth_decoding.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc/Eth_header_decoder.h")
	
	# source file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_NCOM_class_def.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_NCOM_message_decoding.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_RCOM_class_def.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_RCOM_message_decoding.cpp")
	# header file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_class_definition.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_message_decode_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_message_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_signal_macros.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_class_definition.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_message_decode_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_message_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_signal_macros.h")

	# source file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/iface/src/DGPS_CAN_decoding.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/iface/src/DGPS_CAN_signal_debug_print.cpp")
	
	# header file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/iface/inc/DGPS_CAN_decoding.h")
	
	# source file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/src/autogen_RT_RANGE_CAN_definition.cpp")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/src/autogen_RT_RANGE_CAN_message_decoding.cpp")
	# header file
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/inc/autogen_RT_RANGE_CAN_definition.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/inc/autogen_RT_RANGE_CAN_message_decode_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/inc/autogen_RT_RANGE_CAN_message_encoded_types.h")
	list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/inc/autogen_RT_RANGE_CAN_signal_macros.h")
	
	#list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/Z7BLogSMIface.cpp")
	list(APPEND SOURCE_ALL "${G7_UDP_ROOT_PATH}/ROT_object_stream.h")
	list(APPEND SOURCE_ALL "${G7_UDP_ROOT_PATH}/stream_handler.h")
	list(APPEND SOURCE_ALL "${G7_UDP_ROOT_PATH}/stream_handler.cpp")

list(APPEND SOURCE_ALL 			"${RECU_SIL_CORE_SIL_SOURCE}/SIL_Radar_cals.cpp")
list(APPEND SOURCE_ALL			"${RECU_SIL_SOURCE_WRAPPER_PLP}/Wrapper_PLP.cpp")
list(APPEND SOURCE_ALL          "${RECU_SIL_SOURCE_ERROR_CODE}/Source/SIL_Error_Codes.cpp")
list(APPEND SOURCE_ALL          "${RECU_SIL_SOURCE_ERROR_CODE}/Include/SIL_Error_Codes.h")
list(APPEND SOURCE_ALL          "${RECU_SIL_SOURCE_ERROR_CODE}/Include/apt_sil_error_enum.h")
list(APPEND SOURCE_ALL          "${RECU_ADCAM_PATH}/ADCAM_Lane_Input.c")
list(APPEND SOURCE_ALL          "${RECU_ADCAM_PATH}/ADCAM_Lane_Input.h")
list(APPEND SOURCE_ALL			"${CMAKE_CURRENT_SOURCE_DIR}/../Emb_Lib_Output_Control/Include/Emb_Lib_Output_Control.h")
list(APPEND SOURCE_ALL			"${CMAKE_CURRENT_SOURCE_DIR}/../Emb_Lib_Output_Control/Source/Emb_Lib_Output_Control.cpp")


list(APPEND highfive "${hdf_PROJ}/highfive")


list(APPEND highfive "${hdf_PROJ}/highfive/H5PropertyList.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Reference.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Selection.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Utility.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Version.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Version.hpp.in")
list(APPEND highfive "${hdf_PROJ}/highfive/half_float.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/highfive.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/span.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/xtensor.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/boost.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/eigen.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Attribute.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5DataSet.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5DataSpace.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5DataType.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Easy.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Exception.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5File.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Group.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Object.hpp")


#list(APPEND highfive "${HDF5_INC}/hdf5.h")

#Creating Static Library
add_library(${target_name} STATIC "")

# calling ${CUST} CmakeLists.cmake.
# All customer dependent files path set and append to SOURCE_ALL is taken care there
include("${RECU_SIL_CUSTOMER}/${CUST}.cmake")

set(UDP_CORE_INC_FILES 			"${UDP_LOGGING_CORE_INC_PATH}/crc32.h")
list(APPEND UDP_CORE_INC_FILES 	"${UDP_LOGGING_CORE_INC_PATH}/UDP_logging_data.h")
list(APPEND UDP_CORE_INC_FILES 	"${UDP_LOGGING_CORE_INC_PATH}/udp_stream_log.h")

set(UDP_CORE_SRC_FILES 			"${UDP_LOGGING_CORE_SRC_PATH}/crc32.c")
list(APPEND UDP_CORE_SRC_FILES 		"${UDP_LOGGING_CORE_SRC_PATH}/UDP_logging_data.c")
set(UDP_CUSTOMER_INC_FILES			"${UDP_LOGGING_CUSTOMER_INC_PATH}/UDP_CAL_Logging_Buffer.h")
list(APPEND UDP_CUSTOMER_INC_FILES	"${UDP_LOGGING_CUSTOMER_INC_PATH}/UDP_TrackerIAL_Logging_Buffer.h")
list(APPEND UDP_CUSTOMER_INC_FILES	"${UDP_LOGGING_CUSTOMER_INC_PATH}/UDP_TrackerOAL_Logging_Buffer.h")
list(APPEND UDP_CUSTOMER_INC_FILES	"${UDP_LOGGING_CUSTOMER_INC_PATH}/UDP_TrackerInternal_Loggng_Buffer.h")

set(UDP_CUSTOMER_SRC_FILES			"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_CAL_Logging_Buffer.c")
list(APPEND UDP_CUSTOMER_SRC_FILES	"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_FFOut_Logging_Buffer.c")
list(APPEND UDP_CUSTOMER_SRC_FILES	"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_TrackerIAL_Logging_Buffer.c")
list(APPEND UDP_CUSTOMER_SRC_FILES	"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_TrackerOAL_Logging_Buffer.c")
list(APPEND UDP_CUSTOMER_SRC_FILES	"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_TrackerInternal_Loggng_Buffer.c")
list(APPEND UDP_CUSTOMER_SRC_FILES	"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_VRU_ML_Logging_Buffer.c")
list(APPEND UDP_CUSTOMER_SRC_FILES	"${UDP_LOGGING_CUSTOMER_SRC_PATH}/UDP_OSI_Logging_Buffer.c")

set(UDP_STREAM_SRC_FILES			"${UDP_LOGGING_STREAM}/Core3/radar_ecu_CORE3.c")
list(APPEND UDP_STREAM_SRC_FILES	"${UDP_LOGGING_STREAM}/Core0/radar_ecu_CORE0.c")
list(APPEND UDP_STREAM_SRC_FILES	"${UDP_LOGGING_STREAM}/Core1/radar_ecu_CORE1.c")
list(APPEND UDP_STREAM_SRC_FILES	"${UDP_LOGGING_STREAM_C2VRU}/radar_ecu_vru_classifier.c")
list(APPEND UDP_STREAM_SRC_FILES	"${UDP_LOGGING_STREAM_OSI_LIME}/OSI_Input_Structure.c")
#list(APPEND UDP_STREAM_SRC_FILES	"${RECU_SFL_COMMON_INC}/common/Endian_Swap.c")

# Activate grouping in Project in accordance with the folder structure of the files above
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/../.. FILES ${SOURCE_ALL})

# Always enable BIN_FILES
set(BINARY_DEBUG_ACTIVE ON)
if(BINARY_DEBUG_ACTIVE)
	target_compile_definitions(${target_name} PUBLIC BINARY_DEBUG)
endif(BINARY_DEBUG_ACTIVE)
target_compile_definitions(${target_name} PRIVATE -DENABLE_DEBUGGING)
target_compile_definitions(${target_name} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_compile_definitions(${target_name} PRIVATE -DWINDOWS_IGNORE_PACKING_MISMATCH)
target_compile_definitions(${target_name} PRIVATE -DH5_BUILT_AS_DYNAMIC_LIB)
if(CUSTOMER)
target_compile_definitions(${target_name} PRIVATE -DCUSTOMER_${CUSTOMER})
endif(CUSTOMER)

if(NOT CMAKE_CROSSCOMPILING)
	target_compile_definitions(${target_name} PRIVATE -DPC_RESIM)
endif(NOT CMAKE_CROSSCOMPILING)

add_subdirectory("${pugixml_PROJ}" "${CMAKE_CURRENT_BINARY_DIR}/pugixml_PROJ/")
add_library(DC_RESIM_IFACE INTERFACE)
#link include directories

#target_include_directories(${PROJECTNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Emb_Lib_Output_Control/Include)
target_include_directories(DC_RESIM_IFACE INTERFACE 	
							${RECU_SIL_SOURCE_CORE}/Emb_Lib_Output_Control/Include
							${pugixml_PROJ}
							${hdf_PROJ}
							${HDF5_INC}
							${RECU_SIL_SOURCE_CORE_INCLUDE}
							${RECU_SIL_SOURCE_CORE_DEPENDENCIES_INC}
							${RECU_SIL_CORE_SIL_INCLUDE}
							${RECU_SIL_CUST_SIL_INCLUDE}
							${RECU_SIL_SOURCE_CORE_COMMON}
							${UDP_LOGGING_CORE_SRC_PATH}
							${UDP_LOGGING_CORE_INC_PATH}
							${UDP_LOGGING_STREAM_C0}
							${UDP_LOGGING_STREAM_C1}
							${UDP_LOGGING_STREAM_C1INTERNAL}
							${UDP_LOGGING_STREAM_C2}
							${UDP_LOGGING_STREAM_C3}
							${UDP_LOGGING_STREAM_C2VRU}
							${UDP_LOGGING_STREAM_C4CALIB}
							${UDP_LOGGING_STREAM_OSI_LIME}
							${UDP_LOGGING_STREAM_DSPACE}
							${UDP_LOGGING_CUSTOMER_INC_PATH}
							${RECU_SIL_CUST_SIL_XML_TRACE}
							${RECU_SIL_DEPENDENCIES_INC_PATH}
							${RECU_SOMEIP_PATH}
							${RECU_ADCAM_PATH}
							${RECU_SIL_SOURCE_CUST_PCAN}
							${RECU_SIL_SOURCE_CUST_PCANSYNC}
							${RECU_SIL_SOURCE_VCAN}
							${RECU_SIL_SOURCE_SOMEIP_PLP}
							${RECU_SIL_SOURCE_WRAPPER_PLP}
							${RECU_SIL_SOURCE_ERROR_CODE}/Include
							${RECU_SIL_SOURCE_UPCONVERSION_SOMEIP}
							${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc
							${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/iface/inc
							${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc
							${RECU_SIL_SOURCE_DGPS_DECODING_CAN}/Autogen/inc
							${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc
							${RECU_SFL_COMMON_INC}
							${G7_UDP_ROOT_PATH}
							${ROOT_PATH}/Application/FeatureFunctions/SFL_Interface
							)
							
target_include_directories(DC_RESIM_IFACE INTERFACE 	
								${OSILibraryIncludes}
								${ProtoLibraryIncludes}							
								${SIL_SMIFACE_INCLUDE})
								
add_library(UDP_LOGGING_IFACE INTERFACE)
target_include_directories(UDP_LOGGING_IFACE INTERFACE  ${UDP_LOGGING_CUSTOMER_INC_PATH}  ${UDP_LOGGING_STREAM_C0}
		${UDP_LOGGING_STREAM_C1} ${UDP_LOGGING_STREAM_C1INTERNAL} ${UDP_LOGGING_STREAM_C2} ${UDP_LOGGING_STREAM_C3} 
		${UDP_LOGGING_STREAM_C3VRU}  ${UDP_LOGGING_STREAM_C4CALIB} ${UDP_LOGGING_STREAM_OSI_LIME} ${UDP_LOGGING_STREAM_DSPACE})
												
target_sources(${target_name} PRIVATE ${SOURCE_ALL} ${UDP_CORE_INC_FILES}  ${UDP_CORE_SRC_FILES} ${RECU_SIL_SOURCE_CORE_COMMON}
										${UDP_CUSTOMER_INC_FILES}	${UDP_CUSTOMER_SRC_FILES}	${UDP_STREAM_SRC_FILES} ${highfive}
										)
									
target_include_directories(${target_name} PRIVATE ${RECU_SIL_CORE_SIL_INCLUDE} 
												  ${ROOT_PATH}/Application/FeatureFunctions/SFL_Interface
												  ${highfive})
												  
# OSI Stream if set as an option in cmake then only OSI streams linked to project default is ON	
option(OSI_STREAM "Set the OSI stream" ON)									
if(OSI_STREAM)
	message(STATUS "Build Contains OSI_STREAM")
	set(UDP_LOGGING_OSI_STREAM	"${ROOT_PATH}/Application/UDP_Logging/Stream_Generator/OSI_LIME")
	add_library(UDP_LOGGING_OSI_IFACE INTERFACE)
	target_include_directories(UDP_LOGGING_OSI_IFACE INTERFACE  ${UDP_LOGGING_OSI_STREAM}/OSI_Input_Structure.h
																#${UDP_LOGGING_OSI_STREAM}/OSI_Input_Structure.c
																)
	target_sources(${target_name} PRIVATE ${UDP_LOGGING_OSI_STREAM}/OSI_Input_Structure.c)
	target_link_libraries(${target_name} UDP_LOGGING_OSI_IFACE)
endif()

if(MSVC)
	set(HDF_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../HDF/lib")
else()
	set(HDF_LIB_DIR_UNIX "${CMAKE_CURRENT_SOURCE_DIR}/../HDF/lib")
endif()

if(MSVC)
	target_link_libraries(${target_name} ${HDF_LIB_DIR}/zlib-static.lib
										 ${HDF_LIB_DIR}/hdf5.lib
										 ${HDF_LIB_DIR}/hdf5_cpp.lib
										 ${HDF_LIB_DIR}/hdf5_hl.lib
										 ${HDF_LIB_DIR}/hdf5_hl_cpp.lib
										 ${HDF_LIB_DIR}/hdf5_java.lib	
		                                 ${HDF_LIB_DIR}/hdf5_tools.lib
										 )

else()	
	target_link_libraries(${PROJECTNAME} ${HDF_LIB_DIR_UNIX}/libhdf5.so
										 ${HDF_LIB_DIR_UNIX}/libhdf5_cpp.so
										 )
endif()

# Section to link all needed module Libraries with DC_RESIM
# Common Libraries linking for all Customers
target_link_libraries(${target_name} 	AS-bin-writer-lib)
target_link_libraries(${target_name}	ecu_sync)
target_link_libraries(${target_name} 	DC_RESIM_IFACE)
target_link_libraries(${target_name} 	UDP_LOGGING_IFACE)
target_link_libraries(${target_name}	pugixml)
target_link_libraries(${target_name} 	Mdf_Log_Ecu)

# customer dependent Libraries linking
if(${CUST} STREQUAL "CEER")
	target_link_libraries(${target_name} 	CED)
	target_link_libraries(${target_name} 	LCDA)
	target_link_libraries(${target_name} 	CTA)
	target_link_libraries(${target_name} 	PT)
	target_link_libraries(${target_name} 	TA)
	target_link_libraries(${target_name}    RECW)
	target_link_libraries(${target_name}    SCW)
	target_link_libraries(${target_name}    LTB)
	target_link_libraries(${target_name}    ESA)
	target_link_libraries(${target_name} 	sp_25_tracker_wrapper)
	target_link_libraries(${target_name} 	MLMathLibrary)
	target_link_libraries(${target_name}	VSE_Core)
	target_link_libraries(${target_name}	SFL_Interface)

endif()

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
			RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
			ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _XKEYCHECK_H)
endif(MSVC)

#Enable COM_MAT_KW20_A290_A310 if required to run A310 software
#Enable COM_MAT_KW35_A330_A350 if required to run A350 software
target_compile_definitions(${target_name} PRIVATE COM_MAT_KW35_A330_A350)

set(VARIANT_TRACKER "f360_variant_A")
set(RSPP_VARIANT_TRACKER "rspp_variant_A")
add_definitions(-Df360_variant_A=${VARIANT_TRACKER})
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_TRACKER})

cmake_minimum_required(VERSION 3.15.0)

project(Dgps_Decoding)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_FOLDER "Dgps_Decoding")
# set default install folder to cmake build folder
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

if(MSVC)
	set(OS_FOLDER_TYPE "Windows")
else()
	set(OS_FOLDER_TYPE "Linux")
	set(CMAKE_CXX_FLAGS "-fPIC")
endif()

# Including osi libs
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	set(BUILD_FOLDER_TYPE "Release")
	set(protolibname "libprotobuf.a")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	set(BUILD_FOLDER_TYPE "Debug")
	set(protolibname "libprotobufd.a")
endif()

# DGPS Files
set(SIL_SMIFACE_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/SM_iface/inc")
set(SIL_SMIFACE_SOURCE "${CMAKE_CURRENT_LIST_DIR}/SM_iface/src")
set(RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET	"${CMAKE_CURRENT_LIST_DIR}/ethernet")
set(RECU_SIL_SOURCE_DGPS_DECODING_CAN		"${CMAKE_CURRENT_LIST_DIR}/can")
set(RECU_SIL_SOURCE_DGPS_DECODING_COMMON	"${CMAKE_CURRENT_LIST_DIR}/common_iface")
# set Libs path
set(LIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/SM_iface/Libs")
# set Protobuf and OSI library versions
set(ProtoLibraryVersion "protobuf-3.6.1")
set(OSILibraryVersion "open-simulation-interface-3.5.0_w_Proto_3.6.1")
# set Protobuf and OSI library Includes
set(OSILibraryIncludes "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/include/osi3")
set(ProtoLibraryIncludes "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/include")

# SMIface files
set(SOURCE_ALL "${SIL_SMIFACE_INCLUDE}/datatypes.h")

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
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/DSPACE_Common_Input_Structure.h")
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_INCLUDE}/OSI_Input_Structure.h")
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/GenericSMIface.cpp")
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/DGPSSMIface.cpp")
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/GenericSensorDLL.cpp")
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/RadarSensorDLL.cpp")
list(APPEND SOURCE_ALL			"${SIL_SMIFACE_SOURCE}/DSPACE_Common_Input_Structure.cpp")
##########################################################################################################
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/lib_types.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/types_.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/DGPS_Msg_Data.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc/Endianness_conversion.h")

list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/src/Endianness_conversion.cpp")
##########################################################################################################

# Ethernet source file
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/src/DGPS_Eth_decoding.cpp")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/src/DGPS_Eth_signal_debug_print.cpp")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/src/Eth_header_decoder.cpp")

# Ethernet header file
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc/Eth_types.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc/DGPS_Eth_decoding.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc/Eth_header_decoder.h")

# Autogen source file
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_NCOM_class_def.cpp")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_NCOM_message_decoding.cpp")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_RCOM_class_def.cpp")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/src/autogen_RCOM_message_decoding.cpp")
# Autogen header file
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_class_definition.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_message_decode_types.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_message_types.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_NCOM_signal_macros.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_class_definition.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_message_decode_types.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_message_types.h")
list(APPEND SOURCE_ALL "${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc/autogen_RCOM_signal_macros.h")

#Creating Static Library
add_library(Dgps_Decoding STATIC "")

target_sources(Dgps_Decoding PUBLIC ${SOURCE_ALL})
target_include_directories(Dgps_Decoding PUBLIC ${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/Autogen/inc
												 ${RECU_SIL_SOURCE_DGPS_DECODING_ETHERNET}/iface/inc
												 ${RECU_SIL_SOURCE_DGPS_DECODING_COMMON}/inc
												 ${SIL_SMIFACE_INCLUDE}
												 ${OSILibraryIncludes}
												 ${ProtoLibraryIncludes}
												 )
												 
target_include_directories(Dgps_Decoding PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../sil_wrappers
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_input
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_output
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_debug_file
												 ${CMAKE_CURRENT_LIST_DIR}/../../Application/F360Tracker/rspp/include
												 ${CMAKE_CURRENT_LIST_DIR}/../../Application/F360Tracker/VSE_Core
												 ${CMAKE_CURRENT_LIST_DIR}/../../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/core
												 ${CMAKE_CURRENT_LIST_DIR}/../../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/Logging
												 ${CMAKE_CURRENT_LIST_DIR}/../../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/Types
												 ${CMAKE_CURRENT_LIST_DIR}/../../Application/F360Tracker/OLP_Core/includes
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_engine_headers
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_smc
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_smc/sil_autogen
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_error_codes
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_config_read
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_ext_libs/inc
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_ext_libs/AS_BIN_WRITER
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_ext_libs/AS_BIN_WRITER/ports/visualC12
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_ext_libs/pugixml-1.10/src
												 ${CMAKE_CURRENT_LIST_DIR}/../sil_component_streams/A5)
												 
target_link_libraries(Dgps_Decoding PRIVATE F360-Tracker
											rspp
											occupancy_grid
											stationary_geometries
											VSE_Core
											autogen_smc
											OLP_Core
											)

# link protobuf and osi libraries
if(MSVC)
	set(PROTOBUF_DEBUG_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/Windows/Debug")
	set(PROTOBUF_RELEASE_LIB_PATH "${LIBS_DIR}/${ProtoLibraryVersion}/Windows/Release")
	set(OSI_DEBUG_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/Windows/Debug")
	set(OSI_RELEASE_LIB_PATH "${LIBS_DIR}/${OSILibraryVersion}/Windows/Release")
	find_library(PROTOBUF_DEBUG_LIB libprotobufd PATHS "${PROTOBUF_DEBUG_LIB_PATH}/lib")
	find_library(PROTOBUF_RELEASE_LIB libprotobuf PATHS "${PROTOBUF_RELEASE_LIB_PATH}/lib")
	find_library(OSI_STATIC_DEBUG_LIB open_simulation_interface_static PATHS "${OSI_DEBUG_LIB_PATH}/lib/osi3")
	find_library(OSI_STATIC_RELEASE_LIB open_simulation_interface_static PATHS "${OSI_RELEASE_LIB_PATH}/lib/osi3")
	target_link_libraries(Dgps_Decoding PUBLIC
						$<$<CONFIG:Debug>:${PROTOBUF_DEBUG_LIB}>
            		$<$<CONFIG:Release>:${PROTOBUF_RELEASE_LIB}>					
						$<$<CONFIG:Debug>:${OSI_STATIC_DEBUG_LIB}>
            		$<$<CONFIG:Release>:${OSI_STATIC_RELEASE_LIB}>
						shlwapi.lib
						)
else()
	set(CMAKE_CXX_FLAGS "-fPIC")
	get_filename_component(ProtobufStaticLib "${LIBS_DIR}/${ProtoLibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/${protolibname}" ABSOLUTE)
	get_filename_component(OpenSimulationInterfaceStaticLib "${LIBS_DIR}/${OSILibraryVersion}/${OS_FOLDER_TYPE}/${BUILD_FOLDER_TYPE}/lib/osi3/libopen_simulation_interface_static.a" ABSOLUTE)
	target_link_libraries(Dgps_Decoding PRIVATE ${OpenSimulationInterfaceStaticLib})
	target_link_libraries(Dgps_Decoding PRIVATE ${ProtobufStaticLib} )
	target_link_libraries(Dgps_Decoding PRIVATE -lpthread -static-libgcc -static-libstdc++ stdc++fs)	
endif()

target_compile_definitions(Dgps_Decoding PUBLIC _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
target_compile_definitions(Dgps_Decoding PUBLIC _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
target_compile_definitions(Dgps_Decoding PRIVATE SRR_DC)

set_target_properties(Dgps_Decoding PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN,-Bsymbolic")

# Activate grouping in Project in accordance with the folder structure of the files above
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/.. FILES ${SOURCE_ALL})
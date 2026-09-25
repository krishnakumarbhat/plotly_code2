#Build Steps
#Complier MSVC

#Debug Mode:
#	Step1 cmake -G "Visual Studio 11 Win64" ..\CMake
#	Step2 cmake --build . --target ALL_BUILD --config Debug

#Compiler GNU CXX
#	Step1 cmake ../CMake
#	Step2 make

##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "radar_stream_decoder")
project(${PROJECTNAME})

# Setting Source projects
set(STREAM_DECODER_PROJ "${CMAKE_CURRENT_LIST_DIR}/..")

# set path for Common includes
set(COMMON_INC "${STREAM_DECODER_PROJ}/CommonFiles")

#Adding required common header files
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONFUNCHEADERS "${COMMON_INC}/inc/BMW/sFunctionheader")
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")
set(COMMONINCAUDI "${COMMON_INC}/inc/AUDI")
set(HDF5_INC "${STREAM_DECODER_PROJ}/HDF/include")
add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)

#Linking directories
link_directories("${STREAM_DECODER_PROJ}/HDF/lib")
link_directories("${STREAM_DECODER_PROJ}/HDF/bin")

#link_directories(${OUTPUT_PATH})
set(OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/output/${CMAKE_VS_PLATFORM_NAME}")

#including sources and headers

set(SourceFiles "${STREAM_DECODER_PROJ}")
#message(${STREAM_DECODER_PROJ})
#message(${SourceFiles})
message(${COMMON_INC})
set(COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/IRadarStream.cpp")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/Z_Logging.cpp")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/IRadarStream.h")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/Z_Logging.h")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/IRadarStream_Version.h")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/CDC_Logging.h")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/CDC_Logging.cpp")
list(APPEND COMMON_SOURCE "${COMMONHELPER}/compute.c")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/IRadarStream_Lib_Loader.h")
list(APPEND COMMON_SOURCE "${STREAM_DECODER_PROJ}/Common/IRadarStream_Lib_Loader.cpp")

source_group(\\HDF FILES ${HDF})

#-----------------------------------------------------------------------------------------------------------------------------------------------
#GEN6P Dspace Source file
list(APPEND GEN6P_DSPACE	 "${SourceFiles}/GEN6P_DSPACE/GEN6P_DSPACE_Logging.cpp"
								 "${SourceFiles}/GEN6P_DSPACE/GEN6P_DSPACE_Logging.h")	
						
#Grouping GEN6P source file
source_group(\\GEN6P_DSPACE FILES ${GEN6P_DSPACE})

#GEN6P source file
list(APPEND GEN6P_V2				"${SourceFiles}/GEN6P_DSPACE/V2/DSPACE_GEN6P_Logging_V2.cpp"
									"${SourceFiles}/GEN6P_DSPACE/V2/DSPACE_GEN6P_Logging_V2.h"
									"${SourceFiles}/GEN6P_DSPACE/V2/DSPACE_Common_Input_Structure.h")
									
list(APPEND GEN6P_V3				"${SourceFiles}/GEN6P_DSPACE/V3/DSPACE_GEN6P_Logging_V3.cpp"
									"${SourceFiles}/GEN6P_DSPACE/V3/DSPACE_GEN6P_Logging_V3.h"
									"${SourceFiles}/GEN6P_DSPACE/V3/DSPACE_Common_Input_Structure.h")

#Grouping GEN6P Source file
source_group(\\GEN6P_DSPACE\\V2  FILES ${GEN6P_V2})
source_group(\\GEN6P_DSPACE\\V3  FILES ${GEN6P_V3})
#-----------------------------------------------------------------------------------------------------------------------------------------------
#GEN7 Streams 
#GPO_V1
#SRR7 Streams
#GEN7_GPO_V1 ALIGNMENT Stream Source File
list(APPEND GEN7_GPO_V1_SRR7_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/Alignment_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GEN7_GPO_V1 ALIGNMENT Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ALIGNMENT FILES ${GEN7_GPO_V1_SRR7_ALIGNMENT_SOURCE})

#GEN7_GPO_V1 BLOCKAGE Stream Source File
list(APPEND GEN7_GPO_V1_SRR7_BLOCKAGE_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/BLOCKAGE/Blockage_Logging.cpp"
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/BLOCKAGE/Blockage_Logging.h")
					
#Grouping GEN7_GPO_V1 BLOCKAGE Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\BLOCKAGE FILES ${GEN7_GPO_V1_SRR7_BLOCKAGE_SOURCE})

#GEN7_GPO_V1 CALIB Stream Source File
list(APPEND GEN7_GPO_V1_SRR7_CALIB_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/CALIB/Calib_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/CALIB/Calib_Logging.h")
					
#Grouping GEN7_GPO_V1 CALIB Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\CALIB FILES ${GEN7_GPO_V1_SRR7_CALIB_SOURCE})

#GEN7_GPO_V1 CDC Stream Source File
list(APPEND GEN7_GPO_V1_SRR7_CDC_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/CDC/CDC_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/CDC/CDC_Logging.h")
					
#Grouping GEN7_GPO_V1 CDC Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\CDC FILES ${GEN7_GPO_V1_SRR7_CDC_SOURCE})


#GEN7_GPO_V1 DETECTION Stream Source File
list(APPEND GEN7_GPO_V1_SRR7_DET_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/Detection_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/Detection_Logging.h")
					
#Grouping GEN7_GPO_V1 DETECTION Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION FILES ${GEN7_GPO_V1_SRR7_DET_SOURCE})


#GEN7_GPO_V1 HEADER Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_HDR_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/header_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/header_Logging.h")

#Grouping GEN7_GPO_V1 HEADER Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER FILES ${GEN7_GPO_V1_SRR7_HDR_SOURCE})

#GEN7_GPO_V1 ID Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ID_SOURCE 		    "${SourceFiles}/GEN7/GPO_V1/SRR7/ID/ID_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/ID_Logging.h")

#Grouping GEN7_GPO_V1 ID Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ID FILES ${GEN7_GPO_V1_SRR7_ID_SOURCE})


#GEN7_GPO_V1 RDD Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_RDD_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/RDD_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/RDD_Logging.h")

#Grouping GEN7_GPO_V1 RDD Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD FILES ${GEN7_GPO_V1_SRR7_RDD_SOURCE})

#GEN7_GPO_V1 STATUS Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_STATUS_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/Status_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/Status_Logging.h")

#Grouping GEN7_GPO_V1 STATUS Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\STATUS   FILES ${GEN7_GPO_V1_SRR7_STATUS_SOURCE})

#GEN7_GPO_V1 DEBUG Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_DEBUG_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/Debug_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/Debug_Logging.h")

#Grouping GEN7_GPO_V1 DEBUG Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\DEBUG   FILES ${GEN7_GPO_V1_SRR7_DEBUG_SOURCE})

#GEN7_GPO_V1 DOWN_SELECTION Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/Down_Selection_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/Down_Selection_Logging.h")

#Grouping GEN7_GPO_V1 DOWN_SELECTION Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION   FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_SOURCE})

#GEN7_GPO_V1 DYNAMIC ALIGNMENT Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_SOURCE 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.h")

#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\DYNAMIC_ALIGNMENT   FILES ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_SOURCE})

#GEN7_GPO_V1 VSE Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_VSE_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/VSE_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/VSE_Logging.h")

#Grouping GEN7_GPO_V1 VSE Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\VSE   FILES ${GEN7_GPO_V1_SRR7_VSE_SOURCE})

#GEN7_GPO_V1 MMIC Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_MMIC_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/MMIC_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/MMIC_Logging.h")

#Grouping GEN7_GPO_V1 MMIC Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC   FILES ${GEN7_GPO_V1_SRR7_MMIC_SOURCE})

#GEN7_GPO_V1 RADAR_CAPABILITY Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/Radar_Capability_Logging.h")

#Grouping GEN7_GPO_V1 RADAR_CAPABILITY Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY   FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_SOURCE})

#GEN7_GPO_V1 ROT_TRACKER_INFO Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.h")

#Grouping GEN7_GPO_V1 ROT_TRACKER_INFO Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_TRACKER_INFO FILES ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_SOURCE})

#GEN7_GPO_V1 ROT_VEHICLE_INFO Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.h")

#Grouping GEN7_GPO_V1 ROT_VEHICLE_INFO Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_VEHICLE_INFO   FILES ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_SOURCE})

#GEN7_GPO_V1 ROT_SAFETY_FAULTS Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.h")

#Grouping GEN7_GPO_V1 ROT_SAFETY_FAULTS Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_SOURCE})

#GEN7_GPO_V1 ROT_OBJECT Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/ROT_Object_Stream_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/ROT_Object_Stream_Logging.h")

#Grouping GEN7_GPO_V1 ROT_OBJECT Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_OBJECT_STREAM  FILES ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_SOURCE})

#GEN7_GPO_V1 ROT_ISOobject Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.cpp"
												                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.h")

#Grouping GEN7_GPO_V1 ROT_ISOobject Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_STREAM  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_STREAM_SOURCE})

#GEN7_GPO_V1 ROT Processed Detection Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.cpp"
												                        "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.h")

#Grouping GEN7_GPO_V1 ROT Processed Detection Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_PROCESSEDDETECTION_STREAM  FILES ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE})

#GEN7_GPO_V1 TOI Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_TOI_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/TOI_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/TOI_Logging.h")

#Grouping GEN7_GPO_V1 TOI Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI   FILES ${GEN7_GPO_V1_SRR7_TOI_SOURCE})

#GEN7_GPO_V1 TOI DS Stream Source File												
list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/TOI_DS_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/TOI_DS_Logging.h")

#Grouping GEN7_GPO_V1 TOI Stream Source File
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS   FILES ${GEN7_GPO_V1_SRR7_TOI_DS_SOURCE})

#GEN7_GPO_V1 ALIGNMENT source file
list(APPEND GEN7_GPO_V1_SRR7_ALIGNMENT_V9 	"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V9/alignment_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V9/alignment_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V9/alignment_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_ALIGNMENT_V10 	"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V10/alignment_stream_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V10/alignment_stream_hdf_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V10/alignment_stream_V10.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V10/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V10/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_ALIGNMENT_V11 	"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V11/alignment_stream_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V11/alignment_stream_hdf_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V11/alignment_stream_V11.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V11/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/ALIGNMENT/V11/radar_sw_config.h")											

#Grouping GEN7_GPO_V1 ALIGNMENT source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ALIGNMENT\\V9  FILES ${GEN7_GPO_V1_SRR7_ALIGNMENT_V9})
source_group(\\GEN7\\GPO_V1\\SRR7\\ALIGNMENT\\V10  FILES ${GEN7_GPO_V1_SRR7_ALIGNMENT_V10})
source_group(\\GEN7\\GPO_V1\\SRR7\\ALIGNMENT\\V11  FILES ${GEN7_GPO_V1_SRR7_ALIGNMENT_V11})

#GEN7_GPO_V1 BLOCKAGE source file
list(APPEND GEN7_GPO_V1_SRR7_BLOCKAGE_V1 	"${SourceFiles}/GEN7/GPO_V1/SRR7/BLOCKAGE/V1/blockage_stream_V1.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/BLOCKAGE/V1/blockage_stream_V1.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/BLOCKAGE/V1/blockage_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/BLOCKAGE/V1/radar_sw_config.h")

#Grouping GEN7_GPO_V1 BLOCKAGE source file
source_group(\\GEN7\\GPO_V1\\SRR7\\BLOCKAGE\\V1  FILES ${GEN7_GPO_V1_SRR7_BLOCKAGE_V1})

#GEN7_GPO_V1 CALIB source file										
list(APPEND GEN7_GPO_V1_SRR7_CALIB_V52	    "${SourceFiles}/GEN7/GPO_V1/SRR7/CALIB/V52/calib_stream_V52.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/CALIB/V52/calib_stream_V52.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/CALIB/V52/calib_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/CALIB/V52/radar_sw_config.h")													
																						
#Grouping GEN7_GPO_V1 CALIB source file
source_group(\\GEN7\\GPO_V1\\SRR7\\CALIB\\V52  FILES ${GEN7_GPO_V1_SRR7_CALIB_V52})								


#GEN7_GPO_V1 CDC source file
list(APPEND GEN7_GPO_V1_SRR7_CDC_V8 	"${SourceFiles}/GEN7/GPO_V1/SRR7/CDC/V8/cdc_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/CDC/V8/cdc_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/CDC/V8/cdc_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/CDC/V8/radar_sw_config.h")

#Grouping GEN7_GPO_V1 CDC source file
source_group(\\GEN7\\GPO_V1\\SRR7\\CDC\\V8  FILES ${GEN7_GPO_V1_SRR7_CDC_V8})

#GEN7_GPO_V1 DET source file
list(APPEND GEN7_GPO_V1_SRR7_DET_V13	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V13/detection_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V13/detection_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V13/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V13/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V13/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V14	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V14/detection_stream_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V14/detection_stream_V14.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V14/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V14/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V14/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V15	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V15/detection_stream_V15.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V15/detection_stream_V15.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V15/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V15/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V15/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V19	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V19/detection_stream_V19.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V19/detection_stream_V19.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V19/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V19/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V19/radar_sw_config.h")
							
list(APPEND GEN7_GPO_V1_SRR7_DET_V21	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V21/detection_stream_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V21/detection_stream_V21.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V21/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V21/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V21/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V22	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V22/detection_stream_V22.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V22/detection_stream_V22.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V22/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V22/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V22/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V25	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V25/detection_stream_V25.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V25/detection_stream_V25.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V25/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V25/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V27	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V27/detection_stream_V27.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V27/detection_stream_V27.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V27/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V27/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V30	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V30/detection_stream_V30.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V30/detection_stream_hdf_V30.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V30/detection_stream_V30.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V30/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V30/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_DET_V32	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V32/detection_stream_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V32/detection_stream_hdf_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V32/detection_stream_V32.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V32/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V32/radar_sw_config.h")


list(APPEND GEN7_GPO_V1_SRR7_DET_V100	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V100/detection_stream_V100.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V100/detection_stream_V100.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V100/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V100/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V102	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V102/detection_stream_V102.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V102/detection_stream_V102.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V102/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V102/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V103	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V103/detection_stream_V103.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V103/detection_stream_V103.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V103/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V103/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V104	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V104/detection_stream_V104.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V104/detection_stream_V104.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V104/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V104/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V105	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V105/detection_stream_V105.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V105/detection_stream_hdf_V105.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V105/detection_stream_V105.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V105/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V105/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DET_V107	"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V107/detection_stream_V107.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V107/detection_stream_hdf_V107.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V107/detection_stream_V107.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V107/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DETECTION/V107/radar_sw_config.h")

#Grouping GEN7_GPO_V1 DET source file
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V13  FILES ${GEN7_GPO_V1_SRR7_DET_V13})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V14  FILES ${GEN7_GPO_V1_SRR7_DET_V14})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V15  FILES ${GEN7_GPO_V1_SRR7_DET_V15})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V19  FILES ${GEN7_GPO_V1_SRR7_DET_V19})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V21  FILES ${GEN7_GPO_V1_SRR7_DET_V21})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V22  FILES ${GEN7_GPO_V1_SRR7_DET_V22})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V25  FILES ${GEN7_GPO_V1_SRR7_DET_V25})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V27  FILES ${GEN7_GPO_V1_SRR7_DET_V27})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V30  FILES ${GEN7_GPO_V1_SRR7_DET_V30})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V32  FILES ${GEN7_GPO_V1_SRR7_DET_V32})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V100 FILES ${GEN7_GPO_V1_SRR7_DET_V100})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V102 FILES ${GEN7_GPO_V1_SRR7_DET_V102})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V103 FILES ${GEN7_GPO_V1_SRR7_DET_V103})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V104 FILES ${GEN7_GPO_V1_SRR7_DET_V104})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V105 FILES ${GEN7_GPO_V1_SRR7_DET_V105})
source_group(\\GEN7\\GPO_V1\\SRR7\\DETECTION\\V107 FILES ${GEN7_GPO_V1_SRR7_DET_V107})
										
#GEN7_GPO_V1 HDR source file
list(APPEND GEN7_GPO_V1_SRR7_HDR_V7		"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V7/header_stream_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V7/header_stream_V7.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V7/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V7/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_HDR_V8  	"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V8/header_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V8/header_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V8/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V8/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_HDR_V9  	"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V9/header_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V9/header_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V9/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V9/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_HDR_V10  	"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V10/header_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V10/header_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V10/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V10/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_HDR_V11  	"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V11/header_stream_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V11/header_stream_hdf_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V11/header_stream_V11.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V11/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V11/radar_sw_config.h")											
list(APPEND GEN7_GPO_V1_SRR7_HDR_V14  	"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V14/header_stream_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V14/header_stream_hdf_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V14/header_stream_V14.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V14/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/HEADER/V14/radar_sw_config.h")											

#Grouping GEN7_GPO_V1 HDR source file
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER\\V7  FILES ${GEN7_GPO_V1_SRR7_HDR_V7})
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER\\V8  FILES ${GEN7_GPO_V1_SRR7_HDR_V8})
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER\\V9  FILES ${GEN7_GPO_V1_SRR7_HDR_V9})
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER\\V10  FILES ${GEN7_GPO_V1_SRR7_HDR_V10})
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER\\V11  FILES ${GEN7_GPO_V1_SRR7_HDR_V11})
source_group(\\GEN7\\GPO_V1\\SRR7\\HEADER\\V14  FILES ${GEN7_GPO_V1_SRR7_HDR_V14})

#GEN7_GPO_V1 ID source file										
list(APPEND GEN7_GPO_V1_SRR7_ID_V1		"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V1/id_stream_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V1/id_stream_V1.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V1/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V1/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_ID_V2		"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V2/id_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V2/id_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V2/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ID_V3		"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V3/id_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V3/id_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V3/id_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V3/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V3/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V1_SRR7_ID_V4		"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V4/id_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V4/id_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V4/id_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V4/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V4/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ID_V5		"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V5/id_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V5/id_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V5/id_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V5/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/ID/V5/radar_sw_config.h")										
																						
#Grouping GEN7_GPO_V1 ID source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ID\\V1  FILES ${GEN7_GPO_V1_SRR7_ID_V1})
source_group(\\GEN7\\GPO_V1\\SRR7\\ID\\V2  FILES ${GEN7_GPO_V1_SRR7_ID_V2})
source_group(\\GEN7\\GPO_V1\\SRR7\\ID\\V3  FILES ${GEN7_GPO_V1_SRR7_ID_V3})
source_group(\\GEN7\\GPO_V1\\SRR7\\ID\\V4  FILES ${GEN7_GPO_V1_SRR7_ID_V4})
source_group(\\GEN7\\GPO_V1\\SRR7\\ID\\V5  FILES ${GEN7_GPO_V1_SRR7_ID_V5})
										
#GEN7_GPO_V1 RDD source file										
list(APPEND GEN7_GPO_V1_SRR7_RDD_V25	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V25/rdd_stream_V25.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V25/rdd_stream_V25.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V25/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V25/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V1_SRR7_RDD_V27	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V27/rdd_stream_V27.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V27/rdd_stream_V27.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V27/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V27/radar_sw_config.h")		

										
list(APPEND GEN7_GPO_V1_SRR7_RDD_V30	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V30/rdd_stream_V30.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V30/rdd_stream_V30.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V30/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V30/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_RDD_V31	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V31/rdd_stream_V31.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V31/rdd_stream_V31.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V31/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V31/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_RDD_V32	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V32/rdd_stream_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V32/rdd_stream_hdf_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V32/rdd_stream_V32.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V32/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V32/radar_sw_config.h")										
list(APPEND GEN7_GPO_V1_SRR7_RDD_V33	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V33/rdd_stream_V33.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V33/rdd_stream_hdf_V33.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V33/rdd_stream_V33.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V33/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V33/radar_sw_config.h")	
list(APPEND GEN7_GPO_V1_SRR7_RDD_V34	"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V34/rdd_stream_V34.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V34/rdd_stream_hdf_V34.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V34/rdd_stream_V34.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V34/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/RDD/V34/radar_sw_config.h")											
																						
#Grouping GEN7_GPO_V1 RDD source file
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V25  FILES ${GEN7_GPO_V1_SRR7_RDD_V25})
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V27  FILES ${GEN7_GPO_V1_SRR7_RDD_V27})
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V30  FILES ${GEN7_GPO_V1_SRR7_RDD_V30})
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V31  FILES ${GEN7_GPO_V1_SRR7_RDD_V31})
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V32  FILES ${GEN7_GPO_V1_SRR7_RDD_V32})
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V33  FILES ${GEN7_GPO_V1_SRR7_RDD_V33})
source_group(\\GEN7\\GPO_V1\\SRR7\\RDD\\V34  FILES ${GEN7_GPO_V1_SRR7_RDD_V34})


#GEN7_GPO_V1 STATUS source file
list(APPEND GEN7_GPO_V1_SRR7_STATUS_V9 	"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V9/status_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V9/status_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V9/status_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V9/radar_sw_config.h")
									
list(APPEND GEN7_GPO_V1_SRR7_STATUS_V10 	"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V10/status_stream_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V10/status_stream_hdf_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V10/status_stream_V10.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V10/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V10/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_STATUS_V11 	"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V11/status_stream_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V11/status_stream_hdf_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V11/status_stream_V11.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V11/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/STATUS/V11/radar_sw_config.h")

#Grouping GEN7_GPO_V1 STATUS source file
source_group(\\GEN7\\GPO_V1\\SRR7\\STATUS\\V9  FILES ${GEN7_GPO_V1_SRR7_STATUS_V9})
source_group(\\GEN7\\GPO_V1\\SRR7\\STATUS\\V10  FILES ${GEN7_GPO_V1_SRR7_STATUS_V10})
source_group(\\GEN7\\GPO_V1\\SRR7\\STATUS\\V11  FILES ${GEN7_GPO_V1_SRR7_STATUS_V11})

#GEN7_GPO_V1 DEBUG source file
list(APPEND GEN7_GPO_V1_SRR7_DEBUG_V5 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V5/debug_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V5/debug_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V5/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V5/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DEBUG_V10 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V10/debug_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V10/debug_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V10/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_DEBUG_V12 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V12/debug_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V12/debug_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V12/debug_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V12/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V12/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_DEBUG_V13 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V13/debug_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V13/debug_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V13/debug_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V13/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/DEBUG/V13/radar_sw_config.h")

#Grouping GEN7_GPO_V1 DEBUG source file
source_group(\\GEN7\\GPO_V1\\SRR7\\DEBUG\\V5  FILES ${GEN7_GPO_V1_SRR7_DEBUG_V5})
source_group(\\GEN7\\GPO_V1\\SRR7\\DEBUG\\V10  FILES ${GEN7_GPO_V1_SRR7_DEBUG_V10})
source_group(\\GEN7\\GPO_V1\\SRR7\\DEBUG\\V12  FILES ${GEN7_GPO_V1_SRR7_DEBUG_V12})
source_group(\\GEN7\\GPO_V1\\SRR7\\DEBUG\\V13  FILES ${GEN7_GPO_V1_SRR7_DEBUG_V13})

#GEN7_GPO_V1 DOWN_SELECTION source file
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V2 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V2/down_selection_stream_V2.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V2/down_selection_stream_V2.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V2/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V2/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V2/radar_sw_config.h")
		
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V5 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V5/down_selection_stream_V5.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V5/down_selection_stream_V5.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V5/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V5/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V5/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V7 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V7/down_selection_stream_V7.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V7/down_selection_stream_V7.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V7/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V7/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V7/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V100 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V100/down_selection_stream_V100.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V100/down_selection_stream_V100.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V100/down_selection_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V100/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V9 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V9/down_selection_stream_V9.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V9/down_selection_stream_V9.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V9/down_selection_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V9/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V11 "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V11/down_selection_stream_V11.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V11/down_selection_stream_V11.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V11/down_selection_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V11/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V14 "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V14/down_selection_stream_V14.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V14/down_selection_stream_hdf_V14.cpp"			
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V14/down_selection_stream_V14.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V14/down_selection_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V14/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V16 "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V16/down_selection_stream_V16.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V16/down_selection_stream_hdf_V16.cpp"			
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V16/down_selection_stream_V16.h"
										        "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V16/down_selection_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V16/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V102 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V102/down_selection_stream_V102.cpp"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V102/down_selection_stream_V102.h"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V102/down_selection_stream.h"												
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V102/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V103 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V103/down_selection_stream_V103.cpp"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V103/down_selection_stream_V103.h"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V103/down_selection_stream.h"												
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V103/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V104 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V104/down_selection_stream_V104.cpp"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V104/down_selection_stream_V104.h"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V104/down_selection_stream.h"												
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V104/radar_sw_config.h")
													
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V105 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V105/down_selection_stream_V105.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V105/down_selection_stream_hdf_V105.cpp"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V105/down_selection_stream_V105.h"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V105/down_selection_stream.h"												
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V105/radar_sw_config.h")
													
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V107 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V107/down_selection_stream_V107.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V107/down_selection_stream_hdf_V107.cpp"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V107/down_selection_stream_V107.h"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V107/down_selection_stream.h"												
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V107/radar_sw_config.h")
								
list(APPEND GEN7_GPO_V1_SRR7_DOWN_SELECTION_V108 	"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V108/down_selection_stream_V108.cpp"
													#"${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V108/down_selection_stream_hdf_V108.cpp"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V108/down_selection_stream_V108.h"
										            "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V108/down_selection_stream.h"												
												    "${SourceFiles}/GEN7/GPO_V1/SRR7/DOWN_SELECTION/V108/radar_sw_config.h")								

#Grouping GEN7_GPO_V1 DEBUG source file
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V2  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V2})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V5  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V5})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V7  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V7})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V9  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V9})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V11  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V11})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V14  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V14})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V16  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V16})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V100  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V100})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V102  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V102})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V103  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V103})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V104  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V104})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V105  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V105})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V107  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V107})
source_group(\\GEN7\\GPO_V1\\SRR7\\DOWN_SELECTION\\V108  FILES ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V108})

#GEN7_GPO_V1 DYNAMIC ALIGNMENT source file										
list(APPEND GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V12	"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config.h")		
list(APPEND GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V14	"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream_V14.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V14/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V14/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V15	"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_alignment_stream_V15.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_alignment_stream_V15.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V15/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V15/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V16	"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream_V16.cpp"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream_V16.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V16/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/SRR7/DYNAMIC_ALIGNMENT/V16/radar_sw_config.h")														
												
																						
#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT source file
source_group(\\GEN7\\GPO_V1\\SRR7\\DYNAMIC_ALIGNMENT\\V12  FILES ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V12})	
source_group(\\GEN7\\GPO_V1\\SRR7\\DYNAMIC_ALIGNMENT\\V14  FILES ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V14})	
source_group(\\GEN7\\GPO_V1\\SRR7\\DYNAMIC_ALIGNMENT\\V15  FILES ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V15})	
source_group(\\GEN7\\GPO_V1\\SRR7\\DYNAMIC_ALIGNMENT\\V16  FILES ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V16})	
														
#GEN7_GPO_V1 VSE source file										
list(APPEND GEN7_GPO_V1_SRR7_VSE_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V3/vse_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V3/vse_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V3/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V3/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_VSE_V4	    "${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V4/vse_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V4/vse_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V4/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V4/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_VSE_V5	    "${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V5/vse_stream_V5.cpp"
										#"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V5/vse_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V5/vse_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V5/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V5/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_VSE_V6	    "${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V6/vse_stream_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V6/vse_stream_hdf_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V6/vse_stream_V6.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V6/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V6/radar_sw_config.h")										
list(APPEND GEN7_GPO_V1_SRR7_VSE_V7	    "${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V7/vse_stream_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V7/vse_stream_hdf_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V7/vse_stream_V7.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V7/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/VSE/V7/radar_sw_config.h")										
									
																						
#Grouping GEN7_GPO_V1 VSE source file
source_group(\\GEN7\\GPO_V1\\SRR7\\VSE\\V3  FILES ${GEN7_GPO_V1_SRR7_VSE_V3})	
source_group(\\GEN7\\GPO_V1\\SRR7\\VSE\\V4  FILES ${GEN7_GPO_V1_SRR7_VSE_V4})
source_group(\\GEN7\\GPO_V1\\SRR7\\VSE\\V5  FILES ${GEN7_GPO_V1_SRR7_VSE_V5})
source_group(\\GEN7\\GPO_V1\\SRR7\\VSE\\V6  FILES ${GEN7_GPO_V1_SRR7_VSE_V6})		
source_group(\\GEN7\\GPO_V1\\SRR7\\VSE\\V7  FILES ${GEN7_GPO_V1_SRR7_VSE_V7})	

#GEN7_GPO_V1 MMIC source file										
list(APPEND GEN7_GPO_V1_SRR7_MMIC_V25	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V25/mmic_stream_V25.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V25/mmic_stream_V25.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V25/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V25/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V26	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V26/mmic_stream_V26.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V26/mmic_stream_V26.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V26/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V26/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V28	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V28/mmic_stream_V28.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V28/mmic_stream_V28.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V28/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V28/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V30	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V30/mmic_stream_V30.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V30/mmic_stream_V30.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V30/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V30/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V31	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V31/mmic_stream_V31.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V31/mmic_stream_V31.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V31/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V31/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V32	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V32/mmic_stream_V32.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V32/mmic_stream_V32.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V32/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V32/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V34	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V34/mmic_stream_V34.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V34/mmic_stream_V34.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V34/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V34/radar_sw_config.h")


list(APPEND GEN7_GPO_V1_SRR7_MMIC_V36	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V36/mmic_stream_V36.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V36/mmic_stream_V36.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V36/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V36/radar_sw_config.h")	
											
list(APPEND GEN7_GPO_V1_SRR7_MMIC_V37	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V37/mmic_stream_V37.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V37/mmic_stream_V37.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V37/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V37/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_MMIC_V40	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V40/mmic_stream_V40.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V40/mmic_stream_hdf_V40.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V40/mmic_stream_V40.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V40/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V40/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_MMIC_V42	    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V42/mmic_stream_V42.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V42/mmic_stream_hdf_V42.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V42/mmic_stream_V42.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V42/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/MMIC/V42/radar_sw_config.h")
																																										
#Grouping GEN7_GPO_V1 MMIC source file
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V25  FILES ${GEN7_GPO_V1_SRR7_MMIC_V25})	
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V26  FILES ${GEN7_GPO_V1_SRR7_MMIC_V26})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V28  FILES ${GEN7_GPO_V1_SRR7_MMIC_V28})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V30  FILES ${GEN7_GPO_V1_SRR7_MMIC_V30})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V31  FILES ${GEN7_GPO_V1_SRR7_MMIC_V31})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V32  FILES ${GEN7_GPO_V1_SRR7_MMIC_V32})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V34  FILES ${GEN7_GPO_V1_SRR7_MMIC_V34})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V37  FILES ${GEN7_GPO_V1_SRR7_MMIC_V37})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V36  FILES ${GEN7_GPO_V1_SRR7_MMIC_V36})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V40  FILES ${GEN7_GPO_V1_SRR7_MMIC_V40})
source_group(\\GEN7\\GPO_V1\\SRR7\\MMIC\\V42  FILES ${GEN7_GPO_V1_SRR7_MMIC_V42})

#GEN7_GPO_V1 RADAR CAPABILITY source file										
list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V29	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V29/radar_capability_stream_V29.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V29/radar_capability_stream_V29.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V29/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V29/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V29/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V31	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V31/radar_capability_stream_V31.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V31/radar_capability_stream_V31.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V31/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V31/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V31/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V32	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V32/radar_capability_stream_V32.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V32/radar_capability_stream_V32.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V32/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V32/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V32/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V33	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V33/radar_capability_stream_V33.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V33/radar_capability_stream_V33.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V33/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V33/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V33/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V34	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V34/radar_capability_stream_V34.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V34/radar_capability_stream_V34.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V34/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V34/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V34/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V39	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream_hdf_V39.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V39/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V39/radar_sw_config.h")														
															

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V35	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream_V35.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream_V35.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V35/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V35/radar_sw_config.h")															
list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V45	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V45/radar_capability_stream_V45.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V45/radar_capability_stream_hdf_V45.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V45/radar_capability_stream_V45.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V45/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V45/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V45/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V46	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream_V46.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream_hdf_V46.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream_V46.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V46/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V46/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V49	    "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream_V49.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream_hdf_V49.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream_V49.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V49/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/RADAR_CAPABILITY/V49/radar_sw_config.h")															
																						
#Grouping GEN7_GPO_V1 RADAR CAPABILITY source file
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V29  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V29})	
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V31  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V31})	
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V32  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V32})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V33  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V33})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V34  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V34})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V39  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V39})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V35  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V35})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V45  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V45})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V46  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V46})
source_group(\\GEN7\\GPO_V1\\SRR7\\RADAR_CAPABILITY\\V49  FILES ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V49})


#GEN7_GPO_V1 ROT_TRACKER_INFO source file										
list(APPEND GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V3/ROT_trackerInfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V3/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V4	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V4/ROT_trackerInfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V4/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V4/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V5	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V5/ROT_tracker_info_stream_V5.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V5/ROT_tracker_info_stream_V5.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V5/ROT_trackerInfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V5/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V5/radar_sw_config.h")														
list(APPEND GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V12	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V12/ROT_tracker_info_stream_V12.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V12/ROT_tracker_info_stream_V12.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V12/ROT_trackerInfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V12/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V12/radar_sw_config.h")	
														
list(APPEND GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V212	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V212/ROT_tracker_info_stream_V212.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V212/ROT_tracker_info_stream_V212.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V212/ROT_trackerInfo_stream.h"												
										               # "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V212/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_TRACKER_INFO/V212/radar_sw_config.h")	
	
#Grouping GEN7_GPO_V1 ROT_TRACKER_INFO source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_TRACKER_INFO\\V3  FILES ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V3})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_TRACKER_INFO\\V4  FILES ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V4})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_TRACKER_INFO\\V5  FILES ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V5})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_TRACKER_INFO\\V12  FILES ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V12})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_TRACKER_INFO\\V212  FILES ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V212})

#GEN7_GPO_V1 ROT_VEHICLE_INFO source file										
list(APPEND GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V2	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V2/ROT_vehicle_info_stream_V2.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V2/ROT_vehicle_info_stream_V2.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V2/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V2/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V3/ROT_vehicle_info_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V3/ROT_vehicle_info_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V3/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V3/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V4	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicle_info_stream_V4.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicleinfo_stream_hdf_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicle_info_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V4/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V4/radar_sw_config.h")														
list(APPEND GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V12	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V12/ROT_vehicle_info_stream_V12.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V12/ROT_vehicleinfo_stream_hdf_V12.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V12/ROT_vehicle_info_stream_V12.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V12/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V12/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V12/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V113	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicle_info_stream_V113.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicleinfo_stream_hdf_V113.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicle_info_stream_V113.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V113/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_VEHICLE_INFO/V113/radar_sw_config.h")
														
																						
#Grouping GEN7_GPO_V1 ROT_VEHICLE_INFO source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_VEHICLE_INFO\\V2  FILES ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V2})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_VEHICLE_INFO\\V3  FILES ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V3})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_VEHICLE_INFO\\V4  FILES ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V4})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_VEHICLE_INFO\\V12  FILES ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V12})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_VEHICLE_INFO\\V113  FILES ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V113})

#GEN7_GPO_V1 ROT_SAFETY_FAULTS source file										
list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V2	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream_V2.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream_V2.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V2/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V3/ROT_safety_faults_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V3/ROT_safety_faults_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V3/ROT_safety_faults_stream.h"								
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V3/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V4	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_V4.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_hdf_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream.h"								
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V4/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V4/radar_sw_config.h")														

list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V12	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream_V12.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream_hdf_V12.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream_V12.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream.h"								
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V12/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V12/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V13	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream_V13.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream_hdf_V13.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream_V13.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream.h"								
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V13/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V13/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V113	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream_V113.cpp"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream_hdf_V113.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream_V113.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream.h"								
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V113/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_SAFETY_FAULTS/V113/radar_sw_config.h")														
#Grouping GEN7_GPO_V1 ROT_SAFETY_FAULTS source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS\\V2  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V2})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS\\V3  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V3})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS\\V4  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V4})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS\\V12  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V12})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS\\V13  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V13})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_SAFETY_FAULTS\\V113  FILES ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V113})
	

#GEN7_GPO_V1 ROT_OBJECT_STREAM source file										
list(APPEND GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V2	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V2/ROT_object_stream_V2.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V2/ROT_object_stream_V2.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V2/ROT_object_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V2/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V3/ROT_object_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V3/ROT_object_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V3/ROT_object_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V3/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V4	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V4/ROT_object_stream_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V4/ROT_object_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V4/ROT_object_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V4/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_OBJECT_STREAM/V4/radar_sw_config.h")														
																						
#Grouping GEN7_GPO_V1 ROT_OBJECT_STREAM source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_OBJECT_STREAM\\V2  FILES ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V2})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_OBJECT_STREAM\\V3  FILES ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V3})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_OBJECT_STREAM\\V4  FILES ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V4})	

#GEN7_GPO_V1 ROT_ISOobject_Stream source file										
list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V2	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V2/ROT_ISOobject_stream_V2.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V2/ROT_ISOobject_stream_V2.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V2/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V2/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream_V3.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream_V3.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V3/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V3/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V4	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V4/ROT_ISOobject_stream_V4.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V4/ROT_ISOobject_stream_V4.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V4/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V4/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V4/radar_sw_config.h")															
list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V12	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V12/ROT_ISOobject_stream_V12.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V12/ROT_ISOobject_stream_V12.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V12/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V12/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V12/radar_sw_config.h")	
															
list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V112	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V112/ROT_ISOobject_stream_V112.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V112/ROT_ISOobject_stream_V112.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V112/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V112/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V112/radar_sw_config.h")
															
list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V113	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V113/ROT_ISOobject_stream_V113.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V113/ROT_ISOobject_stream_V113.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V113/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V113/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V113/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V213	    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V213/ROT_ISOobject_stream_V213.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V213/ROT_ISOobject_stream_V213.h"
										                    "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V213/ROT_ISOobject_stream.h"												
										                   # "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V213/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_ISO_OBJECT/V213/radar_sw_config.h")															
																						
#Grouping GEN7_GPO_V1 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V2  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V2})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V3  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V3})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V4  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V4})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V12  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V12})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V112  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V112})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V113  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V113})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_ISOobject_Stream\\V213  FILES ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V213})

#GEN7_GPO_V1 ROT_PROCESSEDDETECTION_Stream source file										
list(APPEND GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V2	"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V2/ROT_processeddetection_stream_V2.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V2/ROT_processeddetection_stream_V2.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V2/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V2/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V3	"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream_V3.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream_V3.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V3/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V3/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V4	"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V4/ROT_processeddetection_stream_V4.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V4/ROT_processeddetection_stream_V4.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V4/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V4/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V4/radar_sw_config.h")																		
list(APPEND GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V12	"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V12/ROT_processeddetection_stream_V12.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V12/ROT_processeddetection_stream_V12.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V12/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V12/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V12/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V113	"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V113/ROT_processeddetection_stream_V113.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V113/ROT_processeddetection_stream_V113.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V113/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V113/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/SRR7/ROT_PROCESSED_DETECTION/V113/radar_sw_config.h")																
																						
#Grouping GEN7_GPO_V1 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V2  FILES ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V2})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V3  FILES ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V3})
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V4  FILES ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V4})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V12  FILES ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V12})	
source_group(\\GEN7\\GPO_V1\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V113  FILES ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V113})	

#GEN7_GPO_V1 TOI source file										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V2	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V2/toi_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V2/toi_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V2/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_TOI_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V3/toi_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V3/toi_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V3/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V3/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V3/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V6     "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V6/toi_stream_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V6/toi_stream_V6.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V6/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V6/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V6/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V9	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V9/toi_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V9/toi_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V9/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V10	"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V10/toi_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V10/toi_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V10/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V12	"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V12/toi_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V12/toi_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V12/toi_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V12/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V12/radar_sw_config.h")										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V13	"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V13/toi_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V13/toi_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V13/toi_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V13/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V13/radar_sw_config.h")										
										
list(APPEND GEN7_GPO_V1_SRR7_TOI_V100	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V100/toi_stream_V100.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V100/toi_stream_V100.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V100/toi_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V100/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V1_SRR7_TOI_V101	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V101/toi_stream_V101.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V101/toi_stream_V101.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V101/toi_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V101/radar_sw_config.h")
			
list(APPEND GEN7_GPO_V1_SRR7_TOI_V103	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V103/toi_stream_V103.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V103/toi_stream_hdf_V103.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V103/toi_stream_V103.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V103/toi_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V103/radar_sw_config.h")
			
list(APPEND GEN7_GPO_V1_SRR7_TOI_V104	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V104/toi_stream_V104.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V104/toi_stream_hdf_V104.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V104/toi_stream_V104.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V104/toi_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V104/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V1_SRR7_TOI_V105	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V105/toi_stream_V105.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V105/toi_stream_hdf_V105.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V105/toi_stream_V105.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V105/toi_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V105/radar_sw_config.h")											

list(APPEND GEN7_GPO_V1_SRR7_TOI_V106	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V106/toi_stream_V106.cpp"
											#"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V106/toi_stream_hdf_V106.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V106/toi_stream_V106.h"
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V106/toi_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI/V106/radar_sw_config.h")		
																						
#Grouping GEN7_GPO_V1 TOI source file
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V2  FILES ${GEN7_GPO_V1_SRR7_TOI_V2})		
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V3  FILES ${GEN7_GPO_V1_SRR7_TOI_V3})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V6  FILES ${GEN7_GPO_V1_SRR7_TOI_V6})	
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V9  FILES ${GEN7_GPO_V1_SRR7_TOI_V9})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V10  FILES ${GEN7_GPO_V1_SRR7_TOI_V10})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V12  FILES ${GEN7_GPO_V1_SRR7_TOI_V12})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V13  FILES ${GEN7_GPO_V1_SRR7_TOI_V13})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V100  FILES ${GEN7_GPO_V1_SRR7_TOI_V100})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V101  FILES ${GEN7_GPO_V1_SRR7_TOI_V101})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V103  FILES ${GEN7_GPO_V1_SRR7_TOI_V103})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V104  FILES ${GEN7_GPO_V1_SRR7_TOI_V104})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V105  FILES ${GEN7_GPO_V1_SRR7_TOI_V105})
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI\\V106  FILES ${GEN7_GPO_V1_SRR7_TOI_V106})

#Grouping GEN7_GPO_V1 TOI DS source file

list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V2	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V2/toi_ds_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V2/toi_ds_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V2/toi_ds_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V2/toi_ds_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V2/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V3	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V3/toi_ds_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V3/toi_ds_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V3/toi_ds_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V3/toi_ds_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V3/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V101	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V101/toi_ds_stream_V101.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V101/toi_ds_stream_hdf_V101.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V101/toi_ds_stream_V101.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V101/toi_ds_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V101/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V102	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V102/toi_ds_stream_V102.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V102/toi_ds_stream_hdf_V102.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V102/toi_ds_stream_V102.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V102/toi_ds_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V102/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V103	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V103/toi_ds_stream_V103.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V103/toi_ds_stream_hdf_V103.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V103/toi_ds_stream_V103.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V103/toi_ds_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V103/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V104	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V104/toi_ds_stream_V104.cpp"
												#"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V104/toi_ds_stream_hdf_V104.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V104/toi_ds_stream_V104.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V104/toi_ds_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V104/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_SRR7_TOI_DS_V105	    "${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V105/toi_ds_stream_V105.cpp"
												#"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V105/toi_ds_stream_hdf_V105.cpp"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V105/toi_ds_stream_V105.h"
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V105/toi_ds_stream.h"												
												"${SourceFiles}/GEN7/GPO_V1/SRR7/TOI_DS/V105/radar_sw_config.h")


source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V2  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V2})												
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V3  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V3})												
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V101  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V101})	
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V102  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V102})	
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V103  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V103})	
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V104  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V104})	
source_group(\\GEN7\\GPO_V1\\SRR7\\TOI_DS\\V105  FILES ${GEN7_GPO_V1_SRR7_TOI_DS_V105})	

#GEN7 Streams 
#GPO_V1
#FLR7 Streams
#GEN7_GPO_V1 ALIGNMENT Stream Source File
#GEN7_GPO_V1 MMIC Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_MMIC_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/MMIC_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/MMIC_Logging.h")

#Grouping GEN7_GPO_V1 MMIC Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC   FILES ${GEN7_GPO_V1_FLR7_MMIC_SOURCE})

#GEN7_GPO_V1 RADAR_CAPABILITY Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/Radar_Capability_Logging.h")

#Grouping GEN7_GPO_V1 RADAR_CAPABILITY Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY   FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_SOURCE})


#GEN7_GPO_V1 RDD Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_RDD_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/RDD_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/RDD_Logging.h")

#Grouping GEN7_GPO_V1 RDD Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\RDD FILES ${GEN7_GPO_V1_FLR7_RDD_SOURCE})

#GEN7_GPO_V1 ROT_ISOobject Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ROT_ISOobject_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.cpp"
												                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.h")

#Grouping GEN7_GPO_V1 ROT_ISOobject Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_ISOobject_STREAM  FILES ${GEN7_GPO_V1_FLR7_ROT_ISOobject_STREAM_SOURCE})

#GEN7_GPO_V1 ROT_OBJECT Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/ROT_Object_Stream_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/ROT_Object_Stream_Logging.h")

#Grouping GEN7_GPO_V1 ROT_OBJECT Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_OBJECT_STREAM  FILES ${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_SOURCE})

#GEN7_GPO_V1 ROT Processed Detection Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.cpp"
												                        "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.h")

#Grouping GEN7_GPO_V1 ROT Processed Detection Stream Source File
source_group(\\GEN7\\GPO_V1\\FlR7\\ROT_PROCESSEDDETECTION_STREAM  FILES ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE})

#GEN7_GPO_V1 ROT_SAFETY_FAULTS Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.h")

#Grouping GEN7_GPO_V1 ROT_SAFETY_FAULTS Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_SAFETY_FAULTS  FILES ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_SOURCE})

#GEN7_GPO_V1 MMIC source file										
list(APPEND GEN7_GPO_V1_FLR7_MMIC_V25	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V25/mmic_stream_V25.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V25/mmic_stream_V25.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V25/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V25/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V25/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_MMIC_V26	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V26/mmic_stream_V26.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V26/mmic_stream_V26.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V26/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V26/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V26/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_MMIC_V28	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V28/mmic_stream_V28.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V28/mmic_stream_V28.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V28/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V28/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V28/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_MMIC_V30	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V30/mmic_stream_V30.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V30/mmic_stream_V30.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V30/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V30/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V30/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_MMIC_V31	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V31/mmic_stream_V31.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V31/mmic_stream_V31.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V31/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V31/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V31/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_MMIC_V40	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V40/mmic_stream_V40.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V40/mmic_stream_hdf_V40.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V40/mmic_stream_V40.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V40/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V40/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V40/radar_sw_config.h")											
list(APPEND GEN7_GPO_V1_FLR7_MMIC_V42	    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V42/mmic_stream_V42.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V42/mmic_stream_hdf_V42.cpp"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V42/mmic_stream_V42.h"
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V42/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V42/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/MMIC/V42/radar_sw_config.h")											
																						
#Grouping GEN7_GPO_V1 MMIC source file
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V25  FILES ${GEN7_GPO_V1_FLR7_MMIC_V25})	
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V26  FILES ${GEN7_GPO_V1_FLR7_MMIC_V26})
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V28  FILES ${GEN7_GPO_V1_FLR7_MMIC_V28})
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V30  FILES ${GEN7_GPO_V1_FLR7_MMIC_V30})
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V31  FILES ${GEN7_GPO_V1_FLR7_MMIC_V31})
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V40  FILES ${GEN7_GPO_V1_FLR7_MMIC_V40})
source_group(\\GEN7\\GPO_V1\\FLR7\\MMIC\\V42  FILES ${GEN7_GPO_V1_FLR7_MMIC_V42})


#GEN7_GPO_V1 RADAR CAPABILITY source file										
list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V29	    "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V29/radar_capability_stream_V29.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V29/radar_capability_stream_V29.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V29/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V29/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V29/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V31	    "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V31/radar_capability_stream_V31.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V31/radar_capability_stream_V31.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V31/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V31/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V31/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V32	    "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V32/radar_capability_stream_V32.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V32/radar_capability_stream_V32.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V32/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V32/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V32/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V33	    "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V33/radar_capability_stream_V33.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V33/radar_capability_stream_V33.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V33/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V33/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V33/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V39	    "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.cpp"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream_hdf_V39.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V39/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V39/radar_sw_config.h")														
list(APPEND GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V45	    "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V45/radar_capability_stream_V45.cpp"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V45/radar_capability_stream_hdf_V45.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V45/radar_capability_stream_V45.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V45/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V45/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/RADAR_CAPABILITY/V45/radar_sw_config.h")														
																						
#Grouping GEN7_GPO_V1 RADAR CAPABILITY source file
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY\\V29  FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V29})	
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY\\V31  FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V31})	
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY\\V32  FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V32})
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY\\V33  FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V33})
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY\\V39  FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V39})
source_group(\\GEN7\\GPO_V1\\FLR7\\RADAR_CAPABILITY\\V45  FILES ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V45})

#GEN7 Streams 
#GPO_V1
#FLR7 Streams	
#GEN7_GPO_V1 DYNAMIC ALIGNMENT Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_SOURCE 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.h")

#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\DYNAMIC_ALIGNMENT   FILES ${GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_SOURCE})

#GEN7_GPO_V1 STATUS Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_STATUS_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/Status_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/Status_Logging.h")

#Grouping GEN7_GPO_V1 STATUS Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\STATUS   FILES ${GEN7_GPO_V1_FLR7_STATUS_SOURCE})


#GEN7_GPO_V1 VSE Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_VSE_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/VSE_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/VSE_Logging.h")

#Grouping GEN7_GPO_V1 VSE Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\VSE   FILES ${GEN7_GPO_V1_FLR7_VSE_SOURCE})



#GEN7_GPO_V1 ROT_TRACKER_INFO Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.h")

#Grouping GEN7_GPO_V1 ROT_TRACKER_INFO Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_TRACKER_INFO FILES ${GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_SOURCE})

#GEN7_GPO_V1 ROT_VEHICLE_INFO Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.h")

#Grouping GEN7_GPO_V1 ROT_VEHICLE_INFO Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_VEHICLE_INFO   FILES ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_SOURCE})


#GEN7_GPO_V1 TOI Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_TOI_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/TOI_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/TOI_Logging.h")

#Grouping GEN7_GPO_V1 TOI Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI   FILES ${GEN7_GPO_V1_FLR7_TOI_SOURCE})

#GEN7_GPO_V1 TOI DS Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_TOI_DS_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/TOI_DS_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/TOI_DS_Logging.h")

#Grouping GEN7_GPO_V1 TOI Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI_DS   FILES ${GEN7_GPO_V1_FLR7_TOI_DS_SOURCE})


#GEN7_GPO_V1 DYNAMIC ALIGNMENT source file										
list(APPEND GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_V12	"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config.h")		
												
list(APPEND GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_V14	"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream_V14.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V14/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DYNAMIC_ALIGNMENT/V14/radar_sw_config.h")	
																						
#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT source file
source_group(\\GEN7\\GPO_V1\\FLR7\\DYNAMIC_ALIGNMENT\\V12  FILES ${GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_V12})	
source_group(\\GEN7\\GPO_V1\\FLR7\\DYNAMIC_ALIGNMENT\\V14  FILES ${GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_V14})	

#GEN7 Streams 
#GPO_V1
#FLR7 Streams
#GEN7_GPO_V1 ALIGNMENT Stream Source File
list(APPEND GEN7_GPO_V1_FLR7_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/Alignment_Logging.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GEN7_GPO_V1 ALIGNMENT Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ALIGNMENT FILES ${GEN7_GPO_V1_FLR7_ALIGNMENT_SOURCE})

#GEN7_GPO_V1 STATUS source file
list(APPEND GEN7_GPO_V1_FLR7_STATUS_V9 	"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V9/status_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V9/status_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V9/status_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V9/radar_sw_config.h")
									
list(APPEND GEN7_GPO_V1_FLR7_STATUS_V10 	"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V10/status_stream_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V10/status_stream_hdf_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V10/status_stream_V10.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V10/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V10/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_FLR7_STATUS_V11 	"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V11/status_stream_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V11/status_stream_hdf_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V11/status_stream_V11.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V11/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/FLR7/STATUS/V11/radar_sw_config.h")
#Grouping GEN7_GPO_V1 STATUS source file
source_group(\\GEN7\\GPO_V1\\FLR7\\STATUS\\V9  FILES ${GEN7_GPO_V1_FLR7_STATUS_V9})
source_group(\\GEN7\\GPO_V1\\FLR7\\STATUS\\V10  FILES ${GEN7_GPO_V1_FLR7_STATUS_V10})
source_group(\\GEN7\\GPO_V1\\FLR7\\STATUS\\V11  FILES ${GEN7_GPO_V1_FLR7_STATUS_V11})

#GEN7_GPO_V1 DETECTION Stream Source File
list(APPEND GEN7_GPO_V1_FLR7_DET_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/Detection_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/Detection_Logging.h")
					
#Grouping GEN7_GPO_V1 DETECTION Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION FILES ${GEN7_GPO_V1_FLR7_DET_SOURCE})


#GEN7_GPO_V1 HEADER Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_HDR_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/header_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/header_Logging.h")

#Grouping GEN7_GPO_V1 HEADER Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\HEADER FILES ${GEN7_GPO_V1_FLR7_HDR_SOURCE})

#GEN7_GPO_V1 ID Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_ID_SOURCE 		    "${SourceFiles}/GEN7/GPO_V1/FLR7/ID/ID_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/ID_Logging.h")

#Grouping GEN7_GPO_V1 ID Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\ID FILES ${GEN7_GPO_V1_FLR7_ID_SOURCE})

#GEN7_GPO_V1 DEBUG Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_DEBUG_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/Debug_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/Debug_Logging.h")

#Grouping GEN7_GPO_V1 DEBUG Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\DEBUG   FILES ${GEN7_GPO_V1_FLR7_DEBUG_SOURCE})

#GEN7_GPO_V1 DOWN_SELECTION Stream Source File												
list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_SOURCE 		"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/Down_Selection_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/Down_Selection_Logging.h")

#Grouping GEN7_GPO_V1 DOWN_SELECTION Stream Source File
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION   FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_SOURCE})

#GEN7_GPO_V1 ALIGNMENT source file
list(APPEND GEN7_GPO_V1_FLR7_ALIGNMENT_V9 	"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V9/alignment_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V9/alignment_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V9/alignment_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V9/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_ALIGNMENT_V10 	"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V10/alignment_stream_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V10/alignment_stream_hdf_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V10/alignment_stream_V10.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V10/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V10/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V10/radar_sw_config.h")										

list(APPEND GEN7_GPO_V1_FLR7_ALIGNMENT_V11 	"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V11/alignment_stream_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V11/alignment_stream_hdf_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V11/alignment_stream_V11.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V11/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V11/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/ALIGNMENT/V11/radar_sw_config.h")											
#Grouping GEN7_GPO_V1 ALIGNMENT source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ALIGNMENT\\V9  FILES ${GEN7_GPO_V1_FLR7_ALIGNMENT_V9})
source_group(\\GEN7\\GPO_V1\\FLR7\\ALIGNMENT\\V10  FILES ${GEN7_GPO_V1_FLR7_ALIGNMENT_V10})
source_group(\\GEN7\\GPO_V1\\FLR7\\ALIGNMENT\\V11  FILES ${GEN7_GPO_V1_FLR7_ALIGNMENT_V11})

#GEN7_GPO_V1 DET source file
list(APPEND GEN7_GPO_V1_FLR7_DET_V13	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V13/detection_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V13/detection_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V13/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V13/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V13/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_DET_V14	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V14/detection_stream_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V14/detection_stream_V14.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V14/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V14/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V14/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_DET_V15	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V15/detection_stream_V15.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V15/detection_stream_V15.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V15/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V15/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V15/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_DET_V19	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V19/detection_stream_V19.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V19/detection_stream_V19.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V19/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V19/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V19/radar_sw_config.h")
							
list(APPEND GEN7_GPO_V1_FLR7_DET_V21	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V21/detection_stream_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V21/detection_stream_V21.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V21/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V21/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V21/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_DET_V25	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V25/detection_stream_V25.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V25/detection_stream_V25.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V25/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V25/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V25/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_DET_V30	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V30/detection_stream_V30.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V30/detection_stream_hdf_V30.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V30/detection_stream_V30.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V30/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V30/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V30/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_DET_V32	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V32/detection_stream_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V32/detection_stream_hdf_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V32/detection_stream_V32.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V32/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V32/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V32/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_DET_V100	"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V100/detection_stream_V100.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V100/detection_stream_V100.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V100/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V100/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DETECTION/V100/radar_sw_config.h")

#Grouping GEN7_GPO_V1 DET source file
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V13  FILES ${GEN7_GPO_V1_FLR7_DET_V13})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V14  FILES ${GEN7_GPO_V1_FLR7_DET_V14})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V15  FILES ${GEN7_GPO_V1_FLR7_DET_V15})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V19  FILES ${GEN7_GPO_V1_FLR7_DET_V19})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V21  FILES ${GEN7_GPO_V1_FLR7_DET_V21})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V25  FILES ${GEN7_GPO_V1_FLR7_DET_V25})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V30  FILES ${GEN7_GPO_V1_FLR7_DET_V30})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V32  FILES ${GEN7_GPO_V1_FLR7_DET_V32})
source_group(\\GEN7\\GPO_V1\\FLR7\\DETECTION\\V100 FILES ${GEN7_GPO_V1_FLR7_DET_V100})
										
#GEN7_GPO_V1 HDR source file
list(APPEND GEN7_GPO_V1_FLR7_HDR_V7		"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V7/header_stream_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V7/header_stream_V7.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V7/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V7/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V7/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_HDR_V8  	"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V8/header_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V8/header_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V8/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V8/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V8/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_HDR_V9  	"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V9/header_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V9/header_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V9/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V9/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_HDR_V11  	"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V11/header_stream_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V11/header_stream_hdf_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V11/header_stream_V11.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V11/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V11/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V11/radar_sw_config.h")											

list(APPEND GEN7_GPO_V1_FLR7_HDR_V14  	"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V14/header_stream_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V14/header_stream_hdf_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V14/header_stream_V14.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V14/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V14/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/HEADER/V14/radar_sw_config.h")										
#Grouping GEN7_GPO_V1 HDR source file
source_group(\\GEN7\\GPO_V1\\FLR7\\HEADER\\V7  FILES ${GEN7_GPO_V1_FLR7_HDR_V7})
source_group(\\GEN7\\GPO_V1\\FLR7\\HEADER\\V8  FILES ${GEN7_GPO_V1_FLR7_HDR_V8})
source_group(\\GEN7\\GPO_V1\\FLR7\\HEADER\\V9  FILES ${GEN7_GPO_V1_FLR7_HDR_V9})
source_group(\\GEN7\\GPO_V1\\FLR7\\HEADER\\V11  FILES ${GEN7_GPO_V1_FLR7_HDR_V11})
source_group(\\GEN7\\GPO_V1\\FLR7\\HEADER\\V14  FILES ${GEN7_GPO_V1_FLR7_HDR_V14})

#GEN7_GPO_V1 ID source file										
list(APPEND GEN7_GPO_V1_FLR7_ID_V1		"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V1/id_stream_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V1/id_stream_V1.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V1/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V1/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V1/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_FLR7_ID_V2		"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V2/id_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V2/id_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V2/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_ID_V3		"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V3/id_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V3/id_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V3/id_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V3/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V3/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V3/radar_sw_config.h")													
list(APPEND GEN7_GPO_V1_FLR7_ID_V4		"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V4/id_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V4/id_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V4/id_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V4/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V4/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/ID/V4/radar_sw_config.h")											
																						
#Grouping GEN7_GPO_V1 ID source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ID\\V1  FILES ${GEN7_GPO_V1_FLR7_ID_V1})
source_group(\\GEN7\\GPO_V1\\FLR7\\ID\\V2  FILES ${GEN7_GPO_V1_FLR7_ID_V2})
source_group(\\GEN7\\GPO_V1\\FLR7\\ID\\V3  FILES ${GEN7_GPO_V1_FLR7_ID_V3})
source_group(\\GEN7\\GPO_V1\\FLR7\\ID\\V4  FILES ${GEN7_GPO_V1_FLR7_ID_V4})

#GEN7_GPO_V1 DEBUG source file
list(APPEND GEN7_GPO_V1_FLR7_DEBUG_V5 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V5/debug_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V5/debug_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V5/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V5/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V5/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_DEBUG_V10 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V10/debug_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V10/debug_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V10/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V10/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_DEBUG_V12 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V12/debug_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V12/debug_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V12/debug_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V12/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V12/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V12/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_FLR7_DEBUG_V13 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V13/debug_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V13/debug_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V13/debug_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V13/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V13/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/DEBUG/V13/radar_sw_config.h")

#Grouping GEN7_GPO_V1 DEBUG source file
source_group(\\GEN7\\GPO_V1\\FLR7\\DEBUG\\V5  FILES ${GEN7_GPO_V1_FLR7_DEBUG_V5})
source_group(\\GEN7\\GPO_V1\\FLR7\\DEBUG\\V10  FILES ${GEN7_GPO_V1_FLR7_DEBUG_V10})
source_group(\\GEN7\\GPO_V1\\FLR7\\DEBUG\\V12  FILES ${GEN7_GPO_V1_FLR7_DEBUG_V12})
source_group(\\GEN7\\GPO_V1\\FLR7\\DEBUG\\V13  FILES ${GEN7_GPO_V1_FLR7_DEBUG_V13})

#GEN7_GPO_V1 DOWN_SELECTION source file
list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V2 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V2/down_selection_stream_V2.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V2/down_selection_stream_V2.h"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V2/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V2/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V2/radar_sw_config.h")
		
list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V5 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V5/down_selection_stream_V5.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V5/down_selection_stream_V5.h"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V5/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V5/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V5/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V7 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V7/down_selection_stream_V7.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V7/down_selection_stream_V7.h"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V7/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V7/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V7/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V100 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V100/down_selection_stream_V100.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V100/down_selection_stream_V100.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V100/down_selection_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V100/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V100/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V9 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V9/down_selection_stream_V9.cpp"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V9/down_selection_stream_V9.h"
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V9/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V9/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V9/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V14 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V14/down_selection_stream_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V14/down_selection_stream_hdf_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V14/down_selection_stream_V14.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V14/down_selection_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V14/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V14/radar_sw_config.h")												

list(APPEND GEN7_GPO_V1_FLR7_DOWN_SELECTION_V16 	"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V16/down_selection_stream_V16.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V16/down_selection_stream_hdf_V16.cpp"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V16/down_selection_stream_V16.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V16/down_selection_stream.h"												
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V16/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V1/FLR7/DOWN_SELECTION/V16/radar_sw_config.h")														
#Grouping GEN7_GPO_V1 DEBUG source file
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V2  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V2})
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V5  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V5})
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V7  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V7})
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V9  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V9})
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V14  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V14})
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V16  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V16})
source_group(\\GEN7\\GPO_V1\\FLR7\\DOWN_SELECTION\\V100  FILES ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V100})

#Grouping GEN7_GPO_V1 STATUS source file
source_group(\\GEN7\\GPO_V1\\FLR7\\STATUS\\V9  FILES ${GEN7_GPO_V1_FLR7_STATUS_V9})
source_group(\\GEN7\\GPO_V1\\FLR7\\STATUS\\V10  FILES ${GEN7_GPO_V1_FLR7_STATUS_V10})




#GEN7_GPO_V1 RDD source file										
list(APPEND GEN7_GPO_V1_FLR7_RDD_V25	"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V25/rdd_stream_V25.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V25/rdd_stream_V25.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V25/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V25/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V25/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V1_FLR7_RDD_V27	"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V27/rdd_stream_V27.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V27/rdd_stream_V27.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V27/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V27/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V27/radar_sw_config.h")		

										
list(APPEND GEN7_GPO_V1_FLR7_RDD_V30	"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V30/rdd_stream_V30.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V30/rdd_stream_V30.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V30/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V30/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V30/radar_sw_config.h")	


list(APPEND GEN7_GPO_V1_FLR7_RDD_V32	"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V32/rdd_stream_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V32/rdd_stream_hdf_V32.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V32/rdd_stream_V32.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V32/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V32/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V32/radar_sw_config.h")										
list(APPEND GEN7_GPO_V1_FLR7_RDD_V33	"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V33/rdd_stream_V33.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V33/rdd_stream_hdf_V33.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V33/rdd_stream_V33.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V33/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V33/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/RDD/V33/radar_sw_config.h")										
																						
#Grouping GEN7_GPO_V1 RDD source file
source_group(\\GEN7\\GPO_V1\\FLR7\\RDD\\V25  FILES ${GEN7_GPO_V1_FLR7_RDD_V25})
source_group(\\GEN7\\GPO_V1\\FLR7\\RDD\\V27  FILES ${GEN7_GPO_V1_FLR7_RDD_V27})
source_group(\\GEN7\\GPO_V1\\FLR7\\RDD\\V30  FILES ${GEN7_GPO_V1_FLR7_RDD_V30})
source_group(\\GEN7\\GPO_V1\\FLR7\\RDD\\V32  FILES ${GEN7_GPO_V1_FLR7_RDD_V32})
source_group(\\GEN7\\GPO_V1\\FLR7\\RDD\\V33  FILES ${GEN7_GPO_V1_FLR7_RDD_V33})

#GEN7_GPO_V1 ROT_ISOobject_Stream source file										
list(APPEND GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V2	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V2/ROT_ISOobject_stream_V2.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V2/ROT_ISOobject_stream_V2.h"
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V2/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V2/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V3	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream_V3.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream_V3.h"
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V3/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V3/radar_sw_config.h")																
list(APPEND GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V12	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V12/ROT_ISOobject_stream_V12.cpp"
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V12/ROT_ISOobject_stream_V12.h"
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V12/ROT_ISOobject_stream.h"												
										                    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V12/radar_sw_config_satellite.h"
															"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_ISO_OBJECT/V12/radar_sw_config.h")															
																						
#Grouping GEN7_GPO_V1 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_ISOobject_Stream\\V2  FILES ${GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V2})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_ISOobject_Stream\\V3  FILES ${GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V3})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_ISOobject_Stream\\V12  FILES ${GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V12})	

#GEN7_GPO_V1 ROT_OBJECT_STREAM source file										
list(APPEND GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V2	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V2/ROT_object_stream_V2.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V2/ROT_object_stream_V2.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V2/ROT_object_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V2/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V3	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V3/ROT_object_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V3/ROT_object_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V3/ROT_object_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_OBJECT_STREAM/V3/radar_sw_config.h")															
																						
#Grouping GEN7_GPO_V1 ROT_OBJECT_STREAM source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_OBJECT_STREAM\\V2  FILES ${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V2})
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_OBJECT_STREAM\\V3  FILES ${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V3})


#GEN7_GPO_V1 ROT_PROCESSEDDETECTION_Stream source file										
list(APPEND GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V2	"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V2/ROT_processeddetection_stream_V2.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V2/ROT_processeddetection_stream_V2.h"
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V2/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V2/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V3	"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream_V3.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream_V3.h"
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V3/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V3/radar_sw_config.h")																		
list(APPEND GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V12	"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V12/ROT_processeddetection_stream_V12.cpp"
										                        "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V12/ROT_processeddetection_stream_V12.h"
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V12/ROT_processeddetection_stream.h"												
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V12/radar_sw_config_satellite.h"
																"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_PROCESSED_DETECTION/V12/radar_sw_config.h")																
																						
#Grouping GEN7_GPO_V1 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_PROCESSEDDETECTION_Stream\\V2  FILES ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V2})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_PROCESSEDDETECTION_Stream\\V3  FILES ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V3})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_PROCESSEDDETECTION_Stream\\V12  FILES ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V12})	



#GEN7_GPO_V1 ROT_SAFETY_FAULTS source file										
list(APPEND GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V2	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream_V2.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream_V2.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V2/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V4	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_V4.cpp"
														 "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_hdf_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V4/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V4/radar_sw_config.h")
list(APPEND GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V12	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream_V12.cpp"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream_hdf_V12.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream_V12.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V12/ROT_safety_faults_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V12/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_SAFETY_FAULTS/V12/radar_sw_config.h")
																						
#Grouping GEN7_GPO_V1 ROT_SAFETY_FAULTS source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_SAFETY_FAULTS\\V2  FILES ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V2})
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_SAFETY_FAULTS\\V4  FILES ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V4})
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_SAFETY_FAULTS\\V12  FILES ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V12})



#GEN7_GPO_V1 VSE source file										
list(APPEND GEN7_GPO_V1_FLR7_VSE_V3	    "${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V3/vse_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V3/vse_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V3/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V3/radar_sw_config.h")		

list(APPEND GEN7_GPO_V1_FLR7_VSE_V4	    "${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V4/vse_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V4/vse_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V4/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V4/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_VSE_V6	    "${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V6/vse_stream_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V6/vse_stream_hdf_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V6/vse_stream_V6.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V6/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V6/radar_sw_config.h")											
list(APPEND GEN7_GPO_V1_FLR7_VSE_V7	    "${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V7/vse_stream_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V7/vse_stream_hdf_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V7/vse_stream_V7.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V7/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/VSE/V7/radar_sw_config.h")										
																						
#Grouping GEN7_GPO_V1 VSE source file
source_group(\\GEN7\\GPO_V1\\FLR7\\VSE\\V3  FILES ${GEN7_GPO_V1_FLR7_VSE_V3})	
source_group(\\GEN7\\GPO_V1\\FLR7\\VSE\\V4  FILES ${GEN7_GPO_V1_FLR7_VSE_V4})		
source_group(\\GEN7\\GPO_V1\\FLR7\\VSE\\V6  FILES ${GEN7_GPO_V1_FLR7_VSE_V6})		
source_group(\\GEN7\\GPO_V1\\FLR7\\VSE\\V7  FILES ${GEN7_GPO_V1_FLR7_VSE_V7})	


#GEN7_GPO_V1 ROT_TRACKER_INFO source file										
list(APPEND GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_V3	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V3/ROT_trackerInfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V3/radar_sw_config.h")													
list(APPEND GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_V12	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V12/ROT_tracker_info_stream_V12.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V12/ROT_tracker_info_stream_V12.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V12/ROT_trackerInfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V12/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_TRACKER_INFO/V12/radar_sw_config.h")														
																						
#Grouping GEN7_GPO_V1 ROT_TRACKER_INFO source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_TRACKER_INFO\\V3  FILES ${GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_V3})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_TRACKER_INFO\\V12  FILES ${GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_V12})	

#GEN7_GPO_V1 ROT_VEHICLE_INFO source file										
list(APPEND GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V2	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V2/ROT_vehicle_info_stream_V2.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V2/ROT_vehicle_info_stream_V2.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V2/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V2/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V4	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V4/ROT_vehicle_info_stream_V4.cpp"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V4/ROT_vehicleinfo_stream_hdf_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V4/ROT_vehicle_info_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V4/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V4/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V4/radar_sw_config.h")													
list(APPEND GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V12	    "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V12/ROT_vehicle_info_stream_V12.cpp"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V12/ROT_vehicleinfo_stream_hdf_V12.cpp"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V12/ROT_vehicle_info_stream_V12.h"
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V12/ROT_vehicleinfo_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V12/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V1/FLR7/ROT_VEHICLE_INFO/V12/radar_sw_config.h")														
																						
#Grouping GEN7_GPO_V1 ROT_VEHICLE_INFO source file
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_VEHICLE_INFO\\V2  FILES ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V2})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_VEHICLE_INFO\\V4  FILES ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V4})	
source_group(\\GEN7\\GPO_V1\\FLR7\\ROT_VEHICLE_INFO\\V12  FILES ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V12})	


#GEN7_GPO_V1 TOI source file										
list(APPEND GEN7_GPO_V1_FLR7_TOI_V2	    "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V2/toi_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V2/toi_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V2/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V1_FLR7_TOI_V3	    "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V3/toi_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V3/toi_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V3/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V3/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V3/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_TOI_V9	    "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V9/toi_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V9/toi_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V9/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V1_FLR7_TOI_V12	"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V12/toi_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V12/toi_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V12/toi_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V12/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V12/radar_sw_config.h")										
list(APPEND GEN7_GPO_V1_FLR7_TOI_V13	"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V13/toi_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V13/toi_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V13/toi_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V13/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V13/radar_sw_config.h")										
										
list(APPEND GEN7_GPO_V1_FLR7_TOI_V100	 "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V100/toi_stream_V100.cpp"
										 "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V100/toi_stream_V100.h"
										 "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V100/toi_stream.h"												
										 "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI/V100/radar_sw_config.h")
																						
#Grouping GEN7_GPO_V1 TOI source file
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI\\V2  FILES ${GEN7_GPO_V1_FLR7_TOI_V2})		
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI\\V3  FILES ${GEN7_GPO_V1_FLR7_TOI_V3})	
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI\\V9  FILES ${GEN7_GPO_V1_FLR7_TOI_V9})
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI\\V12  FILES ${GEN7_GPO_V1_FLR7_TOI_V12})
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI\\V13  FILES ${GEN7_GPO_V1_FLR7_TOI_V13})
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI\\V100  FILES ${GEN7_GPO_V1_FLR7_TOI_V100})


#GEN7_GPO_V1 TOI_DS source file
list(APPEND GEN7_GPO_V1_FLR7_TOI_DS_V2	    "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V2/toi_ds_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V2/toi_ds_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V2/toi_ds_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V2/toi_ds_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V2/radar_sw_config.h")	

list(APPEND GEN7_GPO_V1_FLR7_TOI_DS_V3	    "${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V3/toi_ds_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V3/toi_ds_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V3/toi_ds_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V3/toi_ds_stream.h"												
											"${SourceFiles}/GEN7/GPO_V1/FLR7/TOI_DS/V3/radar_sw_config.h")											
#Grouping GEN7_GPO_V1 TOI source file
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI_DS\\V2  FILES ${GEN7_GPO_V1_FLR7_TOI_DS_V2})											
source_group(\\GEN7\\GPO_V1\\FLR7\\TOI_DS\\V3  FILES ${GEN7_GPO_V1_FLR7_TOI_DS_V3})											

#GEN7 Streams 
#GPO_V2
#SRR7 Streams

#GEN7_GPO_V2 ALIGNMENT Stream Source File
list(APPEND GEN7_GPO_V2_SRR7_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/Alignment_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GEN7_GPO_V2 DETECTION Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ALIGNMENT FILES ${GEN7_GPO_V2_SRR7_ALIGNMENT_SOURCE})

#GEN7_GPO_V2 ALIGNMENT_INTERNALS Stream Source File
list(APPEND GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_SOURCE 	"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/Alignment_Internals_Logging.cpp"
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/Alignment_Internals_Logging.h")
#Grouping GEN7_GPO_V2 ALIGNMENT_INTERNALS Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ALIGNMENT_INTERNALS FILES ${GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_SOURCE})
#GEN7_GPO_V1 CDC Stream Source File
list(APPEND GEN7_GPO_V2_SRR7_CDC_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/CDC_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/CDC_Logging.h")
					
#Grouping GEN7_GPO_V1 CDC Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\CDC FILES ${GEN7_GPO_V2_SRR7_CDC_SOURCE})


#GEN7_GPO_V2 DETECTION Stream Source File
list(APPEND GEN7_GPO_V2_SRR7_DET_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/Detection_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/Detection_Logging.h")
					
#Grouping GEN7_GPO_V2 DETECTION Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION FILES ${GEN7_GPO_V2_SRR7_DET_SOURCE})

#GEN7_GPO_V2 DETECTION_DEBUG Stream Source File
list(APPEND GEN7_GPO_V2_SRR7_DET_DEBUG_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/Detection_Debug_Logging.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/Detection_Debug_Logging.h")
					
#Grouping GEN7_GPO_V2 DETECTION_DEBUG Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION_DEBUG FILES ${GEN7_GPO_V2_SRR7_DET_DEBUG_SOURCE})

#GEN7_GPO_V2 HEADER Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_HDR_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/header_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/header_Logging.h")
                                                
#Grouping GEN7_GPO_V2 HEADER Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER FILES ${GEN7_GPO_V2_SRR7_HDR_SOURCE})

#GEN7_GPO_V2 ID Stream Source File												
list(APPEND GEN7_GPO_V2_SRR7_ID_SOURCE 		    "${SourceFiles}/GEN7/GPO_V2/SRR7/ID/ID_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/ID_Logging.h")

#Grouping GEN7_GPO_V2 ID Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ID FILES ${GEN7_GPO_V2_SRR7_ID_SOURCE})


#GEN7_GPO_V2 MMIC Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_MMIC_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/MMIC_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/MMIC_Logging.h")

#Grouping GEN7_GPO_V2 MMIC Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC   FILES ${GEN7_GPO_V2_SRR7_MMIC_SOURCE})


#GEN7_GPO_V2 Down_Selection Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/Down_Selection_Logging.cpp"
                                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/Down_Selection_Logging.h")

#Grouping GEN7_GPO_V2 DOWN_SELECTION Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION   FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_SOURCE})


#Grouping GEN7_GPO_V2 DYNAMIC ALIGNMENT Stream Source File												
list(APPEND GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_SOURCE 	"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.h")
														

#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\DYNAMIC_ALIGNMENT   FILES ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_SOURCE})


#GEN7_GPO_V2 VSE Debug Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/Debug_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/Debug_Logging.h")

#Grouping GEN7_GPO_V2 DEBUG Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG   FILES ${GEN7_GPO_V2_SRR7_DEBUG_SOURCE})


#GEN7_GPO_V2 ROT_ISO_OBJECT Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_SOURCE   "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.cpp"
                                                     "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.h")
                                                       
#Grouping GEN7_GPO_V2 ROT_ISO_OBJECT Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_ISO_OBJECT FILES ${GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_SOURCE})


#GEN7_GPO_V2 ROT_SAFETY_FAULTS Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.cpp"
                                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.h")

#Grouping GEN7_GPO_V2 ROT_SAFETY_FAULTS Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_SAFETY_FAULTS  FILES ${GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_SOURCE})


#GEN7_GPO_V2 ROT Processed Detection Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE     "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.cpp"
                                                                    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.h")



#Grouping GEN7_GPO_V2 ROT Processed Detection Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_PROCESSEDDETECTION_STREAM  FILES ${GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE})


#GEN7_GPO_V2 ROT_SAE_OBJECT Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_SOURCE     "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/ROT_SAE_Object_Stream_Logging.cpp"
                                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/ROT_SAE_Object_Stream_Logging.h")

#Grouping GEN7_GPO_V2 ROT_OBJECT Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_SAE_OBJECT_STREAM  FILES ${GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_SOURCE})


#GEN7_GPO_V2 ROT_VEHICLE_INFO Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.cpp"
                                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.h")

#Grouping GEN7_GPO_V2 ROT_VEHICLE_INFO Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_VEHICLE_INFO FILES ${GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_SOURCE})


#GEN7_GPO_V2 ROT_TRACKER_INFO Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.cpp"
                                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.h")

#Grouping GEN7_GPO_V2 ROT_TRACKER_INFO Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_TRACKER_INFO FILES ${GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_SOURCE})


#GEN7_GPO_V2 STATUS Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_STATUS_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/Status_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/Status_Logging.h")

#Grouping GEN7_GPO_V2 STATUS Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\STATUS   FILES ${GEN7_GPO_V2_SRR7_STATUS_SOURCE})


#GEN7_GPO_V2 VSE Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_VSE_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/VSE_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/VSE_Logging.h")
												
#Grouping GEN7_GPO_V2 VSE Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\VSE   FILES ${GEN7_GPO_V2_SRR7_VSE_SOURCE})

#GEN7_GPO_V2 RDD Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_RDD_SOURCE         "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/RDD_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/RDD_Logging.h")

#Grouping GEN7_GPO_V2 RDD Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD FILES ${GEN7_GPO_V2_SRR7_RDD_SOURCE})

#GEN7_GPO_V2 RDD_DEBUG Stream Source File                                                
list(APPEND GEN7_GPO_V2_SRR7_RDD_DEBUG_SOURCE   "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/RDD_DEBUG_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/RDD_DEBUG_Logging.h")

#Grouping GEN7_GPO_V2 RDD_DEBUG Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD_DEBUG FILES ${GEN7_GPO_V2_SRR7_RDD_DEBUG_SOURCE})

#GEN7_GPO_V2 TOI Stream Source File												
list(APPEND GEN7_GPO_V2_SRR7_TOI_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/TOI_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/TOI_Logging.h")

#Grouping GEN7_GPO_V2 TOI Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI   FILES ${GEN7_GPO_V2_SRR7_TOI_SOURCE})

#GEN7_GPO_V2 RADAR_CAPABILITY Stream Source File												
list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/Radar_Capability_Logging.h")

#Grouping GEN7_GPO_V2 RADAR_CAPABILITY Stream Source File
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY   FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_SOURCE})

#GEN7_GPO_V2 ALIGNMENT source file
list(APPEND GEN7_GPO_V2_SRR7_ALIGNMENT_V10 	"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V10/alignment_stream_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V10/alignment_stream_hdf_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V10/alignment_stream_V10.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V10/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V10/radar_sw_config.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V10/radar_sw_config_satellite.h")
										
list(APPEND GEN7_GPO_V2_SRR7_ALIGNMENT_V11 	"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V11/alignment_stream_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V11/alignment_stream_hdf_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V11/alignment_stream_V11.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V11/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V11/radar_sw_config.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT/V11/radar_sw_config_satellite.h")										

#Grouping GEN7_GPO_V2 ALIGNMENT source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ALIGNMENT\\V10  FILES ${GEN7_GPO_V2_SRR7_ALIGNMENT_V10})
source_group(\\GEN7\\GPO_V2\\SRR7\\ALIGNMENT\\V11  FILES ${GEN7_GPO_V2_SRR7_ALIGNMENT_V11})

#GEN7_GPO_V2 ALIGNMENT_INTERNALS source file
list(APPEND GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_V1 	"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V1/alignment_internals_stream_V1.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V1/alignment_internals_stream_V1.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V1/alignment_internals_stream.h"												
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V1/radar_sw_config.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V1/radar_sw_config_satellite.h")
														
														

list(APPEND GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_V2 	"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream_V2.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream_V2.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V2/radar_sw_config.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ALIGNMENT_INTERNALS/V2/radar_sw_config_satellite.h")
														
#Grouping GEN7_GPO_V2 ALIGNMENT_INTERNALS source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ALIGNMENT_INTERNALS\\V1  FILES ${GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_V1})
source_group(\\GEN7\\GPO_V2\\SRR7\\ALIGNMENT_INTERNALS\\V2  FILES ${GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_V2})
#GEN7_GPO_V2 CDC source file
list(APPEND GEN7_GPO_V2_SRR7_CDC_V9 	"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V9/cdc_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V9/cdc_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V9/cdc_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_CDC_V10 	"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V10/cdc_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V10/cdc_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V10/cdc_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_CDC_V11 	"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V11/cdc_stream_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V11/cdc_stream_V11.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V11/cdc_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V11/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_CDC_V12 	"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V12/cdc_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V12/cdc_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V12/cdc_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/CDC/V12/radar_sw_config.h")										

#Grouping GEN7_GPO_V2 CDC source file
source_group(\\GEN7\\GPO_V2\\SRR7\\CDC\\V9  FILES ${GEN7_GPO_V2_SRR7_CDC_V9})
source_group(\\GEN7\\GPO_V2\\SRR7\\CDC\\V10  FILES ${GEN7_GPO_V2_SRR7_CDC_V10})
source_group(\\GEN7\\GPO_V2\\SRR7\\CDC\\V11  FILES ${GEN7_GPO_V2_SRR7_CDC_V11})
source_group(\\GEN7\\GPO_V2\\SRR7\\CDC\\V12  FILES ${GEN7_GPO_V2_SRR7_CDC_V12})


#GEN7_GPO_V2 DET source file
list(APPEND GEN7_GPO_V2_SRR7_DET_V10	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V10/detection_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V10/detection_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V10/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V10/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_DET_V12	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V12/detection_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V12/detection_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V12/detection_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V12/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V12/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V12/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_DET_V15	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V15/detection_stream_V15.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V15/detection_stream_hdf_V15.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V15/detection_stream_V15.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V15/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V15/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V15/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_DET_V16	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V16/detection_stream_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V16/detection_stream_hdf_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V16/detection_stream_V16.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V16/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V16/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V16/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_DET_V17	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V17/detection_stream_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V17/detection_stream_hdf_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V17/detection_stream_V17.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V17/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V17/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V17/radar_sw_config.h")										
										
list(APPEND GEN7_GPO_V2_SRR7_DET_V19	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V19/detection_stream_V19.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V19/detection_stream_hdf_V19.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V19/detection_stream_V19.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V19/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V19/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V19/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_DET_V21	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V21/detection_stream_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V21/detection_stream_hdf_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V21/detection_stream_V21.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V21/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V21/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION/V21/radar_sw_config.h")										
																				
#Grouping GEN7_GPO_V2 DET source file
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V10  FILES ${GEN7_GPO_V2_SRR7_DET_V10})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V12  FILES ${GEN7_GPO_V2_SRR7_DET_V12})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V15  FILES ${GEN7_GPO_V2_SRR7_DET_V15})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V16  FILES ${GEN7_GPO_V2_SRR7_DET_V16})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V17  FILES ${GEN7_GPO_V2_SRR7_DET_V17})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V19  FILES ${GEN7_GPO_V2_SRR7_DET_V19})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION\\V21  FILES ${GEN7_GPO_V2_SRR7_DET_V21})
#GEN7_GPO_V2 DET_DEBUG source file
list(APPEND GEN7_GPO_V2_SRR7_DET_DEBUG_V1	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V1/detection_debug_stream_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V1/detection_debug_stream_hdf_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V1/detection_debug_stream_V1.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V1/detection_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V1/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V1/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_DET_DEBUG_V2	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V2/detection_debug_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V2/detection_debug_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V2/detection_debug_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V2/detection_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V2/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_DET_DEBUG_V3	"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V3/detection_debug_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V3/detection_debug_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V3/detection_debug_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V3/detection_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V3/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/DETECTION_DEBUG/V3/radar_sw_config.h")


#Grouping GEN7_GPO_V2 DET_DEBUG source file
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION_DEBUG\\V1 FILES ${GEN7_GPO_V2_SRR7_DET_DEBUG_V1})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION_DEBUG\\V2 FILES ${GEN7_GPO_V2_SRR7_DET_DEBUG_V2})
source_group(\\GEN7\\GPO_V2\\SRR7\\DETECTION_DEBUG\\V3 FILES ${GEN7_GPO_V2_SRR7_DET_DEBUG_V3})

#GEN7_GPO_V2 HDR source file
list(APPEND GEN7_GPO_V2_SRR7_HDR_V2		"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V2/header_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V2/header_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V2/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V2/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_HDR_V4		"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V4/header_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V4/header_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V4/header_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V4/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V4/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_HDR_V5		"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V5/header_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V5/header_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V5/header_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V5/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V5/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_HDR_V7		"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V7/header_stream_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V7/header_stream_hdf_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V7/header_stream_V7.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V7/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V7/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_HDR_V9		"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V9/header_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V9/header_stream_hdf_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V9/header_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V9/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V9/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_HDR_V10	"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V10/header_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V10/header_stream_hdf_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V10/header_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V10/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_HDR_V11	"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V11/header_stream_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V11/header_stream_hdf_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V11/header_stream_V11.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V11/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/HEADER/V11/radar_sw_config.h")
										
										

#Grouping GEN7_GPO_V2 HDR source file
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V2  FILES ${GEN7_GPO_V2_SRR7_HDR_V2})
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V4  FILES ${GEN7_GPO_V2_SRR7_HDR_V4})
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V5  FILES ${GEN7_GPO_V2_SRR7_HDR_V5})
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V7  FILES ${GEN7_GPO_V2_SRR7_HDR_V7})
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V9  FILES ${GEN7_GPO_V2_SRR7_HDR_V9})
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V10  FILES ${GEN7_GPO_V2_SRR7_HDR_V10})
source_group(\\GEN7\\GPO_V2\\SRR7\\HEADER\\V11  FILES ${GEN7_GPO_V2_SRR7_HDR_V11})

#GEN7_GPO_V2 ID source file
list(APPEND GEN7_GPO_V2_SRR7_ID_V3		"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V3/id_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V3/id_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V3/id_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V3/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V3/radar_sw_config.h")													
			
list(APPEND GEN7_GPO_V2_SRR7_ID_V5		"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V5/id_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V5/id_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V5/id_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V5/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/ID/V5/radar_sw_config.h")			
#Grouping GEN7_GPO_V2 ID source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ID\\V3  FILES ${GEN7_GPO_V2_SRR7_ID_V3})
source_group(\\GEN7\\GPO_V2\\SRR7\\ID\\V5  FILES ${GEN7_GPO_V2_SRR7_ID_V5})		
									
#GEN7_GPO_V2 MMIC source file   

#GEN7_GPO_V2 MMIC source file										
list(APPEND GEN7_GPO_V2_SRR7_MMIC_V4	    "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V4/mmic_stream_V4.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V4/mmic_stream_hdf_V4.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V4/mmic_stream_V4.h"
										    "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V4/mmic_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V4/radar_sw_config.h")
                                     
list(APPEND GEN7_GPO_V2_SRR7_MMIC_V7        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V7/mmic_stream_V7.cpp"
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V7/mmic_stream_hdf_V7.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V7/mmic_stream_V7.h"
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V7/mmic_stream.h"                                                
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V7/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_MMIC_V8        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V8/mmic_stream_V8.cpp"
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V8/mmic_stream_hdf_V8.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V8/mmic_stream_V8.h"
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V8/mmic_stream.h"                                                
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V8/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_MMIC_V9        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V9/mmic_stream_V9.cpp"
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V9/mmic_stream_hdf_V9.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V9/mmic_stream_V9.h"
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V9/mmic_stream.h"                                                
                                            "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V9/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_MMIC_V12        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V12/mmic_stream_V12.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V12/mmic_stream_hdf_V12.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V12/mmic_stream_V12.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V12/mmic_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V12/radar_sw_config.h")		
											
list(APPEND GEN7_GPO_V2_SRR7_MMIC_V13        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V13/mmic_stream_V13.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V13/mmic_stream_hdf_V13.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V13/mmic_stream_V13.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V13/mmic_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V13/radar_sw_config.h")											

list(APPEND GEN7_GPO_V2_SRR7_MMIC_V16        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V16/mmic_stream_V16.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V16/mmic_stream_hdf_V16.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V16/mmic_stream_V16.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V16/mmic_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V16/radar_sw_config.h")			

list(APPEND GEN7_GPO_V2_SRR7_MMIC_V17        "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V17/mmic_stream_V17.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V17/mmic_stream_hdf_V17.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V17/mmic_stream_V17.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V17/mmic_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/MMIC/V17/radar_sw_config.h")												 
																						
#Grouping GEN7_GPO_V2 MMIC source file
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V4  FILES ${GEN7_GPO_V2_SRR7_MMIC_V4})    
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V7  FILES ${GEN7_GPO_V2_SRR7_MMIC_V7})    
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V8  FILES ${GEN7_GPO_V2_SRR7_MMIC_V8})    
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V9  FILES ${GEN7_GPO_V2_SRR7_MMIC_V9})    
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V12  FILES ${GEN7_GPO_V2_SRR7_MMIC_V12})    
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V13  FILES ${GEN7_GPO_V2_SRR7_MMIC_V13})    
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V16  FILES ${GEN7_GPO_V2_SRR7_MMIC_V16})
source_group(\\GEN7\\GPO_V2\\SRR7\\MMIC\\V17  FILES ${GEN7_GPO_V2_SRR7_MMIC_V17})
#GEN7_GPO_V2 Down_Selection source file
list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V8     	"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V8/Down_Selection_stream_V8.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V8/down_selection_stream_V8.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V8/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V8/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V8/radar_sw_config.h")
													
list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V9     	"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V9/Down_Selection_stream_V9.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V9/down_selection_stream_hdf_V9.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V9/down_selection_stream_V9.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V9/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V9/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V9/radar_sw_config.h")
													
list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V11     "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V11/Down_Selection_stream_V11.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V11/down_selection_stream_hdf_V11.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V11/down_selection_stream_V11.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V11/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V11/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V11/radar_sw_config.h")	
													
list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V12     "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V12/Down_Selection_stream_V12.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V12/down_selection_stream_hdf_V12.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V12/down_selection_stream_V12.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V12/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V12/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V12/radar_sw_config.h")				
													
list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V13     "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V13/Down_Selection_stream_V13.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V13/down_selection_stream_hdf_V13.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V13/down_selection_stream_V13.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V13/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V13/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V13/radar_sw_config.h")														

list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V14     "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V14/Down_Selection_stream_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V14/down_selection_stream_hdf_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V14/down_selection_stream_V14.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V14/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V14/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V14/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_DOWN_SELECTION_V15     "${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V15/Down_Selection_stream_V15.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V15/down_selection_stream_hdf_V15.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V15/down_selection_stream_V15.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V15/down_selection_stream.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V15/down_selection_stream_MCIP.h"                                                
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DOWN_SELECTION/V15/radar_sw_config.h")														

                                                                             
#Grouping GEN7_GPO_V2 Down_Selection source file
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V8  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V8})                                        
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V9  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V9})                                        
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V11  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V11})                                        
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V12  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V12})
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V13  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V13})
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V14  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V14})
source_group(\\GEN7\\GPO_V2\\SRR7\\DOWN_SELECTION\\V15  FILES ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V15})
#GEN7_GPO_V2 DYNAMIC ALIGNMENT source file		

list(APPEND GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V12	"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_align_stream_hdf_V12.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config.h")	                                        

#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT source file
source_group(\\GEN7\\GPO_V2\\SRR7\\DYNAMIC_ALIGNMENT\\V12  FILES ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V12})	 
								
list(APPEND GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V14	"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_align_stream_hdf_V14.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream_V14.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V14/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V14/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V14/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V15	"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_alignment_stream_V15.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_align_stream_hdf_V15.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_alignment_stream_V15.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V15/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V15/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V15/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V16	"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream_V16.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_align_stream_hdf_V16.cpp"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream_V16.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V16/radar_sw_config_satellite.h"
													"${SourceFiles}/GEN7/GPO_V2/SRR7/DYNAMIC_ALIGNMENT/V16/radar_sw_config.h")														

#Grouping GEN7_GPO_V1 DYNAMIC ALIGNMENT source file
source_group(\\GEN7\\GPO_V2\\SRR7\\DYNAMIC_ALIGNMENT\\V14  FILES ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V14})
source_group(\\GEN7\\GPO_V2\\SRR7\\DYNAMIC_ALIGNMENT\\V15  FILES ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V15})	                                        
source_group(\\GEN7\\GPO_V2\\SRR7\\DYNAMIC_ALIGNMENT\\V16  FILES ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V16})	                                        


#GEN7_GPO_V2 Debug source file                                        
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V2        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V2/debug_stream_V2.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V2/debug_stream_V2.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V2/debug_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V2/radar_sw_config.h")  
											 
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V3        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V3/debug_stream_V3.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V3/debug_stream_hdf_V3.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V3/debug_stream_V3.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V3/debug_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V3/radar_sw_config.h") 

list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V5        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V5/debug_stream_V5.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V5/debug_stream_hdf_V5.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V5/debug_stream_V5.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V5/debug_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V5/radar_sw_config.h") 	
											 
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V8        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V8/debug_stream_V8.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V8/debug_stream_hdf_V8.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V8/debug_stream_V8.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V8/debug_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V8/radar_sw_config.h") 	

list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V9        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V9/debug_stream_V9.cpp"
											 "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V9/debug_stream_hdf_V9.cpp"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V9/debug_stream_V9.h"
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V9/debug_stream.h"                                                
                                             "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V9/radar_sw_config.h")
											 
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V11        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V11/debug_stream_V11.cpp"
											  "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V11/debug_stream_hdf_V11.cpp"
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V11/debug_stream_V11.h"
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V11/debug_stream.h"                                                
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V11/radar_sw_config.h") 	
											  
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V20        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V20/debug_stream_V20.cpp"
											  "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V20/debug_stream_hdf_V20.cpp"
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V20/debug_stream_V20.h"
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V20/debug_stream.h"                                                
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V20/radar_sw_config.h") 											 
                                                                       
list(APPEND GEN7_GPO_V2_SRR7_DEBUG_V21        "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V21/debug_stream_V21.cpp"
											  "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V21/debug_stream_hdf_V21.cpp"
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V21/debug_stream_V21.h"
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V21/debug_stream.h"                                                
                                              "${SourceFiles}/GEN7/GPO_V2/SRR7/DEBUG/V21/radar_sw_config.h") 																	   
                                                                                        
#Grouping GEN7_GPO_V2 Debug source file
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V2  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V2})   
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V3  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V3})   
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V5  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V5})   
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V8  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V8})  
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V9  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V9}) 
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V11  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V11}) 
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V20  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V20}) 
source_group(\\GEN7\\GPO_V2\\SRR7\\DEBUG\\V21  FILES ${GEN7_GPO_V2_SRR7_DEBUG_V21}) 


#GEN7_GPO_V2 ROT_ISOobject_Stream source file										
list(APPEND GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_V5	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V5/ROT_ISOobject_stream_V5.cpp"
										            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V5/ROT_ISOobject_stream_V5.h"
										            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V5/rot_iso_object_stream.h"		
													"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V5/radar_sw_config.h")	
												
list(APPEND GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_V7	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream_V7.cpp"
										            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream_V7.h"
										            "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V7/rot_iso_object_stream.h"		
													"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_ISO_OBJECT/V7/radar_sw_config.h")													

#Grouping GEN7_GPO_V2 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_ISO_OBJECT\\V5  FILES ${GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_V5})	
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_ISO_OBJECT\\V7  FILES ${GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_V7})	


#GEN7_GPO_V2 ROT_PROCESSEDDETECTION_Stream source file										
list(APPEND GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_Stream_V6	"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream_V6.cpp"
										                        "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream_V6.h"
																"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V6/rot_processed_detection_stream.h"	
																"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V6/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_Stream_V7	"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V7/ROT_processeddetection_stream_V7.cpp"
										                        "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V7/ROT_processeddetection_stream_V7.h"
																"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V7/rot_processed_detection_stream.h"	
																"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_PROCESSED_DETECTION/V7/radar_sw_config.h")																	

#Grouping GEN7_GPO_V2 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V6  FILES ${GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_Stream_V6})	
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_PROCESSEDDETECTION_Stream\\V7  FILES ${GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_Stream_V7})

#GEN7_GPO_V2 ROT_SAFETY_FAULTS source file										
list(APPEND GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_V5	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream_V5.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream_V5.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V5/rot_safety_faults_stream.h"	
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V5/radar_sw_config.h")
														
list(APPEND GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_V6	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V6/ROT_safety_faults_stream_V6.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V6/ROT_safety_faults_stream_V6.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V6/rot_safety_faults_stream.h"	
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAFETY_FAULTS/V6/radar_sw_config.h")														
					
#Grouping GEN7_GPO_V2 ROT_SAFETY_FAULTS source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_SAFETY_FAULTS\\V5  FILES ${GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_V5})
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_SAFETY_FAULTS\\V6  FILES ${GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_V6})


#GEN7_GPO_V2 ROT_TRACKER_INFO source file										
list(APPEND GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_V3	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V3/rot_tracker_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V3/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_V4	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V4/rot_tracker_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_TRACKER_INFO/V4/radar_sw_config.h")															
																						
#Grouping GEN7_GPO_V2 ROT_TRACKER_INFO source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_TRACKER_INFO\\V3  FILES ${GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_V3})	
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_TRACKER_INFO\\V4  FILES ${GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_V4})

#GEN7_GPO_V2 ROT_VEHICLE_INFO source file										
list(APPEND GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_V5	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V5/ROT_vehicle_info_stream_V5.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V5/ROT_vehicle_info_stream_V5.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V5/rot_vehicle_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V5/radar_sw_config.h")
														
list(APPEND GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_V6	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V6/ROT_vehicle_info_stream_V6.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V6/ROT_vehicle_info_stream_V6.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V6/rot_vehicle_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_VEHICLE_INFO/V6/radar_sw_config.h")														
																						
#Grouping GEN7_GPO_V2 ROT_VEHICLE_INFO source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_VEHICLE_INFO\\V5  FILES ${GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_V5})	
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_VEHICLE_INFO\\V6  FILES ${GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_V6})	

#GEN7_GPO_V2 ROT_OBJECT_STREAM source file										
list(APPEND GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_V6	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V6/ROT_sae_object_stream_V6.cpp"
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V6/ROT_sae_object_stream_V6.h"
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V6/rot_sae_object_stream.h"		
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V6/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_V7	    "${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V7/ROT_sae_object_stream_V7.cpp"
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V7/ROT_sae_object_stream_V7.h"
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V7/rot_sae_object_stream.h"		
															"${SourceFiles}/GEN7/GPO_V2/SRR7/ROT_SAE_OBJECT_STREAM/V7/radar_sw_config.h")																
																						
#Grouping GEN7_GPO_V2 ROT_OBJECT_STREAM source file
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_SAE_OBJECT_STREAM\\V6  FILES ${GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_V6})
source_group(\\GEN7\\GPO_V2\\SRR7\\ROT_SAE_OBJECT_STREAM\\V7  FILES ${GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_V7})


#GEN7_GPO_V2 STATUS source file
list(APPEND GEN7_GPO_V2_SRR7_STATUS_V1  "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V1/status_stream_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V1/status_stream_hdf_V1.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V1/status_stream_V1.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V1/status_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V1/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_STATUS_V2  "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V2/status_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V2/status_stream_hdf_V2.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V2/status_stream_V2.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V2/status_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_STATUS_V3  "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V3/status_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V3/status_stream_hdf_V3.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V3/status_stream_V3.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V3/status_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V3/radar_sw_config.h") 
										
list(APPEND GEN7_GPO_V2_SRR7_STATUS_V4  "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V4/status_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V4/status_stream_hdf_V4.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V4/status_stream_V4.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V4/status_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/STATUS/V4/radar_sw_config.h") 										
 
#Grouping GEN7_GPO_V2 STATUS source file
source_group(\\GEN7\\GPO_V2\\SRR7\\STATUS\\V1  FILES ${GEN7_GPO_V2_SRR7_STATUS_V1})                                        
source_group(\\GEN7\\GPO_V2\\SRR7\\STATUS\\V2  FILES ${GEN7_GPO_V2_SRR7_STATUS_V2})                                        
source_group(\\GEN7\\GPO_V2\\SRR7\\STATUS\\V3  FILES ${GEN7_GPO_V2_SRR7_STATUS_V3})           
source_group(\\GEN7\\GPO_V2\\SRR7\\STATUS\\V4  FILES ${GEN7_GPO_V2_SRR7_STATUS_V4})                 
#GEN7_GPO_V2 VSE source file                                        
list(APPEND GEN7_GPO_V2_SRR7_VSE_V2     "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V2/vse_stream_V2.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V2/vse_stream_V2.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V2/vse_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V2/radar_sw_config.h")  
										
list(APPEND GEN7_GPO_V2_SRR7_VSE_V3     "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V3/vse_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V3/vse_stream_hdf_V3.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V3/vse_stream_V3.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V3/vse_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V3/radar_sw_config.h")  

list(APPEND GEN7_GPO_V2_SRR7_VSE_V4     "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V4/vse_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V4/vse_stream_hdf_V4.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V4/vse_stream_V4.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V4/vse_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V4/radar_sw_config.h") 

list(APPEND GEN7_GPO_V2_SRR7_VSE_V5     "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V5/vse_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V5/vse_stream_hdf_V5.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V5/vse_stream_V5.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V5/vse_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V5/radar_sw_config.h") 

list(APPEND GEN7_GPO_V2_SRR7_VSE_V6     "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V6/vse_stream_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V6/vse_stream_hdf_V6.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V6/vse_stream_V6.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V6/vse_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/VSE/V6/radar_sw_config.h") 										
                                                                               
                                                                                        
#Grouping GEN7_GPO_V2 VSE source file
source_group(\\GEN7\\GPO_V2\\SRR7\\VSE\\V2  FILES ${GEN7_GPO_V2_SRR7_VSE_V2})    
source_group(\\GEN7\\GPO_V2\\SRR7\\VSE\\V3  FILES ${GEN7_GPO_V2_SRR7_VSE_V3})    
source_group(\\GEN7\\GPO_V2\\SRR7\\VSE\\V4  FILES ${GEN7_GPO_V2_SRR7_VSE_V4})    
source_group(\\GEN7\\GPO_V2\\SRR7\\VSE\\V5  FILES ${GEN7_GPO_V2_SRR7_VSE_V5}) 
source_group(\\GEN7\\GPO_V2\\SRR7\\VSE\\V6  FILES ${GEN7_GPO_V2_SRR7_VSE_V6})

#GEN7_GPO_V2 RDD source file                                        
list(APPEND GEN7_GPO_V2_SRR7_RDD_V2     "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V2/rdd_stream_V2.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V2/rdd_stream_hdf_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V2/rdd_stream_V2.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V2/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V2/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V2/radar_sw_config.h")
										
#GEN7_GPO_V2 RDD source file                                        
list(APPEND GEN7_GPO_V2_SRR7_RDD_V5     "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V5/rdd_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V5/rdd_stream_hdf_V5.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V5/rdd_stream_V5.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V5/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V5/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V5/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_RDD_V8     "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V8/rdd_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V8/rdd_stream_hdf_V8.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V8/rdd_stream_V8.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V8/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V8/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V8/radar_sw_config.h")		
										
list(APPEND GEN7_GPO_V2_SRR7_RDD_V12    "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V12/rdd_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V12/rdd_stream_hdf_V12.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V12/rdd_stream_V12.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V12/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V12/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V12/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_RDD_V13    "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V13/rdd_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V13/rdd_stream_hdf_V13.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V13/rdd_stream_V13.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V13/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V13/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V13/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_RDD_V14    "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V14/rdd_stream_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V14/rdd_stream_hdf_V14.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V14/rdd_stream_V14.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V14/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V14/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V14/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_RDD_V16    "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V16/rdd_stream_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V16/rdd_stream_hdf_V16.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V16/rdd_stream_V16.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V16/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V16/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V16/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_RDD_V17    "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V17/rdd_stream_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V17/rdd_stream_hdf_V17.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V17/rdd_stream_V17.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V17/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V17/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V17/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_RDD_V18    "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V18/rdd_stream_V18.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V18/rdd_stream_hdf_V18.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V18/rdd_stream_V18.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V18/rdd_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V18/radar_sw_config_satellite.h"
                                        "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD/V18/radar_sw_config.h")										
										
                                        
#Grouping GEN7_GPO_V2 RDD source file
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V2  FILES ${GEN7_GPO_V2_SRR7_RDD_V2})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V5  FILES ${GEN7_GPO_V2_SRR7_RDD_V5})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V8  FILES ${GEN7_GPO_V2_SRR7_RDD_V8})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V12  FILES ${GEN7_GPO_V2_SRR7_RDD_V12})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V13  FILES ${GEN7_GPO_V2_SRR7_RDD_V13})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V14  FILES ${GEN7_GPO_V2_SRR7_RDD_V14})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V16  FILES ${GEN7_GPO_V2_SRR7_RDD_V16})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V17  FILES ${GEN7_GPO_V2_SRR7_RDD_V17})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD\\V18  FILES ${GEN7_GPO_V2_SRR7_RDD_V18})

#GEN7_GPO_V2 RDD_DEBUG source file                                        
list(APPEND GEN7_GPO_V2_SRR7_RDD_DEBUG_V1   "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V1/rdd_debug_stream_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V1/rdd_debug_stream_hdf_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V1/rdd_debug_stream_V1.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V1/rdd_debug_stream.h"                                                
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V1/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V1/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_RDD_DEBUG_V2   "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V2/rdd_debug_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V2/rdd_debug_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V2/rdd_debug_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V2/rdd_debug_stream.h"                                                
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V2/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_RDD_DEBUG_V3   "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V3/rdd_debug_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V3/rdd_debug_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V3/rdd_debug_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V3/rdd_debug_stream.h"                                                
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V3/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V3/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_RDD_DEBUG_V7   "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V7/rdd_debug_stream_V7.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V7/rdd_debug_stream_hdf_V7.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V7/rdd_debug_stream_V7.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V7/rdd_debug_stream.h"                                                
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V7/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V7/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_SRR7_RDD_DEBUG_V9   "${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V9/rdd_debug_stream_V9.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V9/rdd_debug_stream_hdf_V9.cpp"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V9/rdd_debug_stream_V9.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V9/rdd_debug_stream.h"                                                
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V9/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/SRR7/RDD_DEBUG/V9/radar_sw_config.h")
							
							
#Grouping GEN7_GPO_V2 RDD_DEBUG source file
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD_DEBUG\\V1  FILES ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V1})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD_DEBUG\\V2  FILES ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V2})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD_DEBUG\\V3  FILES ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V3})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD_DEBUG\\V7  FILES ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V7})
source_group(\\GEN7\\GPO_V2\\SRR7\\RDD_DEBUG\\V9 FILES ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V9})


#GEN7_GPO_V2 TOI source file										
list(APPEND GEN7_GPO_V2_SRR7_TOI_V1	    "${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V1/toi_stream_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V1/toi_stream_hdf_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V1/toi_stream_V1.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V1/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V1/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V1/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_TOI_V2	    "${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V2/toi_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V2/toi_stream_hdf_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V2/toi_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V2/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V2/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V2_SRR7_TOI_V3	    "${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V3/toi_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V3/toi_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V3/toi_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V3/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V3/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V3/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_TOI_V4	    "${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V4/toi_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V4/toi_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V4/toi_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V4/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V4/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V4/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_SRR7_TOI_V8	    "${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V8/toi_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V8/toi_stream_hdf_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V8/toi_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V8/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V8/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V8/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_TOI_V10	"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V10/toi_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V10/toi_stream_hdf_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V10/toi_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V10/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V10/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V10/radar_sw_config.h")		

list(APPEND GEN7_GPO_V2_SRR7_TOI_V12	"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V12/toi_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V12/toi_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V12/toi_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V12/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V12/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V12/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_TOI_V13	"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V13/toi_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V13/toi_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V13/toi_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V13/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V13/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/SRR7/TOI/V13/radar_sw_config.h")	
										
#Grouping GEN7_GPO_V2 TOI source file
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V1  FILES ${GEN7_GPO_V2_SRR7_TOI_V1})
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V2  FILES ${GEN7_GPO_V2_SRR7_TOI_V2})
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V3  FILES ${GEN7_GPO_V2_SRR7_TOI_V3})
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V4  FILES ${GEN7_GPO_V2_SRR7_TOI_V4})
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V8  FILES ${GEN7_GPO_V2_SRR7_TOI_V8})
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V10  FILES ${GEN7_GPO_V2_SRR7_TOI_V10})			
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V12  FILES ${GEN7_GPO_V2_SRR7_TOI_V12})							
source_group(\\GEN7\\GPO_V2\\SRR7\\TOI\\V13  FILES ${GEN7_GPO_V2_SRR7_TOI_V13})
#GEN7_GPO_V2 RADAR CAPABILITY source file										
list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V35	    "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream_V35.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream_hdf_V35.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream_V35.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V35/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V35/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V35/radar_sw_config.h")
														
#GEN7_GPO_V2 RADAR CAPABILITY source file										
list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V39	    "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream_hdf_V39.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V39/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V39/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V39/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V42	    "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream_V42.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream_hdf_V42.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream_V42.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V42/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V42/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V47	    "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V47/radar_capability_stream_V47.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V47/radar_capability_stream_hdf_V47.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V47/radar_capability_stream_V47.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V47/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V47/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V47/radar_sw_config.h")															

list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V48	    "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V48/radar_capability_stream_V48.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V48/radar_capability_stream_hdf_V48.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V48/radar_capability_stream_V48.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V48/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V48/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V48/radar_sw_config.h")
														
list(APPEND GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V53   "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V53/radar_capability_stream_V53.cpp"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V53/radar_capability_stream_hdf_V53.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V53/radar_capability_stream_V53.h"
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V53/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V53/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/SRR7/RADAR_CAPABILITY/V53/radar_sw_config.h")			
														
#Grouping GEN7_GPO_V2 RADAR CAPABILITY source file
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY\\V35  FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V35})
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY\\V39  FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V39})
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY\\V42  FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V42})
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY\\V47  FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V47})
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY\\V48  FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V48})
source_group(\\GEN7\\GPO_V2\\SRR7\\RADAR_CAPABILITY\\V53  FILES ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V53})

#GEN7 Streams 
#GPO_V2
#FLR7 Streams

#GEN7_GPO_V2 ALIGNMENT Stream Source File
list(APPEND GEN7_GPO_V2_FLR7_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/Alignment_Logging.cpp"
													"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GEN7_GPO_V2 ALIGNMENT Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ALIGNMENT FILES ${GEN7_GPO_V2_FLR7_ALIGNMENT_SOURCE})

#GEN7_GPO_V2 ALIGNMENT_INTERNALS Stream Source File
list(APPEND GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_SOURCE 	"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/Alignment_Internals_Logging.cpp"
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/Alignment_Internals_Logging.h")
#Grouping GEN7_GPO_V2 DETECTION Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ALIGNMENT_INTERNALS FILES ${GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_SOURCE})
#GEN7_GPO_V2 MMIC Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_MMIC_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/MMIC_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/MMIC_Logging.h")

#Grouping GEN7_GPO_V2 MMIC Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC   FILES ${GEN7_GPO_V2_FLR7_MMIC_SOURCE})

#GEN7_GPO_V2 RDD Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_RDD_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/RDD_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/RDD_Logging.h")
#GEN7_GPO_V2 RADAR_CAPABILITY Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_SOURCE 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/Radar_Capability_Logging.h")
														
#Grouping GEN7_GPO_V2 RADAR_CAPABILITY Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY   FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_SOURCE})

#Grouping GEN7_GPO_V2 RDD Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD FILES ${GEN7_GPO_V2_FLR7_RDD_SOURCE})

#GEN7_GPO_V2 ROT_ISOobject Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.h")

#Grouping GEN7_GPO_V2 ROT_ISOobject Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_ISOobject_STREAM  FILES ${GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_SOURCE})

#GEN7_GPO_V2 ROT Processed Detection Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.cpp"
												                        "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.h")

#Grouping GEN7_GPO_V2 ROT Processed Detection Stream Source File
source_group(\\GEN7\\GPO_V2\\FlR7\\ROT_PROCESSEDDETECTION_STREAM  FILES ${GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE})

#GEN7_GPO_V2 ROT_SAFETY_FAULTS Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.h")

#Grouping GEN7_GPO_V2 ROT_SAFETY_FAULTS Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_SAFETY_FAULTS  FILES ${GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_SOURCE})

#GEN7_GPO_V2 STATUS Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_STATUS_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/Status_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/Status_Logging.h")

#Grouping GEN7_GPO_V2 STATUS Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\STATUS   FILES ${GEN7_GPO_V2_FLR7_STATUS_SOURCE})


#GEN7_GPO_V2 VSE Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_VSE_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/VSE_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/VSE_Logging.h")

#Grouping GEN7_GPO_V2 VSE Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\VSE   FILES ${GEN7_GPO_V2_FLR7_VSE_SOURCE})

#GEN7_GPO_V2 ROT_TRACKER_INFO Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.h")

#Grouping GEN7_GPO_V2 ROT_TRACKER_INFO Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_TRACKER_INFO FILES ${GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_SOURCE})

#GEN7_GPO_V2 ROT_VEHICLE_INFO Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.h")

#Grouping GEN7_GPO_V2 ROT_VEHICLE_INFO Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_VEHICLE_INFO   FILES ${GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_SOURCE})

#GEN7_GPO_V2 DETECTION Stream Source File
list(APPEND GEN7_GPO_V2_FLR7_DET_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/Detection_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/Detection_Logging.h")
					
#Grouping GEN7_GPO_V2 DETECTION Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION FILES ${GEN7_GPO_V2_FLR7_DET_SOURCE})

#GEN7_GPO_V2 DETECTION_DEBUG Stream Source File
list(APPEND GEN7_GPO_V2_FLR7_DET_DEBUG_SOURCE 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/Detection_Debug_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/Detection_Debug_Logging.h")
					
#Grouping GEN7_GPO_V2 DETECTION_DEBUG Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION_DEBUG FILES ${GEN7_GPO_V2_FLR7_DET_DEBUG_SOURCE})


#GEN7_GPO_V2 HEADER Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_HDR_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/header_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/header_Logging.h")

#Grouping GEN7_GPO_V2 HEADER Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER FILES ${GEN7_GPO_V2_FLR7_HDR_SOURCE})

#GEN7_GPO_V2 ID Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ID_SOURCE 		    "${SourceFiles}/GEN7/GPO_V2/FLR7/ID/ID_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/ID_Logging.h")

#Grouping GEN7_GPO_V2 ID Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ID FILES ${GEN7_GPO_V2_FLR7_ID_SOURCE})

#GEN7_GPO_V2 DEBUG Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_DEBUG_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/Debug_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/Debug_Logging.h")

#Grouping GEN7_GPO_V2 DEBUG Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG   FILES ${GEN7_GPO_V2_FLR7_DEBUG_SOURCE})

#GEN7_GPO_V2 DOWN_SELECTION Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/Down_Selection_Logging.cpp"
												        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/Down_Selection_Logging.h")

#Grouping GEN7_GPO_V2 DOWN_SELECTION Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION   FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_SOURCE})

#GEN7_GPO_V2 TOI Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_TOI_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/TOI_Logging.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/TOI_Logging.h")

#Grouping GEN7_GPO_V2 TOI Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI   FILES ${GEN7_GPO_V2_FLR7_TOI_SOURCE})

#GEN7_GPO_V2 ROT_SAE_OBJECT Stream Source File												
list(APPEND GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_SOURCE 		"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/ROT_SAE_Object_Stream_Logging.cpp"
																"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/ROT_SAE_Object_Stream_Logging.h")

#Grouping GEN7_GPO_V2 ROT_SAE_OBJECT Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_SAE_OBJECT_STREAM  FILES ${GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_SOURCE})

#GEN7_GPO_V2 RDD_DEBUG Stream Source File                                                
list(APPEND GEN7_GPO_V2_FLR7_RDD_DEBUG_SOURCE   "${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/RDD_DEBUG_Logging.cpp"
                                                "${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/RDD_DEBUG_Logging.h")

#Grouping GEN7_GPO_V2 RDD_DEBUG Stream Source File
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD_DEBUG FILES ${GEN7_GPO_V2_FLR7_RDD_DEBUG_SOURCE})

#GEN7_GPO_V2 ALIGNMENT source file
list(APPEND GEN7_GPO_V2_FLR7_ALIGNMENT_V10 	"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V10/alignment_stream_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V10/alignment_stream_hdf_V10.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V10/alignment_stream_V10.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V10/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V10/radar_sw_config.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V10/radar_sw_config_satellite.h")
			
list(APPEND GEN7_GPO_V2_FLR7_ALIGNMENT_V11 	"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V11/alignment_stream_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V11/alignment_stream_hdf_V11.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V11/alignment_stream_V11.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V11/alignment_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V11/radar_sw_config.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT/V11/radar_sw_config_satellite.h")			

#Grouping GEN7_GPO_V2 ALIGNMENT source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ALIGNMENT\\V10  FILES ${GEN7_GPO_V2_FLR7_ALIGNMENT_V10})
source_group(\\GEN7\\GPO_V2\\FLR7\\ALIGNMENT\\V11  FILES ${GEN7_GPO_V2_FLR7_ALIGNMENT_V11})

#GEN7_GPO_V2 ALIGNMENT_INTERNALS source file
list(APPEND GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_V1 	"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V1/alignment_internals_stream_V1.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V1/alignment_internals_stream_V1.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V1/alignment_internals_stream.h"												
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V1/radar_sw_config.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V1/radar_sw_config_satellite.h")
														
list(APPEND GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_V2 	"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream_V2.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream_hdf_V2.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream_V2.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V2/alignment_internals_stream.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V2/radar_sw_config.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ALIGNMENT_INTERNALS/V2/radar_sw_config_satellite.h")														
														
#Grouping GEN7_GPO_V2 ALIGNMENT_INTERNALS source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ALIGNMENT_INTERNALS\\V1  FILES ${GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_V1})
source_group(\\GEN7\\GPO_V2\\FLR7\\ALIGNMENT_INTERNALS\\V2  FILES ${GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_V2})
#GEN7_GPO_V2 MMIC source file										
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V4	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V4/mmic_stream_V4.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V4/mmic_stream_hdf_V4.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V4/mmic_stream_V4.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V4/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V4/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V4/radar_sw_config.h")
																					
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V7	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V7/mmic_stream_V7.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V7/mmic_stream_hdf_V7.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V7/mmic_stream_V7.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V7/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V7/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V7/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_MMIC_V8	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V8/mmic_stream_V8.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V8/mmic_stream_hdf_V8.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V8/mmic_stream_V8.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V8/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V8/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V8/radar_sw_config.h")		
											
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V9	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V9/mmic_stream_V9.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V9/mmic_stream_hdf_V9.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V9/mmic_stream_V9.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V9/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V9/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V9/radar_sw_config.h")											
	
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V12	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V12/mmic_stream_V12.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V12/mmic_stream_hdf_V12.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V12/mmic_stream_V12.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V12/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V12/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V12/radar_sw_config.h")											
	
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V13	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V13/mmic_stream_V13.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V13/mmic_stream_hdf_V13.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V13/mmic_stream_V13.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V13/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V13/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V13/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V16	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V16/mmic_stream_V16.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V16/mmic_stream_hdf_V16.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V16/mmic_stream_V16.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V16/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V16/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V16/radar_sw_config.h")	
											
list(APPEND GEN7_GPO_V2_FLR7_MMIC_V17	    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V17/mmic_stream_V17.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V17/mmic_stream_hdf_V17.cpp"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V17/mmic_stream_V17.h"
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V17/mmic_stream.h"												
										    "${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V17/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/MMIC/V17/radar_sw_config.h")

									
#Grouping GEN7_GPO_V2 MMIC source file
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V4  FILES ${GEN7_GPO_V2_FLR7_MMIC_V4})	
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V7  FILES ${GEN7_GPO_V2_FLR7_MMIC_V7})	
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V8  FILES ${GEN7_GPO_V2_FLR7_MMIC_V8})	
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V9  FILES ${GEN7_GPO_V2_FLR7_MMIC_V9})	
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V12  FILES ${GEN7_GPO_V2_FLR7_MMIC_V12})	
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V13  FILES ${GEN7_GPO_V2_FLR7_MMIC_V13})	
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V16  FILES ${GEN7_GPO_V2_FLR7_MMIC_V16})
source_group(\\GEN7\\GPO_V2\\FLR7\\MMIC\\V17  FILES ${GEN7_GPO_V2_FLR7_MMIC_V17})
#GEN7_GPO_V2 RADAR CAPABILITY source file										
list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V35	    "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V35/radar_capability_stream_V35.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V35/radar_capability_stream_hdf_V35.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V35/radar_capability_stream_V35.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V35/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V35/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V35/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V39	    "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream_hdf_V39.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream_V39.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V39/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V39/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V39/radar_sw_config.h")
														
list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V42	    "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V42/radar_capability_stream_V42.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V42/radar_capability_stream_hdf_V42.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V42/radar_capability_stream_V42.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V42/radar_capability_stream.h"												
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V42/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V42/radar_sw_config.h")														
	
list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V47	    "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V47/radar_capability_stream_V47.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V47/radar_capability_stream_hdf_V47.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V47/radar_capability_stream_V47.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V47/radar_capability_stream.h"												
										                #"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V47/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V47/radar_sw_config.h")	
														
list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V48	    "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V48/radar_capability_stream_V48.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V48/radar_capability_stream_hdf_V48.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V48/radar_capability_stream_V48.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V48/radar_capability_stream.h"												
										                #"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V7/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V48/radar_sw_config.h")	  
  
 list(APPEND GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V53	    "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V53/radar_capability_stream_V53.cpp"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V53/radar_capability_stream_hdf_V53.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V53/radar_capability_stream_V53.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V53/radar_capability_stream.h"												
										                #"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V7/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/GPO_V2/FLR7/RADAR_CAPABILITY/V53/radar_sw_config.h")	 
  
#Grouping GEN7_GPO_V2 RADAR CAPABILITY source file
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY\\V35  FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V35})
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY\\V39  FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V39})
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY\\V42  FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V42})
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY\\V47  FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V47})
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY\\V48  FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V48})
source_group(\\GEN7\\GPO_V2\\FLR7\\RADAR_CAPABILITY\\V53  FILES ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V53})

#GEN7_GPO_V2 STATUS source file
list(APPEND GEN7_GPO_V2_FLR7_STATUS_V1 		"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V1/status_stream_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V1/status_stream_hdf_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V1/status_stream_V1.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V1/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V1/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_FLR7_STATUS_V2 		"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V2/status_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V2/status_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V2/status_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V2/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V2/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_STATUS_V3 		"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V3/status_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V3/status_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V3/status_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V3/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V3/radar_sw_config.h")		

list(APPEND GEN7_GPO_V2_FLR7_STATUS_V4 		"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V4/status_stream_V4.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V4/status_stream_hdf_V4.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V4/status_stream_V4.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V4/status_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/STATUS/V4/radar_sw_config.h")											

#Grouping GEN7_GPO_V2 STATUS source file
source_group(\\GEN7\\GPO_V2\\FLR7\\STATUS\\V1  FILES ${GEN7_GPO_V2_FLR7_STATUS_V1})
source_group(\\GEN7\\GPO_V2\\FLR7\\STATUS\\V2  FILES ${GEN7_GPO_V2_FLR7_STATUS_V2})
source_group(\\GEN7\\GPO_V2\\FLR7\\STATUS\\V3  FILES ${GEN7_GPO_V2_FLR7_STATUS_V3})
source_group(\\GEN7\\GPO_V2\\FLR7\\STATUS\\V4  FILES ${GEN7_GPO_V2_FLR7_STATUS_V4})
#GEN7_GPO_V2 DET source file
list(APPEND GEN7_GPO_V2_FLR7_DET_V10	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V10/detection_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V10/detection_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V10/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V10/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V10/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_DET_V12	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V12/detection_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V12/detection_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V12/detection_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V12/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V12/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V12/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DET_V15	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V15/detection_stream_V15.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V15/detection_stream_hdf_V15.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V15/detection_stream_V15.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V15/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V15/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V15/radar_sw_config.h")											

list(APPEND GEN7_GPO_V2_FLR7_DET_V16	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V16/detection_stream_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V16/detection_stream_hdf_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V16/detection_stream_V16.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V16/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V16/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V16/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DET_V17	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V17/detection_stream_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V17/detection_stream_hdf_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V17/detection_stream_V17.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V17/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V17/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DET_V19	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V19/detection_stream_V19.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V19/detection_stream_hdf_V19.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V19/detection_stream_V19.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V19/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V19/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V2_FLR7_DET_V21	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V21/detection_stream_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V21/detection_stream_hdf_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V21/detection_stream_V21.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V21/detection_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION/V21/radar_sw_config.h")											
									
#Grouping GEN7_GPO_V2 DET source file
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V10 FILES ${GEN7_GPO_V2_FLR7_DET_V10})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V12 FILES ${GEN7_GPO_V2_FLR7_DET_V12})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V15 FILES ${GEN7_GPO_V2_FLR7_DET_V15})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V16 FILES ${GEN7_GPO_V2_FLR7_DET_V16})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V17 FILES ${GEN7_GPO_V2_FLR7_DET_V17})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V19 FILES ${GEN7_GPO_V2_FLR7_DET_V19})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION\\V21 FILES ${GEN7_GPO_V2_FLR7_DET_V21})
#GEN7_GPO_V2 DET_DEBUG source file
list(APPEND GEN7_GPO_V2_FLR7_DET_DEBUG_V1	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V1/detection_debug_stream_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V1/detection_debug_stream_hdf_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V1/detection_debug_stream_V1.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V1/detection_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V1/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V1/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_FLR7_DET_DEBUG_V2	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V2/detection_debug_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V2/detection_debug_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V2/detection_debug_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V2/detection_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V2/radar_sw_config.h")
											
list(APPEND GEN7_GPO_V2_FLR7_DET_DEBUG_V3	"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V3/detection_debug_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V3/detection_debug_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V3/detection_debug_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V3/detection_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V3/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/DETECTION_DEBUG/V3/radar_sw_config.h")											

#Grouping GEN7_GPO_V2 DET_DEBUG source file
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION_DEBUG\\V1 FILES ${GEN7_GPO_V2_FLR7_DET_DEBUG_V1})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION_DEBUG\\V2 FILES ${GEN7_GPO_V2_FLR7_DET_DEBUG_V2})
source_group(\\GEN7\\GPO_V2\\FLR7\\DETECTION_DEBUG\\V3 FILES ${GEN7_GPO_V2_FLR7_DET_DEBUG_V3})
										
#GEN7_GPO_V2 HDR source file
list(APPEND GEN7_GPO_V2_FLR7_HDR_V2		"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V2/header_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V2/header_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V2/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V2/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_HDR_V4		"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V4/header_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V4/header_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V4/header_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V4/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V4/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V4/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_HDR_V5		"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V5/header_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V5/header_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V5/header_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V5/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V5/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V5/radar_sw_config.h")											

list(APPEND GEN7_GPO_V2_FLR7_HDR_V7		"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V7/header_stream_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V7/header_stream_hdf_V7.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V7/header_stream_V7.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V7/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V7/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V7/radar_sw_config.h")										

list(APPEND GEN7_GPO_V2_FLR7_HDR_V9		"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V9/header_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V9/header_stream_hdf_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V9/header_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V9/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V9/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V9/radar_sw_config.h")		

list(APPEND GEN7_GPO_V2_FLR7_HDR_V10	"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V10/header_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V10/header_stream_hdf_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V10/header_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V10/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V10/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V10/radar_sw_config.h")												

list(APPEND GEN7_GPO_V2_FLR7_HDR_V11	"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V11/header_stream_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V11/header_stream_hdf_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V11/header_stream_V11.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V11/header_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V11/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/HEADER/V11/radar_sw_config.h")
									
#Grouping GEN7_GPO_V2 HDR source file
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V2  FILES ${GEN7_GPO_V2_FLR7_HDR_V2})
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V4  FILES ${GEN7_GPO_V2_FLR7_HDR_V4})
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V5  FILES ${GEN7_GPO_V2_FLR7_HDR_V5})
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V7  FILES ${GEN7_GPO_V2_FLR7_HDR_V7})
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V9  FILES ${GEN7_GPO_V2_FLR7_HDR_V9})
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V10 FILES ${GEN7_GPO_V2_FLR7_HDR_V10})
source_group(\\GEN7\\GPO_V2\\FLR7\\HEADER\\V11 FILES ${GEN7_GPO_V2_FLR7_HDR_V11})

#GEN7_GPO_V2 ID source file
list(APPEND GEN7_GPO_V2_FLR7_ID_V3		"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V3/id_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V3/id_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V3/id_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V3/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V3/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V2_FLR7_ID_V5		"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V5/id_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V5/id_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V5/id_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V5/id_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/ID/V5/radar_sw_config.h")													
								
				
#Grouping GEN7_GPO_V2 ID source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ID\\V3  FILES ${GEN7_GPO_V2_FLR7_ID_V3})
source_group(\\GEN7\\GPO_V2\\FLR7\\ID\\V5  FILES ${GEN7_GPO_V2_FLR7_ID_V5})

#GEN7_GPO_V2 DEBUG source file
list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V2 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V2/debug_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V2/debug_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V2/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V2/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V3 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V3/debug_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V3/debug_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V3/debug_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V3/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V3/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V3/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V5 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V5/debug_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V5/debug_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V5/debug_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V5/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V5/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V5/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V8 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V8/debug_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V8/debug_stream_hdf_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V8/debug_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V8/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V8/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V8/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V9 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V9/debug_stream_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V9/debug_stream_hdf_V9.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V9/debug_stream_V9.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V9/debug_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V9/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V9/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V11  "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V11/debug_stream_V11.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V11/debug_stream_hdf_V11.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V11/debug_stream_V11.h"
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V11/debug_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V11/radar_sw_config.h") 		
										
list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V20  "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V20/debug_stream_V20.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V20/debug_stream_hdf_V20.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V20/debug_stream_V20.h"
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V20/debug_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V20/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DEBUG_V21  "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V21/debug_stream_V21.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V21/debug_stream_hdf_V21.cpp"
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V21/debug_stream_V21.h"
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V21/debug_stream.h"                                                
                                        "${SourceFiles}/GEN7/GPO_V2/FLR7/DEBUG/V21/radar_sw_config.h")											
#Grouping GEN7_GPO_V2 DEBUG source file
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V2  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V2})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V3  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V3})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V5  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V5})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V8  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V8})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V9  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V9})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V11  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V11})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V20  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V20})
source_group(\\GEN7\\GPO_V2\\FLR7\\DEBUG\\V21  FILES ${GEN7_GPO_V2_FLR7_DEBUG_V21})
#GEN7_GPO_V2 DOWN_SELECTION source file
list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V8 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V8/Down_Selection_stream_V8.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V8/down_selection_stream_V8.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V8/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V8/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V8/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V8/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V9 	"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/Down_Selection_stream_V9.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/down_selection_stream_hdf_V9.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/down_selection_stream_V9.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V9/radar_sw_config.h")
												
list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V11 "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/Down_Selection_stream_V11.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/down_selection_stream_hdf_V11.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/down_selection_stream_V11.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V11/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V12 "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/Down_Selection_stream_V12.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/down_selection_stream_hdf_V12.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/down_selection_stream_V12.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V12/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V13 "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/Down_Selection_stream_V13.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/down_selection_stream_hdf_V13.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/down_selection_stream_V13.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V13/radar_sw_config.h")	
												
list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V14 "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/Down_Selection_stream_V14.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/down_selection_stream_hdf_V14.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/down_selection_stream_V14.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V14/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_DOWN_SELECTION_V15 "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/Down_Selection_stream_V15.cpp"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/down_selection_stream_hdf_V15.cpp"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/down_selection_stream_V15.h"
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/down_selection_stream_MCIP.h"												
										        "${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/radar_sw_config_satellite.h"
												"${SourceFiles}/GEN7/GPO_V2/FLR7/DOWN_SELECTION/V15/radar_sw_config.h")													
												
#Grouping GEN7_GPO_V2 DOWN_SELECTION source file
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V8  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V8})
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V9  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V9})
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V11  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V11})
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V12  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V12})
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V13  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V13})
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V14  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V14})
source_group(\\GEN7\\GPO_V2\\FLR7\\DOWN_SELECTION\\V15  FILES ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V15})

#GEN7_GPO_V2 RDD source file										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V2 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V2/rdd_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V2/rdd_stream_hdf_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V2/rdd_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V2/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V2/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V5 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V5/rdd_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V5/rdd_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V5/rdd_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V5/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V5/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V5/radar_sw_config.h")										
										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V8 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V8/rdd_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V8/rdd_stream_hdf_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V8/rdd_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V8/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V8/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V8/radar_sw_config.h")	
										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V12 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V12/rdd_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V12/rdd_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V12/rdd_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V12/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V12/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V12/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_RDD_V13 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V13/rdd_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V13/rdd_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V13/rdd_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V13/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V13/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V13/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V14 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V14/rdd_stream_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V14/rdd_stream_hdf_V14.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V14/rdd_stream_V14.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V14/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V14/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V14/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V16 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V16/rdd_stream_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V16/rdd_stream_hdf_V16.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V16/rdd_stream_V16.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V16/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V16/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V16/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_RDD_V17 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V17/rdd_stream_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V17/rdd_stream_hdf_V17.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V17/rdd_stream_V17.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V17/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V17/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V17/radar_sw_config.h")
									
list(APPEND GEN7_GPO_V2_FLR7_RDD_V18 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V18/rdd_stream_V18.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V18/rdd_stream_hdf_V18.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V18/rdd_stream_V18.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V18/rdd_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V18/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD/V18/radar_sw_config.h")									
										
#Grouping GEN7_GPO_V2 RDD source file
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V2  FILES ${GEN7_GPO_V2_FLR7_RDD_V2})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V5  FILES ${GEN7_GPO_V2_FLR7_RDD_V5})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V8  FILES ${GEN7_GPO_V2_FLR7_RDD_V8})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V12  FILES ${GEN7_GPO_V2_FLR7_RDD_V12})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V13  FILES ${GEN7_GPO_V2_FLR7_RDD_V13})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V14  FILES ${GEN7_GPO_V2_FLR7_RDD_V14})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V16  FILES ${GEN7_GPO_V2_FLR7_RDD_V16})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V17  FILES ${GEN7_GPO_V2_FLR7_RDD_V17})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD\\V18  FILES ${GEN7_GPO_V2_FLR7_RDD_V18})
#GEN7_GPO_V2 RDD_DEBUG source file										
list(APPEND GEN7_GPO_V2_FLR7_RDD_DEBUG_V1 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V1/rdd_debug_stream_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V1/rdd_debug_stream_hdf_V1.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V1/rdd_debug_stream_V1.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V1/rdd_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V1/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V1/radar_sw_config.h")	
											
list(APPEND GEN7_GPO_V2_FLR7_RDD_DEBUG_V2 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/rdd_debug_stream_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/rdd_debug_stream_hdf_V2.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/rdd_debug_stream_V2.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/rdd_debug_stream.h"												
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/radar_sw_config.h")	
											
list(APPEND GEN7_GPO_V2_FLR7_RDD_DEBUG_V3 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V3/rdd_debug_stream_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V3/rdd_debug_stream_hdf_V3.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V3/rdd_debug_stream_V3.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V3/rdd_debug_stream.h"												
											#"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V3/radar_sw_config.h")	
											
list(APPEND GEN7_GPO_V2_FLR7_RDD_DEBUG_V7 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V7/rdd_debug_stream_V7.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V7/rdd_debug_stream_hdf_V7.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V7/rdd_debug_stream_V7.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V7/rdd_debug_stream.h"												
											#"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V7/radar_sw_config.h")											
list(APPEND GEN7_GPO_V2_FLR7_RDD_DEBUG_V9 	"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V9/rdd_debug_stream_V9.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V9/rdd_debug_stream_hdf_V9.cpp"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V9/rdd_debug_stream_V9.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V9/rdd_debug_stream.h"												
											#"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V2/radar_sw_config_satellite.h"
											"${SourceFiles}/GEN7/GPO_V2/FLR7/RDD_DEBUG/V9/radar_sw_config.h")	

#Grouping GEN7_GPO_V2 RDD_DEBUG source file
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD_DEBUG\\V1  FILES ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V1})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD_DEBUG\\V2  FILES ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V2})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD_DEBUG\\V3  FILES ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V3})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD_DEBUG\\V7  FILES ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V7})
source_group(\\GEN7\\GPO_V2\\FLR7\\RDD_DEBUG\\V9  FILES ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V9})

#GEN7_GPO_V2 TOI source file										
list(APPEND GEN7_GPO_V2_FLR7_TOI_V1	    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V1/toi_stream_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V1/toi_stream_hdf_V1.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V1/toi_stream_V1.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V1/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V1/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V1/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_TOI_V2	    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V2/toi_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V2/toi_stream_hdf_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V2/toi_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V2/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V2/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V2/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_TOI_V3	    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V3/toi_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V3/toi_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V3/toi_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V3/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V3/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V3/radar_sw_config.h")		
										
list(APPEND GEN7_GPO_V2_FLR7_TOI_V4	    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V4/toi_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V4/toi_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V4/toi_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V4/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V4/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V4/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_TOI_V8	    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V8/toi_stream_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V8/toi_stream_hdf_V8.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V8/toi_stream_V8.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V8/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V8/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V8/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_TOI_V10    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V10/toi_stream_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V10/toi_stream_hdf_V10.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V10/toi_stream_V10.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V10/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V10/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V10/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_TOI_V12    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V12/toi_stream_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V12/toi_stream_hdf_V12.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V12/toi_stream_V12.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V12/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V12/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V12/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_TOI_V13    "${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V13/toi_stream_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V13/toi_stream_hdf_V13.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V13/toi_stream_V13.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V13/toi_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V13/radar_sw_config_satellite.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/TOI/V13/radar_sw_config.h")										
								
#Grouping GEN7_GPO_V2 TOI source file
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V1  FILES ${GEN7_GPO_V2_FLR7_TOI_V1})
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V2  FILES ${GEN7_GPO_V2_FLR7_TOI_V2})	
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V3  FILES ${GEN7_GPO_V2_FLR7_TOI_V3})	
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V4  FILES ${GEN7_GPO_V2_FLR7_TOI_V4})		
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V8  FILES ${GEN7_GPO_V2_FLR7_TOI_V8})		
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V10  FILES ${GEN7_GPO_V2_FLR7_TOI_V10})
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V12  FILES ${GEN7_GPO_V2_FLR7_TOI_V12})
source_group(\\GEN7\\GPO_V2\\FLR7\\TOI\\V13  FILES ${GEN7_GPO_V2_FLR7_TOI_V13})
#GEN7_GPO_V2 VSE source file										
list(APPEND GEN7_GPO_V2_FLR7_VSE_V2	    "${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V2/vse_stream_V2.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V2/vse_stream_V2.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V2/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V2/radar_sw_config.h")		

list(APPEND GEN7_GPO_V2_FLR7_VSE_V3	    "${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V3/vse_stream_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V3/vse_stream_hdf_V3.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V3/vse_stream_V3.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V3/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V3/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_VSE_V4	    "${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V4/vse_stream_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V4/vse_stream_hdf_V4.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V4/vse_stream_V4.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V4/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V4/radar_sw_config.h")

list(APPEND GEN7_GPO_V2_FLR7_VSE_V5	    "${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V5/vse_stream_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V5/vse_stream_hdf_V5.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V5/vse_stream_V5.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V5/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V5/radar_sw_config.h")
										
list(APPEND GEN7_GPO_V2_FLR7_VSE_V6	    "${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V6/vse_stream_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V6/vse_stream_hdf_V6.cpp"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V6/vse_stream_V6.h"
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V6/vse_stream.h"												
										"${SourceFiles}/GEN7/GPO_V2/FLR7/VSE/V6/radar_sw_config.h")										
#Grouping GEN7_GPO_V2 VSE source file
source_group(\\GEN7\\GPO_V2\\FLR7\\VSE\\V2  FILES ${GEN7_GPO_V2_FLR7_VSE_V2})	
source_group(\\GEN7\\GPO_V2\\FLR7\\VSE\\V3  FILES ${GEN7_GPO_V2_FLR7_VSE_V3})	
source_group(\\GEN7\\GPO_V2\\FLR7\\VSE\\V4  FILES ${GEN7_GPO_V2_FLR7_VSE_V4})	
source_group(\\GEN7\\GPO_V2\\FLR7\\VSE\\V5  FILES ${GEN7_GPO_V2_FLR7_VSE_V5})
source_group(\\GEN7\\GPO_V2\\FLR7\\VSE\\V6  FILES ${GEN7_GPO_V2_FLR7_VSE_V6})

#GEN7_GPO_V2 ROT_ISOobject_Stream source file										
list(APPEND GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_V5	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V5/ROT_ISOobject_stream_V5.cpp"
										            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V5/ROT_ISOobject_stream_V5.h"
										            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V5/rot_iso_object_stream.h"		
													"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V5/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_V7	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream_V7.cpp"
										            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream_V7.h"
										            "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V7/rot_iso_object_stream.h"		
													"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_ISO_OBJECT/V7/radar_sw_config.h")	

#Grouping GEN7_GPO_V2 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_ISOobject_Stream\\V5  FILES ${GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_V5})	
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_ISOobject_Stream\\V7  FILES ${GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_V7})

#GEN7_GPO_V2 ROT_PROCESSEDDETECTION_Stream source file										
list(APPEND GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_Stream_V6	"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream_V6.cpp"
										                        "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream_V6.h"
																"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V6/rot_processed_detection_stream.h"	
																"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V6/radar_sw_config.h")	

list(APPEND GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_Stream_V7	"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V7/ROT_processeddetection_stream_V7.cpp"
										                        "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V7/ROT_processeddetection_stream_V7.h"
																"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V7/rot_processed_detection_stream.h"	
																"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_PROCESSED_DETECTION/V7/radar_sw_config.h")	
#Grouping GEN7_GPO_V2 ROT_ISOobject_Stream source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_PROCESSEDDETECTION_Stream\\V6  FILES ${GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_Stream_V6})	
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_PROCESSEDDETECTION_Stream\\V7  FILES ${GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_Stream_V7})	


#GEN7_GPO_V2 ROT_SAFETY_FAULTS source file										
list(APPEND GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_V5	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream_V5.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream_V5.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V5/rot_safety_faults_stream.h"	
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V5/radar_sw_config.h")
					
list(APPEND GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_V6	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V6/ROT_safety_faults_stream_V6.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V6/ROT_safety_faults_stream_V6.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V6/rot_safety_faults_stream.h"	
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAFETY_FAULTS/V6/radar_sw_config.h")					
#Grouping GEN7_GPO_V2 ROT_SAFETY_FAULTS source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_SAFETY_FAULTS\\V5  FILES ${GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_V5})
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_SAFETY_FAULTS\\V6  FILES ${GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_V6})

#GEN7_GPO_V2 ROT_TRACKER_INFO source file										
list(APPEND GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_V3	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V3/rot_tracker_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V3/radar_sw_config.h")													

list(APPEND GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_V4	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V4/rot_tracker_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_TRACKER_INFO/V4/radar_sw_config.h")																						
#Grouping GEN7_GPO_V2 ROT_TRACKER_INFO source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_TRACKER_INFO\\V3  FILES ${GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_V3})	
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_TRACKER_INFO\\V4  FILES ${GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_V4})	
#GEN7_GPO_V2 ROT_VEHICLE_INFO source file										
list(APPEND GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_V5	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V5/ROT_vehicle_info_stream_V5.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V5/ROT_vehicle_info_stream_V5.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V5/rot_vehicle_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V5/radar_sw_config.h")	
														
list(APPEND GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_V6	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V6/ROT_vehicle_info_stream_V6.cpp"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V6/ROT_vehicle_info_stream_V6.h"
										                "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V6/rot_vehicle_info_stream.h"		
														"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_VEHICLE_INFO/V6/radar_sw_config.h")															
													
																						
#Grouping GEN7_GPO_V2 ROT_VEHICLE_INFO source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_VEHICLE_INFO\\V5  FILES ${GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_V5})	
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_VEHICLE_INFO\\V6  FILES ${GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_V6})	

#GEN7_GPO_V2 ROT_OBJECT_STREAM source file										
list(APPEND GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_V6	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V6/ROT_sae_object_stream_V6.cpp"
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V6/ROT_sae_object_stream_V6.h"
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V6/rot_sae_object_stream.h"		
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V6/radar_sw_config.h")																
																						
list(APPEND GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_V7	    "${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V7/ROT_sae_object_stream_V7.cpp"
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V7/ROT_sae_object_stream_V7.h"
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V7/rot_sae_object_stream.h"		
															"${SourceFiles}/GEN7/GPO_V2/FLR7/ROT_SAE_OBJECT_STREAM/V7/radar_sw_config.h")																						
#Grouping GEN7_GPO_V2 ROT_OBJECT_STREAM source file
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_SAE_OBJECT_STREAM\\V6  FILES ${GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_V6})
source_group(\\GEN7\\GPO_V2\\FLR7\\ROT_SAE_OBJECT_STREAM\\V7  FILES ${GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_V7})
#GEN7 Streams 
#RNA_GEN7
#SRR7 Streams
#GEN7_RNA RFFT Stream Source File
list(APPEND GEN7_RNA_SRR7_RFFT_SOURCE 			"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RFFT/RFFT_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RFFT/RFFT_Logging.h")
					
#Grouping GEN7_RNA RFFT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RFFT FILES ${GEN7_RNA_SRR7_RFFT_SOURCE})


#GEN7_RNA ALIGNMENT Stream Source File
list(APPEND GEN7_RNA_SRR7_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/Alignment_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GEN7_RNA ALIGNMENT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ALIGNMENT FILES ${GEN7_RNA_SRR7_ALIGNMENT_SOURCE})


#GEN7_RNA DETECTION Stream Source File
list(APPEND GEN7_RNA_SRR7_DET_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/Detection_Logging.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/Detection_Logging.h")
					
#Grouping GEN7_RNA DETECTION Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DETECTION FILES ${GEN7_RNA_SRR7_DET_SOURCE})


#GEN7_RNA VSE Stream Source File
list(APPEND GEN7_RNA_SRR7_VSE_SOURCE 			"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/VSE_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/VSE_Logging.h")
					
#Grouping GEN7_RNA VSE Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\VSE FILES ${GEN7_RNA_SRR7_VSE_SOURCE})



#GEN7_RNA RADAR_CAPABILITY Stream Source File												
list(APPEND GEN7_RNA_SRR7_RADAR_CAPABILITY_SOURCE 	  "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
												      "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/Radar_Capability_Logging.h")

#Grouping GEN7_RNA RADAR_CAPABILITY Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RADAR_CAPABILITY FILES ${GEN7_RNA_SRR7_RADAR_CAPABILITY_SOURCE})

#GEN7_RNA DOWN SELECTION Stream Source File												
list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/Down_Selection_Logging.cpp"
												    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/Down_Selection_Logging.h")

#Grouping GEN7_RNA DOWN_SELECTION Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_SOURCE})

#GEN7_RNA ROT_OBJECT Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_OBJECT_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/ROT_Object_Stream_Logging.cpp"
												    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/ROT_Object_Stream_Logging.h")

#Grouping GEN7_RNA ROT_OBJECT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_OBJECT   FILES ${GEN7_RNA_SRR7_ROT_OBJECT_SOURCE})

#GEN7_RNA ROT_ISOobject Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_SOURCE 	  "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.cpp"
												  "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/ROT_ISOobject_Stream_Logging.h")

#Grouping GEN7_RNA ROT_ISOobject Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject   FILES ${GEN7_RNA_SRR7_ROT_ISOobject_SOURCE})

#GEN7_RNA BLOCKAGE Stream Source File												
list(APPEND GEN7_RNA_SRR7_BLOCKAGE_SOURCE 	      "${SourceFiles}/GEN7/RNA_GEN7/SRR7/BLOCKAGE/Blockage_Logging.cpp"
												  "${SourceFiles}/GEN7/RNA_GEN7/SRR7/BLOCKAGE/Blockage_Logging.h")

#Grouping GEN7_RNA BLOCKAGE Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\BLOCKAGE   FILES ${GEN7_RNA_SRR7_BLOCKAGE_SOURCE})


#GEN7_RNA CDC Stream Source File
list(APPEND GEN7_RNA_SRR7_CDC_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/CDC_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/CDC_Logging.h")
					
#Grouping GEN7_RNA CDC Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CDC FILES ${GEN7_RNA_SRR7_CDC_SOURCE})
#GEN7_RNA MMIC Stream Source File												
list(APPEND GEN7_RNA_SRR7_MMIC_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/MMIC_Logging.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/MMIC_Logging.h")
#ROT_SAFETY_FAULTS Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_SOURCE 			"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/ROT_Safety_Faults_Logging.h")
#Grouping GEN7_RNA ROT_SAFETY_FAULTS Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_SOURCE})

#GEN7_RNA HEADER Stream Source File												
list(APPEND GEN7_RNA_SRR7_HDR_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/header_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/header_Logging.h")

#Grouping GEN7_RNA HEADER Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\HEADER FILES ${GEN7_RNA_SRR7_HDR_SOURCE})

#GEN7_RNA RDD Stream Source File												
list(APPEND GEN7_RNA_SRR7_RDD_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/RDD_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/RDD_Logging.h")

#Grouping GEN7_RNA RDD Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RDD FILES ${GEN7_RNA_SRR7_RDD_SOURCE})

#GEN7_RNA STATUS Stream Source File												
list(APPEND GEN7_RNA_SRR7_STATUS_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/STATUS/Status_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/STATUS/Status_Logging.h")


#Grouping GEN7_RNA MMIC Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\MMIC   FILES ${GEN7_RNA_SRR7_MMIC_SOURCE})

#GEN7_RNA ROT_TRACKER_INFO Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.cpp"
												    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/ROT_Tracker_Info_Logging.h")

#Grouping GEN7_RNA ROT_TRACKER_INFO Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_SOURCE})

#GEN7_RNA OLP_OBJECT Stream Source File												
list(APPEND GEN7_RNA_SRR7_OLP_OBJECT_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/OLP_OBJECT/OLP_OBJECT_Logging.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/OLP_OBJECT/OLP_OBJECT_Logging.h")
										
#Grouping GEN7_RNA OLP_OBJECT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\OLP_OBJECT   FILES ${GEN7_RNA_SRR7_OLP_OBJECT_SOURCE})

#GEN7_RNA PMO_OBJECT Stream Source File												
list(APPEND GEN7_RNA_SRR7_PMO_OBJECT_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/PMO_OBJECT/PMO_OBJECT_Logging.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/PMO_OBJECT/PMO_OBJECT_Logging.h")
										
#Grouping GEN7_RNA PMO_OBJECT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\PMO_OBJECT   FILES ${GEN7_RNA_SRR7_PMO_OBJECT_SOURCE})

#GEN7_RNA DRA_INTERNALS Stream Source File												
list(APPEND GEN7_RNA_SRR7_DRA_INTERNALS_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DRA_INTERNALS/DRA_INTERNALS_Logging.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DRA_INTERNALS/DRA_INTERNALS_Logging.h")
										
#Grouping GEN7_RNA DRA_INTERNALS Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DRA_INTERNALS   FILES ${GEN7_RNA_SRR7_DRA_INTERNALS_SOURCE})

#GEN7_RNA ROT_PROCESSED_DETECTION Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_PROCESSEDDETECTION_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.cpp"
															"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/ROT_ProcessedDetection_Stream_Logging.h")

#Grouping GEN7_RNA ROT_PROCESSED_DETECTION Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM FILES ${GEN7_RNA_SRR7_ROT_PROCESSEDDETECTION_SOURCE})

#GEN7_RNA LCDA CUST OUTPUT Stream Source File
list(APPEND GEN7_RNA_SRR7_LCDA_CUST_OUTPUT_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/LCDA_CUST_OUTPUT/LCDA_cust_output_Logging.cpp"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/LCDA_CUST_OUTPUT/LCDA_cust_output_Logging.h")
					
#Grouping GEN7_RNA LCDA CUST OUTPUT source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\LCDA_CUST_OUTPUT FILES ${GEN7_RNA_SRR7_LCDA_CUST_OUTPUT_SOURCE})

#GEN7_RNA VID Stream Source File
list(APPEND GEN7_RNA_SRR7_VID_SOURCE 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VID/VID_Logging.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VID/VID_Logging.h")
					
#Grouping GEN7_RNA LCDA CUST OUTPUT source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\VID FILES ${GEN7_RNA_SRR7_VID_SOURCE})


#ID Stream Source File												
list(APPEND GEN7_RNA_SRR7_ID_SOURCE 			            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/id_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/id_Logging.h")
#Grouping GEN7_RNA ID Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ID  FILES ${GEN7_RNA_SRR7_ID_SOURCE})

#TOI Stream Source File												
list(APPEND GEN7_RNA_SRR7_TOI_SOURCE 			            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/toi_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/toi_Logging.h")
#Grouping GEN7_RNA TOI Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI  FILES ${GEN7_RNA_SRR7_TOI_SOURCE})

#TOI_DS Stream Source File												
list(APPEND GEN7_RNA_SRR7_TOI_DS_SOURCE 			            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/toi_ds_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/toi_ds_Logging.h")
#Grouping GEN7_RNA TOI_DS Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS  FILES ${GEN7_RNA_SRR7_TOI_DS_SOURCE})

#CED_CUST_OUTPUT Stream Source File												
list(APPEND GEN7_RNA_SRR7_CED_CUST_OUTPUT_SOURCE 			"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CED_CUST/ced_cust_output_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CED_CUST/ced_cust_output_Logging.h")
#Grouping GEN7_RNA CED_CUST_OUTPUT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CED_CUST_OUTPUT  FILES ${GEN7_RNA_SRR7_CED_CUST_OUTPUT_SOURCE})

#CTA_CUST_OUTPUT Stream Source File												
list(APPEND GEN7_RNA_SRR7_CTA_CUST_OUTPUT_SOURCE 			"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CTA_CUST/cta_cust_output_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CTA_CUST/cta_cust_output_Logging.h")
#Grouping GEN7_RNA CTA_CUST_OUTPUT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CTA_CUST_OUTPUT  FILES ${GEN7_RNA_SRR7_CTA_CUST_OUTPUT_SOURCE})

#CUST_ALGO_INFO Stream Source File												
list(APPEND GEN7_RNA_SRR7_CUST_ALGO_SOURCE 			        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/cust_algo_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/cust_algo_Logging.h")
#Grouping GEN7_RNA CUST_ALGO_INFO Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CUST_ALGO  FILES ${GEN7_RNA_SRR7_CUST_ALGO_SOURCE})


#Grouping GEN7_RNA STATUS Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\STATUS   FILES ${GEN7_RNA_SRR7_STATUS_SOURCE})

#GEN7_RNA DEBUG Stream Source File												
list(APPEND GEN7_RNA_SRR7_DEBUG_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/Debug_Logging.cpp"
												"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/Debug_Logging.h")

#Grouping GEN7_RNA DEBUG Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DEBUG   FILES ${GEN7_RNA_SRR7_DEBUG_SOURCE})

#RNA_GEN7 ROT_VEHICLE_INFO Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_SOURCE 			"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/ROT_Vehicle_Info_Logging.h")

#Grouping RNA_GEN7 ROT_VEHICLE_INFO Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO   FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_SOURCE})

#RNA_GEN7 ROT_INTERNALS Stream Source File												
list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_SOURCE 				"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/ROT_internals_Logging.cpp"
												            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/ROT_internals_Logging.h")

#Grouping RNA_GEN7 ROT_VEHICLE_INFO Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS   FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_SOURCE})

#RNA_GEN7 DYNAMIC ALIGNMENT Stream Source File												
list(APPEND GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.cpp"
												        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.h")

#Grouping RNA_GEN7 DYNAMIC ALIGNMENT Stream Source File
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DYNAMIC_ALIGNMENT   FILES ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_SOURCE})


								


#GEN7_RNA VSE source file
list(APPEND GEN7_RNA_SRR7_VSE_V4 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V4/vse_stream_V4.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V4/vse_stream_V4.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V4/vse_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V4/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_VSE_V5 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V5/vse_stream_V5.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V5/vse_stream_V5.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V5/vse_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V5/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_VSE_V6 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V6/vse_stream_V6.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V6/vse_stream_V6.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V6/vse_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VSE/V6/radar_sw_config.h")

#Grouping GEN7_RNA VSE source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\VSE\\V4  FILES ${GEN7_RNA_SRR7_VSE_V4})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\VSE\\V5  FILES ${GEN7_RNA_SRR7_VSE_V5})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\VSE\\V6  FILES ${GEN7_RNA_SRR7_VSE_V6})

#GEN7_RNA RDARA_CAPABILITY source file				
list(APPEND GEN7_RNA_SRR7_RADAR_CAPABILITY_V32  	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V32/radar_capability_stream_V32.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V32/radar_capability_stream_V32.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V32/radar_capability_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V32/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_RADAR_CAPABILITY_V34  	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V34/radar_capability_stream_V34.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V34/radar_capability_stream_V34.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V34/radar_capability_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V34/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_RADAR_CAPABILITY_V42  	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream_V42.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream_V42.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V42/radar_capability_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V42/radar_sw_config.h")		

list(APPEND GEN7_RNA_SRR7_RADAR_CAPABILITY_V46  	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream_V46.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream_V46.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V46/radar_capability_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V46/radar_sw_config.h")				

list(APPEND GEN7_RNA_SRR7_RADAR_CAPABILITY_V49  	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream_V49.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream_V49.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V49/radar_capability_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/RADAR_CAPABILITY/V49/radar_sw_config.h")															

#Grouping GEN7_RNA RADAR_CAPABILITY source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RADAR_CAPABILITY\\V32  FILES ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V32})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RADAR_CAPABILITY\\V34  FILES ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V34})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RADAR_CAPABILITY\\V42  FILES ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V42})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RADAR_CAPABILITY\\V46  FILES ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V46})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RADAR_CAPABILITY\\V49  FILES ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V49})

 #GEN7_RNA ROT_OBJECT source file										
list(APPEND GEN7_RNA_SRR7_ROT_OBJECT_V3		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V3/ROT_object_stream_V3.cpp"
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V3/ROT_object_stream_V3.h"
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V3/ROT_object_stream.h"												
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V3/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_OBJECT_V5		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V5/ROT_object_stream_V5.cpp"
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V5/ROT_object_stream_V5.h"
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V5/ROT_object_stream.h"												
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V5/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_OBJECT_V6		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V6/ROT_object_stream_V6.cpp"
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V6/ROT_object_stream_V6.h"
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V6/ROT_object_stream.h"												
											"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_OBJECT/V6/radar_sw_config.h")											
																						
 #Grouping GEN7_RNA ROT_OBJECT source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_OBJECT\\V3  FILES ${GEN7_RNA_SRR7_ROT_OBJECT_V3})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_OBJECT\\V5  FILES ${GEN7_RNA_SRR7_ROT_OBJECT_V5})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_OBJECT\\V6  FILES ${GEN7_RNA_SRR7_ROT_OBJECT_V6})
#GEN7_RNA ROT_ISOobject source file
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V3 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream_V3.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream_V3.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V3/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V3/radar_sw_config.h")
																					
#GEN7_RNA ROT_ISOobject source file
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V7 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream_V7.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream_V7.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V7/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V7/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V8 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V8/ROT_ISOobject_stream_V8.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V8/ROT_ISOobject_stream_V8.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V8/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V8/radar_sw_config.h")
																					
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V13 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V13/ROT_ISOobject_stream_V13.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V13/ROT_ISOobject_stream_V13.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V13/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V13/radar_sw_config.h")
																																										
																					
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V108 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V108/ROT_ISOobject_stream_V108.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V108/ROT_ISOobject_stream_V108.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V108/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V108/radar_sw_config.h")
																					
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V112 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V112/ROT_ISOobject_stream_V112.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V112/ROT_ISOobject_stream_V112.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V112/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V112/radar_sw_config.h")
																					
list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V113 										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V113/ROT_ISOobject_stream_V113.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V113/ROT_ISOobject_stream_V113.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V113/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V113/radar_sw_config.h")																					

list(APPEND GEN7_RNA_SRR7_ROT_ISOobject_V213 										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V213/ROT_ISOobject_stream_V213.cpp"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V213/ROT_ISOobject_stream_V213.h"
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V213/ROT_ISOobject_stream.h"												
																					"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_ISO_OBJECT/V213/radar_sw_config.h")	

#Grouping GEN7_RNA STATUS source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V3  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V3})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V7  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V7})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V8  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V8})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V13  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V13})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V108  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V108})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V112  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V112})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V113  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V113})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_ISOobject\\V213  FILES ${GEN7_RNA_SRR7_ROT_ISOobject_V213})

#GEN7_RNA DOWN_SELECTION source file
list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_V7 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V7/down_selection_stream_V7.cpp"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V7/down_selection_stream_V7.h"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V7/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V7/radar_sw_config.h")
												
list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_V100 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V100/down_selection_stream_V100.cpp"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V100/down_selection_stream_V100.h"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V100/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V100/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_V105 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V105/down_selection_stream_V105.cpp"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V105/down_selection_stream_V105.h"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V105/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V105/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_V107 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V107/down_selection_stream_V107.cpp"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V107/down_selection_stream_V107.h"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V107/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V107/radar_sw_config.h")		

list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_V108 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V108/down_selection_stream_V108.cpp"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V108/down_selection_stream_V108.h"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V108/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V108/radar_sw_config.h")	
												
list(APPEND GEN7_RNA_SRR7_DOWN_SELECTION_V109 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V109/down_selection_stream_V109.cpp"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V109/down_selection_stream_V109.h"
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V109/down_selection_stream.h"												
										        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DOWN_SELECTION/V109/radar_sw_config.h")	
												
#Grouping GEN7_RNA DOWN_SELECTION source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION\\V7  FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_V7})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION\\V100  FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_V100})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION\\V105  FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_V105})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION\\V107  FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_V107})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION\\V108  FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_V108})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DOWN_SELECTION\\V109  FILES ${GEN7_RNA_SRR7_DOWN_SELECTION_V109})

#GEN7_RNA ALIGNMENT source file
list(APPEND GEN7_RNA_SRR7_ALIGNMENT_V9 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V9/alignment_stream_V9.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V9/alignment_stream_V9.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V9/alignment_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V9/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_ALIGNMENT_V10 "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V10/alignment_stream_V10.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V10/alignment_stream_V10.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V10/alignment_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V10/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_ALIGNMENT_V11 "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V11/alignment_stream_V11.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V11/alignment_stream_V11.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V11/alignment_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ALIGNMENT/V11/radar_sw_config.h")
										
#Grouping GEN7_RNA ALIGNMENT source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ALIGNMENT\\V9  FILES ${GEN7_RNA_SRR7_ALIGNMENT_V9})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ALIGNMENT\\V10  FILES ${GEN7_RNA_SRR7_ALIGNMENT_V10})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ALIGNMENT\\V11  FILES ${GEN7_RNA_SRR7_ALIGNMENT_V11})
							


#GEN7_RNA CDC source file
list(APPEND GEN7_RNA_SRR7_CDC_V7 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V7/cdc_stream_V7.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V7/cdc_stream_V7.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V7/cdc_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V7/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_CDC_V8 		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V8/cdc_stream_V8.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V8/cdc_stream_V8.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V8/cdc_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CDC/V8/radar_sw_config.h")

#Grouping GEN7_RNA CDC source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CDC\\V7  FILES ${GEN7_RNA_SRR7_CDC_V7})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CDC\\V8  FILES ${GEN7_RNA_SRR7_CDC_V8})

#GEN7_RNA HDR source file				
list(APPEND GEN7_RNA_SRR7_HDR_V9  		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V9/header_stream_V9.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V9/header_stream_V9.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V9/header_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V9/radar_sw_config.h")									

list(APPEND GEN7_RNA_SRR7_HDR_V11  		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V11/header_stream_V11.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V11/header_stream_V11.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V11/header_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V11/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_HDR_V14  		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V14/header_stream_V14.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V14/header_stream_V14.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V14/header_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/HEADER/V14/radar_sw_config.h")	

#Grouping GEN7_RNA HDR source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\HEADER\\V9  FILES ${GEN7_RNA_SRR7_HDR_V9})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\HEADER\\V11  FILES ${GEN7_RNA_SRR7_HDR_V11})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\HEADER\\V14  FILES ${GEN7_RNA_SRR7_HDR_V14})

#GEN7_RNA RDD source file										
list(APPEND GEN7_RNA_SRR7_RDD_V30		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V30/rdd_stream_V30.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V30/rdd_stream_V30.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V30/rdd_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V30/radar_sw_config.h")		

list(APPEND GEN7_RNA_SRR7_RDD_V32		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V32/rdd_stream_V32.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V32/rdd_stream_V32.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V32/rdd_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V32/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_RDD_V34		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V34/rdd_stream_V34.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V34/rdd_stream_V34.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V34/rdd_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/RDD/V34/radar_sw_config.h")											
																						
#Grouping GEN7_RNA RDD source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RDD\\V30  FILES ${GEN7_RNA_SRR7_RDD_V30})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RDD\\V32  FILES ${GEN7_RNA_SRR7_RDD_V32})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\RDD\\V34  FILES ${GEN7_RNA_SRR7_RDD_V34})

#GEN7_RNA STATUS source file
list(APPEND GEN7_RNA_SRR7_STATUS_V10 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/STATUS/V10/status_stream_V10.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/STATUS/V10/status_stream_V10.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/STATUS/V10/status_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/STATUS/V10/radar_sw_config.h")

#Grouping GEN7_RNA STATUS source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\STATUS\\V10  FILES ${GEN7_RNA_SRR7_STATUS_V10})

#GEN7_RNA DEBUG source file
list(APPEND GEN7_RNA_SRR7_DEBUG_V10 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V10/debug_stream_V10.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V10/debug_stream_V10.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V10/debug_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V10/radar_sw_config.h")
										
list(APPEND GEN7_RNA_SRR7_DEBUG_V12 	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V12/debug_stream_V12.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V12/debug_stream_V12.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V12/debug_stream.h"												
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DEBUG/V12/radar_sw_config.h")
										
#Grouping GEN7_RNA DEBUG source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DEBUG\\V10  FILES ${GEN7_RNA_SRR7_DEBUG_V10})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DEBUG\\V12  FILES ${GEN7_RNA_SRR7_DEBUG_V12})

#GEN7_RNA ROT_TRACKER_INFO source file										
list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_V3	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V3/ROT_tracker_info_stream_V3.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V3/ROT_trackerInfo_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V3/radar_sw_config_satellite.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V3/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_V4	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V4/ROT_tracker_info_stream_V4.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V4/ROT_trackerInfo_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V4/radar_sw_config_satellite.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V4/radar_sw_config.h")			

list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_V8	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V8/ROT_tracker_info_stream_V8.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V8/ROT_tracker_info_stream_V8.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V8/ROT_trackerInfo_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V8/radar_sw_config.h")		

list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_V9	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V9/ROT_tracker_info_stream_V9.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V9/ROT_tracker_info_stream_V9.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V9/ROT_trackerInfo_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V9/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_V12	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V12/ROT_tracker_info_stream_V12.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V12/ROT_tracker_info_stream_V12.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V12/ROT_trackerInfo_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V12/radar_sw_config.h")															

list(APPEND GEN7_RNA_SRR7_ROT_TRACKER_INFO_V212	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V212/ROT_tracker_info_stream_V212.cpp"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V212/ROT_tracker_info_stream_V212.h"
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V212/ROT_trackerInfo_stream.h"												
										            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_TRACKER_INFO/V212/radar_sw_config.h")	

#Grouping GEN7_RNA ROT_TRACKER_INFO source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO\\V3  FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V3})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO\\V4  FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V4})	
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO\\V8  FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V8})	
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO\\V9  FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V9})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO\\V12  FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V12})	
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_TRACKER_INFO\\V212  FILES ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V212})		

#GEN7_RNA ROT_PROCESSED_DETECTION source file										
list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V3	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream_V3.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream_V3.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V3/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V3/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V3/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V103	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V103/ROT_processeddetection_stream_V103.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V103/ROT_processeddetection_stream_V103.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V103/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V103/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V103/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V5	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V5/ROT_processeddetection_stream_V5.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V5/ROT_processeddetection_stream_V5.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V5/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V5/radar_sw_config.h")		

list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V6	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream_V6.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream_V6.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V6/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V6/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V13	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V13/ROT_processeddetection_stream_V13.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V13/ROT_processeddetection_stream_V13.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V13/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V13/radar_sw_config.h")


list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V112	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V112/ROT_processeddetection_stream_V112.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V112/ROT_processeddetection_stream_V112.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V112/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V112/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V112/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V113	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V113/ROT_processeddetection_stream_V113.cpp"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V113/ROT_processeddetection_stream_V113.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V113/ROT_processeddetection_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V113/radar_sw_config_satellite.h"
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_PROCESSED_DETECTION/V113/radar_sw_config.h")															

#Grouping GEN7_RNA ROT_PROCESSED_DETECTION source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V3    FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V3})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V103  FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V103})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V5  FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V5})	
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V6  FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V6})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V13  FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V13})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V112  FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V112})	
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_PROCESSEDDETECTION_STREAM\\V113  FILES ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V113})		

#GEN7_RNA MMIC source file										
list(APPEND GEN7_RNA_SRR7_MMIC_V30	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V30/mmic_stream_V30.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V30/mmic_stream_V30.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V30/mmic_stream.h"												
							            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V30/radar_sw_config.h")
										
										
list(APPEND GEN7_RNA_SRR7_MMIC_V34	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V34/mmic_stream_V34.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V34/mmic_stream_V34.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V34/mmic_stream.h"												
							            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V34/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_MMIC_V40	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V40/mmic_stream_V40.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V40/mmic_stream_V40.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V40/mmic_stream.h"												
							            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/MMIC/V40/radar_sw_config.h")
										
#Grouping GEN7_RNA MMIC source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\MMIC\\V30  FILES ${GEN7_RNA_SRR7_MMIC_V30})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\MMIC\\V34  FILES ${GEN7_RNA_SRR7_MMIC_V34})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\MMIC\\V40  FILES ${GEN7_RNA_SRR7_MMIC_V40})


#GEN7_RNA OLP_OBJECT source file										
list(APPEND GEN7_RNA_SRR7_OLP_OBJECT_V1	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/OLP_OBJECT/V1/olp_object_stream_V1.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/OLP_OBJECT/V1/olp_object_stream_V1.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/OLP_OBJECT/V1/olp_object_stream.h"												
							            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/OLP_OBJECT/V1/radar_sw_config.h")

source_group(\\GEN7\\RNA_GEN7\\SRR7\\OLP_OBJECT\\V1  FILES ${GEN7_RNA_SRR7_OLP_OBJECT_V1})

#GEN7_RNA PMO_OBJECT source file
list(APPEND GEN7_RNA_SRR7_PMO_OBJECT_V1	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/PMO_OBJECT/V1/pmo_object_stream_V1.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/PMO_OBJECT/V1/pmo_object_stream_V1.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/PMO_OBJECT/V1/pmo_object_stream.h"												
							            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/PMO_OBJECT/V1/radar_sw_config.h")

source_group(\\GEN7\\RNA_GEN7\\SRR7\\PMO_OBJECT\\V1  FILES ${GEN7_RNA_SRR7_PMO_OBJECT_V1})

list(APPEND GEN7_RNA_SRR7_DRA_INTERNALS_V1	    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DRA_INTERNALS/V1/dra_internals_stream_V1.cpp"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DRA_INTERNALS/V1/dra_internals_stream_V1.h"
										"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DRA_INTERNALS/V1/dra_internals_stream.h"												
							            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/DRA_INTERNALS/V1/radar_sw_config.h")

source_group(\\GEN7\\RNA_GEN7\\SRR7\\DRA_INTERNALS\\V1  FILES ${GEN7_RNA_SRR7_DRA_INTERNALS_V1})

#GEN7_RNA DET source file
list(APPEND GEN7_RNA_SRR7_DET_V22	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V22/detection_stream_V22.cpp"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V22/detection_stream_V22.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V22/detection_stream.h"												
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V22/radar_sw_config_satellite.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V22/radar_sw_config.h")
									
list(APPEND GEN7_RNA_SRR7_DET_V100	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V100/detection_stream_V100.cpp"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V100/detection_stream_V100.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V100/detection_stream.h"												
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V100/radar_sw_config_satellite.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V100/radar_sw_config.h")
									
list(APPEND GEN7_RNA_SRR7_DET_V105	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V105/detection_stream_V105.cpp"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V105/detection_stream_V105.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V105/detection_stream.h"												
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V105/radar_sw_config.h")
									
list(APPEND GEN7_RNA_SRR7_DET_V107	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V107/detection_stream_V107.cpp"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V107/detection_stream_V107.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V107/detection_stream.h"												
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DETECTION/V107/radar_sw_config.h")
										
#Grouping GEN7_RNA DET source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DETECTION\\V22  FILES ${GEN7_RNA_SRR7_DET_V22})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DETECTION\\V100  FILES ${GEN7_RNA_SRR7_DET_V100})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DETECTION\\V105  FILES ${GEN7_RNA_SRR7_DET_V105})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DETECTION\\V107  FILES ${GEN7_RNA_SRR7_DET_V107})



#GEN7_RNA LCDA CUST OUTPUT Stream Source File
list(APPEND GEN7_RNA_SRR7_LCDA_CUST_OUTPUT_V2    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/LCDA_CUST_OUTPUT/V2/LCDA_cust_output_stream_V2.cpp"
												 "${SourceFiles}/GEN7/RNA_GEN7/SRR7/LCDA_CUST_OUTPUT/V2/LCDA_cust_output_stream_V2.h"
												 "${SourceFiles}/GEN7/RNA_GEN7/SRR7/LCDA_CUST_OUTPUT/V2/lcda_cust_output_stream.h"												
												 "${SourceFiles}/GEN7/RNA_GEN7/SRR7/LCDA_CUST_OUTPUT/V2/radar_sw_config.h")

#Grouping GEN7_RNA LCDA CUST OUTPUT source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\LCDA_CUST_OUTPUT\\V2  FILES ${GEN7_RNA_SRR7_LCDA_CUST_OUTPUT_V2})	

#GEN7_RNA VID Stream Source File
list(APPEND GEN7_RNA_SRR7_VID_V2    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/VID/V2/VID_stream_V2.cpp"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VID/V2/VID_stream_V2.h"
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VID/V2/vid_stream.h"												
									"${SourceFiles}/GEN7/RNA_GEN7/SRR7/VID/V2/radar_sw_config.h")

#Grouping GEN7_RNA VID source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\VID\\V2  FILES ${GEN7_RNA_SRR7_VID_V2})	




#Grouping GEN7_RNA ROT_SAFETY_FAULTS Source File
list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V2   		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream_V2.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream_V2.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V2/ROT_safety_faults_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V2/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V4   		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_V4.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream_V4.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V4/ROT_safety_faults_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V4/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V5   		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream_V5.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream_V5.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V5/ROT_safety_faults_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V5/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V13   		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream_V13.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream_V13.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V13/ROT_safety_faults_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V13/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V105   		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V105/ROT_safety_faults_stream_V105.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V105/ROT_safety_faults_stream_V105.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V105/ROT_safety_faults_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V105/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V113   		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream_V113.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream_V113.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V113/ROT_safety_faults_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_SAFETY_FAULTS/V113/radar_sw_config.h")														
														
#Grouping GEN7_RNA ROT_SAFETY_FAULTS  source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS\\V2  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V2})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS\\V4  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V4})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS\\V5  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V5})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS\\V13  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V13})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS\\V105  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V105})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_SAFETY_FAULTS\\V113  FILES ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V113})

#Grouping GEN7_RNA ID Source File
list(APPEND GEN7_RNA_SRR7_ID_V3   						"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V3/id_stream_V3.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V3/id_stream_V3.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V3/id_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V3/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_ID_V5   						"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V5/id_stream_V5.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V5/id_stream_V5.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V5/id_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ID/V5/radar_sw_config.h")
#Grouping GEN7_RNA ID  source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ID\\V3  FILES ${GEN7_RNA_SRR7_ID_V3})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ID\\V5  FILES ${GEN7_RNA_SRR7_ID_V5})

#Grouping GEN7_RNA TOI Source File
list(APPEND GEN7_RNA_SRR7_TOI_V6   		                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V6/toi_stream_V6.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V6/toi_stream_V6.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V6/toi_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V6/radar_sw_config.h")	
														
														
list(APPEND GEN7_RNA_SRR7_TOI_V100   		            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V100/toi_stream_V100.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V100/toi_stream_V100.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V100/toi_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V100/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_TOI_V103   		            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V103/toi_stream_V103.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V103/toi_stream_V103.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V103/toi_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V103/radar_sw_config.h")	
														

														
list(APPEND GEN7_RNA_SRR7_TOI_V105   		            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V105/toi_stream_V105.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V105/toi_stream_V105.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V105/toi_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V105/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_TOI_V106   		            "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V106/toi_stream_V106.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V106/toi_stream_V106.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V106/toi_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI/V106/radar_sw_config.h")	

#Grouping GEN7_RNA TOI  source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI\\V6  	FILES ${GEN7_RNA_SRR7_TOI_V6})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI\\V100  FILES ${GEN7_RNA_SRR7_TOI_V100})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI\\V103  FILES ${GEN7_RNA_SRR7_TOI_V103})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI\\V105  FILES ${GEN7_RNA_SRR7_TOI_V105})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI\\V106  FILES ${GEN7_RNA_SRR7_TOI_V106})

list(APPEND GEN7_RNA_SRR7_TOI_DS_V101   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V101/toi_ds_stream_V101.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V101/toi_ds_stream_V101.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V101/toi_ds_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V101/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_TOI_DS_V102   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V102/toi_ds_stream_V102.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V102/toi_ds_stream_V102.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V102/toi_ds_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V102/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_TOI_DS_V103   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V103/toi_ds_stream_V103.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V103/toi_ds_stream_V103.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V103/toi_ds_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V103/radar_sw_config.h")
														
list(APPEND GEN7_RNA_SRR7_TOI_DS_V104   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V104/toi_ds_stream_V104.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V104/toi_ds_stream_V104.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V104/toi_ds_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V104/radar_sw_config.h")														
														
list(APPEND GEN7_RNA_SRR7_TOI_DS_V105   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V105/toi_ds_stream_V105.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V105/toi_ds_stream_V105.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V105/toi_ds_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V105/radar_sw_config.h")	



list(APPEND GEN7_RNA_SRR7_TOI_DS_V106   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V106/toi_ds_stream_V106.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V106/toi_ds_stream_V106.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V106/toi_ds_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/TOI_DS/V106/radar_sw_config.h")															
		
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS\\V101  	FILES ${GEN7_RNA_SRR7_TOI_DS_V101})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS\\V102  	FILES ${GEN7_RNA_SRR7_TOI_DS_V102})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS\\V103  	FILES ${GEN7_RNA_SRR7_TOI_DS_V103})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS\\V104  	FILES ${GEN7_RNA_SRR7_TOI_DS_V104})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS\\V105  	FILES ${GEN7_RNA_SRR7_TOI_DS_V105})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\TOI_DS\\V106  	FILES ${GEN7_RNA_SRR7_TOI_DS_V106})

#Grouping GEN7_RNA CED_CUST_OUTPUT Source File
list(APPEND GEN7_RNA_SRR7_CED_CUST_OUTPUT_V2   		    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CED_CUST/V2/ced_cust_output_stream_V2.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CED_CUST/V2/ced_cust_output_stream_V2.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CED_CUST/V2/ced_cust_output_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CED_CUST/V2/radar_sw_config.h")	
#Grouping GEN7_RNA CED_CUST_OUTPUT  source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CED_CUST_OUTPUT\\V2  FILES ${GEN7_RNA_SRR7_CED_CUST_OUTPUT_V2})

#Grouping GEN7_RNA CTA_CUST_OUTPUT Source File
list(APPEND GEN7_RNA_SRR7_CTA_CUST_OUTPUT_V2   		    "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CTA_CUST/V2/cta_cust_output_stream_V2.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CTA_CUST/V2/cta_cust_output_stream_V2.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CTA_CUST/V2/cta_cust_output_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CTA_CUST/V2/radar_sw_config.h")	
#Grouping GEN7_RNA CTA_CUST_OUTPUT  source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CTA_CUST_OUTPUT\\V2  FILES ${GEN7_RNA_SRR7_CTA_CUST_OUTPUT_V2})

#Grouping GEN7_RNA CUST_ALGO_INFO Source File
list(APPEND GEN7_RNA_SRR7_CUST_ALGO_V2   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V2/cust_algo_stream_V2.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V2/cust_algo_stream_V2.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V2/cust_algo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V2/radar_sw_config.h")	
														
list(APPEND GEN7_RNA_SRR7_CUST_ALGO_V3   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V3/cust_algo_stream_V3.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V3/cust_algo_stream_V3.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V3/cust_algo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V3/radar_sw_config.h")		


list(APPEND GEN7_RNA_SRR7_CUST_ALGO_V5   		        "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V5/cust_algo_stream_V5.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V5/cust_algo_stream_V5.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V5/cust_algo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/CUST_ALGO/V5/radar_sw_config.h")	
														
#Grouping GEN7_RNA CUST_ALGO_INFO  source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CUST_ALGO\\V2  FILES ${GEN7_RNA_SRR7_CUST_ALGO_V2})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CUST_ALGO\\V3  FILES ${GEN7_RNA_SRR7_CUST_ALGO_V3})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\CUST_ALGO\\V5  FILES ${GEN7_RNA_SRR7_CUST_ALGO_V5})



#RNA_GEN7 ROT_VEHICLE_INFO source file										
list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V2	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V2/ROT_vehicle_info_stream_V2.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V2/ROT_vehicle_info_stream_V2.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V2/ROT_vehicleinfo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V2/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V4	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicle_info_stream_V4.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicle_info_stream_V4.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V4/ROT_vehicleinfo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V4/radar_sw_config.h")																
																						
list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V5	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V5/ROT_vehicle_info_stream_V5.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V5/ROT_vehicle_info_stream_V5.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V5/ROT_vehicleinfo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V5/radar_sw_config.h")
														
														
list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V13	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V13/ROT_vehicle_info_stream_V13.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V13/ROT_vehicle_info_stream_V13.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V13/ROT_vehicleinfo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V13/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V105	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V105/ROT_vehicle_info_stream_V105.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V105/ROT_vehicle_info_stream_V105.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V105/ROT_vehicleinfo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V105/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V113	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicle_info_stream_V113.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicle_info_stream_V113.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V113/ROT_vehicleinfo_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_VEHICLE_INFO/V113/radar_sw_config.h")														
																						
#Grouping RNA_GEN7 ROT_VEHICLE_INFO source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO\\V2  FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V2})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO\\V4  FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V4})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO\\V5  FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V5})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO\\V13  FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V13})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO\\V105  FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V105})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_VEHICLE_INFO\\V113  FILES ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V113})
#RNA_GEN7 DYNAMIC ALIGNMENT source file										
list(APPEND GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V11		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V11/dynamic_alignment_stream_V11.cpp"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V11/dynamic_alignment_stream_V11.h"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V11/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V11/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V12		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.cpp"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream_V12.h"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V12/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V12/radar_sw_config.h")
													
list(APPEND GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V16		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream_V16.cpp"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream_V16.h"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V16/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V16/radar_sw_config.h")													
													
													
list(APPEND GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V45		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V45/dynamic_alignment_stream_V45.cpp"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V45/dynamic_alignment_stream_V45.h"
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V45/dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN7/RNA_GEN7/SRR7/DYNAMIC_ALIGNMENT/V45/radar_sw_config.h")

													
												
																						
#Grouping RNA_GEN7 DYNAMIC ALIGNMENT source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DYNAMIC_ALIGNMENT\\V11	  FILES ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V11})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DYNAMIC_ALIGNMENT\\V12	  FILES ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V12})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DYNAMIC_ALIGNMENT\\V16	  FILES ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V16})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\DYNAMIC_ALIGNMENT\\V45	  FILES ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V45})		

#RNA_GEN7 ROT_INTERNALS source file										
list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_V2	    		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V2/ROT_internals_stream_V2.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V2/ROT_internals_stream_V2.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V2/ROT_internals_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V2/radar_sw_config.h")													

#RNA_GEN7 ROT_INTERNALS source file										
list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_V5	    		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V5/ROT_internals_stream_V5.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V5/ROT_internals_stream_V5.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V5/ROT_internals_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V5/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_V6	    		"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V6/ROT_internals_stream_V6.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V6/ROT_internals_stream_V6.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V6/ROT_internals_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V6/radar_sw_config.h")		

list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_V106	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V106/ROT_internals_stream_V106.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V106/ROT_internals_stream_V106.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V106/ROT_internals_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V106/radar_sw_config.h")

list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_V113	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V113/ROT_internals_stream_V113.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V113/ROT_internals_stream_V113.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V113/ROT_internals_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V113/radar_sw_config.h")	

list(APPEND GEN7_RNA_SRR7_ROT_INTERNALS_V114	    	"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V114/ROT_internals_stream_V114.cpp"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V114/ROT_internals_stream_V114.h"
										                "${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V114/ROT_internals_stream.h"												
														"${SourceFiles}/GEN7/RNA_GEN7/SRR7/ROT_INTERNALS/V114/radar_sw_config.h")															
																						
#Grouping RNA_GEN7 ROT_VEHICLE_INFO source file
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS\\V2  FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_V2})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS\\V5  FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_V5})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS\\V6  FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_V6})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS\\V106  FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_V106})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS\\V113  FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_V113})
source_group(\\GEN7\\RNA_GEN7\\SRR7\\ROT_INTERNALS\\V114  FILES ${GEN7_RNA_SRR7_ROT_INTERNALS_V114})
#set the source


#GEN8_SRR8
#GEN8_SRR8
#GPO_GEN8_SRR8 ALIGNMENT Stream Source File
list(APPEND GPO_GEN8_SRR8_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/Alignment_Logging.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GPO_GEN8_SRR8 ALIGNMENT Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\ALIGNMENT FILES ${GPO_GEN8_SRR8_ALIGNMENT_SOURCE})

#GPO_GEN8_SRR8 ALIGNMENT source file										
list(APPEND GPO_GEN8_SRR8_ALIGNMENT_V10	    	"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/V10/alignment_stream_V10.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/V10/alignment_stream_V10.h"
												"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/V10/alignment_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/V10/alignment_stream_hdf_V10.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/ALIGNMENT/V10/radar_sw_config.h")	
												
#Grouping GPO_GEN8_SRR8 ALIGNMENT source file
source_group(\\GEN8\\GPO\\SRR8\\ALIGNMENT\\V10  FILES ${GPO_GEN8_SRR8_ALIGNMENT_V10})


#GPO_GEN8_SRR8 DETECTION Stream Source File
list(APPEND GPO_GEN8_SRR8_DETECTION_SOURCE 		"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/Detection_Logging.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/Detection_Logging.h")
					
#Grouping GPO_GEN8_SRR8 DETECTION Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\DETECTION FILES ${GPO_GEN8_SRR8_DETECTION_SOURCE})

#GPO_GEN8_SRR8 DETECTION source file										
list(APPEND GPO_GEN8_SRR8_DETECTION_V5	    	"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V5/detection_stream_V5.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V5/detection_stream_V5.h"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V5/detection_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V5/detection_stream_hdf_V5.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V5/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_SRR8_DETECTION_V115	    "${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V115/detection_stream_V115.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V115/detection_stream_V115.h"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V115/detection_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V115/detection_stream_hdf_V115.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V115/radar_sw_config.h")	

list(APPEND GPO_GEN8_SRR8_DETECTION_V200	    "${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V200/detection_stream_V200.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V200/detection_stream_V200.h"
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V200/detection_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V200/detection_stream_hdf_V200.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/DETECTION/V200/radar_sw_config.h")												
#Grouping GPO_GEN8_SRR8 DETECTION source file
source_group(\\GEN8\\GPO\\SRR8\\DETECTION\\V5  		FILES ${GPO_GEN8_SRR8_DETECTION_V5})
source_group(\\GEN8\\GPO\\SRR8\\DETECTION\\V115  	FILES ${GPO_GEN8_SRR8_DETECTION_V115})
source_group(\\GEN8\\GPO\\SRR8\\DETECTION\\V200  	FILES ${GPO_GEN8_SRR8_DETECTION_V200})

#GPO_GEN8_SRR8 DOWN_SELECTION Stream Source File
list(APPEND GPO_GEN8_SRR8_DOWN_SELECTION_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/Down_Selection_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/Down_Selection_Logging.h")
					
#Grouping GPO_GEN8_SRR8 DOWN_SELECTION Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\DOWN_SELECTION FILES ${GPO_GEN8_SRR8_DOWN_SELECTION_SOURCE})

#GPO_GEN8_SRR8 DOWN_SELECTION source file										
list(APPEND GPO_GEN8_SRR8_DOWN_SELECTION_V1	    "${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V1/down_selection_stream_V1.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V1/down_selection_stream_V1.h"
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V1/down_selection_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V1/down_selection_stream_hdf_V1.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V1/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_SRR8_DOWN_SELECTION_V101	"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V101/down_selection_stream_V101.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V101/down_selection_stream_V101.h"
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V101/down_selection_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V101/down_selection_stream_hdf_V101.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V101/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_SRR8_DOWN_SELECTION_V200   "${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V200/down_selection_stream_V200.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V200/down_selection_stream_V200.h"
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V200/down_selection_stream.h"												
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V200/down_selection_stream_hdf_V200.cpp"												
												"${SourceFiles}/GEN8/GPO/SRR8/DOWN_SELECTION/V200/radar_sw_config.h")													
#Grouping GPO_GEN8_SRR8 DOWN_SELECTION source file
source_group(\\GEN8\\GPO\\SRR8\\DOWN_SELECTION\\V1  	FILES ${GPO_GEN8_SRR8_DOWN_SELECTION_V1})
source_group(\\GEN8\\GPO\\SRR8\\DOWN_SELECTION\\V101  	FILES ${GPO_GEN8_SRR8_DOWN_SELECTION_V101})
source_group(\\GEN8\\GPO\\SRR8\\DOWN_SELECTION\\V200  	FILES ${GPO_GEN8_SRR8_DOWN_SELECTION_V200})

#GPO_GEN8_SRR8 HEADER Stream Source File
list(APPEND GPO_GEN8_SRR8_HEADER_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/HEADER/Header_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/HEADER/Header_Logging.h")
					
#Grouping GPO_GEN8_SRR8 HEADER Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\HEADER FILES ${GPO_GEN8_SRR8_HEADER_SOURCE})

#GPO_GEN8_SRR8 HEADER source file										
list(APPEND GPO_GEN8_SRR8_HEADER_V7	    "${SourceFiles}/GEN8/GPO/SRR8/HEADER/V7/header_stream_V7.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V7/header_stream_V7.h"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V7/header_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V7/header_stream_hdf_V7.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V7/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_SRR8_HEADER_V8	    "${SourceFiles}/GEN8/GPO/SRR8/HEADER/V8/header_stream_V8.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V8/header_stream_V8.h"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V8/header_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V8/header_stream_hdf_V8.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V8/radar_sw_config.h")		
												
list(APPEND GPO_GEN8_SRR8_HEADER_V9	    "${SourceFiles}/GEN8/GPO/SRR8/HEADER/V9/header_stream_V9.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V9/header_stream_V9.h"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V9/header_stream.h"												
										#"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V9/header_stream_hdf_V9.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V9/radar_sw_config.h")													
#Grouping GPO_GEN8_SRR8 HEADER source file
source_group(\\GEN8\\GPO\\SRR8\\HEADER\\V7  FILES ${GPO_GEN8_SRR8_HEADER_V7})
source_group(\\GEN8\\GPO\\SRR8\\HEADER\\V8  FILES ${GPO_GEN8_SRR8_HEADER_V8})
source_group(\\GEN8\\GPO\\SRR8\\HEADER\\V9  FILES ${GPO_GEN8_SRR8_HEADER_V9})

list(APPEND GPO_GEN8_SRR8_HEADER_V10    "${SourceFiles}/GEN8/GPO/SRR8/HEADER/V10/header_stream_V10.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V10/header_stream_V10.h"
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V10/header_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V10/header_stream_hdf_V10.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/HEADER/V10/radar_sw_config.h")												
#Grouping GPO_GEN8_SRR8 HEADER source file
source_group(\\GEN8\\GPO\\SRR8\\HEADER\\V7  FILES ${GPO_GEN8_SRR8_HEADER_V7})
source_group(\\GEN8\\GPO\\SRR8\\HEADER\\V8  FILES ${GPO_GEN8_SRR8_HEADER_V8})
source_group(\\GEN8\\GPO\\SRR8\\HEADER\\V10  FILES ${GPO_GEN8_SRR8_HEADER_V10})

#GPO_GEN8_SRR8 MMIC Stream Source File
list(APPEND GPO_GEN8_SRR8_MMIC_SOURCE 		"${SourceFiles}/GEN8/GPO/SRR8/MMIC/MMIC_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/MMIC/MMIC_Logging.h")
					
#Grouping GPO_GEN8_SRR8 MMIC Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\MMIC FILES ${GPO_GEN8_SRR8_MMIC_SOURCE})

#GPO_GEN8_SRR8 MMIC source file										
list(APPEND GPO_GEN8_SRR8_MMIC_V7	    "${SourceFiles}/GEN8/GPO/SRR8/MMIC/V7/mmic_stream_V7.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V7/mmic_stream_V7.h"
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V7/mmic_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V7/mmic_stream_hdf_V7.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V7/radar_sw_config.h")	
									
list(APPEND GPO_GEN8_SRR8_MMIC_V20	    "${SourceFiles}/GEN8/GPO/SRR8/MMIC/V20/mmic_stream_V20.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V20/mmic_stream_V20.h"
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V20/mmic_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V20/mmic_stream_hdf_V20.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/MMIC/V20/radar_sw_config.h")										
												
#Grouping GPO_GEN8_SRR8 MMIC source file
source_group(\\GEN8\\GPO\\SRR8\\MMIC\\V7  FILES ${GPO_GEN8_SRR8_MMIC_V7})
source_group(\\GEN8\\GPO\\SRR8\\MMIC\\V20  FILES ${GPO_GEN8_SRR8_MMIC_V20})

#GPO_GEN8_SRR8 RADAR_CAPABILITY Stream Source File
list(APPEND GPO_GEN8_SRR8_RADAR_CAPABILITY_SOURCE 		"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/Radar_Capability_Logging.h")
					
#Grouping GPO_GEN8_SRR8 RADAR_CAPABILITY Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\RADAR_CAPABILITY FILES ${GPO_GEN8_SRR8_RADAR_CAPABILITY_SOURCE})

#GPO_GEN8_SRR8 RADAR_CAPABILITY source file										
list(APPEND GPO_GEN8_SRR8_RADAR_CAPABILITY_V49	    "${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V49/radar_capability_stream_V49.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V49/radar_capability_stream_V49.h"
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V49/radar_capability_stream.h"												
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V49/radar_capability_stream_hdf_V49.cpp"												
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V49/radar_sw_config.h")

list(APPEND GPO_GEN8_SRR8_RADAR_CAPABILITY_V53	    "${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V53/radar_capability_stream_V53.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V53/radar_capability_stream_V53.h"
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V53/radar_capability_stream.h"												
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V53/radar_capability_stream_hdf_V53.cpp"												
													"${SourceFiles}/GEN8/GPO/SRR8/RADAR_CAPABILITY/V53/radar_sw_config.h")													
												
#Grouping GPO_GEN8_SRR8 RADAR_CAPABILITY source file
source_group(\\GEN8\\GPO\\SRR8\\RADAR_CAPABILITY\\V49  FILES ${GPO_GEN8_SRR8_RADAR_CAPABILITY_V49})
source_group(\\GEN8\\GPO\\SRR8\\RADAR_CAPABILITY\\V53  FILES ${GPO_GEN8_SRR8_RADAR_CAPABILITY_V53})

#GPO_GEN8_SRR8 RDD Stream Source File
list(APPEND GPO_GEN8_SRR8_RDD_SOURCE 		"${SourceFiles}/GEN8/GPO/SRR8/RDD/RDD_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/RDD/RDD_Logging.h")
					
#Grouping GPO_GEN8_SRR8 RDD Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\RDD FILES ${GPO_GEN8_SRR8_RDD_SOURCE})

#GPO_GEN8_SRR8 RDD source file										
list(APPEND GPO_GEN8_SRR8_RDD_V13	    "${SourceFiles}/GEN8/GPO/SRR8/RDD/V13/rdd_stream_V13.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V13/rdd_stream_V13.h"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V13/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V13/rdd_stream_hdf_V13.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V13/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_SRR8_RDD_V113	    "${SourceFiles}/GEN8/GPO/SRR8/RDD/V113/rdd_stream_V113.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V113/rdd_stream_V113.h"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V113/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V113/rdd_stream_hdf_V113.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V113/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_SRR8_RDD_V18	    "${SourceFiles}/GEN8/GPO/SRR8/RDD/V18/rdd_stream_V18.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V18/rdd_stream_V18.h"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V18/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V18/rdd_stream_hdf_V18.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V18/radar_sw_config.h")		

										
list(APPEND GPO_GEN8_SRR8_RDD_V117	    "${SourceFiles}/GEN8/GPO/SRR8/RDD/V117/rdd_stream_V117.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V117/rdd_stream_V117.h"
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V117/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V117/rdd_stream_hdf_V117.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/RDD/V117/radar_sw_config.h")										
												
#Grouping GPO_GEN8_SRR8 RDD source file
source_group(\\GEN8\\GPO\\SRR8\\RDD\\V13   FILES ${GPO_GEN8_SRR8_RDD_V13})
source_group(\\GEN8\\GPO\\SRR8\\RDD\\V113  FILES ${GPO_GEN8_SRR8_RDD_V113})
source_group(\\GEN8\\GPO\\SRR8\\RDD\\V18   FILES ${GPO_GEN8_SRR8_RDD_V18})
source_group(\\GEN8\\GPO\\SRR8\\RDD\\V117   FILES ${GPO_GEN8_SRR8_RDD_V117})
#GPO_GEN8_SRR8 STATUS Stream Source File
list(APPEND GPO_GEN8_SRR8_STATUS_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/STATUS/Status_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/Status_Logging.h")
					
#Grouping GPO_GEN8_SRR8 STATUS Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\STATUS FILES ${GPO_GEN8_SRR8_STATUS_SOURCE})

#GPO_GEN8_SRR8 STATUS source file										
list(APPEND GPO_GEN8_SRR8_STATUS_V4	        "${SourceFiles}/GEN8/GPO/SRR8/STATUS/V4/status_stream_V4.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V4/status_stream_V4.h"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V4/status_stream.h"												
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V4/status_stream_hdf_V4.cpp"												
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V4/radar_sw_config.h")	

list(APPEND GPO_GEN8_SRR8_STATUS_V7	        "${SourceFiles}/GEN8/GPO/SRR8/STATUS/V7/status_stream_V7.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V7/status_stream_V7.h"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V7/status_stream.h"												
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V7/status_stream_hdf_V7.cpp"												
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V7/radar_sw_config.h")	

list(APPEND GPO_GEN8_SRR8_STATUS_V8	        "${SourceFiles}/GEN8/GPO/SRR8/STATUS/V8/status_stream_V8.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V8/status_stream_V8.h"
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V8/status_stream.h"												
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V8/status_stream_hdf_V8.cpp"												
											"${SourceFiles}/GEN8/GPO/SRR8/STATUS/V8/radar_sw_config.h")											
#Grouping GPO_GEN8_SRR8 STATUS source file
source_group(\\GEN8\\GPO\\SRR8\\STATUS\\V4  FILES ${GPO_GEN8_SRR8_STATUS_V4})
source_group(\\GEN8\\GPO\\SRR8\\STATUS\\V7  FILES ${GPO_GEN8_SRR8_STATUS_V7})
source_group(\\GEN8\\GPO\\SRR8\\STATUS\\V8  FILES ${GPO_GEN8_SRR8_STATUS_V8})

#GPO_GEN8_SRR8 DYNAMIC_ALIGNMENT Stream Source File
list(APPEND GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.h")
					
#Grouping GPO_GEN8_SRR8 DYNAMIC_ALIGNMENT Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\DYNAMIC_ALIGNMENT FILES ${GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_SOURCE})

#GPO_GEN8_SRR8 DYNAMIC_ALIGNMENT source file										
list(APPEND GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_V13	    "${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream_V13.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream_V13.h"
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream_hdf_V13.cpp"												
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V13/radar_sw_config.h")	
						
list(APPEND GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_V16	    "${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream_V16.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream_V16.h"
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream_hdf_V16.cpp"												
													"${SourceFiles}/GEN8/GPO/SRR8/DYNAMIC_ALIGNMENT/V16/radar_sw_config.h")							
												
#Grouping GPO_GEN8_SRR8 DYNAMIC_ALIGNMENT source file
source_group(\\GEN8\\GPO\\SRR8\\DYNAMIC_ALIGNMENT\\V13  FILES ${GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_V13})
source_group(\\GEN8\\GPO\\SRR8\\DYNAMIC_ALIGNMENT\\V16  FILES ${GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_V16})


#GPO_GEN8_SRR8 TOI_DS Stream Source File
list(APPEND GPO_GEN8_SRR8_TOI_DS_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/TOI_DS_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/TOI_DS_Logging.h")
					
#Grouping GPO_GEN8_SRR8 TOI_DS Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\TOI_DS FILES ${GPO_GEN8_SRR8_TOI_DS_SOURCE})

#GPO_GEN8_SRR8 TOI_DS source file										
list(APPEND GPO_GEN8_SRR8_TOI_DS_V1	    "${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V1/toi_ds_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V1/toi_ds_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V1/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V1/toi_ds_stream_hdf_V1.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V1/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_SRR8_TOI_DS_V2	    "${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V2/toi_ds_stream_V2.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V2/toi_ds_stream_V2.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V2/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V2/toi_ds_stream_hdf_V2.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V2/radar_sw_config.h")			

list(APPEND GPO_GEN8_SRR8_TOI_DS_V4	    "${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V4/toi_ds_stream_V4.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V4/toi_ds_stream_V4.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V4/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V4/toi_ds_stream_hdf_V4.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V4/radar_sw_config.h")

list(APPEND GPO_GEN8_SRR8_TOI_DS_V105	"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V105/toi_ds_stream_V105.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V105/toi_ds_stream_V105.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V105/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V105/toi_ds_stream_hdf_V105.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI_DS/V105/radar_sw_config.h")										
												
#Grouping GPO_GEN8_SRR8 TOI_DS source file
source_group(\\GEN8\\GPO\\SRR8\\TOI_DS\\V1  FILES ${GPO_GEN8_SRR8_TOI_DS_V1})
source_group(\\GEN8\\GPO\\SRR8\\TOI_DS\\V2  FILES ${GPO_GEN8_SRR8_TOI_DS_V2})
source_group(\\GEN8\\GPO\\SRR8\\TOI_DS\\V4  FILES ${GPO_GEN8_SRR8_TOI_DS_V4})
source_group(\\GEN8\\GPO\\SRR8\\TOI_DS\\V105  FILES ${GPO_GEN8_SRR8_TOI_DS_V105})
#GPO_GEN8_SRR8 TOI Stream Source File
list(APPEND GPO_GEN8_SRR8_TOI_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/TOI/TOI_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/TOI_Logging.h")
					
#Grouping GPO_GEN8_SRR8 TOI Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\TOI FILES ${GPO_GEN8_SRR8_TOI_SOURCE})

#GPO_GEN8_SRR8 TOI source file										
list(APPEND GPO_GEN8_SRR8_TOI_V1	    "${SourceFiles}/GEN8/GPO/SRR8/TOI/V1/toi_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V1/toi_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V1/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V1/toi_stream_hdf_V1.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V1/radar_sw_config.h")	
										
										
list(APPEND GPO_GEN8_SRR8_TOI_V3	    "${SourceFiles}/GEN8/GPO/SRR8/TOI/V3/toi_stream_V3.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V3/toi_stream_V3.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V3/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V3/toi_stream_hdf_V3.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V3/radar_sw_config.h")										
												
list(APPEND GPO_GEN8_SRR8_TOI_V5	    "${SourceFiles}/GEN8/GPO/SRR8/TOI/V5/toi_stream_V5.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V5/toi_stream_V5.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V5/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V5/toi_stream_hdf_V5.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V5/radar_sw_config.h")

list(APPEND GPO_GEN8_SRR8_TOI_V106      "${SourceFiles}/GEN8/GPO/SRR8/TOI/V106/toi_stream_V106.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V106/toi_stream_V106.h"
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V106/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V106/toi_stream_hdf_V106.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/TOI/V106/radar_sw_config.h")										
#Grouping GPO_GEN8_SRR8 TOI source file
source_group(\\GEN8\\GPO\\SRR8\\TOI\\V1  FILES ${GPO_GEN8_SRR8_TOI_V1})
source_group(\\GEN8\\GPO\\SRR8\\TOI\\V3  FILES ${GPO_GEN8_SRR8_TOI_V3})
source_group(\\GEN8\\GPO\\SRR8\\TOI\\V5  FILES ${GPO_GEN8_SRR8_TOI_V5})
source_group(\\GEN8\\GPO\\SRR8\\TOI\\V106  FILES ${GPO_GEN8_SRR8_TOI_V106})

#GPO_GEN8_SRR8 VID Stream Source File
list(APPEND GPO_GEN8_SRR8_VID_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/VID/VID_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/VID/VID_Logging.h")
					
#Grouping GPO_GEN8_SRR8 VID Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\VID FILES ${GPO_GEN8_SRR8_VID_SOURCE})

#GPO_GEN8_SRR8 VID source file										
list(APPEND GPO_GEN8_SRR8_VID_V2	    "${SourceFiles}/GEN8/GPO/SRR8/VID/V2/vid_stream_V2.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/VID/V2/vid_stream_V2.h"
										"${SourceFiles}/GEN8/GPO/SRR8/VID/V2/vid_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/VID/V2/vid_stream_hdf_V2.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/VID/V2/radar_sw_config.h")	
												
#Grouping GPO_GEN8_SRR8 VID source file
source_group(\\GEN8\\GPO\\SRR8\\VID\\V2  FILES ${GPO_GEN8_SRR8_VID_V2})


#GPO_GEN8_SRR8 VSE Stream Source File
list(APPEND GPO_GEN8_SRR8_VSE_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/VSE/VSE_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/VSE_Logging.h")
					
#Grouping GPO_GEN8_SRR8 VSE Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\VSE FILES ${GPO_GEN8_SRR8_VSE_SOURCE})

#GPO_GEN8_SRR8 VSE source file										
list(APPEND GPO_GEN8_SRR8_VSE_V2	    "${SourceFiles}/GEN8/GPO/SRR8/VSE/V2/vse_stream_V2.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V2/vse_stream_V2.h"
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V2/vse_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V2/vse_stream_hdf_V2.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V2/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_SRR8_VSE_V3	    "${SourceFiles}/GEN8/GPO/SRR8/VSE/V3/vse_stream_V3.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V3/vse_stream_V3.h"
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V3/vse_stream.h"												
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V3/vse_stream_hdf_V3.cpp"												
										"${SourceFiles}/GEN8/GPO/SRR8/VSE/V3/radar_sw_config.h")												
#Grouping GPO_GEN8_SRR8 VSE source file
source_group(\\GEN8\\GPO\\SRR8\\VSE\\V2  FILES ${GPO_GEN8_SRR8_VSE_V2})
source_group(\\GEN8\\GPO\\SRR8\\VSE\\V3  FILES ${GPO_GEN8_SRR8_VSE_V3})

#Tracker_Streams for GEN8 - Common for SRR8 and FLR8

#GPO_GEN8_SRR8 SYNC_INFO_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_SYNC_INFO_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/SYNC_INFO_LOG/SYNC_INFO_LOG_Logging.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/SYNC_INFO_LOG/SYNC_INFO_LOG_Logging.h")
					
#Grouping GPO_GEN8_SRR8 SYNC_INFO_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\SYNC_INFO_LOG FILES ${GPO_GEN8_SRR8_SYNC_INFO_LOG_SOURCE})

#GPO_GEN8_SRR8 SYNC_INFO_LOG source file										
list(APPEND GPO_GEN8_SRR8_SYNC_INFO_LOG_V2	    "${SourceFiles}/GEN8/GPO/SRR8/SYNC_INFO_LOG/V2/SyncInfoLog_stream_V2.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/SYNC_INFO_LOG/V2/SyncInfoLog_stream_V2.h"
												"${SourceFiles}/GEN8/GPO/SRR8/SYNC_INFO_LOG/V2/SyncInfoLog_stream.h")	
												
#Grouping GPO_GEN8_SRR8 SYNC_INFO_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\SYNC_INFO_LOG\\V2  FILES ${GPO_GEN8_SRR8_SYNC_INFO_LOG_V2})


#GPO_GEN8_SRR8 BSIS Stream Source File
list(APPEND GPO_GEN8_SRR8_BSIS_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/BSIS/BSIS_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/BSIS/BSIS_Logging.h")

#Grouping GPO_GEN8_SRR8 BSIS Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\BSIS FILES ${GPO_GEN8_SRR8_BSIS_SOURCE})

#GPO_GEN8_SRR8 BSIS source file
list(APPEND GPO_GEN8_SRR8_BSIS_V1	    "${SourceFiles}/GEN8/GPO/SRR8/BSIS/V1/bsis_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/BSIS/V1/bsis_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/SRR8/BSIS/V1/bsis_stream.h")

#Grouping GPO_GEN8_SRR8 BSIS source file
source_group(\\GEN8\\GPO\\SRR8\\BSIS\\V1  FILES ${GPO_GEN8_SRR8_BSIS_V1})


#GPO_GEN8_SRR8 MOIS Stream Source File
list(APPEND GPO_GEN8_SRR8_MOIS_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/MOIS/MOIS_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/MOIS/MOIS_Logging.h")

#Grouping GPO_GEN8_SRR8 MOIS Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\MOIS FILES ${GPO_GEN8_SRR8_MOIS_SOURCE})

#GPO_GEN8_SRR8 MOIS source file
list(APPEND GPO_GEN8_SRR8_MOIS_V1	    "${SourceFiles}/GEN8/GPO/SRR8/MOIS/V1/mois_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/MOIS/V1/mois_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/SRR8/MOIS/V1/mois_stream.h")

#Grouping GPO_GEN8_SRR8 MOIS source file
source_group(\\GEN8\\GPO\\SRR8\\MOIS\\V1  FILES ${GPO_GEN8_SRR8_MOIS_V1})


#GPO_GEN8_SRR8 OLP Stream Source File
list(APPEND GPO_GEN8_SRR8_OLP_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/OLP/OLP_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/OLP/OLP_Logging.h")

#Grouping GPO_GEN8_SRR8 OLP Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\OLP FILES ${GPO_GEN8_SRR8_OLP_SOURCE})

#GPO_GEN8_SRR8 OLP source file
list(APPEND GPO_GEN8_SRR8_OLP_V1	    "${SourceFiles}/GEN8/GPO/SRR8/OLP/V1/olp_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/SRR8/OLP/V1/olp_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/SRR8/OLP/V1/olp_stream.h")

#Grouping GPO_GEN8_SRR8 OLP source file
source_group(\\GEN8\\GPO\\SRR8\\OLP\\V1  FILES ${GPO_GEN8_SRR8_OLP_V1})


#GPO_GEN8_SRR8 F360_DETECTION_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_DETECTION_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_DETECTION_LOG/F360_DETECTION_LOG_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_DETECTION_LOG/F360_DETECTION_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_DETECTION_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_DETECTION_LOG FILES ${GPO_GEN8_SRR8_F360_DETECTION_LOG_SOURCE})

#GPO_GEN8_SRR8 F360_DETECTION_LOG source file
list(APPEND GPO_GEN8_SRR8_F360_DETECTION_LOG_V3	    "${SourceFiles}/GEN8/GPO/SRR8/F360_DETECTION_LOG/V3/f360_detection_log_stream_V3.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_DETECTION_LOG/V3/f360_detection_log_stream_V3.h"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_DETECTION_LOG/V3/f360_detection_log_stream.h")

#Grouping GPO_GEN8_SRR8 F360_DETECTION_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\F360_DETECTION_LOG\\V3  FILES ${GPO_GEN8_SRR8_F360_DETECTION_LOG_V3})


#GPO_GEN8_SRR8 F360_HOST_PROPS_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_HOST_PROPS_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_HOST_PROPS_LOG/F360_HOST_PROPS_LOG_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_HOST_PROPS_LOG/F360_HOST_PROPS_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_HOST_PROPS_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_HOST_PROPS_LOG FILES ${GPO_GEN8_SRR8_F360_HOST_PROPS_LOG_SOURCE})

#GPO_GEN8_SRR8 F360_HOST_PROPS_LOG source file
list(APPEND GPO_GEN8_SRR8_F360_HOST_PROPS_LOG_V2	    "${SourceFiles}/GEN8/GPO/SRR8/F360_HOST_PROPS_LOG/V2/F360HostPropsLog_stream_V2.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_HOST_PROPS_LOG/V2/F360HostPropsLog_stream_V2.h"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_HOST_PROPS_LOG/V2/F360HostPropsLog_stream.h")

#Grouping GPO_GEN8_SRR8 F360_HOST_PROPS_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\F360_HOST_PROPS_LOG\\V2  FILES ${GPO_GEN8_SRR8_F360_HOST_PROPS_LOG_V2})


#GPO_GEN8_SRR8 F360_INTERNAL_CLUSTER Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/F360_INTERNAL_CLUSTER_Logging.cpp"
															"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/F360_INTERNAL_CLUSTER_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_CLUSTER Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_CLUSTER FILES ${GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_SOURCE})

#GPO_GEN8_SRR8 F360_INTERNAL_CLUSTER source file
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_V4	    "${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/V4/f360_internal_cluster_stream_V4.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/V4/f360_internal_cluster_stream_V4.h"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/V4/f360_internal_cluster_stream.h")

list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_V5	    "${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/V5/f360_internal_cluster_stream_V5.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/V5/f360_internal_cluster_stream_V5.h"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CLUSTER/V5/f360_internal_cluster_stream.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_CLUSTER source file
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_CLUSTER\\V4  FILES ${GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_V4})
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_CLUSTER\\V5  FILES ${GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_V5})


#GPO_GEN8_SRR8 F360_INTERNAL_CWD Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_CWD_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CWD/F360_INTERNAL_CWD_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CWD/F360_INTERNAL_CWD_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_CWD Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_CWD FILES ${GPO_GEN8_SRR8_F360_INTERNAL_CWD_SOURCE})

#GPO_GEN8_SRR8 F360_INTERNAL_CWD source file
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_CWD_V3	    "${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CWD/V3/f360_internal_cwd_stream_V3.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CWD/V3/f360_internal_cwd_stream_V3.h"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_CWD/V3/f360_internal_cwd_stream.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_CWD source file
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_CWD\\V3  FILES ${GPO_GEN8_SRR8_F360_INTERNAL_CWD_V3})


#GPO_GEN8_SRR8 F360_INTERNAL_DETECTION_HISTORY Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_DETECTION_HISTORY_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_DETECTION_HISTORY/F360_INTERNAL_DETECTION_HISTORY_Logging.cpp"
																	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_DETECTION_HISTORY/F360_INTERNAL_DETECTION_HISTORY_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_DETECTION_HISTORY Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_DETECTION_HISTORY FILES ${GPO_GEN8_SRR8_F360_INTERNAL_DETECTION_HISTORY_SOURCE})

#GPO_GEN8_SRR8 F360_INTERNAL_DETECTION_HISTORY source file
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_DETECTION_HISTORY_V4	    "${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_DETECTION_HISTORY/V4/f360_internal_detection_history_stream_V4.cpp"
																	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_DETECTION_HISTORY/V4/f360_internal_detection_history_stream_V4.h"
																	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_DETECTION_HISTORY/V4/f360_internal_detection_history_stream.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_DETECTION_HISTORY source file
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_DETECTION_HISTORY\\V4  FILES ${GPO_GEN8_SRR8_F360_INTERNAL_DETECTION_HISTORY_V4})


#GPO_GEN8_SRR8 F360_INTERNAL_OBJECT Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_OBJECT_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_OBJECT/F360_INTERNAL_OBJECT_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_OBJECT/F360_INTERNAL_OBJECT_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_OBJECT Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_OBJECT FILES ${GPO_GEN8_SRR8_F360_INTERNAL_OBJECT_SOURCE})

#GPO_GEN8_SRR8 F360_INTERNAL_OBJECT source file
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_OBJECT_V35	    "${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_OBJECT/V35/f360_internal_object_stream_V35.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_OBJECT/V35/f360_internal_object_stream_V35.h"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_OBJECT/V35/f360_internal_object_stream.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_OBJECT source file
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_OBJECT\\V35  FILES ${GPO_GEN8_SRR8_F360_INTERNAL_OBJECT_V35})


#GPO_GEN8_SRR8 F360_INTERNAL_REFLECTION_BUFFER Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_REFLECTION_BUFFER_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_REFLECTION_BUFFER/F360_INTERNAL_REFLECTION_BUFFER_Logging.cpp"
																	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_REFLECTION_BUFFER/F360_INTERNAL_REFLECTION_BUFFER_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_REFLECTION_BUFFER Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_REFLECTION_BUFFER FILES ${GPO_GEN8_SRR8_F360_INTERNAL_REFLECTION_BUFFER_SOURCE})

#GPO_GEN8_SRR8 F360_INTERNAL_REFLECTION_BUFFER source file
list(APPEND GPO_GEN8_SRR8_F360_INTERNAL_REFLECTION_BUFFER_V1	    "${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_REFLECTION_BUFFER/V1/f360_internal_reflection_buffer_stream_V1.cpp"
																	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_REFLECTION_BUFFER/V1/f360_internal_reflection_buffer_stream_V1.h"
																	"${SourceFiles}/GEN8/GPO/SRR8/F360_INTERNAL_REFLECTION_BUFFER/V1/f360_internal_reflection_buffer_stream.h")

#Grouping GPO_GEN8_SRR8 F360_INTERNAL_REFLECTION_BUFFER source file
source_group(\\GEN8\\GPO\\SRR8\\F360_INTERNAL_REFLECTION_BUFFER\\V1  FILES ${GPO_GEN8_SRR8_F360_INTERNAL_REFLECTION_BUFFER_V1})


#GPO_GEN8_SRR8 F360_OBJECTS_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_OBJECTS_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_OBJECTS_LOG/F360_OBJECTS_LOG_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_OBJECTS_LOG/F360_OBJECTS_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_OBJECTS_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_OBJECTS_LOG FILES ${GPO_GEN8_SRR8_F360_OBJECTS_LOG_SOURCE})

#GPO_GEN8_SRR8 F360_OBJECTS_LOG source file
list(APPEND GPO_GEN8_SRR8_F360_OBJECTS_LOG_V10	    "${SourceFiles}/GEN8/GPO/SRR8/F360_OBJECTS_LOG/V10/f360_object_log_stream_V10.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_OBJECTS_LOG/V10/f360_object_log_stream_V10.h"
													"${SourceFiles}/GEN8/GPO/SRR8/F360_OBJECTS_LOG/V10/f360_object_log_stream.h")

#Grouping GPO_GEN8_SRR8 F360_OBJECTS_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\F360_OBJECTS_LOG\\V10  FILES ${GPO_GEN8_SRR8_F360_OBJECTS_LOG_V10})


#GPO_GEN8_SRR8 F360_FUNCTIONAL_SAFETY_FAULTS Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_FUNCTIONAL_SAFETY_FAULTS_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_FUNCTIONAL_SAFETY_FAULTS/F360_FUNCTIONAL_SAFETY_FAULTS_Logging.cpp"
																"${SourceFiles}/GEN8/GPO/SRR8/F360_FUNCTIONAL_SAFETY_FAULTS/F360_FUNCTIONAL_SAFETY_FAULTS_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_FUNCTIONAL_SAFETY_FAULTS Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_FUNCTIONAL_SAFETY_FAULTS FILES ${GPO_GEN8_SRR8_F360_FUNCTIONAL_SAFETY_FAULTS_SOURCE})

#GPO_GEN8_SRR8 F360_FUNCTIONAL_SAFETY_FAULTS source file
list(APPEND GPO_GEN8_SRR8_F360_FUNCTIONAL_SAFETY_FAULTS_V12	    "${SourceFiles}/GEN8/GPO/SRR8/F360_FUNCTIONAL_SAFETY_FAULTS/V12/f360_functional_safety_faults_log_stream_V12.cpp"
																"${SourceFiles}/GEN8/GPO/SRR8/F360_FUNCTIONAL_SAFETY_FAULTS/V12/f360_functional_safety_faults_log_stream_V12.h"
																"${SourceFiles}/GEN8/GPO/SRR8/F360_FUNCTIONAL_SAFETY_FAULTS/V12/f360_functional_safety_faults_log_stream.h")

#Grouping GPO_GEN8_SRR8 F360_FUNCTIONAL_SAFETY_FAULTS source file
source_group(\\GEN8\\GPO\\SRR8\\F360_FUNCTIONAL_SAFETY_FAULTS\\V12  FILES ${GPO_GEN8_SRR8_F360_FUNCTIONAL_SAFETY_FAULTS_V12})


#GPO_GEN8_SRR8 F360_SENSOR_CALIB_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_F360_SENSOR_CALIB_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/F360_SENSOR_CALIB_LOG/F360_SENSOR_CALIB_LOG_Logging.cpp"
															"${SourceFiles}/GEN8/GPO/SRR8/F360_SENSOR_CALIB_LOG/F360_SENSOR_CALIB_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 F360_SENSOR_CALIB_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\F360_SENSOR_CALIB_LOG FILES ${GPO_GEN8_SRR8_F360_SENSOR_CALIB_LOG_SOURCE})

#GPO_GEN8_SRR8 F360_SENSOR_CALIB_LOG source file
list(APPEND GPO_GEN8_SRR8_F360_SENSOR_CALIB_LOG_V4	    "${SourceFiles}/GEN8/GPO/SRR8/F360_SENSOR_CALIB_LOG/V4/f360_sensor_calib_log_stream_V4.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_SENSOR_CALIB_LOG/V4/f360_sensor_calib_log_stream_V4.h"
														"${SourceFiles}/GEN8/GPO/SRR8/F360_SENSOR_CALIB_LOG/V4/f360_sensor_calib_log_stream.h")

#Grouping GPO_GEN8_SRR8 F360_SENSOR_CALIB_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\F360_SENSOR_CALIB_LOG\\V4  FILES ${GPO_GEN8_SRR8_F360_SENSOR_CALIB_LOG_V4})


#GPO_GEN8_SRR8 HOST_CALIBS_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_HOST_CALIBS_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/HOST_CALIBS_LOG/HOST_CALIBS_LOG_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/HOST_CALIBS_LOG/HOST_CALIBS_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 HOST_CALIBS_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\HOST_CALIBS_LOG FILES ${GPO_GEN8_SRR8_HOST_CALIBS_LOG_SOURCE})

#GPO_GEN8_SRR8 HOST_CALIBS_LOG source file
list(APPEND GPO_GEN8_SRR8_HOST_CALIBS_LOG_V3	    "${SourceFiles}/GEN8/GPO/SRR8/HOST_CALIBS_LOG/V3/HostCalibsLog_stream_V3.cpp"
												"${SourceFiles}/GEN8/GPO/SRR8/HOST_CALIBS_LOG/V3/HostCalibsLog_stream_V3.h"
												"${SourceFiles}/GEN8/GPO/SRR8/HOST_CALIBS_LOG/V3/HostCalibsLog_stream.h")

#Grouping GPO_GEN8_SRR8 HOST_CALIBS_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\HOST_CALIBS_LOG\\V3  FILES ${GPO_GEN8_SRR8_HOST_CALIBS_LOG_V3})


#GPO_GEN8_SRR8 STATIC_ENV_POLYS_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_STATIC_ENV_POLYS_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/STATIC_ENV_POLYS_LOG/STATIC_ENV_POLYS_LOG_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/STATIC_ENV_POLYS_LOG/STATIC_ENV_POLYS_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 STATIC_ENV_POLYS_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\STATIC_ENV_POLYS_LOG FILES ${GPO_GEN8_SRR8_STATIC_ENV_POLYS_LOG_SOURCE})

#GPO_GEN8_SRR8 STATIC_ENV_POLYS_LOG source file
list(APPEND GPO_GEN8_SRR8_STATIC_ENV_POLYS_LOG_V3	    "${SourceFiles}/GEN8/GPO/SRR8/STATIC_ENV_POLYS_LOG/V3/StaticEnvPolysLog_stream_V3.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/STATIC_ENV_POLYS_LOG/V3/StaticEnvPolysLog_stream_V3.h"
														"${SourceFiles}/GEN8/GPO/SRR8/STATIC_ENV_POLYS_LOG/V3/StaticEnvPolysLog_stream.h")

#Grouping GPO_GEN8_SRR8 STATIC_ENV_POLYS_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\STATIC_ENV_POLYS_LOG\\V3  FILES ${GPO_GEN8_SRR8_STATIC_ENV_POLYS_LOG_V3})


#GPO_GEN8_SRR8 TIMING_INFO_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_TIMING_INFO_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/TIMING_INFO_LOG/TIMING_INFO_LOG_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/TIMING_INFO_LOG/TIMING_INFO_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 TIMING_INFO_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\TIMING_INFO_LOG FILES ${GPO_GEN8_SRR8_TIMING_INFO_LOG_SOURCE})

#GPO_GEN8_SRR8 TIMING_INFO_LOG source file
list(APPEND GPO_GEN8_SRR8_TIMING_INFO_LOG_V22	    "${SourceFiles}/GEN8/GPO/SRR8/TIMING_INFO_LOG/V22/TimingInfoLog_stream_V22.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/TIMING_INFO_LOG/V22/TimingInfoLog_stream_V22.h"
													"${SourceFiles}/GEN8/GPO/SRR8/TIMING_INFO_LOG/V22/TimingInfoLog_stream.h")

#Grouping GPO_GEN8_SRR8 TIMING_INFO_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\TIMING_INFO_LOG\\V22  FILES ${GPO_GEN8_SRR8_TIMING_INFO_LOG_V22})


#GPO_GEN8_SRR8 TRACKER_INFO_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_TRACKER_INFO_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/TRACKER_INFO_LOG/TRACKER_INFO_LOG_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/TRACKER_INFO_LOG/TRACKER_INFO_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 TRACKER_INFO_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\TRACKER_INFO_LOG FILES ${GPO_GEN8_SRR8_TRACKER_INFO_LOG_SOURCE})

#GPO_GEN8_SRR8 TRACKER_INFO_LOG source file
list(APPEND GPO_GEN8_SRR8_TRACKER_INFO_LOG_V9	    "${SourceFiles}/GEN8/GPO/SRR8/TRACKER_INFO_LOG/V9/TrackerInfoLog_stream_V9.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/TRACKER_INFO_LOG/V9/TrackerInfoLog_stream_V9.h"
													"${SourceFiles}/GEN8/GPO/SRR8/TRACKER_INFO_LOG/V9/TrackerInfoLog_stream.h")

#Grouping GPO_GEN8_SRR8 TRACKER_INFO_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\TRACKER_INFO_LOG\\V9  FILES ${GPO_GEN8_SRR8_TRACKER_INFO_LOG_V9})


#GPO_GEN8_SRR8 TRAILER_DETECTOR_INTERNAL_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_TRAILER_DETECTOR_INTERNAL_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_INTERNAL_LOG/TRAILER_DETECTOR_INTERNAL_LOG_Logging.cpp"
																	"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_INTERNAL_LOG/TRAILER_DETECTOR_INTERNAL_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 TRAILER_DETECTOR_INTERNAL_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\TRAILER_DETECTOR_INTERNAL_LOG FILES ${GPO_GEN8_SRR8_TRAILER_DETECTOR_INTERNAL_LOG_SOURCE})

#GPO_GEN8_SRR8 TRAILER_DETECTOR_INTERNAL_LOG source file
list(APPEND GPO_GEN8_SRR8_TRAILER_DETECTOR_INTERNAL_LOG_V3	    "${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_INTERNAL_LOG/V3/TrailerDetectorInternalLog_stream_V3.cpp"
																"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_INTERNAL_LOG/V3/TrailerDetectorInternalLog_stream_V3.h"
																"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_INTERNAL_LOG/V3/TrailerDetectorInternalLog_stream.h")

#Grouping GPO_GEN8_SRR8 TRAILER_DETECTOR_INTERNAL_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\TRAILER_DETECTOR_INTERNAL_LOG\\V3  FILES ${GPO_GEN8_SRR8_TRAILER_DETECTOR_INTERNAL_LOG_V3})


#GPO_GEN8_SRR8 TRAILER_DETECTOR_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_TRAILER_DETECTOR_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_LOG/TRAILER_DETECTOR_LOG_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_LOG/TRAILER_DETECTOR_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 TRAILER_DETECTOR_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\TRAILER_DETECTOR_LOG FILES ${GPO_GEN8_SRR8_TRAILER_DETECTOR_LOG_SOURCE})

#GPO_GEN8_SRR8 TRAILER_DETECTOR_LOG source file
list(APPEND GPO_GEN8_SRR8_TRAILER_DETECTOR_LOG_V2	    "${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_LOG/V2/TrailerDetectorLog_stream_V2.cpp"
														"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_LOG/V2/TrailerDetectorLog_stream_V2.h"
														"${SourceFiles}/GEN8/GPO/SRR8/TRAILER_DETECTOR_LOG/V2/TrailerDetectorLog_stream.h")

#Grouping GPO_GEN8_SRR8 TRAILER_DETECTOR_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\TRAILER_DETECTOR_LOG\\V2  FILES ${GPO_GEN8_SRR8_TRAILER_DETECTOR_LOG_V2})


#GPO_GEN8_SRR8 VEHICLE_INFO_LOG Stream Source File
list(APPEND GPO_GEN8_SRR8_VEHICLE_INFO_LOG_SOURCE 	"${SourceFiles}/GEN8/GPO/SRR8/VEHICLE_INFO_LOG/VEHICLE_INFO_LOG_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/VEHICLE_INFO_LOG/VEHICLE_INFO_LOG_Logging.h")

#Grouping GPO_GEN8_SRR8 VEHICLE_INFO_LOG Stream Source File
source_group(\\GEN8\\GPO\\SRR8\\VEHICLE_INFO_LOG FILES ${GPO_GEN8_SRR8_VEHICLE_INFO_LOG_SOURCE})

#GPO_GEN8_SRR8 VEHICLE_INFO_LOG source file
list(APPEND GPO_GEN8_SRR8_VEHICLE_INFO_LOG_V9	    "${SourceFiles}/GEN8/GPO/SRR8/VEHICLE_INFO_LOG/V9/VehicleInfoLog_stream_V9.cpp"
													"${SourceFiles}/GEN8/GPO/SRR8/VEHICLE_INFO_LOG/V9/VehicleInfoLog_stream_V9.h"
													"${SourceFiles}/GEN8/GPO/SRR8/VEHICLE_INFO_LOG/V9/VehicleInfoLog_stream.h")

#Grouping GPO_GEN8_SRR8 VEHICLE_INFO_LOG source file
source_group(\\GEN8\\GPO\\SRR8\\VEHICLE_INFO_LOG\\V9  FILES ${GPO_GEN8_SRR8_VEHICLE_INFO_LOG_V9})


#GEN8_FLR8
#GPO_GEN8_FLR8 ALIGNMENT Stream Source File
list(APPEND GPO_GEN8_FLR8_ALIGNMENT_SOURCE 		"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/Alignment_Logging.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/Alignment_Logging.h")
					
#Grouping GPO_GEN8_FLR8 ALIGNMENT Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\ALIGNMENT FILES ${GPO_GEN8_FLR8_ALIGNMENT_SOURCE})

#GPO_GEN8_FLR8 ALIGNMENT source file										
list(APPEND GPO_GEN8_FLR8_ALIGNMENT_V10	    	"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/V10/alignment_stream_V10.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/V10/alignment_stream_V10.h"
												"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/V10/alignment_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/V10/alignment_stream_hdf_V10.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/ALIGNMENT/V10/radar_sw_config.h")	
												
#Grouping GPO_GEN8_FLR8 ALIGNMENT source file
source_group(\\GEN8\\GPO\\FLR8\\ALIGNMENT\\V10  FILES ${GPO_GEN8_FLR8_ALIGNMENT_V10})


#GPO_GEN8_FLR8 DETECTION Stream Source File
list(APPEND GPO_GEN8_FLR8_DETECTION_SOURCE 		"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/Detection_Logging.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/Detection_Logging.h")
					
#Grouping GPO_GEN8_FLR8 DETECTION Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\DETECTION FILES ${GPO_GEN8_FLR8_DETECTION_SOURCE})

#GPO_GEN8_FLR8 DETECTION source file										
list(APPEND GPO_GEN8_FLR8_DETECTION_V5	    	"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V5/detection_stream_V5.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V5/detection_stream_V5.h"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V5/detection_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V5/detection_stream_hdf_V5.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V5/radar_sw_config.h")	
									
list(APPEND GPO_GEN8_FLR8_DETECTION_V115	    "${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V115/detection_stream_V115.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V115/detection_stream_V115.h"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V115/detection_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V115/detection_stream_hdf_V115.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V115/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_FLR8_DETECTION_V200	    "${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V200/detection_stream_V200.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V200/detection_stream_V200.h"
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V200/detection_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V200/detection_stream_hdf_V200.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/DETECTION/V200/radar_sw_config.h")													
												
												
#Grouping GPO_GEN8_FLR8 DETECTION source file
source_group(\\GEN8\\GPO\\FLR8\\DETECTION\\V5    FILES ${GPO_GEN8_FLR8_DETECTION_V5})
source_group(\\GEN8\\GPO\\FLR8\\DETECTION\\V115  FILES ${GPO_GEN8_FLR8_DETECTION_V115})
source_group(\\GEN8\\GPO\\FLR8\\DETECTION\\V200  FILES ${GPO_GEN8_FLR8_DETECTION_V200})

#GPO_GEN8_FLR8 DOWN_SELECTION Stream Source File
list(APPEND GPO_GEN8_FLR8_DOWN_SELECTION_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/Down_Selection_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/Down_Selection_Logging.h")
					
#Grouping GPO_GEN8_FLR8 DOWN_SELECTION Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\DOWN_SELECTION FILES ${GPO_GEN8_FLR8_DOWN_SELECTION_SOURCE})

#GPO_GEN8_FLR8 DOWN_SELECTION source file										
list(APPEND GPO_GEN8_FLR8_DOWN_SELECTION_V1	    "${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V1/down_selection_stream_V1.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V1/down_selection_stream_V1.h"
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V1/down_selection_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V1/down_selection_stream_hdf_V1.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V1/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_FLR8_DOWN_SELECTION_V101	"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V101/down_selection_stream_V101.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V101/down_selection_stream_V101.h"
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V101/down_selection_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V101/down_selection_stream_hdf_V101.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V101/radar_sw_config.h")	
												
list(APPEND GPO_GEN8_FLR8_DOWN_SELECTION_V200	"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V200/down_selection_stream_V200.cpp"
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V200/down_selection_stream_V200.h"
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V200/down_selection_stream.h"												
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V200/down_selection_stream_hdf_V200.cpp"												
												"${SourceFiles}/GEN8/GPO/FLR8/DOWN_SELECTION/V200/radar_sw_config.h")													
#Grouping GPO_GEN8_FLR8 DOWN_SELECTION source file
source_group(\\GEN8\\GPO\\FLR8\\DOWN_SELECTION\\V1  	FILES ${GPO_GEN8_FLR8_DOWN_SELECTION_V1})
source_group(\\GEN8\\GPO\\FLR8\\DOWN_SELECTION\\V101  	FILES ${GPO_GEN8_FLR8_DOWN_SELECTION_V101})
source_group(\\GEN8\\GPO\\FLR8\\DOWN_SELECTION\\V200  	FILES ${GPO_GEN8_FLR8_DOWN_SELECTION_V200})

#GPO_GEN8_FLR8 HEADER Stream Source File
list(APPEND GPO_GEN8_FLR8_HEADER_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/HEADER/Header_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/HEADER/Header_Logging.h")
					
#Grouping GPO_GEN8_FLR8 HEADER Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\HEADER FILES ${GPO_GEN8_FLR8_HEADER_SOURCE})

#GPO_GEN8_FLR8 HEADER source file										
list(APPEND GPO_GEN8_FLR8_HEADER_V7	    "${SourceFiles}/GEN8/GPO/FLR8/HEADER/V7/header_stream_V7.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V7/header_stream_V7.h"
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V7/header_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V7/header_stream_hdf_V7.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V7/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_FLR8_HEADER_V8	    "${SourceFiles}/GEN8/GPO/FLR8/HEADER/V8/header_stream_V8.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V8/header_stream_V8.h"
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V8/header_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V8/header_stream_hdf_V8.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V8/radar_sw_config.h")	

list(APPEND GPO_GEN8_FLR8_HEADER_V10	"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V10/header_stream_V10.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V10/header_stream_V10.h"
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V10/header_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V10/header_stream_hdf_V10.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/HEADER/V10/radar_sw_config.h")											
									
												
#Grouping GPO_GEN8_FLR8 HEADER source file
source_group(\\GEN8\\GPO\\FLR8\\HEADER\\V7  FILES ${GPO_GEN8_FLR8_HEADER_V7})
source_group(\\GEN8\\GPO\\FLR8\\HEADER\\V8  FILES ${GPO_GEN8_FLR8_HEADER_V8})
source_group(\\GEN8\\GPO\\FLR8\\HEADER\\V10  FILES ${GPO_GEN8_FLR8_HEADER_V10})

#GPO_GEN8_FLR8 MMIC Stream Source File
list(APPEND GPO_GEN8_FLR8_MMIC_SOURCE 		"${SourceFiles}/GEN8/GPO/FLR8/MMIC/MMIC_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/MMIC/MMIC_Logging.h")
					
#Grouping GPO_GEN8_FLR8 MMIC Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\MMIC FILES ${GPO_GEN8_FLR8_MMIC_SOURCE})

#GPO_GEN8_FLR8 MMIC source file										
list(APPEND GPO_GEN8_FLR8_MMIC_V7	    "${SourceFiles}/GEN8/GPO/FLR8/MMIC/V7/mmic_stream_V7.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V7/mmic_stream_V7.h"
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V7/mmic_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V7/mmic_stream_hdf_V7.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V7/radar_sw_config.h")	
											
list(APPEND GPO_GEN8_FLR8_MMIC_V20	    "${SourceFiles}/GEN8/GPO/FLR8/MMIC/V20/mmic_stream_V20.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V20/mmic_stream_V20.h"
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V20/mmic_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V20/mmic_stream_hdf_V20.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/MMIC/V20/radar_sw_config.h")												
#Grouping GPO_GEN8_FLR8 MMIC source file
source_group(\\GEN8\\GPO\\FLR8\\MMIC\\V7  FILES ${GPO_GEN8_FLR8_MMIC_V7})
source_group(\\GEN8\\GPO\\FLR8\\MMIC\\V20  FILES ${GPO_GEN8_FLR8_MMIC_V20})

#GPO_GEN8_FLR8 RADAR_CAPABILITY Stream Source File
list(APPEND GPO_GEN8_FLR8_RADAR_CAPABILITY_SOURCE 		"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/Radar_Capability_Logging.cpp"
														"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/Radar_Capability_Logging.h")
					
#Grouping GPO_GEN8_FLR8 RADAR_CAPABILITY Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\RADAR_CAPABILITY FILES ${GPO_GEN8_FLR8_RADAR_CAPABILITY_SOURCE})

#GPO_GEN8_FLR8 RADAR_CAPABILITY source file										
list(APPEND GPO_GEN8_FLR8_RADAR_CAPABILITY_V49	    "${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V49/radar_capability_stream_V49.cpp"
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V49/radar_capability_stream_V49.h"
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V49/radar_capability_stream.h"												
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V49/radar_capability_stream_hdf_V49.cpp"												
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V49/radar_sw_config.h")	

list(APPEND GPO_GEN8_FLR8_RADAR_CAPABILITY_V53	    "${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V53/radar_capability_stream_V53.cpp"
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V53/radar_capability_stream_V53.h"
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V53/radar_capability_stream.h"												
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V53/radar_capability_stream_hdf_V53.cpp"												
													"${SourceFiles}/GEN8/GPO/FLR8/RADAR_CAPABILITY/V53/radar_sw_config.h")												
#Grouping GPO_GEN8_FLR8 RADAR_CAPABILITY source file
source_group(\\GEN8\\GPO\\FLR8\\RADAR_CAPABILITY\\V49  FILES ${GPO_GEN8_FLR8_RADAR_CAPABILITY_V49})
source_group(\\GEN8\\GPO\\FLR8\\RADAR_CAPABILITY\\V53  FILES ${GPO_GEN8_FLR8_RADAR_CAPABILITY_V53})

#GPO_GEN8_FLR8 RDD Stream Source File
list(APPEND GPO_GEN8_FLR8_RDD_SOURCE 		"${SourceFiles}/GEN8/GPO/FLR8/RDD/RDD_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/RDD/RDD_Logging.h")
					
#Grouping GPO_GEN8_FLR8 RDD Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\RDD FILES ${GPO_GEN8_FLR8_RDD_SOURCE})

#GPO_GEN8_FLR8 RDD source file										
list(APPEND GPO_GEN8_FLR8_RDD_V13	    "${SourceFiles}/GEN8/GPO/FLR8/RDD/V13/rdd_stream_V13.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V13/rdd_stream_V13.h"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V13/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V13/rdd_stream_hdf_V13.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V13/radar_sw_config.h")	

list(APPEND GPO_GEN8_FLR8_RDD_V18	    "${SourceFiles}/GEN8/GPO/FLR8/RDD/V18/rdd_stream_V18.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V18/rdd_stream_V18.h"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V18/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V18/rdd_stream_hdf_V18.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V18/radar_sw_config.h")											
										
list(APPEND GPO_GEN8_FLR8_RDD_V113	    "${SourceFiles}/GEN8/GPO/FLR8/RDD/V113/rdd_stream_V113.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V113/rdd_stream_V113.h"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V113/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V113/rdd_stream_hdf_V113.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V113/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_FLR8_RDD_V117	    "${SourceFiles}/GEN8/GPO/FLR8/RDD/V117/rdd_stream_V117.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V117/rdd_stream_V117.h"
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V117/rdd_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V117/rdd_stream_hdf_V117.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/RDD/V117/radar_sw_config.h")											
										
										
										
												
#Grouping GPO_GEN8_FLR8 RDD source file
source_group(\\GEN8\\GPO\\FLR8\\RDD\\V13   FILES ${GPO_GEN8_FLR8_RDD_V13})
source_group(\\GEN8\\GPO\\FLR8\\RDD\\V18   FILES ${GPO_GEN8_FLR8_RDD_V18})
source_group(\\GEN8\\GPO\\FLR8\\RDD\\V113  FILES ${GPO_GEN8_FLR8_RDD_V113})
source_group(\\GEN8\\GPO\\FLR8\\RDD\\V117  FILES ${GPO_GEN8_FLR8_RDD_V117})

#GPO_GEN8_FLR8 STATUS Stream Source File
list(APPEND GPO_GEN8_FLR8_STATUS_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/STATUS/Status_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/Status_Logging.h")
					
#Grouping GPO_GEN8_FLR8 STATUS Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\STATUS FILES ${GPO_GEN8_FLR8_STATUS_SOURCE})

#GPO_GEN8_FLR8 STATUS source file										
list(APPEND GPO_GEN8_FLR8_STATUS_V4	    "${SourceFiles}/GEN8/GPO/FLR8/STATUS/V4/status_stream_V4.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V4/status_stream_V4.h"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V4/status_stream.h"												
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V4/status_stream_hdf_V4.cpp"												
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V4/radar_sw_config.h")	
											
list(APPEND GPO_GEN8_FLR8_STATUS_V7	    "${SourceFiles}/GEN8/GPO/FLR8/STATUS/V7/status_stream_V7.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V7/status_stream_V7.h"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V7/status_stream.h"												
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V7/status_stream_hdf_V7.cpp"												
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V7/radar_sw_config.h")		

list(APPEND GPO_GEN8_FLR8_STATUS_V8	    "${SourceFiles}/GEN8/GPO/FLR8/STATUS/V8/status_stream_V8.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V8/status_stream_V8.h"
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V8/status_stream.h"												
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V8/status_stream_hdf_V8.cpp"												
											"${SourceFiles}/GEN8/GPO/FLR8/STATUS/V8/radar_sw_config.h")												
												
#Grouping GPO_GEN8_FLR8 STATUS source file
source_group(\\GEN8\\GPO\\FLR8\\STATUS\\V4  FILES ${GPO_GEN8_FLR8_STATUS_V4})
source_group(\\GEN8\\GPO\\FLR8\\STATUS\\V7  FILES ${GPO_GEN8_FLR8_STATUS_V7})
source_group(\\GEN8\\GPO\\FLR8\\STATUS\\V8  FILES ${GPO_GEN8_FLR8_STATUS_V8})
#GPO_GEN8_FLR8 DYNAMIC_ALIGNMENT Stream Source File
list(APPEND GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.cpp"
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/Dynamic_Alignment_Logging.h")
					
#Grouping GPO_GEN8_FLR8 DYNAMIC_ALIGNMENT Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\DYNAMIC_ALIGNMENT FILES ${GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_SOURCE})

#GPO_GEN8_FLR8 DYNAMIC_ALIGNMENT source file										
list(APPEND GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_V13	    "${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream_V13.cpp"
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream_V13.h"
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V13/Dynamic_alignment_stream_hdf_V13.cpp"												
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V13/radar_sw_config.h")	

list(APPEND GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_V16	    "${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream_V16.cpp"
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream_V16.h"
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream.h"												
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V16/Dynamic_alignment_stream_hdf_V16.cpp"												
													"${SourceFiles}/GEN8/GPO/FLR8/DYNAMIC_ALIGNMENT/V16/radar_sw_config.h")														
												
#Grouping GPO_GEN8_FLR8 DYNAMIC_ALIGNMENT source file
source_group(\\GEN8\\GPO\\FLR8\\DYNAMIC_ALIGNMENT\\V13  FILES ${GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_V13})
source_group(\\GEN8\\GPO\\FLR8\\DYNAMIC_ALIGNMENT\\V16  FILES ${GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_V16})
#GPO_GEN8_FLR8 TOI_DS Stream Source File
list(APPEND GPO_GEN8_FLR8_TOI_DS_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/TOI_DS_Logging.cpp"
											"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/TOI_DS_Logging.h")
					
#Grouping GPO_GEN8_FLR8 TOI_DS Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\TOI_DS FILES ${GPO_GEN8_FLR8_TOI_DS_SOURCE})

#GPO_GEN8_FLR8 TOI_DS source file										
list(APPEND GPO_GEN8_FLR8_TOI_DS_V1	    "${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V1/toi_ds_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V1/toi_ds_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V1/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V1/toi_ds_stream_hdf_V1.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V1/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_FLR8_TOI_DS_V2	    "${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V2/toi_ds_stream_V2.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V2/toi_ds_stream_V2.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V2/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V2/toi_ds_stream_hdf_V2.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V2/radar_sw_config.h")			

list(APPEND GPO_GEN8_FLR8_TOI_DS_V4	    "${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V4/toi_ds_stream_V4.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V4/toi_ds_stream_V4.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V4/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V4/toi_ds_stream_hdf_V4.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V4/radar_sw_config.h")	

list(APPEND GPO_GEN8_FLR8_TOI_DS_V105	"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V105/toi_ds_stream_V105.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V105/toi_ds_stream_V105.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V105/toi_ds_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V105/toi_ds_stream_hdf_V105.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI_DS/V105/radar_sw_config.h")											
												
#Grouping GPO_GEN8_FLR8 TOI_DS source file
source_group(\\GEN8\\GPO\\FLR8\\TOI_DS\\V1  FILES ${GPO_GEN8_FLR8_TOI_DS_V1})
source_group(\\GEN8\\GPO\\FLR8\\TOI_DS\\V2  FILES ${GPO_GEN8_FLR8_TOI_DS_V2})
source_group(\\GEN8\\GPO\\FLR8\\TOI_DS\\V4  FILES ${GPO_GEN8_FLR8_TOI_DS_V4})
source_group(\\GEN8\\GPO\\FLR8\\TOI_DS\\V105 FILES ${GPO_GEN8_FLR8_TOI_DS_V105})
#GPO_GEN8_FLR8 TOI Stream Source File
list(APPEND GPO_GEN8_FLR8_TOI_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/TOI/TOI_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/TOI_Logging.h")
					
#Grouping GPO_GEN8_FLR8 TOI Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\TOI FILES ${GPO_GEN8_FLR8_TOI_SOURCE})

#GPO_GEN8_FLR8 TOI source file										
list(APPEND GPO_GEN8_FLR8_TOI_V1	    "${SourceFiles}/GEN8/GPO/FLR8/TOI/V1/toi_stream_V1.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V1/toi_stream_V1.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V1/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V1/toi_stream_hdf_V1.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V1/radar_sw_config.h")	
										
list(APPEND GPO_GEN8_FLR8_TOI_V3	    "${SourceFiles}/GEN8/GPO/FLR8/TOI/V3/toi_stream_V3.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V3/toi_stream_V3.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V3/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V3/toi_stream_hdf_V3.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V3/radar_sw_config.h")

list(APPEND GPO_GEN8_FLR8_TOI_V5	    "${SourceFiles}/GEN8/GPO/FLR8/TOI/V5/toi_stream_V5.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V5/toi_stream_V5.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V5/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V5/toi_stream_hdf_V5.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V5/radar_sw_config.h")

list(APPEND GPO_GEN8_FLR8_TOI_V106	    "${SourceFiles}/GEN8/GPO/FLR8/TOI/V106/toi_stream_V106.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V106/toi_stream_V106.h"
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V106/toi_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V106/toi_stream_hdf_V106.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/TOI/V106/radar_sw_config.h")										
										
												
#Grouping GPO_GEN8_FLR8 TOI source file
source_group(\\GEN8\\GPO\\FLR8\\TOI\\V1  FILES ${GPO_GEN8_FLR8_TOI_V1})
source_group(\\GEN8\\GPO\\FLR8\\TOI\\V3  FILES ${GPO_GEN8_FLR8_TOI_V3})
source_group(\\GEN8\\GPO\\FLR8\\TOI\\V5  FILES ${GPO_GEN8_FLR8_TOI_V5})
source_group(\\GEN8\\GPO\\FLR8\\TOI\\V106  FILES ${GPO_GEN8_FLR8_TOI_V106})
#GPO_GEN8_FLR8 VID Stream Source File
list(APPEND GPO_GEN8_FLR8_VID_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/VID/VID_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/VID/VID_Logging.h")
					
#Grouping GPO_GEN8_FLR8 VID Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\VID FILES ${GPO_GEN8_FLR8_VID_SOURCE})

#GPO_GEN8_FLR8 VID source file										
list(APPEND GPO_GEN8_FLR8_VID_V2	    "${SourceFiles}/GEN8/GPO/FLR8/VID/V2/vid_stream_V2.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/VID/V2/vid_stream_V2.h"
										"${SourceFiles}/GEN8/GPO/FLR8/VID/V2/vid_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/VID/V2/vid_stream_hdf_V2.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/VID/V2/radar_sw_config.h")	
												
#Grouping GPO_GEN8_FLR8 VID source file
source_group(\\GEN8\\GPO\\FLR8\\VID\\V2  FILES ${GPO_GEN8_FLR8_VID_V2})


#GPO_GEN8_FLR8 VSE Stream Source File
list(APPEND GPO_GEN8_FLR8_VSE_SOURCE 	"${SourceFiles}/GEN8/GPO/FLR8/VSE/VSE_Logging.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/VSE_Logging.h")
					
#Grouping GPO_GEN8_FLR8 VSE Stream Source File
source_group(\\GEN8\\GPO\\FLR8\\VSE FILES ${GPO_GEN8_FLR8_VSE_SOURCE})

#GPO_GEN8_FLR8 VSE source file										
list(APPEND GPO_GEN8_FLR8_VSE_V2	    "${SourceFiles}/GEN8/GPO/FLR8/VSE/V2/vse_stream_V2.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V2/vse_stream_V2.h"
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V2/vse_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V2/vse_stream_hdf_V2.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V2/radar_sw_config.h")	
										
									
list(APPEND GPO_GEN8_FLR8_VSE_V3	    "${SourceFiles}/GEN8/GPO/FLR8/VSE/V3/vse_stream_V3.cpp"
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V3/vse_stream_V3.h"
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V3/vse_stream.h"												
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V3/vse_stream_hdf_V3.cpp"												
										"${SourceFiles}/GEN8/GPO/FLR8/VSE/V3/radar_sw_config.h")									
												
#Grouping GPO_GEN8_FLR8 VSE source file
source_group(\\GEN8\\GPO\\FLR8\\VSE\\V2  FILES ${GPO_GEN8_FLR8_VSE_V2})
source_group(\\GEN8\\GPO\\FLR8\\VSE\\V3  FILES ${GPO_GEN8_FLR8_VSE_V3})

set(SOURCE_ALL ${HDF} ${COMMON_SOURCE} 
				${GEN6P_DSPACE} ${GEN6P_V2} ${GEN6P_V3}
				${GEN7_GPO_V1_SRR7_ALIGNMENT_SOURCE} ${GEN7_GPO_V1_SRR7_ALIGNMENT_V9} ${GEN7_GPO_V1_SRR7_ALIGNMENT_V10} ${GEN7_GPO_V1_SRR7_ALIGNMENT_V11}
				${GEN7_GPO_V1_SRR7_BLOCKAGE_SOURCE} ${GEN7_GPO_V1_SRR7_BLOCKAGE_V1}
				${GEN7_GPO_V1_SRR7_CDC_SOURCE} ${GEN7_GPO_V1_SRR7_CDC_V8}
				${GEN7_GPO_V1_SRR7_ID_SOURCE} ${GEN7_GPO_V1_SRR7_ID_V1} ${GEN7_GPO_V1_SRR7_ID_V2} ${GEN7_GPO_V1_SRR7_ID_V3} ${GEN7_GPO_V1_SRR7_ID_V4} ${GEN7_GPO_V1_SRR7_ID_V5}
				${GEN7_GPO_V1_SRR7_DET_SOURCE} ${GEN7_GPO_V1_SRR7_DET_V13} ${GEN7_GPO_V1_SRR7_DET_V14} ${GEN7_GPO_V1_SRR7_DET_V15} ${GEN7_GPO_V1_SRR7_DET_V19} ${GEN7_GPO_V1_SRR7_DET_V21} ${GEN7_GPO_V1_SRR7_DET_V22} ${GEN7_GPO_V1_SRR7_DET_V25} ${GEN7_GPO_V1_SRR7_DET_V100} ${GEN7_GPO_V1_SRR7_DET_V27} ${GEN7_GPO_V1_SRR7_DET_V30} ${GEN7_GPO_V1_SRR7_DET_V32} ${GEN7_GPO_V1_SRR7_DET_V102} ${GEN7_GPO_V1_SRR7_DET_V103} ${GEN7_GPO_V1_SRR7_DET_V104} ${GEN7_GPO_V1_SRR7_DET_V105} ${GEN7_GPO_V1_SRR7_DET_V107}
				${GEN7_GPO_V1_SRR7_HDR_SOURCE} ${GEN7_GPO_V1_SRR7_HDR_V7} ${GEN7_GPO_V1_SRR7_HDR_V8} ${GEN7_GPO_V1_SRR7_HDR_V9}  ${GEN7_GPO_V1_SRR7_HDR_V10} ${GEN7_GPO_V1_SRR7_HDR_V11} ${GEN7_GPO_V1_SRR7_HDR_V14}
				${GEN7_GPO_V1_SRR7_RDD_SOURCE} ${GEN7_GPO_V1_SRR7_RDD_V25} ${GEN7_GPO_V1_SRR7_RDD_V27} ${GEN7_GPO_V1_SRR7_RDD_V30} ${GEN7_GPO_V1_SRR7_RDD_V31} ${GEN7_GPO_V1_SRR7_RDD_V32} ${GEN7_GPO_V1_SRR7_RDD_V33} ${GEN7_GPO_V1_SRR7_RDD_V34}
			    ${GEN7_GPO_V1_SRR7_STATUS_SOURCE} ${GEN7_GPO_V1_SRR7_STATUS_V9} ${GEN7_GPO_V1_SRR7_STATUS_V10} ${GEN7_GPO_V1_SRR7_STATUS_V11}
				${GEN7_GPO_V1_SRR7_DOWN_SELECTION_SOURCE} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V2} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V5} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V7} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V9} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V11} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V14} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V16} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V100} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V102} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V103} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V104} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V105} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V107} ${GEN7_GPO_V1_SRR7_DOWN_SELECTION_V108}
				${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_SOURCE} ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V12} ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V14} ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V15} ${GEN7_GPO_V1_SRR7_DYNAMIC_ALIGNMENT_V16}
				${GEN7_GPO_V1_SRR7_DEBUG_SOURCE} ${GEN7_GPO_V1_SRR7_DEBUG_V5} ${GEN7_GPO_V1_SRR7_DEBUG_V10} ${GEN7_GPO_V1_SRR7_DEBUG_V12} ${GEN7_GPO_V1_SRR7_DEBUG_V13}
				${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_SOURCE} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V29} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V31} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V32} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V33} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V34} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V35} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V39} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V45} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V46} ${GEN7_GPO_V1_SRR7_RADAR_CAPABILITY_V49}
				${GEN7_GPO_V1_SRR7_VSE_SOURCE} ${GEN7_GPO_V1_SRR7_VSE_V3} ${GEN7_GPO_V1_SRR7_VSE_V4} ${GEN7_GPO_V1_SRR7_VSE_V5} ${GEN7_GPO_V1_SRR7_VSE_V6} ${GEN7_GPO_V1_SRR7_VSE_V7}
				${GEN7_GPO_V1_SRR7_MMIC_SOURCE} ${GEN7_GPO_V1_SRR7_MMIC_V25} ${GEN7_GPO_V1_SRR7_MMIC_V26} ${GEN7_GPO_V1_SRR7_MMIC_V28} ${GEN7_GPO_V1_SRR7_MMIC_V30} ${GEN7_GPO_V1_SRR7_MMIC_V31} ${GEN7_GPO_V1_SRR7_MMIC_V32} ${GEN7_GPO_V1_SRR7_MMIC_V34} ${GEN7_GPO_V1_SRR7_MMIC_V36} ${GEN7_GPO_V1_SRR7_MMIC_V37} ${GEN7_GPO_V1_SRR7_MMIC_V40} ${GEN7_GPO_V1_SRR7_MMIC_V42}
				${GEN7_GPO_V1_SRR7_CALIB_SOURCE} ${GEN7_GPO_V1_SRR7_CALIB_V52}
				${GEN7_GPO_V1_SRR7_TOI_SOURCE} ${GEN7_GPO_V1_SRR7_TOI_V2} ${GEN7_GPO_V1_SRR7_TOI_V3} ${GEN7_GPO_V1_SRR7_TOI_V6} ${GEN7_GPO_V1_SRR7_TOI_V9} ${GEN7_GPO_V1_SRR7_TOI_V10} ${GEN7_GPO_V1_SRR7_TOI_V12} ${GEN7_GPO_V1_SRR7_TOI_V13} ${GEN7_GPO_V1_SRR7_TOI_V100} ${GEN7_GPO_V1_SRR7_TOI_V101} ${GEN7_GPO_V1_SRR7_TOI_V103} ${GEN7_GPO_V1_SRR7_TOI_V104} ${GEN7_GPO_V1_SRR7_TOI_V105} ${GEN7_GPO_V1_SRR7_TOI_V106}
				${GEN7_GPO_V1_SRR7_TOI_DS_SOURCE} ${GEN7_GPO_V1_SRR7_TOI_DS_V2} ${GEN7_GPO_V1_SRR7_TOI_DS_V3} ${GEN7_GPO_V1_SRR7_TOI_DS_V101} ${GEN7_GPO_V1_SRR7_TOI_DS_V102} ${GEN7_GPO_V1_SRR7_TOI_DS_V103}  ${GEN7_GPO_V1_SRR7_TOI_DS_V104}  ${GEN7_GPO_V1_SRR7_TOI_DS_V105}
				${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_SOURCE} ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V2} ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V3} ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V4} ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V12} ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V13} ${GEN7_GPO_V1_SRR7_ROT_SAFETY_FAULTS_V113}
				${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_SOURCE} ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V3} ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V4} ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V5} ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V12} ${GEN7_GPO_V1_SRR7_ROT_TRACKER_INFO_V212}
				${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_SOURCE} ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V2} ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V3} ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V4} ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V12} ${GEN7_GPO_V1_SRR7_ROT_VEHICLE_INFO_V113}
				${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_SOURCE} ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V2} ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V3}  ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V4}  ${GEN7_GPO_V1_SRR7_ROT_OBJECT_STREAM_V12} 
				${GEN7_GPO_V1_SRR7_ROT_ISOobject_STREAM_SOURCE} ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V2} ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V3}  ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V4} ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V12} ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V112} ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V113} ${GEN7_GPO_V1_SRR7_ROT_ISOobject_Stream_V213}
				${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE} ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V2} ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V3}  ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V4} ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V12} ${GEN7_GPO_V1_SRR7_ROT_PROCESSEDDETECTION_Stream_V113}
				${GEN7_GPO_V1_FLR7_ALIGNMENT_SOURCE} ${GEN7_GPO_V1_FLR7_ALIGNMENT_V9} ${GEN7_GPO_V1_FLR7_ALIGNMENT_V10} ${GEN7_GPO_V1_FLR7_ALIGNMENT_V11}
				${GEN7_GPO_V1_FLR7_ID_SOURCE} ${GEN7_GPO_V1_FLR7_ID_V1} ${GEN7_GPO_V1_FLR7_ID_V2} ${GEN7_GPO_V1_FLR7_ID_V3} ${GEN7_GPO_V1_FLR7_ID_V4}
				${GEN7_GPO_V1_FLR7_DET_SOURCE} ${GEN7_GPO_V1_FLR7_DET_V13} ${GEN7_GPO_V1_FLR7_DET_V14} ${GEN7_GPO_V1_FLR7_DET_V15} ${GEN7_GPO_V1_FLR7_DET_V19} ${GEN7_GPO_V1_FLR7_DET_V21} ${GEN7_GPO_V1_FLR7_DET_V25} ${GEN7_GPO_V1_FLR7_DET_V30} ${GEN7_GPO_V1_FLR7_DET_V32} ${GEN7_GPO_V1_FLR7_DET_V100}
				${GEN7_GPO_V1_FLR7_HDR_SOURCE} ${GEN7_GPO_V1_FLR7_HDR_V7} ${GEN7_GPO_V1_FLR7_HDR_V8} ${GEN7_GPO_V1_FLR7_HDR_V9} ${GEN7_GPO_V1_FLR7_HDR_V11} ${GEN7_GPO_V1_FLR7_HDR_V14}
				${GEN7_GPO_V1_FLR7_DOWN_SELECTION_SOURCE} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V2} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V5} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V7} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V9} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V14} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V16} ${GEN7_GPO_V1_FLR7_DOWN_SELECTION_V100}
				${GEN7_GPO_V1_FLR7_DEBUG_SOURCE} ${GEN7_GPO_V1_FLR7_DEBUG_V5} ${GEN7_GPO_V1_FLR7_DEBUG_V10} ${GEN7_GPO_V1_FLR7_DEBUG_V12} ${GEN7_GPO_V1_FLR7_DEBUG_V13}
				${GEN7_GPO_V1_FLR7_MMIC_SOURCE} ${GEN7_GPO_V1_FLR7_MMIC_V25} ${GEN7_GPO_V1_FLR7_MMIC_V26} ${GEN7_GPO_V1_FLR7_MMIC_V28} ${GEN7_GPO_V1_FLR7_MMIC_V30} ${GEN7_GPO_V1_FLR7_MMIC_V31} ${GEN7_GPO_V1_FLR7_MMIC_V40} ${GEN7_GPO_V1_FLR7_MMIC_V42}
				${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_SOURCE} ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V29} ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V31} ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V32} ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V33} ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V39} ${GEN7_GPO_V1_FLR7_RADAR_CAPABILITY_V45}
				${GEN7_GPO_V1_FLR7_RDD_SOURCE} ${GEN7_GPO_V1_FLR7_RDD_V25} ${GEN7_GPO_V1_FLR7_RDD_V27} ${GEN7_GPO_V1_FLR7_RDD_V30} ${GEN7_GPO_V1_FLR7_RDD_V32} ${GEN7_GPO_V1_FLR7_RDD_V33}
				${GEN7_GPO_V1_FLR7_ROT_ISOobject_STREAM_SOURCE} ${GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V2} ${GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V3} ${GEN7_GPO_V1_FLR7_ROT_ISOobject_Stream_V12} 
				${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_SOURCE} ${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V2} ${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V3} ${GEN7_GPO_V1_FLR7_ROT_OBJECT_STREAM_V12}
				${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE} ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V2} ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V3} ${GEN7_GPO_V1_FLR7_ROT_PROCESSEDDETECTION_Stream_V12}
				${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_SOURCE} ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V2} ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V4} ${GEN7_GPO_V1_FLR7_ROT_SAFETY_FAULTS_V12}
				${GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_SOURCE} ${GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_V12} ${GEN7_GPO_V1_FLR7_DYNAMIC_ALIGNMENT_V14}
				${GEN7_GPO_V1_FLR7_STATUS_SOURCE} ${GEN7_GPO_V1_FLR7_STATUS_V9} ${GEN7_GPO_V1_FLR7_STATUS_V10} ${GEN7_GPO_V1_FLR7_STATUS_V11}
				${GEN7_GPO_V1_FLR7_VSE_SOURCE} ${GEN7_GPO_V1_FLR7_VSE_V3} ${GEN7_GPO_V1_FLR7_VSE_V4} ${GEN7_GPO_V1_FLR7_VSE_V6} ${GEN7_GPO_V1_FLR7_VSE_V7}
				${GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_SOURCE} ${GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_V3} ${GEN7_GPO_V1_FLR7_ROT_TRACKER_INFO_V12} 
				${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_SOURCE} ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V2} ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V4} ${GEN7_GPO_V1_FLR7_ROT_VEHICLE_INFO_V12}
				${GEN7_GPO_V1_FLR7_TOI_SOURCE} ${GEN7_GPO_V1_FLR7_TOI_V2} ${GEN7_GPO_V1_FLR7_TOI_V3} ${GEN7_GPO_V1_FLR7_TOI_V9} ${GEN7_GPO_V1_FLR7_TOI_V12} ${GEN7_GPO_V1_FLR7_TOI_V13} ${GEN7_GPO_V1_FLR7_TOI_V100}
				${GEN7_GPO_V1_FLR7_TOI_DS_SOURCE} ${GEN7_GPO_V1_FLR7_TOI_DS_V2} ${GEN7_GPO_V1_FLR7_TOI_DS_V3}
				${GEN7_GPO_V2_SRR7_ALIGNMENT_SOURCE} ${GEN7_GPO_V2_SRR7_ALIGNMENT_V10} ${GEN7_GPO_V2_SRR7_ALIGNMENT_V11}
				${GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_SOURCE} ${GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_V1} ${GEN7_GPO_V2_SRR7_ALIGNMENT_INTERNALS_V2}
				${GEN7_GPO_V2_SRR7_ID_SOURCE} ${GEN7_GPO_V2_SRR7_ID_V3} ${GEN7_GPO_V2_SRR7_ID_V3} ${GEN7_GPO_V2_SRR7_ID_V5}
				${GEN7_GPO_V2_SRR7_RDD_SOURCE} ${GEN7_GPO_V2_SRR7_RDD_V2} ${GEN7_GPO_V2_SRR7_RDD_V5} ${GEN7_GPO_V2_SRR7_RDD_V8} ${GEN7_GPO_V2_SRR7_RDD_V12} ${GEN7_GPO_V2_SRR7_RDD_V13} ${GEN7_GPO_V2_SRR7_RDD_V14} ${GEN7_GPO_V2_SRR7_RDD_V16} ${GEN7_GPO_V2_SRR7_RDD_V17} ${GEN7_GPO_V2_SRR7_RDD_V18}
				${GEN7_GPO_V2_SRR7_RDD_DEBUG_SOURCE} ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V1} ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V2} ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V3} ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V7} ${GEN7_GPO_V2_SRR7_RDD_DEBUG_V9}
				${GEN7_GPO_V2_SRR7_STATUS_SOURCE} ${GEN7_GPO_V2_SRR7_STATUS_V1} ${GEN7_GPO_V2_SRR7_STATUS_V2} ${GEN7_GPO_V2_SRR7_STATUS_V3} ${GEN7_GPO_V2_SRR7_STATUS_V4}
				${GEN7_GPO_V2_SRR7_VSE_SOURCE} ${GEN7_GPO_V2_SRR7_VSE_V2} ${GEN7_GPO_V2_SRR7_VSE_V3} ${GEN7_GPO_V2_SRR7_VSE_V4} ${GEN7_GPO_V2_SRR7_VSE_V5} ${GEN7_GPO_V2_SRR7_VSE_V6}
				${GEN7_GPO_V2_SRR7_DEBUG_SOURCE} ${GEN7_GPO_V2_SRR7_DEBUG_V2} ${GEN7_GPO_V2_SRR7_DEBUG_V3} ${GEN7_GPO_V2_SRR7_DEBUG_V5} ${GEN7_GPO_V2_SRR7_DEBUG_V8} ${GEN7_GPO_V2_SRR7_DEBUG_V9} ${GEN7_GPO_V2_SRR7_DEBUG_V11} ${GEN7_GPO_V2_SRR7_DEBUG_V20} ${GEN7_GPO_V2_SRR7_DEBUG_V21} 
				${GEN7_GPO_V2_SRR7_DOWN_SELECTION_SOURCE} ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V8} ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V9} ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V11} ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V12}  ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V13} ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V14} ${GEN7_GPO_V2_SRR7_DOWN_SELECTION_V15}
				${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_SOURCE} ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V14} ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V12} ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V15} ${GEN7_GPO_V2_SRR7_DYNAMIC_ALIGNMENT_V16}
				${GEN7_GPO_V2_SRR7_CDC_SOURCE} ${GEN7_GPO_V2_SRR7_CDC_V9} ${GEN7_GPO_V2_SRR7_CDC_V10} ${GEN7_GPO_V2_SRR7_CDC_V11} ${GEN7_GPO_V2_SRR7_CDC_V12}
				${GEN7_GPO_V2_SRR7_DET_SOURCE} ${GEN7_GPO_V2_SRR7_DET_V10} ${GEN7_GPO_V2_SRR7_DET_V12} ${GEN7_GPO_V2_SRR7_DET_V15} ${GEN7_GPO_V2_SRR7_DET_V16} ${GEN7_GPO_V2_SRR7_DET_V17} ${GEN7_GPO_V2_SRR7_DET_V19} ${GEN7_GPO_V2_SRR7_DET_V21}
				${GEN7_GPO_V2_SRR7_DET_DEBUG_SOURCE} ${GEN7_GPO_V2_SRR7_DET_DEBUG_V1} ${GEN7_GPO_V2_SRR7_DET_DEBUG_V2} ${GEN7_GPO_V2_SRR7_DET_DEBUG_V3}
				${GEN7_GPO_V2_SRR7_HDR_SOURCE} ${GEN7_GPO_V2_SRR7_HDR_V2} ${GEN7_GPO_V2_SRR7_HDR_V4} ${GEN7_GPO_V2_SRR7_HDR_V5} ${GEN7_GPO_V2_SRR7_HDR_V7} ${GEN7_GPO_V2_SRR7_HDR_V9} ${GEN7_GPO_V2_SRR7_HDR_V10} ${GEN7_GPO_V2_SRR7_HDR_V11}
				${GEN7_GPO_V2_SRR7_MMIC_SOURCE} ${GEN7_GPO_V2_SRR7_MMIC_V4} ${GEN7_GPO_V2_SRR7_MMIC_V7} ${GEN7_GPO_V2_SRR7_MMIC_V8} ${GEN7_GPO_V2_SRR7_MMIC_V9} ${GEN7_GPO_V2_SRR7_MMIC_V12} ${GEN7_GPO_V2_SRR7_MMIC_V13} ${GEN7_GPO_V2_SRR7_MMIC_V16} ${GEN7_GPO_V2_SRR7_MMIC_V17}
				${GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_SOURCE} ${GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_V3} ${GEN7_GPO_V2_SRR7_ROT_TRACKER_INFO_V4}
				${GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_SOURCE} ${GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_V5} ${GEN7_GPO_V2_SRR7_ROT_ISO_OBJECT_V7}
				${GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_SOURCE} ${GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_V5} ${GEN7_GPO_V2_SRR7_ROT_VEHICLE_INFO_V6}
				${GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE} ${GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_Stream_V6} ${GEN7_GPO_V2_SRR7_ROT_PROCESSEDDETECTION_Stream_V7}
				${GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_SOURCE} ${GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_V6} ${GEN7_GPO_V2_SRR7_ROT_SAE_OBJECT_STREAM_V7}
				${GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_SOURCE} ${GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_V5} ${GEN7_GPO_V2_SRR7_ROT_SAFETY_FAULTS_V6}
				${GEN7_GPO_V2_SRR7_TOI_SOURCE} ${GEN7_GPO_V2_SRR7_TOI_V1} ${GEN7_GPO_V2_SRR7_TOI_V2} ${GEN7_GPO_V2_SRR7_TOI_V3} ${GEN7_GPO_V2_SRR7_TOI_V4} ${GEN7_GPO_V2_SRR7_TOI_V8} ${GEN7_GPO_V2_SRR7_TOI_V10} ${GEN7_GPO_V2_SRR7_TOI_V12} ${GEN7_GPO_V2_SRR7_TOI_V13}
				${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_SOURCE} ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V35} ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V39} ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V42} ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V47} ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V48} ${GEN7_GPO_V2_SRR7_RADAR_CAPABILITY_V53}
				${GEN7_GPO_V2_FLR7_ALIGNMENT_SOURCE} ${GEN7_GPO_V2_FLR7_ALIGNMENT_V10} ${GEN7_GPO_V2_FLR7_ALIGNMENT_V11}
				${GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_SOURCE} ${GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_V1} ${GEN7_GPO_V2_FLR7_ALIGNMENT_INTERNALS_V2}
				${GEN7_GPO_V2_FLR7_ID_SOURCE} ${GEN7_GPO_V2_FLR7_ID_V3} ${GEN7_GPO_V2_FLR7_ID_V5}
				${GEN7_GPO_V2_FLR7_RDD_SOURCE} ${GEN7_GPO_V2_FLR7_RDD_V2} ${GEN7_GPO_V2_FLR7_RDD_V5} ${GEN7_GPO_V2_FLR7_RDD_V8} ${GEN7_GPO_V2_FLR7_RDD_V12} ${GEN7_GPO_V2_FLR7_RDD_V13} ${GEN7_GPO_V2_FLR7_RDD_V14} ${GEN7_GPO_V2_FLR7_RDD_V16} ${GEN7_GPO_V2_FLR7_RDD_V17} ${GEN7_GPO_V2_FLR7_RDD_V18}
				${GEN7_GPO_V2_FLR7_RDD_DEBUG_SOURCE} ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V1} ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V2} ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V3} ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V7} ${GEN7_GPO_V2_FLR7_RDD_DEBUG_V9}
				${GEN7_GPO_V2_FLR7_STATUS_SOURCE} ${GEN7_GPO_V2_FLR7_STATUS_V1} ${GEN7_GPO_V2_FLR7_STATUS_V2} ${GEN7_GPO_V2_FLR7_STATUS_V3} ${GEN7_GPO_V2_FLR7_STATUS_V4}
				${GEN7_GPO_V2_FLR7_VSE_SOURCE} ${GEN7_GPO_V2_FLR7_VSE_V2} ${GEN7_GPO_V2_FLR7_VSE_V3} ${GEN7_GPO_V2_FLR7_VSE_V4} ${GEN7_GPO_V2_FLR7_VSE_V5} ${GEN7_GPO_V2_FLR7_VSE_V6}
				${GEN7_GPO_V2_FLR7_DEBUG_SOURCE} ${GEN7_GPO_V2_FLR7_DEBUG_V2} ${GEN7_GPO_V2_FLR7_DEBUG_V3} ${GEN7_GPO_V2_FLR7_DEBUG_V5} ${GEN7_GPO_V2_FLR7_DEBUG_V8} ${GEN7_GPO_V2_FLR7_DEBUG_V9} ${GEN7_GPO_V2_FLR7_DEBUG_V11} ${GEN7_GPO_V2_FLR7_DEBUG_V20} ${GEN7_GPO_V2_FLR7_DEBUG_V21}
				${GEN7_GPO_V2_FLR7_DOWN_SELECTION_SOURCE} ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V8} ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V9} ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V11} ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V12}  ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V13} ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V14} ${GEN7_GPO_V2_FLR7_DOWN_SELECTION_V15} 
				${GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_SOURCE} ${GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_V3} ${GEN7_GPO_V2_FLR7_ROT_TRACKER_INFO_V4}
				${GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_SOURCE} ${GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_V5} ${GEN7_GPO_V2_FLR7_ROT_ISO_OBJECT_V7}
				${GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_SOURCE} ${GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_V5} ${GEN7_GPO_V2_FLR7_ROT_VEHICLE_INFO_V6}
				${GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_STREAM_SOURCE} ${GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_Stream_V6} ${GEN7_GPO_V2_FLR7_ROT_PROCESSEDDETECTION_Stream_V7}
				${GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_SOURCE} ${GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_V6} ${GEN7_GPO_V2_FLR7_ROT_SAE_OBJECT_STREAM_V7}
				${GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_SOURCE} ${GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_V5} ${GEN7_GPO_V2_FLR7_ROT_SAFETY_FAULTS_V6}
				${GEN7_GPO_V2_FLR7_DET_SOURCE} ${GEN7_GPO_V2_FLR7_DET_V10} ${GEN7_GPO_V2_FLR7_DET_V12} ${GEN7_GPO_V2_FLR7_DET_V15} ${GEN7_GPO_V2_FLR7_DET_V16} ${GEN7_GPO_V2_FLR7_DET_V17} ${GEN7_GPO_V2_FLR7_DET_V19} ${GEN7_GPO_V2_FLR7_DET_V21}
				${GEN7_GPO_V2_FLR7_DET_DEBUG_SOURCE} ${GEN7_GPO_V2_FLR7_DET_DEBUG_V1} ${GEN7_GPO_V2_FLR7_DET_DEBUG_V2} ${GEN7_GPO_V2_FLR7_DET_DEBUG_V3}
				${GEN7_GPO_V2_FLR7_HDR_SOURCE} ${GEN7_GPO_V2_FLR7_HDR_V2} ${GEN7_GPO_V2_FLR7_HDR_V4} ${GEN7_GPO_V2_FLR7_HDR_V5} ${GEN7_GPO_V2_FLR7_HDR_V7} ${GEN7_GPO_V2_FLR7_HDR_V9} ${GEN7_GPO_V2_FLR7_HDR_V10} ${GEN7_GPO_V2_FLR7_HDR_V11}
				${GEN7_GPO_V2_FLR7_MMIC_SOURCE} ${GEN7_GPO_V2_FLR7_MMIC_V4} ${GEN7_GPO_V2_FLR7_MMIC_V7} ${GEN7_GPO_V2_FLR7_MMIC_V8} ${GEN7_GPO_V2_FLR7_MMIC_V9} ${GEN7_GPO_V2_FLR7_MMIC_V12} ${GEN7_GPO_V2_FLR7_MMIC_V13} ${GEN7_GPO_V2_FLR7_MMIC_V16} ${GEN7_GPO_V2_FLR7_MMIC_V17}
				${GEN7_GPO_V2_FLR7_TOI_SOURCE} ${GEN7_GPO_V2_FLR7_TOI_V1} ${GEN7_GPO_V2_FLR7_TOI_V2} ${GEN7_GPO_V2_FLR7_TOI_V3} ${GEN7_GPO_V2_FLR7_TOI_V4} ${GEN7_GPO_V2_FLR7_TOI_V8} ${GEN7_GPO_V2_FLR7_TOI_V10} ${GEN7_GPO_V2_FLR7_TOI_V12} ${GEN7_GPO_V2_FLR7_TOI_V13}
				${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_SOURCE} ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V35} ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V39} ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V42} ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V47}	${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V48} ${GEN7_GPO_V2_FLR7_RADAR_CAPABILITY_V53}		 	
				${GEN7_RNA_SRR7_VSE_SOURCE}	${GEN7_RNA_SRR7_VSE_V4} ${GEN7_RNA_SRR7_VSE_V5} ${GEN7_RNA_SRR7_VSE_V6}
			    ${GEN7_RNA_SRR7_RADAR_CAPABILITY_SOURCE} ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V32} ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V34} ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V42} ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V46} ${GEN7_RNA_SRR7_RADAR_CAPABILITY_V49}
				${GEN7_RNA_SRR7_DOWN_SELECTION_SOURCE} ${GEN7_RNA_SRR7_DOWN_SELECTION_V7} ${GEN7_RNA_SRR7_DOWN_SELECTION_V100} ${GEN7_RNA_SRR7_DOWN_SELECTION_V105} ${GEN7_RNA_SRR7_DOWN_SELECTION_V107} ${GEN7_RNA_SRR7_DOWN_SELECTION_V108} ${GEN7_RNA_SRR7_DOWN_SELECTION_V109}
				${GEN7_RNA_SRR7_ROT_OBJECT_SOURCE} ${GEN7_RNA_SRR7_ROT_OBJECT_V3} ${GEN7_RNA_SRR7_ROT_OBJECT_V5} ${GEN7_RNA_SRR7_ROT_OBJECT_V6}
                ${GEN7_RNA_SRR7_ROT_ISOobject_SOURCE} ${GEN7_RNA_SRR7_ROT_ISOobject_V3} ${GEN7_RNA_SRR7_ROT_ISOobject_V7} ${GEN7_RNA_SRR7_ROT_ISOobject_V8} ${GEN7_RNA_SRR7_ROT_ISOobject_V13} ${GEN7_RNA_SRR7_ROT_ISOobject_V108} ${GEN7_RNA_SRR7_ROT_ISOobject_V112} ${GEN7_RNA_SRR7_ROT_ISOobject_V113} ${GEN7_RNA_SRR7_ROT_ISOobject_V213} 
				${GEN7_RNA_SRR7_ALIGNMENT_SOURCE} ${GEN7_RNA_SRR7_ALIGNMENT_V9} ${GEN7_RNA_SRR7_ALIGNMENT_V10} ${GEN7_RNA_SRR7_ALIGNMENT_V11}
				${GEN7_RNA_SRR7_HDR_SOURCE} ${GEN7_RNA_SRR7_HDR_V9} ${GEN7_RNA_SRR7_HDR_V11} ${GEN7_RNA_SRR7_HDR_V14}
				${GEN7_RNA_SRR7_DEBUG_SOURCE} ${GEN7_RNA_SRR7_DEBUG_V10} ${GEN7_RNA_SRR7_DEBUG_V12} 
				${GEN7_RNA_SRR7_RDD_SOURCE} ${GEN7_RNA_SRR7_RDD_V30} ${GEN7_RNA_SRR7_RDD_V32} ${GEN7_RNA_SRR7_RDD_V34}
				${GEN7_RNA_SRR7_STATUS_SOURCE} ${GEN7_RNA_SRR7_STATUS_V10}
				${GEN7_RNA_SRR7_CALIB_SOURCE} ${GEN7_RNA_SRR7_CALIB_V52}
				${GEN7_RNA_SRR7_CDC_SOURCE} ${GEN7_RNA_SRR7_CDC_V7} ${GEN7_RNA_SRR7_CDC_V8}
				${GEN7_RNA_SRR7_DET_SOURCE} ${GEN7_RNA_SRR7_DET_V22} ${GEN7_RNA_SRR7_DET_V100} ${GEN7_RNA_SRR7_DET_V105} ${GEN7_RNA_SRR7_DET_V107}
				${GEN7_RNA_SRR7_MMIC_SOURCE} ${GEN7_RNA_SRR7_MMIC_V30} ${GEN7_RNA_SRR7_MMIC_V34} ${GEN7_RNA_SRR7_MMIC_V40}
				${GEN7_RNA_SRR7_OLP_OBJECT_SOURCE} ${GEN7_RNA_SRR7_OLP_OBJECT_V1}
				${GEN7_RNA_SRR7_PMO_OBJECT_SOURCE} ${GEN7_RNA_SRR7_PMO_OBJECT_V1}
				${GEN7_RNA_SRR7_DRA_INTERNALS_SOURCE} ${GEN7_RNA_SRR7_DRA_INTERNALS_V1}
				${GEN7_RNA_SRR7_ROT_TRACKER_INFO_SOURCE} ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V3} ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V4} ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V8} ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V9} ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V12} ${GEN7_RNA_SRR7_ROT_TRACKER_INFO_V212}
				${GEN7_RNA_SRR7_ROT_PROCESSEDDETECTION_SOURCE} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V3} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V103} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V5} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V6} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V13} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V112} ${GEN7_RNA_SRR7_ROT_PROCESSED_DETECTION_V113}
				${GEN7_RNA_SRR7_VID_SOURCE} ${GEN7_RNA_SRR7_VID_V2}
				${GEN7_RNA_SRR7_LCDA_CUST_OUTPUT_SOURCE} ${GEN7_RNA_SRR7_LCDA_CUST_OUTPUT_V2} 
				${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_SOURCE} ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V2} ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V4} ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V5} ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V13} ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V105} ${GEN7_RNA_SRR7_ROT_VEHICLE_INFO_V113} 
				${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_SOURCE} ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V2} ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V4} ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V5} ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V13} ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V105} ${GEN7_RNA_SRR7_ROT_SAFETY_FAULTS_V113}
				${GEN7_RNA_SRR7_ID_SOURCE} ${GEN7_RNA_SRR7_ID_V3} ${GEN7_RNA_SRR7_ID_V5}
				${GEN7_RNA_SRR7_TOI_SOURCE} ${GEN7_RNA_SRR7_TOI_V6} ${GEN7_RNA_SRR7_TOI_V100} ${GEN7_RNA_SRR7_TOI_V103} ${GEN7_RNA_SRR7_TOI_V105} ${GEN7_RNA_SRR7_TOI_V106}
				${GEN7_RNA_SRR7_TOI_DS_SOURCE} ${GEN7_RNA_SRR7_TOI_DS_V101} ${GEN7_RNA_SRR7_TOI_DS_V102} ${GEN7_RNA_SRR7_TOI_DS_V103} ${GEN7_RNA_SRR7_TOI_DS_V104} ${GEN7_RNA_SRR7_TOI_DS_V105} ${GEN7_RNA_SRR7_TOI_DS_V106} 
				${GEN7_RNA_SRR7_CED_CUST_OUTPUT_SOURCE} ${GEN7_RNA_SRR7_CED_CUST_OUTPUT_V2}
				${GEN7_RNA_SRR7_CTA_CUST_OUTPUT_SOURCE} ${GEN7_RNA_SRR7_CTA_CUST_OUTPUT_V2}
				${GEN7_RNA_SRR7_CUST_ALGO_SOURCE} ${GEN7_RNA_SRR7_CUST_ALGO_V2} ${GEN7_RNA_SRR7_CUST_ALGO_V3} ${GEN7_RNA_SRR7_CUST_ALGO_V5}
				${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_SOURCE} ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V11} ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V12} ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V16} ${GEN7_RNA_SRR7_DYNAMIC_ALIGNMENT_V45}
				${GEN7_RNA_SRR7_ROT_INTERNALS_SOURCE} ${GEN7_RNA_SRR7_ROT_INTERNALS_V2} ${GEN7_RNA_SRR7_ROT_INTERNALS_V5} ${GEN7_RNA_SRR7_ROT_INTERNALS_V6} ${GEN7_RNA_SRR7_ROT_INTERNALS_V106} ${GEN7_RNA_SRR7_ROT_INTERNALS_V113} ${GEN7_RNA_SRR7_ROT_INTERNALS_V114}
				${GPO_GEN8_SRR8_ALIGNMENT_SOURCE} ${GPO_GEN8_SRR8_ALIGNMENT_V10}
				${GPO_GEN8_SRR8_DETECTION_SOURCE} ${GPO_GEN8_SRR8_DETECTION_V5} ${GPO_GEN8_SRR8_DETECTION_V115}
				${GPO_GEN8_SRR8_DOWN_SELECTION_SOURCE} ${GPO_GEN8_SRR8_DOWN_SELECTION_V1} ${GPO_GEN8_SRR8_DOWN_SELECTION_V101}
				${GPO_GEN8_SRR8_HEADER_SOURCE} ${GPO_GEN8_SRR8_HEADER_V7} ${GPO_GEN8_SRR8_HEADER_V8} ${GPO_GEN8_SRR8_HEADER_V9}
				${GPO_GEN8_SRR8_DETECTION_SOURCE} ${GPO_GEN8_SRR8_DETECTION_V5} ${GPO_GEN8_SRR8_DETECTION_V115} ${GPO_GEN8_SRR8_DETECTION_V200}
				${GPO_GEN8_SRR8_DOWN_SELECTION_SOURCE} ${GPO_GEN8_SRR8_DOWN_SELECTION_V1} ${GPO_GEN8_SRR8_DOWN_SELECTION_V101} ${GPO_GEN8_SRR8_DOWN_SELECTION_V200}
				${GPO_GEN8_SRR8_HEADER_SOURCE} ${GPO_GEN8_SRR8_HEADER_V7} ${GPO_GEN8_SRR8_HEADER_V8} ${GPO_GEN8_SRR8_HEADER_V10}
				${GPO_GEN8_SRR8_MMIC_SOURCE} ${GPO_GEN8_SRR8_MMIC_V7} ${GPO_GEN8_SRR8_MMIC_V20}
				${GPO_GEN8_SRR8_RADAR_CAPABILITY_SOURCE} ${GPO_GEN8_SRR8_RADAR_CAPABILITY_V49} ${GPO_GEN8_SRR8_RADAR_CAPABILITY_V53}
				${GPO_GEN8_SRR8_RDD_SOURCE} ${GPO_GEN8_SRR8_RDD_V13} ${GPO_GEN8_SRR8_RDD_V113} ${GPO_GEN8_SRR8_RDD_V18} ${GPO_GEN8_SRR8_RDD_V117}
				${GPO_GEN8_SRR8_STATUS_SOURCE} ${GPO_GEN8_SRR8_STATUS_V4} ${GPO_GEN8_SRR8_STATUS_V7} ${GPO_GEN8_SRR8_STATUS_V8} 
				${GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_SOURCE} ${GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_V13} ${GPO_GEN8_SRR8_DYNAMIC_ALIGNMENT_V16}
				${GPO_GEN8_SRR8_TOI_DS_SOURCE} ${GPO_GEN8_SRR8_TOI_DS_V1} ${GPO_GEN8_SRR8_TOI_DS_V2} ${GPO_GEN8_SRR8_TOI_DS_V4} ${GPO_GEN8_SRR8_TOI_DS_V105}
				${GPO_GEN8_SRR8_TOI_SOURCE} ${GPO_GEN8_SRR8_TOI_V1} ${GPO_GEN8_SRR8_TOI_V3} ${GPO_GEN8_SRR8_TOI_V5} ${GPO_GEN8_SRR8_TOI_V106}
				${GPO_GEN8_SRR8_VID_SOURCE} ${GPO_GEN8_SRR8_VID_V2}
				${GPO_GEN8_SRR8_VSE_SOURCE} ${GPO_GEN8_SRR8_VSE_V2}
				${GPO_GEN8_SRR8_SYNC_INFO_LOG_SOURCE} ${GPO_GEN8_SRR8_SYNC_INFO_LOG_V2}
				${GPO_GEN8_SRR8_BSIS_SOURCE} ${GPO_GEN8_SRR8_BSIS_V1}
				${GPO_GEN8_SRR8_MOIS_SOURCE} ${GPO_GEN8_SRR8_MOIS_V1}
				${GPO_GEN8_SRR8_OLP_SOURCE} ${GPO_GEN8_SRR8_OLP_V1}
				${GPO_GEN8_SRR8_F360_DETECTION_LOG_SOURCE} ${GPO_GEN8_SRR8_F360_DETECTION_LOG_V3}
				${GPO_GEN8_SRR8_F360_HOST_PROPS_LOG_SOURCE} ${GPO_GEN8_SRR8_F360_HOST_PROPS_LOG_V2}
				${GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_SOURCE} ${GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_V4} ${GPO_GEN8_SRR8_F360_INTERNAL_CLUSTER_V5}
				${GPO_GEN8_SRR8_F360_INTERNAL_CWD_SOURCE} ${GPO_GEN8_SRR8_F360_INTERNAL_CWD_V3}
				${GPO_GEN8_SRR8_F360_INTERNAL_DETECTION_HISTORY_SOURCE} ${GPO_GEN8_SRR8_F360_INTERNAL_DETECTION_HISTORY_V4}
				${GPO_GEN8_SRR8_F360_INTERNAL_OBJECT_SOURCE} ${GPO_GEN8_SRR8_F360_INTERNAL_OBJECT_V35}
				${GPO_GEN8_SRR8_F360_INTERNAL_REFLECTION_BUFFER_SOURCE} ${GPO_GEN8_SRR8_F360_INTERNAL_REFLECTION_BUFFER_V1}
				${GPO_GEN8_SRR8_F360_OBJECTS_LOG_SOURCE} ${GPO_GEN8_SRR8_F360_OBJECTS_LOG_V10}
				${GPO_GEN8_SRR8_F360_FUNCTIONAL_SAFETY_FAULTS_SOURCE} ${GPO_GEN8_SRR8_F360_FUNCTIONAL_SAFETY_FAULTS_V12}
				${GPO_GEN8_SRR8_F360_SENSOR_CALIB_LOG_SOURCE} ${GPO_GEN8_SRR8_F360_SENSOR_CALIB_LOG_V4}
				${GPO_GEN8_SRR8_HOST_CALIBS_LOG_SOURCE} ${GPO_GEN8_SRR8_HOST_CALIBS_LOG_V3}
				${GPO_GEN8_SRR8_STATIC_ENV_POLYS_LOG_SOURCE} ${GPO_GEN8_SRR8_STATIC_ENV_POLYS_LOG_V3}
				${GPO_GEN8_SRR8_TIMING_INFO_LOG_SOURCE} ${GPO_GEN8_SRR8_TIMING_INFO_LOG_V22}
				${GPO_GEN8_SRR8_TRACKER_INFO_LOG_SOURCE} ${GPO_GEN8_SRR8_TRACKER_INFO_LOG_V9}
				${GPO_GEN8_SRR8_TRAILER_DETECTOR_INTERNAL_LOG_SOURCE} ${GPO_GEN8_SRR8_TRAILER_DETECTOR_INTERNAL_LOG_V3}
				${GPO_GEN8_SRR8_TRAILER_DETECTOR_LOG_SOURCE} ${GPO_GEN8_SRR8_TRAILER_DETECTOR_LOG_V2}
				${GPO_GEN8_SRR8_VEHICLE_INFO_LOG_SOURCE} ${GPO_GEN8_SRR8_VEHICLE_INFO_LOG_V9}
				${GPO_GEN8_SRR8_VSE_SOURCE} ${GPO_GEN8_SRR8_VSE_V2} ${GPO_GEN8_SRR8_VSE_V3}
				${GPO_GEN8_SRR8_ROT_ISO_OBJECT_SOURCE} ${GPO_GEN8_SRR8_ROT_ISO_OBJECT_V3}
				${GPO_GEN8_SRR8_ROT_PROCESSED_DETECTION_SOURCE} ${GPO_GEN8_SRR8_ROT_PROCESSED_DETECTION_V3}
				${GPO_GEN8_SRR8_ROT_SAFETY_FAULTS_SOURCE} ${GPO_GEN8_SRR8_ROT_SAFETY_FAULTS_V3}
				${GPO_GEN8_SRR8_ROT_TRACKER_INFO_SOURCE} ${GPO_GEN8_SRR8_ROT_TRACKER_INFO_V2}
				${GPO_GEN8_SRR8_ROT_VEHICLE_INFO_SOURCE} ${GPO_GEN8_SRR8_ROT_VEHICLE_INFO_V3}
				${GPO_GEN8_FLR8_ALIGNMENT_SOURCE} ${GPO_GEN8_FLR8_ALIGNMENT_V10}
				${GPO_GEN8_FLR8_DETECTION_SOURCE} ${GPO_GEN8_FLR8_DETECTION_V5} ${GPO_GEN8_FLR8_DETECTION_V115} ${GPO_GEN8_FLR8_DETECTION_V200}
				${GPO_GEN8_FLR8_DOWN_SELECTION_SOURCE} ${GPO_GEN8_FLR8_DOWN_SELECTION_V1} ${GPO_GEN8_FLR8_DOWN_SELECTION_V101} ${GPO_GEN8_FLR8_DOWN_SELECTION_V200}
				${GPO_GEN8_FLR8_HEADER_SOURCE} ${GPO_GEN8_FLR8_HEADER_V7} ${GPO_GEN8_FLR8_HEADER_V8} ${GPO_GEN8_FLR8_HEADER_V10}
				${GPO_GEN8_FLR8_MMIC_SOURCE} ${GPO_GEN8_FLR8_MMIC_V7} ${GPO_GEN8_FLR8_MMIC_V20}
				${GPO_GEN8_FLR8_RADAR_CAPABILITY_SOURCE} ${GPO_GEN8_FLR8_RADAR_CAPABILITY_V49} ${GPO_GEN8_FLR8_RADAR_CAPABILITY_V53}
				${GPO_GEN8_FLR8_RDD_SOURCE} ${GPO_GEN8_FLR8_RDD_V13} ${GPO_GEN8_FLR8_RDD_V18} ${GPO_GEN8_FLR8_RDD_V113} ${GPO_GEN8_FLR8_RDD_V117}
				${GPO_GEN8_FLR8_STATUS_SOURCE} ${GPO_GEN8_FLR8_STATUS_V4} ${GPO_GEN8_FLR8_STATUS_V7} ${GPO_GEN8_FLR8_STATUS_V8}
				${GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_SOURCE} ${GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_V13} ${GPO_GEN8_FLR8_DYNAMIC_ALIGNMENT_V16}
				${GPO_GEN8_FLR8_TOI_DS_SOURCE} ${GPO_GEN8_FLR8_TOI_DS_V1} ${GPO_GEN8_FLR8_TOI_DS_V2} ${GPO_GEN8_FLR8_TOI_DS_V4} ${GPO_GEN8_FLR8_TOI_DS_V105}
				${GPO_GEN8_FLR8_TOI_SOURCE} ${GPO_GEN8_FLR8_TOI_V1} ${GPO_GEN8_FLR8_TOI_V3} ${GPO_GEN8_FLR8_TOI_V5} ${GPO_GEN8_FLR8_TOI_V106}
				${GPO_GEN8_FLR8_VID_SOURCE} ${GPO_GEN8_FLR8_VID_V2}
				${GPO_GEN8_FLR8_VSE_SOURCE} ${GPO_GEN8_FLR8_VSE_V2}
				${GPO_GEN8_FLR8_VSE_SOURCE} ${GPO_GEN8_FLR8_VSE_V2} ${GPO_GEN8_FLR8_VSE_V3}
				${GPO_GEN8_FLR8_ROT_ISO_OBJECT_SOURCE} ${GPO_GEN8_FLR8_ROT_ISO_OBJECT_V3}
				${GPO_GEN8_FLR8_ROT_PROCESSED_DETECTION_SOURCE} ${GPO_GEN8_FLR8_ROT_PROCESSED_DETECTION_V3}
				${GPO_GEN8_FLR8_ROT_SAFETY_FAULTS_SOURCE} ${GPO_GEN8_FLR8_ROT_SAFETY_FAULTS_V3}
				${GPO_GEN8_FLR8_ROT_TRACKER_INFO_SOURCE} ${GPO_GEN8_FLR8_ROT_TRACKER_INFO_V2}
				${GPO_GEN8_FLR8_ROT_VEHICLE_INFO_SOURCE} ${GPO_GEN8_FLR8_ROT_VEHICLE_INFO_V3}
				
				)
		
add_library (${PROJECTNAME} SHARED "")
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

#link include directories 
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONHELPER} ${COMMONFUNCHEADERS} ${COMMONINC} ${COMMONHEADERS} ${COMMONINCAUDI} ${HDF} ${HDF5_INC})
if(MSVC)
	#target_link_libraries(${PROJECTNAME} ${ADDITIONAL_LIB})
	set(HDF_LIB_DIR "${STREAM_DECODER_PROJ}/HDF/lib")
else()
	set(HDF_LIB_DIR_UNIX "${STREAM_DECODER_PROJ}/HDF/lib")
endif()

target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR PC_RESIM H5_BUILT_AS_DYNAMIC_LIB)

if(MSVC)
	target_link_libraries(${PROJECTNAME} ${HDF_LIB_DIR}/zlib-static.lib
										 ${HDF_LIB_DIR}/hdf5.lib
										 ${HDF_LIB_DIR}/hdf5_cpp.lib
										 ${HDF_LIB_DIR}/hdf5_hl.lib
										 ${HDF_LIB_DIR}/hdf5_hl_cpp.lib
										 ${HDF_LIB_DIR}/hdf5_java.lib	
		                                 ${HDF_LIB_DIR}/hdf5_tools.lib
										 hdf5_tools hdf5 hdf5_cpp hdf5_hl hdf5_hl_cpp hdf5_java
										 )
else()
	target_link_libraries(${PROJECTNAME} 					${HDF_LIB_DIR_UNIX}/libhdf5.so
										 ${HDF_LIB_DIR_UNIX}/libhdf5_cpp.so
										 )
endif()

#target_link_libraries (${PROJECTNAME} DPH_RR_ADAS_LOGGER.dll)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
set(CMAKE_CXX_FLAGS "-fPIC")
set(CFLAGS"-fPIC")
set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
#get_property(prop TARGET ${PROJECTNAME} PROPERTY POSITION_INDEPENDENT_CODE)
#message("prop value:${prop}")
endif()

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

install(
    TARGETS ${PROJECTNAME}
    LIBRARY DESTINATION "${OUTPUT_PATH}"
    ARCHIVE DESTINATION "${OUTPUT_PATH}"
    RUNTIME DESTINATION "${OUTPUT_PATH}"
)

# Multi processor compilation
SET(MSVC_MULTITHREADED_BUILD ON CACHE BOOL "Multi processor compilation build with as many threads as possible when building with microsoft visual studio")
if(MSVC_MULTITHREADED_BUILD)
if(MSVC)
add_definitions(/MP)
endif()
endif()

# Structure Alignment
	if(MSVC)
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
		set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob0")
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
	
	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)

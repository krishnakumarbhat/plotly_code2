#start CMake for CCA ViGEM
#Build Steps
#Complier MSVC

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
set(PROJECTNAME "Stream_Plugin")

project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

# Setting Source projects
set(SRC_FILE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

#Adding required source and header files
list(APPEND SOURCE_ALL "${SRC_FILE_PATH}/stream_plugin.cpp")
list(APPEND SOURCE_ALL "${SRC_FILE_PATH}/stream_plugin.h")

# Streams
#ARTM_MEASUREMENT
list(APPEND ARTM_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/ARTM_MEASUREMENT/ARTM_Stream_V1.h")
list(APPEND ARTM_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/ARTM_MEASUREMENT/UDPHandler_Log_ARTM_Measurement.h")
list(APPEND ARTM_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/ARTM_MEASUREMENT/ARTM_Stream_V1.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\ARTM_MEASUREMENT FILES ${ARTM_inc_files})

#AVI
list(APPEND AVI_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/AVI/AVI_Stream_V2.h")
list(APPEND AVI_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/AVI/UDPHandler_Log_AVI_Output.h")
list(APPEND AVI_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/AVI/AVI_Stream_V2.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\AVI FILES ${AVI_inc_files})

#AVI_OBJECTINFO
list(APPEND AVI_OBJECTINO_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/AVI_OBJECTINFO/AVI_Object_Info_Stream_V2.h")
list(APPEND AVI_OBJECTINO_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/AVI_OBJECTINFO/UDPHandler_Log_AVI_Output.h")
list(APPEND AVI_OBJECTINO_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/AVI_OBJECTINFO/AVI_Object_Info_Stream_V2.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\AVI_OBJECTINFO FILES ${AVI_OBJECTINO_inc_files})

#DEBUG_STREAM_3
list(APPEND DEBUG_3_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_3/Debug_Stream_3_V6.h")
list(APPEND DEBUG_3_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_3/UDP_Log_DebugStream.h")
list(APPEND DEBUG_3_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_3/Debug_Stream_3_V6.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\DEBUG_STREAM_3 FILES ${DEBUG_3_inc_files})

#DEBUG_STREAM_10
list(APPEND DEBUG_10_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_10/Debug_Stream_10_V5.h")
list(APPEND DEBUG_10_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_10/UDP_Log_DebugStream.h")
list(APPEND DEBUG_10_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_10/Debug_Stream_10_V5.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\DEBUG_STREAM_10 FILES ${DEBUG_10_inc_files})

#DEBUG_STREAM_50
list(APPEND DEBUG_50_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_50/Debug_Stream_50_V5.h")
list(APPEND DEBUG_50_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_50/UDP_Log_DebugStream.h")
list(APPEND DEBUG_50_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/DEBUG_STREAM_50/Debug_Stream_50_V5.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\DEBUG_STREAM_50 FILES ${DEBUG_50_inc_files})

#FF_STREAM_10
list(APPEND FF_10_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/FF_STREAM_10/FF_Stream_10_V11.h")
list(APPEND FF_10_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/FF_STREAM_10/UDP_Log_FF_10ms_Output.h")
list(APPEND FF_10_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/FF_STREAM_10/FF_Stream_10_V11.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\FF_STREAM_10 FILES ${FF_10_inc_files})

#FF_STREAM_20
list(APPEND FF_20_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/FF_STREAM_20/FF_Stream_20_V11.h")
list(APPEND FF_20_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/FF_STREAM_20/UDP_Log_FF_20ms_Output.h")
list(APPEND FF_20_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/FF_STREAM_20/FF_Stream_20_V11.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\FF_STREAM_20 FILES ${FF_20_inc_files})

#OBJFUSION
list(APPEND OBJFUSION_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/OBJFUSION/Obj_Fusion_Stream_V4.h")
list(APPEND OBJFUSION_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/OBJFUSION/UDP_Log_ObjectFusion.h")
list(APPEND OBJFUSION_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/OBJFUSION/Obj_Fusion_Stream_V4.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\OBJFUSION FILES ${OBJFUSION_inc_files})

#PROXI_OUTPORT_CAR_CFG_PARAMETER
list(APPEND PROXI_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/PROXI_OUTPORT_CAR_CFG_PARAMETER/Proxi_Stream_V2.h")
list(APPEND PROXI_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/PROXI_OUTPORT_CAR_CFG_PARAMETER/UDPHandler_Log_Proxi_and_Car_Cfg_Param.h")
list(APPEND PROXI_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/PROXI_OUTPORT_CAR_CFG_PARAMETER/Proxi_Stream_V2.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\PROXI_OUTPORT_CAR_CFG_PARAMETER FILES ${PROXI_inc_files})

#RADARIF
list(APPEND RADARIF_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/RADARIF/Radarif_Stream_V1.h")
list(APPEND RADARIF_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/RADARIF/UDP_Log_RadarIF_Output.h")
list(APPEND RADARIF_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/RADARIF/Radarif_Stream_V1.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\RADARIF FILES ${RADARIF_inc_files})

#SATA_SPP
list(APPEND SATA_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/SATA_SPP/Sata_Spp_Stream_V5.h")
list(APPEND SATA_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/SATA_SPP/UDP_Log_SATA_SPP_OTP_VEH.h")
list(APPEND SATA_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/SATA_SPP/Sata_Spp_Stream_V5.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\SATA_SPP FILES ${SATA_inc_files})

#TRACKER_FLR
list(APPEND TRACKER_FLR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/TRACKER_FLR/Tracker_FLR_Stream_V6.h")
list(APPEND TRACKER_FLR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/TRACKER_FLR/UDP_Log_TrackerFLR_Output.h")
list(APPEND TRACKER_FLR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/TRACKER_FLR/Tracker_FLR_Stream_V6.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\TRACKER_FLR FILES ${TRACKER_FLR_inc_files})

#TRACKER_MRR
list(APPEND TRACKER_MRR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/TRACKER_MRR/Tracker_MRR_Stream_V6.h")
list(APPEND TRACKER_MRR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/TRACKER_MRR/UDP_Log_TrackerMRR_Output.h")
list(APPEND TRACKER_MRR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/TRACKER_MRR/Tracker_MRR_Stream_V6.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\TRACKER_MRR FILES ${TRACKER_MRR_inc_files})

#VEH_CALS_SWP4_CALIB
list(APPEND VEH_CALS_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/VEH_CALS_SWP4_CALIB/Veh_Cals_Stream_V2.h")
list(APPEND VEH_CALS_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/VEH_CALS_SWP4_CALIB/UDPHandler_Log_VehStream_Calibration.h")
list(APPEND VEH_CALS_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/VEH_CALS_SWP4_CALIB/Veh_Cals_Stream_V2.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\VEH_CALS_SWP4_CALIB FILES ${VEH_CALS_inc_files})

#VSE
list(APPEND VSE_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/VSE/VSE_Stream_V2.h")
list(APPEND VSE_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/VSE/UDP_Log_VSE_Output.h")
list(APPEND VSE_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/VSE/VSE_Stream_V2.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\VSE FILES ${VSE_inc_files})

#EDR
list(APPEND EDR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/EDR/EDR_Stream_V0.h")
list(APPEND EDR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/EDR/UDP_Log_EDR_Output.h")
list(APPEND EDR_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/EDR/EDR_Stream_V0.cpp")
source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\EDR FILES ${EDR_inc_files})

#UDP_VIRTUAL_STREAM
list(APPEND UDP_DSPACE_VIRTUAL_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/VIRTUAL/DSPACE/UDP_VIRTUAL_STREAM.h")
list(APPEND UDP_DSPACE_VIRTUAL_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/VIRTUAL/DSPACE/UDP_VIRTUAL_STREAM.cpp")
source_group(\\ADCAM_Streams\\VIRTUAL\\DSPACE FILES ${UDP_DSPACE_VIRTUAL_inc_files})

#CAN_over_UDP_FL
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_FL/CAN_over_UDP_FL.h")
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_FL/CAN_over_UDP_FL.cpp")

#CAN_over_UDP_FR
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_FR/CAN_over_UDP_FR.h")
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_FR/CAN_over_UDP_FR.cpp")

#CAN_over_UDP_RL
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_RL/CAN_over_UDP_RL.h")
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_RL/CAN_over_UDP_RL.cpp")

#CAN_over_UDP_RR
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_RR/CAN_over_UDP_RR.h")
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_RR/CAN_over_UDP_RR.cpp")

#CAN_over_UDP_FC
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_FC/CAN_over_UDP_FC.h")
list(APPEND CAN_over_UDP_inc_files "${SRC_FILE_PATH}/ADCAM_Streams/IFV600/STLA_SMALL/CAN_over_UDP_Stream/CAN_over_UDP_FC/CAN_over_UDP_FC.cpp")

source_group(\\ADCAM_Streams\\IFV600\\STLA_SMALL\\CAN_over_UDP_STREAM FILES ${CAN_over_UDP_inc_files})

#Creating Dynamic Library
add_library(${PROJECTNAME} STATIC "")

#link required sources
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL}
									  ${ARTM_inc_files}
									  ${AVI_inc_files}
									  ${AVI_OBJECTINO_inc_files}
									  ${DEBUG_3_inc_files}
									  ${DEBUG_10_inc_files}
									  ${DEBUG_50_inc_files}
									  ${FF_10_inc_files}
									  ${FF_20_inc_files}
									  ${OBJFUSION_inc_files}
									  ${PROXI_inc_files}
									  ${RADARIF_inc_files}
									  ${SATA_inc_files}
									  ${TRACKER_FLR_inc_files}
									  ${TRACKER_MRR_inc_files}
									  ${VEH_CALS_inc_files}
									  ${VSE_inc_files}
									  ${EDR_inc_files}
									  ${UDP_DSPACE_VIRTUAL_inc_files}
									  ${CAN_over_UDP_inc_files})

target_include_directories(${PROJECTNAME} PRIVATE ${SRC_FILE_PATH})

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)
								
# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)

if(UNIX)
	set_target_properties(${PROJECTNAME} PROPERTIES
		BUILD_WITH_INSTALL_RPATH TRUE
		INSTALL_RPATH "$ORIGIN")
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)
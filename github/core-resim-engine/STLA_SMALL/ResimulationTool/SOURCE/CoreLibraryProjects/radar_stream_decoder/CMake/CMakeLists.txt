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

cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "radar_stream_decoder")
project(${PROJECTNAME})

#Adding required common header files
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONFUNCHEADERS "${COMMON_INC}/inc/BMW/sFunctionheader")
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")
set(COMMONINCAUDI "${COMMON_INC}/inc/AUDI")



#Linking directories
link_directories(${OUTPUT_PATH})

#including sources and headers
set(SourceFiles "${STREAM_DECODER_PROJ}/")
set(COMMON_SOURCE "${SourceFiles}/Common/IRadarStream.cpp")
list(APPEND COMMON_SOURCE "${SourceFiles}/Common/Z_Logging.cpp")
list(APPEND COMMON_SOURCE "${SourceFiles}/Common/IRadarStream.h")
list(APPEND COMMON_SOURCE "${SourceFiles}/Common/Z_Logging.h")
list(APPEND COMMON_SOURCE "${SourceFiles}/Common/IRadarStream_Version.h")
list(APPEND COMMON_SOURCE "${COMMONHELPER}/compute.c")


list(APPEND Calibration "${SourceFiles}/Calibration/Calibration.cpp" 
						"${SourceFiles}/Calibration/Calib.h")

list(APPEND usc "${SourceFiles}/Calibration/usc/Vp1/usc_calib_Vp1.cpp" 
								"${SourceFiles}/Calibration/usc/Vp1/usc_calib_Vp1.h"
								"${SourceFiles}/Calibration/usc/Vp1/usc_cal.h")
								
list(APPEND usc "${SourceFiles}/Calibration/usc/Vp2/usc_calib_Vp2.cpp" 
								"${SourceFiles}/Calibration/usc/Vp2/usc_calib_Vp2.h"
								"${SourceFiles}/Calibration/usc/Vp2/usc_cal.h")

list(APPEND usc "${SourceFiles}/Calibration/usc/Vp3/usc_calib_Vp3.cpp" 
								"${SourceFiles}/Calibration/usc/Vp3/usc_calib_Vp3.h"
								"${SourceFiles}/Calibration/usc/Vp3/usc_cal.h")

list(APPEND usc "${SourceFiles}/Calibration/usc/usc_wrap.cpp" 
								"${SourceFiles}/Calibration/usc/usc_wrap.h")
																
list(APPEND Vp1 "${SourceFiles}/Calibration/Vp1/Calib_Logger_lst_Vp1.cpp" 
								"${SourceFiles}/Calibration/Vp1/Calib_Logger_lst_Vp1.h"
								"${SourceFiles}/Calibration/Vp1/calib_logging_structure.h"
								"${SourceFiles}/Calibration/Vp1/Calibration.h")

list(APPEND Vp2 "${SourceFiles}/Calibration/Vp2/Calib_Logger_lst_Vp2.cpp" 
								"${SourceFiles}/Calibration/Vp2/Calib_Logger_lst_Vp2.h"
								"${SourceFiles}/Calibration/Vp2/calib_logging_structure.h"
								"${SourceFiles}/Calibration/Vp2/Calibration.h")


								

								
source_group(\\Calibration FILES ${Calibration})
source_group(\\Calibration\\usc FILES ${usc})
source_group(\\Calibration\\usc\\Vp1 FILES ${usc_calib_Vp1})
source_group(\\Calibration\\usc\\Vp2 FILES ${usc_calib_Vp2})
source_group(\\Calibration\\usc\\Vp3 FILES ${usc_calib_Vp3})
source_group(\\Calibration\\Vp1 FILES ${Vp1})
source_group(\\Calibration\\Vp1 FILES ${Calib_Logger_lst_Vp1})
source_group(\\Calibration\\Vp2 FILES ${Vp2})
source_group(\\Calibration\\Vp2 FILES ${Calib_Logger_lst_Vp1})

								
# Z4 custom AUDi source
list(APPEND Z4CUSTOM "${SourceFiles}/CustomStreams/Z4_CUSTOM/Z4Custom_Logging.cpp" 
								"${SourceFiles}/CustomStreams/Z4_CUSTOM/Z4Custom_Logging.h")

#CustomStreams Z4_CUSTOM SRR5 source file
list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V1	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V1/Z4_BMW_BPIL_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V1/Z4_BMW_BPIL_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V1/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V2	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V2/Z4_BMW_BPIL_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V2/Z4_BMW_BPIL_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V2/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V3	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V3/Z4_BMW_BPIL_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V3/Z4_BMW_BPIL_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V3/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V4	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V4/Z4_BMW_BPIL_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V4/Z4_BMW_BPIL_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V4/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V5	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V5/Z4_BMW_BPIL_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V5/Z4_BMW_BPIL_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V5/z4_z7b_cust_logging_structure.h")										

list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V6	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V6/Z4_BMW_BPIL_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V6/Z4_BMW_BPIL_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V6/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V7	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V7/Z4_BMW_BPIL_Logging_V7.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V7/Z4_BMW_BPIL_Logging_V7.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V7/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_BPIL_V8	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V8/Z4_BMW_BPIL_Logging_V8.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V8/Z4_BMW_BPIL_Logging_V8.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_BPIL/V8/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V1	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V1/Z4_BMW_LOW_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V1/Z4_BMW_LOW_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V1/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V2	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V2/Z4_BMW_LOW_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V2/Z4_BMW_LOW_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V2/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V3	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V3/Z4_BMW_LOW_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V3/Z4_BMW_LOW_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V3/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V4	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V4/Z4_BMW_LOW_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V4/Z4_BMW_LOW_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V4/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V5	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V5/Z4_BMW_LOW_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V5/Z4_BMW_LOW_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V5/z4_z7b_cust_logging_structure.h")
																				
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V6	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V6/Z4_BMW_LOW_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V6/Z4_BMW_LOW_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V6/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V7	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V7/Z4_BMW_LOW_Logging_V7.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V7/Z4_BMW_LOW_Logging_V7.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V7/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V8	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V8/Z4_BMW_LOW_Logging_V8.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V8/Z4_BMW_LOW_Logging_V8.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V8/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V9	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V9/Z4_BMW_LOW_Logging_V9.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V9/Z4_BMW_LOW_Logging_V9.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V9/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V10	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V10/Z4_BMW_LOW_Logging_V10.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V10/Z4_BMW_LOW_Logging_V10.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V10/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V11	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V11/Z4_BMW_LOW_Logging_V11.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V11/Z4_BMW_LOW_Logging_V11.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V11/z4_z7b_cust_logging_structure.h")										

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V12	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V12/Z4_BMW_LOW_Logging_V12.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V12/Z4_BMW_LOW_Logging_V12.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V12/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V13	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V13/Z4_BMW_LOW_Logging_V13.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V13/Z4_BMW_LOW_Logging_V13.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V13/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V14	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V14/Z4_BMW_LOW_Logging_V14.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V14/Z4_BMW_LOW_Logging_V14.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V14/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V15	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V15/Z4_BMW_LOW_Logging_V15.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V15/Z4_BMW_LOW_Logging_V15.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V15/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V16	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V16/Z4_BMW_LOW_Logging_V16.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V16/Z4_BMW_LOW_Logging_V16.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V16/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V17	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V17/Z4_BMW_LOW_Logging_V17.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V17/Z4_BMW_LOW_Logging_V17.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V17/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V18	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V18/Z4_BMW_LOW_Logging_V18.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V18/Z4_BMW_LOW_Logging_V18.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V18/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V19	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V19/Z4_BMW_LOW_Logging_V19.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V19/Z4_BMW_LOW_Logging_V19.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V19/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V20	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V20/Z4_BMW_LOW_Logging_V20.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V20/Z4_BMW_LOW_Logging_V20.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V20/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V21	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V21/Z4_BMW_LOW_Logging_V21.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V21/Z4_BMW_LOW_Logging_V21.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V21/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_LOW_V22	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V22/Z4_BMW_LOW_Logging_V22.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V22/Z4_BMW_LOW_Logging_V22.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_LOW/V22/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_MID_V1	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V1/Z4_BMW_MID_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V1/Z4_BMW_MID_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V1/z4_z7b_cust_logging_structure.h")

										
list(APPEND Z4CUSTOM_SRR5_BMW_MID_V2	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V2/Z4_BMW_MID_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V2/Z4_BMW_MID_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V2/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_MID_V3	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V3/Z4_BMW_MID_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V3/Z4_BMW_MID_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V3/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_BMW_MID_V4	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V4/Z4_BMW_MID_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V4/Z4_BMW_MID_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V4/z4_z7b_cust_logging_structure.h")										

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V5	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V5/Z4_BMW_MID_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V5/Z4_BMW_MID_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V5/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V6	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V6/Z4_BMW_MID_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V6/Z4_BMW_MID_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V6/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V7	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V7/Z4_BMW_MID_Logging_V7.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V7/Z4_BMW_MID_Logging_V7.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V7/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V8	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V8/Z4_BMW_MID_Logging_V8.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V8/Z4_BMW_MID_Logging_V8.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V8/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V9	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V9/Z4_BMW_MID_Logging_V9.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V9/Z4_BMW_MID_Logging_V9.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V9/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V10	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V10/Z4_BMW_MID_Logging_V10.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V10/Z4_BMW_MID_Logging_V10.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V10/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_BMW_MID_V11	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V11/Z4_BMW_MID_Logging_V11.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V11/Z4_BMW_MID_Logging_V11.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/BMW_MID/V11/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_HKMC_SRR5_V1	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/HKMC_SRR5/V1/Z4_HKMC_SRR5_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/HKMC_SRR5/V1/Z4_HKMC_SRR5_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/HKMC_SRR5/V1/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_HKMC_SRR5_V2	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/HKMC_SRR5/V2/Z4_HKMC_SRR5_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/HKMC_SRR5/V2/Z4_HKMC_SRR5_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/HKMC_SRR5/V2/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_RNA_SUV_V1	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V1/Z4_RENAULT_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V1/Z4_RENAULT_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V1/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_RNA_SUV_V2	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V2/Z4_RENAULT_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V2/Z4_RENAULT_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V2/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_RNA_SUV_V3	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V3/Z4_RENAULT_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V3/Z4_RENAULT_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V3/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_RNA_SUV_V4	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V4/Z4_RENAULT_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V4/Z4_RENAULT_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V4/z4_z7b_cust_logging_structure.h")										

list(APPEND Z4CUSTOM_SRR5_RNA_SUV_V5	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V5/Z4_RENAULT_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V5/Z4_RENAULT_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V5/z4_z7b_cust_logging_structure.h")										

list(APPEND Z4CUSTOM_SRR5_RNA_SUV_V6	"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V6/Z4_RENAULT_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V6/Z4_RENAULT_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/RNA_SUV/V6/z4_z7b_cust_logging_structure.h")										
										
list(APPEND Z4CUSTOM_SRR5_GEELY_V1		"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/GEELY/V1/Z4_GEELY_SRR5_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/GEELY/V1/Z4_GEELY_SRR5_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/GEELY/V1/z4_z7b_cust_logging_structure.h")

list(APPEND Z4CUSTOM_SRR5_GEELY_V2		"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/GEELY/V2/Z4_GEELY_SRR5_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/GEELY/V2/Z4_GEELY_SRR5_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/GEELY/V2/z4_z7b_cust_logging_structure.h")
										
list(APPEND Z4CUSTOM_SRR5_CHANGAN_SRR5_V1		"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/CHANGAN_SRR5/V1/Z4_CHANGAN_SRR5_Logging_V1.cpp"
												"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/CHANGAN_SRR5/V1/Z4_CHANGAN_SRR5_Logging_V1.h"
												"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/CHANGAN_SRR5/V1/z4_z7b_cust_logging_structure.h")	

list(APPEND Z4CUSTOM_SRR5_CHANGAN_SRR5_V2		"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/CHANGAN_SRR5/V2/Z4_CHANGAN_SRR5_Logging_V2.cpp"
												"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/CHANGAN_SRR5/V2/Z4_CHANGAN_SRR5_Logging_V2.h"
												"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/CHANGAN_SRR5/V2/z4_z7b_cust_logging_structure.h")													

list(APPEND Z4CUSTOM_SRR5_PSA_SRR5_V1			"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/PSA_SRR5/V1/Z4_PSA_SRR5_Logging_V1.cpp"
												"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/PSA_SRR5/V1/Z4_PSA_SRR5_Logging_V1.h"
												"${SourceFiles}/CustomStreams/Z4_CUSTOM/SRR5/PSA_SRR5/V1/z4_z7b_cust_logging_structure.h")										


list(APPEND Z4CUSTOM_MRR_PSA_V3		"${SourceFiles}/CustomStreams/Z4_CUSTOM/MRR/V3/Z4_PSA_MRR_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/MRR/V3/Z4_PSA_MRR_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z4_CUSTOM/MRR/V3/z4_z7b_cust_logging_structure.h")
				
#Grouping CustomStreams Z4_CUSTOM SRR5 Source file
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V1		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V1})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V2		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V2})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V3		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V3})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V4		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V4})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V5		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V5})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V6		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V6})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V7		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V7})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_BPIL\\V8		FILES ${Z4CUSTOM_SRR5_BMW_BPIL_V8})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V1		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V1})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V2		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V2})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V3		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V3})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V4		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V4})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V5		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V5})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V6		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V6})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V7		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V7})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V8		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V8})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V9		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V9})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V10		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V10})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V11		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V11})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V12		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V12})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V13		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V13})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V14		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V14})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V15		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V15})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V16		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V16})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V17		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V17})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V18		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V18})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V19		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V19})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V20		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V20})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V21		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V21})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_LOW\\V22		FILES ${Z4CUSTOM_SRR5_BMW_LOW_V22})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V1		FILES ${Z4CUSTOM_SRR5_BMW_MID_V1})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V2		FILES ${Z4CUSTOM_SRR5_BMW_MID_V2})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V3		FILES ${Z4CUSTOM_SRR5_BMW_MID_V3})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V4		FILES ${Z4CUSTOM_SRR5_BMW_MID_V4})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V5		FILES ${Z4CUSTOM_SRR5_BMW_MID_V5})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V6		FILES ${Z4CUSTOM_SRR5_BMW_MID_V6})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V7		FILES ${Z4CUSTOM_SRR5_BMW_MID_V7})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V8		FILES ${Z4CUSTOM_SRR5_BMW_MID_V8})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V9		FILES ${Z4CUSTOM_SRR5_BMW_MID_V9})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V10		FILES ${Z4CUSTOM_SRR5_BMW_MID_V10})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\BMW_MID\\V11		FILES ${Z4CUSTOM_SRR5_BMW_MID_V11})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\HKMC_SRR5\\V1    FILES ${Z4CUSTOM_SRR5_HKMC_SRR5_V1})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\HKMC_SRR5\\V2    FILES ${Z4CUSTOM_SRR5_HKMC_SRR5_V2})	
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\RNA_SUV\\V1		FILES ${Z4CUSTOM_SRR5_RNA_SUV_V1})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\RNA_SUV\\V2		FILES ${Z4CUSTOM_SRR5_RNA_SUV_V2})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\RNA_SUV\\V3		FILES ${Z4CUSTOM_SRR5_RNA_SUV_V3})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\RNA_SUV\\V4		FILES ${Z4CUSTOM_SRR5_RNA_SUV_V4})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\RNA_SUV\\V5		FILES ${Z4CUSTOM_SRR5_RNA_SUV_V5})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\RNA_SUV\\V6		FILES ${Z4CUSTOM_SRR5_RNA_SUV_V6})				
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\GEELY\\V1		FILES ${Z4CUSTOM_SRR5_GEELY_V1})	
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\GEELY\\V2		FILES ${Z4CUSTOM_SRR5_GEELY_V2})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\CHANGAN_SRR5\\V1	 	FILES ${Z4CUSTOM_SRR5_CHANGAN_SRR5_V1})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\CHANGAN_SRR5\\V2	 	FILES ${Z4CUSTOM_SRR5_CHANGAN_SRR5_V2})
source_group(\\CustomStreams\\Z4_CUSTOM\\SRR5\\PSA_SRR5\\V1	 	FILES ${Z4CUSTOM_SRR5_PSA_SRR5_V1})	
source_group(\\CustomStreams\\Z4_CUSTOM\\MRR\\V3				FILES ${Z4CUSTOM_MRR_PSA_V3})

#Z7B CUSTOM Source files
list(APPEND Z7BCUSTOM "${SourceFiles}/CustomStreams/Z7B_CUSTOM/Z7BCustom_Logging.cpp"
						"${SourceFiles}/CustomStreams/Z7B_CUSTOM/Z7BCustom_Logging.h")
#Grouping Z7B CUSTOM
source_group(\\CustomStreams\\Z7B_CUSTOM FILES ${Z7BCUSTOM})

#CustomStreams Z7B_CUSTOM SRR5 source file
list(APPEND Z7BCUSTOM_SRR5_BMW_BPIL_V1	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V1/Z7B_BMW_BPIL_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V1/Z7B_BMW_BPIL_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V1/z7b_cust_logging_structure.h")

list(APPEND Z7BCUSTOM_SRR5_BMW_BPIL_V2	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V2/Z7B_BMW_BPIL_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V2/Z7B_BMW_BPIL_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V2/z7b_cust_logging_structure.h")
										
list(APPEND	Z7BCUSTOM_SRR5_BMW_BPIL_V3	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V3/Z7B_BMW_BPIL_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V3/Z7B_BMW_BPIL_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V3/z7b_cust_logging_structure.h")
										
list(APPEND	Z7BCUSTOM_SRR5_BMW_BPIL_V4	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V4/Z7B_BMW_BPIL_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V4/Z7B_BMW_BPIL_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V4/z7b_cust_logging_structure.h")

list(APPEND	Z7BCUSTOM_SRR5_BMW_BPIL_V5	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V5/Z7B_BMW_BPIL_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V5/Z7B_BMW_BPIL_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V5/z7b_cust_logging_structure.h")

list(APPEND	Z7BCUSTOM_SRR5_BMW_BPIL_V6	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V6/Z7B_BMW_BPIL_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V6/Z7B_BMW_BPIL_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_BPIL/V6/z7b_cust_logging_structure.h")

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V1	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V1/Z7B_BMW_LOW_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V1/Z7B_BMW_LOW_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V1/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V2	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V2/Z7B_BMW_LOW_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V2/Z7B_BMW_LOW_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V2/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V3	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V3/Z7B_BMW_LOW_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V3/Z7B_BMW_LOW_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V3/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V4	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V4/Z7B_BMW_LOW_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V4/Z7B_BMW_LOW_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V4/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V5	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V5/Z7B_BMW_LOW_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V5/Z7B_BMW_LOW_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V5/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V6	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V6/Z7B_BMW_LOW_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V6/Z7B_BMW_LOW_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V6/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V7	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V7/Z7B_BMW_LOW_Logging_V7.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V7/Z7B_BMW_LOW_Logging_V7.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V7/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V8	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V8/Z7B_BMW_LOW_Logging_V8.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V8/Z7B_BMW_LOW_Logging_V8.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V8/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V9	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V9/Z7B_BMW_LOW_Logging_V9.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V9/Z7B_BMW_LOW_Logging_V9.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V9/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V11	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V11/Z7B_BMW_LOW_Logging_V11.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V11/Z7B_BMW_LOW_Logging_V11.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V11/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V12	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V12/Z7B_BMW_LOW_Logging_V12.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V12/Z7B_BMW_LOW_Logging_V12.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V12/z7b_cust_logging_structure.h")	

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V13	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V13/Z7B_BMW_LOW_Logging_V13.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V13/Z7B_BMW_LOW_Logging_V13.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V13/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V14	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V14/Z7B_BMW_LOW_Logging_V14.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V14/Z7B_BMW_LOW_Logging_V14.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V14/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V15	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V15/Z7B_BMW_LOW_Logging_V15.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V15/Z7B_BMW_LOW_Logging_V15.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V15/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V16	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V16/Z7B_BMW_LOW_Logging_V16.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V16/Z7B_BMW_LOW_Logging_V16.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V16/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_LOW_V17	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V17/Z7B_BMW_LOW_Logging_V17.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V17/Z7B_BMW_LOW_Logging_V17.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_LOW/V17/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V1	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V1/Z7B_BMW_MID_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V1/Z7B_BMW_MID_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V1/z7b_cust_logging_structure.h")

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V2	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V2/Z7B_BMW_MID_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V2/Z7B_BMW_MID_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V2/z7b_cust_logging_structure.h")

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V3	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V3/Z7B_BMW_MID_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V3/Z7B_BMW_MID_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V3/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V4	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V4/Z7B_BMW_MID_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V4/Z7B_BMW_MID_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V4/z7b_cust_logging_structure.h")		

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V5	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V5/Z7B_BMW_MID_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V5/Z7B_BMW_MID_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V5/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V6	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V6/Z7B_BMW_MID_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V6/Z7B_BMW_MID_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V6/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V7	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V7/Z7B_BMW_MID_Logging_V7.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V7/Z7B_BMW_MID_Logging_V7.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V7/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_BMW_MID_V8	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V8/Z7B_BMW_MID_Logging_V8.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V8/Z7B_BMW_MID_Logging_V8.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/BMW_MID/V8/z7b_cust_logging_structure.h")										
										
list(APPEND Z7BCUSTOM_SRR5_GEELY_V1		"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/GEELY/V1/Z7B_GEELY_SRR5_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/GEELY/V1/Z7B_GEELY_SRR5_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/GEELY/V1/z7b_cust_logging_structure.h")

list(APPEND Z7BCUSTOM_SRR5_HKMC_SRR5_V1	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V1/Z7B_HKMC_SRR5_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V1/Z7B_HKMC_SRR5_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V1/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_HKMC_SRR5_V2	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V2/Z7B_HKMC_SRR5_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V2/Z7B_HKMC_SRR5_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V2/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_HKMC_SRR5_V3	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V3/Z7B_HKMC_SRR5_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V3/Z7B_HKMC_SRR5_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/HKMC_SRR5/V3/z7b_cust_logging_structure.h")

list(APPEND Z7BCUSTOM_SRR5_RNA_SUV_V1	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V1/Z7B_RENAULT_Logging_V1.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V1/Z7B_RENAULT_Logging_V1.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V1/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_RNA_SUV_V2	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V2/Z7B_RENAULT_Logging_V2.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V2/Z7B_RENAULT_Logging_V2.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V2/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_RNA_SUV_V3	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V3/Z7B_RENAULT_Logging_V3.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V3/Z7B_RENAULT_Logging_V3.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V3/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_RNA_SUV_V4	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V4/Z7B_RENAULT_Logging_V4.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V4/Z7B_RENAULT_Logging_V4.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V4/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_RNA_SUV_V5	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V5/Z7B_RENAULT_Logging_V5.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V5/Z7B_RENAULT_Logging_V5.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V5/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_RNA_SUV_V6	"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V6/Z7B_RENAULT_Logging_V6.cpp"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V6/Z7B_RENAULT_Logging_V6.h"
										"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/RNA_SUV/V6/z7b_cust_logging_structure.h")
										
list(APPEND Z7BCUSTOM_SRR5_CHANGAN_SRR5_V1			"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V1/Z7B_CHANGAN_SRR5_Logging_V1.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V1/Z7B_CHANGAN_SRR5_Logging_V1.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V1/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_CHANGAN_SRR5_V2			"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V2/Z7B_CHANGAN_SRR5_Logging_V2.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V2/Z7B_CHANGAN_SRR5_Logging_V2.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V2/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_CHANGAN_SRR5_V3			"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V3/Z7B_CHANGAN_SRR5_Logging_V3.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V3/Z7B_CHANGAN_SRR5_Logging_V3.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V3/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_SRR5_CHANGAN_SRR5_V4			"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V4/Z7B_CHANGAN_SRR5_Logging_V4.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V4/Z7B_CHANGAN_SRR5_Logging_V4.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V4/z7b_cust_logging_structure.h")
													
list(APPEND Z7BCUSTOM_SRR5_CHANGAN_SRR5_V5			"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V5/Z7B_CHANGAN_SRR5_Logging_V5.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V5/Z7B_CHANGAN_SRR5_Logging_V5.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/CHANGAN_SRR5/V5/z7b_cust_logging_structure.h")
													
list(APPEND Z7BCUSTOM_SRR5_GWM_SRR5_V1				"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/GWM_SRR5/V1/Z7B_GWM_SRR5_Logging_V1.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/GWM_SRR5/V1/Z7B_GWM_SRR5_Logging_V1.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/GWM_SRR5/V1/z7b_cust_logging_structure.h")													

list(APPEND Z7BCUSTOM_SRR5_PSA_SRR5_V1				"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/PSA_SRR5/V1/Z7B_PSA_SRR5_Logging_V1.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/PSA_SRR5/V1/Z7B_PSA_SRR5_Logging_V1.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/SRR5/PSA_SRR5/V1/z7b_cust_logging_structure.h")										

list(APPEND Z7BCUSTOM_MRR_PSA_V1					"${SourceFiles}/CustomStreams/Z7B_CUSTOM/MRR/V1/Z7B_PSA_MRR_Logging_V1.cpp"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/MRR/V1/Z7B_PSA_MRR_Logging_V1.h"
													"${SourceFiles}/CustomStreams/Z7B_CUSTOM/MRR/V1/z7b_cust_logging_structure.h")

				
#Grouping CustomStreams Z7B_CUSTOM SRR5 Source file
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_BPIL\\V1	FILES ${Z7BCUSTOM_SRR5_BMW_BPIL_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_BPIL\\V2	FILES ${Z7BCUSTOM_SRR5_BMW_BPIL_V2})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_BPIL\\V3	FILES ${Z7BCUSTOM_SRR5_BMW_BPIL_V3})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_BPIL\\V4	FILES ${Z7BCUSTOM_SRR5_BMW_BPIL_V4})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_BPIL\\V5	FILES ${Z7BCUSTOM_SRR5_BMW_BPIL_V5})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_BPIL\\V6	FILES ${Z7BCUSTOM_SRR5_BMW_BPIL_V6})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V1		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V2		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V2})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V3		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V3})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V4		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V4})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V5		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V5})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V6		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V6})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V7		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V7})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V8		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V8})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V9		FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V9})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V11	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V11})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V12	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V12})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V13	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V13})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V14	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V14})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V15	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V15})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V16	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V16})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_LOW\\V17	FILES ${Z7BCUSTOM_SRR5_BMW_LOW_V17})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V1		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V2		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V2})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V3		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V3})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V4		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V4})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V5		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V5})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V6		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V6})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V7		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V7})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\BMW_MID\\V8		FILES ${Z7BCUSTOM_SRR5_BMW_MID_V8})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\GEELY\\V1		FILES ${Z7BCUSTOM_SRR5_GEELY_V1})	
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\HKMC_SRR5\\V1   FILES ${Z7BCUSTOM_SRR5_HKMC_SRR5_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\HKMC_SRR5\\V2   FILES ${Z7BCUSTOM_SRR5_HKMC_SRR5_V2})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\HKMC_SRR5\\V3   FILES ${Z7BCUSTOM_SRR5_HKMC_SRR5_V3})	
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\RNA_SUV\\V1		FILES ${Z7BCUSTOM_SRR5_RNA_SUV_V1})	
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\RNA_SUV\\V2		FILES ${Z7BCUSTOM_SRR5_RNA_SUV_V2})	
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\RNA_SUV\\V3		FILES ${Z7BCUSTOM_SRR5_RNA_SUV_V3})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\RNA_SUV\\V4		FILES ${Z7BCUSTOM_SRR5_RNA_SUV_V4})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\RNA_SUV\\V5		FILES ${Z7BCUSTOM_SRR5_RNA_SUV_V5})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\RNA_SUV\\V6		FILES ${Z7BCUSTOM_SRR5_RNA_SUV_V6})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\CHANGAN_SRR5\\V1		FILES ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\CHANGAN_SRR5\\V2		FILES ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V2})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\CHANGAN_SRR5\\V3		FILES ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V3})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\CHANGAN_SRR5\\V4		FILES ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V4})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\CHANGAN_SRR5\\V5		FILES ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V5})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\GWM_SRR5\\V1		FILES ${Z7BCUSTOM_SRR5_GWM_SRR5_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\SRR5\\PSA_SRR5\\V1		FILES ${Z7BCUSTOM_SRR5_PSA_SRR5_V1})
source_group(\\CustomStreams\\Z7B_CUSTOM\\MRR\\V1				FILES ${Z7BCUSTOM_MRR_PSA_V1})


#SRR5_CORE CDC Source file					
list(APPEND CDC_SOURCE "${SourceFiles}/SRR5_CORE/CDC/CDC_Logging.cpp")

#Grouping CDC source file						
source_group(\\SRR5_CORE\\CDC\\MRR2\\V1 FILES ${CDC_SOURCE})

#SRR5_CORE CDC MRR2 Source file
list(APPEND CDC_MRR2_V1 "${SourceFiles}/SRR5_CORE/CDC/MRR2/V1/cdc_logging_structure.h"
						"${SourceFiles}/SRR5_CORE/CDC/MRR2/V1/CDC_Logging_V1.cpp"
						"${SourceFiles}/SRR5_CORE/CDC/MRR2/V1/CDC_Logging_V1.h")

#Grouping CDC MRR2 source file						
source_group(\\SRR5_CORE\\CDC\\MRR2\\V1 FILES ${CDC_MRR2_V1})

#SRR5_CORE CDC SRR5 Source file
					
list(APPEND CDC_SRR5_V4 "${SourceFiles}/SRR5_CORE/CDC/SRR5/V4/cdc_logging_structure.h"
						"${SourceFiles}/SRR5_CORE/CDC/SRR5/V4/SRR5_CDC_Logging_V4.cpp"
						"${SourceFiles}/SRR5_CORE/CDC/SRR5/V4/SRR5_CDC_Logging_V4.h")

#Grouping CDC SRR5 source file						
source_group(\\SRR5_CORE\\CDC\\SRR5\\V4 FILES ${CDC_SRR5_V4})

#SRR5_CORE Z4 source file
list(APPEND Z4SOURCE "${SourceFiles}/SRR5_CORE/Z4/Z4_Logging.cpp"
						"${SourceFiles}/SRR5_CORE/Z4/Z4_Logging.h")

#Grouping core stream Z4 source file
source_group(\\SRR5_CORE\\Z4 FILES ${Z4SOURCE})

#SRR5_CORE Z4 SRR5_CORE source file
list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V1 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V1/Z4_SRR5_Logging_V1.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V1/Z4_SRR5_Logging_V1.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V1/Z4_Z7B_CORE.h")
																				 
list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V2 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V2/Z4_SRR5_Logging_V2.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V2/Z4_SRR5_Logging_V2.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V2/Z4_Z7B_CORE.h")
																				 
list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V3 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V3/Z4_SRR5_Logging_V3.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V3/Z4_SRR5_Logging_V3.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V3/Z4_Z7B_CORE.h")
																				 
list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V4 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V4/Z4_SRR5_Logging_V4.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V4/Z4_SRR5_Logging_V4.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V4/Z4_Z7B_CORE.h")
																				 
list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V5 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V5/Z4_SRR5_Logging_V5.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V5/Z4_SRR5_Logging_V5.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V5/Z4_Z7B_CORE.h")

list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V6 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V6/Z4_SRR5_Logging_V6.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V6/Z4_SRR5_Logging_V6.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V6/Z4_Z7B_CORE.h")

list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V7 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V7/Z4_SRR5_Logging_V7.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V7/Z4_SRR5_Logging_V7.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V7/Z4_Z7B_CORE.h")

list(APPEND Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V8 	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V8/Z4_SRR5_Logging_V8.cpp"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V8/Z4_SRR5_Logging_V8.h"
													"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_CORE_50/V8/Z4_Z7B_CORE.h")
											
list(APPEND Z4_SRR5_CORE_RNA_V1	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V1/Z4_SRR5_RNA_Logging_V1.cpp"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V1/Z4_SRR5_RNA_Logging_V1.h"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V1/Z4_Z7B_64_FD_CORE.h")
															 
list(APPEND Z4_SRR5_CORE_RNA_V2	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V2/Z4_SRR5_RNA_Logging_V2.cpp"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V2/Z4_SRR5_RNA_Logging_V2.h"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V2/Z4_Z7B_64_FD_CORE.h")	
															 
list(APPEND Z4_SRR5_CORE_RNA_V3	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V3/Z4_SRR5_RNA_Logging_V3.cpp"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V3/Z4_SRR5_RNA_Logging_V3.h"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V3/Z4_Z7B_64_FD_CORE.h")
								
list(APPEND Z4_SRR5_CORE_RNA_V4	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V4/Z4_SRR5_RNA_Logging_V4.cpp"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V4/Z4_SRR5_RNA_Logging_V4.h"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_64_FD_CORE_51/V4/Z4_Z7B_64_FD_CORE.h")	

list(APPEND Z4_SRR5_CORE_PSA_V1	"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_SRR5_SAT_CORE_52/V1/Z4_SRR5_PSA_Logging_V1.cpp"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_SRR5_SAT_CORE_52/V1/Z4_SRR5_PSA_Logging_V1.h"
								"${SourceFiles}/SRR5_CORE/Z4/Z4_Z7B_SRR5_SAT_CORE_52/V1/Z4_Z7B_SRR5_SAT_CORE.h")								


list(APPEND Z4_MRR_PSA_V1		"${SourceFiles}/SRR5_CORE/Z4/MRR/Z4_Z7B_MRR_SAT_CORE_53/V1/Z4_MRR_PSA_Logging_V1.cpp"
								"${SourceFiles}/SRR5_CORE/Z4/MRR/Z4_Z7B_MRR_SAT_CORE_53/V1/Z4_MRR_PSA_Logging_V1.h"
								"${SourceFiles}/SRR5_CORE/Z4/MRR/Z4_Z7B_MRR_SAT_CORE_53/V1/Z4_Z7B_MRR_SAT_CORE.h")	
								

#Grouping SRR5_CORE Z4 SCANIA Source file
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_64_FD_CORE_51\\V1  FILES ${Z4_SRR5_CORE_RNA_V1})		
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_64_FD_CORE_51\\V2  FILES ${Z4_SRR5_CORE_RNA_V2})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_64_FD_CORE_51\\V3  FILES ${Z4_SRR5_CORE_RNA_V3})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_64_FD_CORE_51\\V4  FILES ${Z4_SRR5_CORE_RNA_V4})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V1  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V1})	
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V2  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V2})	
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V3  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V3})	
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V4  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V4})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V5  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V5})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V6  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V6})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V7  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V7})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_CORE_50\\V8  FILES ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V8})
source_group(\\SRR5_CORE\\Z4\\Z4_Z7B_SRR5_SAT_CORE_52\\V1  FILES ${Z4_SRR5_CORE_PSA_V1})		
source_group(\\SRR5_CORE\\Z4\\MRR\\Z4_Z7B_MRR_SAT_CORE_53\\V1  FILES ${Z4_MRR_PSA_V1})



#SRR5_CORE Z7A Source file
list(APPEND Z7ASOURCE "${SourceFiles}/SRR5_CORE/Z7A/Z7A_Logging.cpp"
						"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Logging.h")	
						
#Grouping Z7A source file
source_group(\\SRR5_CORE\\Z7A FILES ${Z7ASOURCE})

#SRR5_CORE Z7A SRR5_CORE source file
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V1				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V1/Z7A_Logging_BMW_BPIL_SRR5_V1.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V1/Z7A_Logging_BMW_BPIL_SRR5_V1.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V1/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")		
																				  
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V2				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V2/Z7A_Logging_BMW_BPIL_SRR5_V2.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V2/Z7A_Logging_BMW_BPIL_SRR5_V2.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V2/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")	
																				 
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V3				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V3/Z7A_Logging_BMW_BPIL_SRR5_V3.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V3/Z7A_Logging_BMW_BPIL_SRR5_V3.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V3/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")											
																				 
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V4				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V4/Z7A_Logging_BMW_BPIL_SRR5_V4.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V4/Z7A_Logging_BMW_BPIL_SRR5_V4.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V4/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")	
																				
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V5				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V5/Z7A_Logging_BMW_BPIL_SRR5_V5.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V5/Z7A_Logging_BMW_BPIL_SRR5_V5.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V5/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")
																				 
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V6				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V6/Z7A_Logging_BMW_BPIL_SRR5_V6.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V6/Z7A_Logging_BMW_BPIL_SRR5_V6.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V6/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")
																				  
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V7				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V7/Z7A_Logging_BMW_BPIL_SRR5_V7.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V7/Z7A_Logging_BMW_BPIL_SRR5_V7.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V7/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")
																				
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V8				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V8/Z7A_Logging_BMW_BPIL_SRR5_V8.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V8/Z7A_Logging_BMW_BPIL_SRR5_V8.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V8/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")
																				  
list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V9				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V9/Z7A_Logging_BMW_BPIL_SRR5_V9.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V9/Z7A_Logging_BMW_BPIL_SRR5_V9.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V9/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V10				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V10/Z7A_Logging_BMW_BPIL_SRR5_V10.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V10/Z7A_Logging_BMW_BPIL_SRR5_V10.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V10/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V11				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V11/Z7A_Logging_BMW_BPIL_SRR5_V11.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V11/Z7A_Logging_BMW_BPIL_SRR5_V11.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V11/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V12				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V12/Z7A_Logging_BMW_BPIL_SRR5_V12.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V12/Z7A_Logging_BMW_BPIL_SRR5_V12.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V12/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V13				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V13/Z7A_Logging_BMW_BPIL_SRR5_V13.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V13/Z7A_Logging_BMW_BPIL_SRR5_V13.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V13/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V14				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V14/Z7A_Logging_BMW_BPIL_SRR5_V14.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V14/Z7A_Logging_BMW_BPIL_SRR5_V14.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V14/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_BPIL_V15				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V15/Z7A_Logging_BMW_BPIL_SRR5_V15.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V15/Z7A_Logging_BMW_BPIL_SRR5_V15.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28/V15/Z7A_Z7B_SRR5P_116RBIN_150D_CORE.h")
			
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V1				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V1/Z7A_Logging_BMW_LV_SRR5_V1.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V1/Z7A_Logging_BMW_LV_SRR5_V1.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V1/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V2				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V2/Z7A_Logging_BMW_LV_SRR5_V2.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V2/Z7A_Logging_BMW_LV_SRR5_V2.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V2/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V3				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V3/Z7A_Logging_BMW_LV_SRR5_V3.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V3/Z7A_Logging_BMW_LV_SRR5_V3.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V3/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V4				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V4/Z7A_Logging_BMW_LV_SRR5_V4.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V4/Z7A_Logging_BMW_LV_SRR5_V4.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V4/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")	
			
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V5				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V5/Z7A_Logging_BMW_LV_SRR5_V5.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V5/Z7A_Logging_BMW_LV_SRR5_V5.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V5/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")	
			
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V6				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V6/Z7A_Logging_BMW_LV_SRR5_V6.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V6/Z7A_Logging_BMW_LV_SRR5_V6.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V6/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
			
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V7				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V7/Z7A_Logging_BMW_LV_SRR5_V7.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V7/Z7A_Logging_BMW_LV_SRR5_V7.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V7/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V8				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V8/Z7A_Logging_BMW_LV_SRR5_V8.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V8/Z7A_Logging_BMW_LV_SRR5_V8.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V8/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V9				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V9/Z7A_Logging_BMW_LV_SRR5_V9.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V9/Z7A_Logging_BMW_LV_SRR5_V9.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V9/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V10				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V10/Z7A_Logging_BMW_LV_SRR5_V10.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V10/Z7A_Logging_BMW_LV_SRR5_V10.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V10/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_LOW_V11				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V11/Z7A_Logging_BMW_LV_SRR5_V11.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V11/Z7A_Logging_BMW_LV_SRR5_V11.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V11/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V12				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V12/Z7A_Logging_BMW_LV_SRR5_V12.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V12/Z7A_Logging_BMW_LV_SRR5_V12.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V12/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V13				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V13/Z7A_Logging_BMW_LV_SRR5_V13.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V13/Z7A_Logging_BMW_LV_SRR5_V13.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V13/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V14				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V14/Z7A_Logging_BMW_LV_SRR5_V14.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V14/Z7A_Logging_BMW_LV_SRR5_V14.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V14/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V15				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V15/Z7A_Logging_BMW_LV_SRR5_V15.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V15/Z7A_Logging_BMW_LV_SRR5_V15.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V15/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V16				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V16/Z7A_Logging_BMW_LV_SRR5_V16.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V16/Z7A_Logging_BMW_LV_SRR5_V16.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V16/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V17				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V17/Z7A_Logging_BMW_LV_SRR5_V17.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V17/Z7A_Logging_BMW_LV_SRR5_V17.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V17/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V18				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V18/Z7A_Logging_BMW_LV_SRR5_V18.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V18/Z7A_Logging_BMW_LV_SRR5_V18.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V18/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V19				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V19/Z7A_Logging_BMW_LV_SRR5_V19.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V19/Z7A_Logging_BMW_LV_SRR5_V19.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V19/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")

list(APPEND Z7A_SRR5_CORE_BMW_LOW_V20				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V20/Z7A_Logging_BMW_LV_SRR5_V20.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V20/Z7A_Logging_BMW_LV_SRR5_V20.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27/V20/Z7A_Z7B_SRR5P_116RBIN_64D_CORE.h")
			
list(APPEND Z7A_SRR5_CORE_BMW_MID_V1				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V1/Z7A_Logging_BMW_MID_SRR5_V1.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V1/Z7A_Logging_BMW_MID_SRR5_V1.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V1/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_MID_V2				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V2/Z7A_Logging_BMW_MID_SRR5_V2.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V2/Z7A_Logging_BMW_MID_SRR5_V2.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V2/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
													
list(APPEND Z7A_SRR5_CORE_BMW_MID_V3				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V3/Z7A_Logging_BMW_MID_SRR5_V3.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V3/Z7A_Logging_BMW_MID_SRR5_V3.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V3/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
							
list(APPEND Z7A_SRR5_CORE_BMW_MID_V4				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V4/Z7A_Logging_BMW_MID_SRR5_V4.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V4/Z7A_Logging_BMW_MID_SRR5_V4.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V4/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_MID_V5				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V5/Z7A_Logging_BMW_MID_SRR5_V5.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V5/Z7A_Logging_BMW_MID_SRR5_V5.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V5/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
list(APPEND Z7A_SRR5_CORE_BMW_MID_V6				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V6/Z7A_Logging_BMW_MID_SRR5_V6.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V6/Z7A_Logging_BMW_MID_SRR5_V6.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V6/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
list(APPEND	Z7A_SRR5_CORE_BMW_MID_V7				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V7/Z7A_Logging_BMW_MID_SRR5_V7.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V7/Z7A_Logging_BMW_MID_SRR5_V7.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V7/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
list(APPEND	Z7A_SRR5_CORE_BMW_MID_V8				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V8/Z7A_Logging_BMW_MID_SRR5_V8.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V8/Z7A_Logging_BMW_MID_SRR5_V8.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V8/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
													
list(APPEND	Z7A_SRR5_CORE_BMW_MID_V9				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V9/Z7A_Logging_BMW_MID_SRR5_V9.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V9/Z7A_Logging_BMW_MID_SRR5_V9.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V9/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V10				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V10/Z7A_Logging_BMW_MID_SRR5_V10.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V10/Z7A_Logging_BMW_MID_SRR5_V10.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V10/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V11				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V11/Z7A_Logging_BMW_MID_SRR5_V11.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V11/Z7A_Logging_BMW_MID_SRR5_V11.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V11/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V12				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V12/Z7A_Logging_BMW_MID_SRR5_V12.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V12/Z7A_Logging_BMW_MID_SRR5_V12.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V12/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V13				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V13/Z7A_Logging_BMW_MID_SRR5_V13.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V13/Z7A_Logging_BMW_MID_SRR5_V13.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V13/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V14				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V14/Z7A_Logging_BMW_MID_SRR5_V14.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V14/Z7A_Logging_BMW_MID_SRR5_V14.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V14/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V15				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V15/Z7A_Logging_BMW_MID_SRR5_V15.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V15/Z7A_Logging_BMW_MID_SRR5_V15.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V15/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V16				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V16/Z7A_Logging_BMW_MID_SRR5_V16.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V16/Z7A_Logging_BMW_MID_SRR5_V16.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V16/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V17				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V17/Z7A_Logging_BMW_MID_SRR5_V17.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V17/Z7A_Logging_BMW_MID_SRR5_V17.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V17/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V18				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V18/Z7A_Logging_BMW_MID_SRR5_V18.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V18/Z7A_Logging_BMW_MID_SRR5_V18.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V18/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V19				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V19/Z7A_Logging_BMW_MID_SRR5_V19.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V19/Z7A_Logging_BMW_MID_SRR5_V19.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V19/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")

list(APPEND	Z7A_SRR5_CORE_BMW_MID_V20				"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V20/Z7A_Logging_BMW_MID_SRR5_V20.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V20/Z7A_Logging_BMW_MID_SRR5_V20.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29/V20/Z7A_Z7B_SRR5P_116RBIN_200D_CORE.h")
										
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V1		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V1/Z7A_Logging_SRR5_CORE_V1.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V1/Z7A_Logging_SRR5_CORE_V1.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V1/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V2		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V2/Z7A_Logging_SRR5_CORE_V2.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V2/Z7A_Logging_SRR5_CORE_V2.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V2/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V3		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V3/Z7A_Logging_SRR5_CORE_V3.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V3/Z7A_Logging_SRR5_CORE_V3.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V3/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V4		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V4/Z7A_Logging_SRR5_CORE_V4.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V4/Z7A_Logging_SRR5_CORE_V4.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V4/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V5		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V5/Z7A_Logging_SRR5_CORE_V5.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V5/Z7A_Logging_SRR5_CORE_V5.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V5/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V6		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V6/Z7A_Logging_SRR5_CORE_V6.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V6/Z7A_Logging_SRR5_CORE_V6.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V6/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V7		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V7/Z7A_Logging_SRR5_CORE_V7.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V7/Z7A_Logging_SRR5_CORE_V7.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V7/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V8		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V8/Z7A_Logging_SRR5_CORE_V8.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V8/Z7A_Logging_SRR5_CORE_V8.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V8/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V9		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V9/Z7A_Logging_SRR5_CORE_V9.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V9/Z7A_Logging_SRR5_CORE_V9.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V9/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V10	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V10/Z7A_Logging_SRR5_CORE_V10.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V10/Z7A_Logging_SRR5_CORE_V10.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V10/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V11	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V11/Z7A_Logging_SRR5_CORE_V11.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V11/Z7A_Logging_SRR5_CORE_V11.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V11/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V12	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V12/Z7A_Logging_SRR5_CORE_V12.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V12/Z7A_Logging_SRR5_CORE_V12.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V12/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")	
																					  
list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V13	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V13/Z7A_Logging_SRR5_CORE_V13.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V13/Z7A_Logging_SRR5_CORE_V13.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V13/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")												

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V14	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V14/Z7A_Logging_SRR5_CORE_V14.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V14/Z7A_Logging_SRR5_CORE_V14.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V14/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")													

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V15	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V15/Z7A_Logging_SRR5_CORE_V15.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V15/Z7A_Logging_SRR5_CORE_V15.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V15/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")													

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V16	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V16/Z7A_Logging_SRR5_CORE_V16.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V16/Z7A_Logging_SRR5_CORE_V16.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V16/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")													

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V17	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V17/Z7A_Logging_SRR5_CORE_V17.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V17/Z7A_Logging_SRR5_CORE_V17.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V17/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")													

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V18	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V18/Z7A_Logging_SRR5_CORE_V18.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V18/Z7A_Logging_SRR5_CORE_V18.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V18/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V19	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V19/Z7A_Logging_SRR5_CORE_V19.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V19/Z7A_Logging_SRR5_CORE_V19.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V19/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")

list(APPEND Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V20	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V20/Z7A_Logging_SRR5_CORE_V20.cpp"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V20/Z7A_Logging_SRR5_CORE_V20.h"
													"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20/V20/Z7A_Z7B_IPC_SRR5_116RBIN_CORE.h")													

list(APPEND Z7A_SRR5_CORE_RNA_210_V1	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V1/Z7A_Logging_SRR5_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V1/Z7A_Logging_SRR5_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V1/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V2	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V2/Z7A_Logging_SRR5_V2.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V2/Z7A_Logging_SRR5_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V2/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V3	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V3/Z7A_Logging_SRR5_V3.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V3/Z7A_Logging_SRR5_V3.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V3/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V4	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V4/Z7A_Logging_SRR5_V4.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V4/Z7A_Logging_SRR5_V4.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V4/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V5	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V5/Z7A_Logging_SRR5_V5.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V5/Z7A_Logging_SRR5_V5.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V5/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V7	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V7/Z7A_Logging_SRR5_V7.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V7/Z7A_Logging_SRR5_V7.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V7/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")										
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V8	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V8/Z7A_Logging_SRR5_V8.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V8/Z7A_Logging_SRR5_V8.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V8/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")	
																	  
list(APPEND Z7A_SRR5_CORE_RNA_210_V9	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V9/Z7A_Logging_SRR5_V9.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V9/Z7A_Logging_SRR5_V9.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V9/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")										

list(APPEND Z7A_SRR5_CORE_RNA_210_V11	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V11/Z7A_Logging_SRR5_V11.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V11/Z7A_Logging_SRR5_V11.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V11/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")										

list(APPEND Z7A_SRR5_CORE_RNA_210_V12	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V12/Z7A_Logging_SRR5_V12.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V12/Z7A_Logging_SRR5_V12.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V12/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")

list(APPEND Z7A_SRR5_CORE_RNA_210_V13	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V13/Z7A_Logging_SRR5_V13.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V13/Z7A_Logging_SRR5_V13.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V13/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")

list(APPEND Z7A_SRR5_CORE_RNA_210_V14	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V14/Z7A_Logging_SRR5_V14.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V14/Z7A_Logging_SRR5_V14.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V14/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")

list(APPEND Z7A_SRR5_CORE_RNA_210_V15	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V15/Z7A_Logging_SRR5_V15.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V15/Z7A_Logging_SRR5_V15.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V15/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
										
list(APPEND Z7A_SRR5_CORE_RNA_210_V16	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V16/Z7A_Logging_SRR5_V16.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V16/Z7A_Logging_SRR5_V16.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V16/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
										
list(APPEND Z7A_SRR5_CORE_RNA_210_V17	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V17/Z7A_Logging_SRR5_V17.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V17/Z7A_Logging_SRR5_V17.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V17/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
										
list(APPEND Z7A_SRR5_CORE_RNA_210_V18	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V18/Z7A_Logging_SRR5_V18.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V18/Z7A_Logging_SRR5_V18.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V18/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")
										
list(APPEND Z7A_SRR5_CORE_RNA_210_V19	"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V19/Z7A_Logging_SRR5_V19.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V19/Z7A_Logging_SRR5_V19.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21/210/V19/Z7A_Z7B_IPC_SRR5_210RBIN_CORE.h")

list(APPEND Z7A_SRR5_CORE_PSA_V1		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_SAT_CORE_31/V1/Z7A_Logging_PSA_SRR5_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_SAT_CORE_31/V1/Z7A_Logging_PSA_SRR5_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_SAT_CORE_31/V1/Z7A_Z7B_IPC_SRR5_SAT_CORE.h")										

list(APPEND Z7A_SRR5_CORE_PSA_V2		"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_SAT_CORE_31/V2/Z7A_Logging_PSA_SRR5_V2.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_SAT_CORE_31/V2/Z7A_Logging_PSA_SRR5_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7A/Z7A_Z7B_IPC_SRR5_SAT_CORE_31/V2/Z7A_Z7B_IPC_SRR5_SAT_CORE.h")										

list(APPEND Z7A_MRR_PSA_V1				"${SourceFiles}/SRR5_CORE/Z7A/MRR/Z7A_Z7B_IPC_MRR_CORE_32/V1/Z7A_Logging_PSA_MRR_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/MRR/Z7A_Z7B_IPC_MRR_CORE_32/V1/Z7A_Logging_PSA_MRR_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7A/MRR/Z7A_Z7B_IPC_MRR_CORE_32/V1/Z7A_Z7B_IPC_MRR_CORE.h")										

list(APPEND Z7A_MRR_PSA_V2				"${SourceFiles}/SRR5_CORE/Z7A/MRR/Z7A_Z7B_IPC_MRR_CORE_32/V2/Z7A_Logging_PSA_MRR_V2.cpp"
										"${SourceFiles}/SRR5_CORE/Z7A/MRR/Z7A_Z7B_IPC_MRR_CORE_32/V2/Z7A_Logging_PSA_MRR_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7A/MRR/Z7A_Z7B_IPC_MRR_CORE_32/V2/Z7A_Z7B_IPC_MRR_CORE.h")										

						
#Grouping SRR5_CORE Z7A SRR5_CORE Source file
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V1  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V1})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V2  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V2})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V3  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V3})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V4  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V4})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V5  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V5})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V6  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V6})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V7  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V7})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V8  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V8})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V9  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V9})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V10  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V10})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V11  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V11})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V12  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V12})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V13  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V13})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V14  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V14})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_150D_CORE_28\\V15  FILES ${Z7A_SRR5_CORE_BMW_BPIL_V15})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V1   FILES ${Z7A_SRR5_CORE_BMW_LOW_V1})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V2   FILES ${Z7A_SRR5_CORE_BMW_LOW_V2})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V3   FILES ${Z7A_SRR5_CORE_BMW_LOW_V3})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V4   FILES ${Z7A_SRR5_CORE_BMW_LOW_V4})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V5   FILES ${Z7A_SRR5_CORE_BMW_LOW_V5})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V6   FILES ${Z7A_SRR5_CORE_BMW_LOW_V6})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V7   FILES ${Z7A_SRR5_CORE_BMW_LOW_V7})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V8   FILES ${Z7A_SRR5_CORE_BMW_LOW_V8})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V9   FILES ${Z7A_SRR5_CORE_BMW_LOW_V9})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V10   FILES ${Z7A_SRR5_CORE_BMW_LOW_V10})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V11   FILES ${Z7A_SRR5_CORE_BMW_LOW_V11})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V12   FILES ${Z7A_SRR5_CORE_BMW_LOW_V12})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V13   FILES ${Z7A_SRR5_CORE_BMW_LOW_V13})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V14   FILES ${Z7A_SRR5_CORE_BMW_LOW_V14})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V15   FILES ${Z7A_SRR5_CORE_BMW_LOW_V15})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V16   FILES ${Z7A_SRR5_CORE_BMW_LOW_V16})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V17   FILES ${Z7A_SRR5_CORE_BMW_LOW_V17})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V18   FILES ${Z7A_SRR5_CORE_BMW_LOW_V18})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V19   FILES ${Z7A_SRR5_CORE_BMW_LOW_V19})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_64D_CORE_27\\V20   FILES ${Z7A_SRR5_CORE_BMW_LOW_V20})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V1   FILES ${Z7A_SRR5_CORE_BMW_MID_V1})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V2   FILES ${Z7A_SRR5_CORE_BMW_MID_V2})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V3   FILES ${Z7A_SRR5_CORE_BMW_MID_V3})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V4   FILES ${Z7A_SRR5_CORE_BMW_MID_V4})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V5   FILES ${Z7A_SRR5_CORE_BMW_MID_V5})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V6   FILES ${Z7A_SRR5_CORE_BMW_MID_V6})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V7   FILES ${Z7A_SRR5_CORE_BMW_MID_V7})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V8   FILES ${Z7A_SRR5_CORE_BMW_MID_V8})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V9   FILES ${Z7A_SRR5_CORE_BMW_MID_V9})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V10   FILES ${Z7A_SRR5_CORE_BMW_MID_V10})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V11   FILES ${Z7A_SRR5_CORE_BMW_MID_V11})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V12   FILES ${Z7A_SRR5_CORE_BMW_MID_V12})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V13   FILES ${Z7A_SRR5_CORE_BMW_MID_V13})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V14   FILES ${Z7A_SRR5_CORE_BMW_MID_V14})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V15   FILES ${Z7A_SRR5_CORE_BMW_MID_V15})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V16   FILES ${Z7A_SRR5_CORE_BMW_MID_V16})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V17   FILES ${Z7A_SRR5_CORE_BMW_MID_V17})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V18   FILES ${Z7A_SRR5_CORE_BMW_MID_V18})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V19   FILES ${Z7A_SRR5_CORE_BMW_MID_V19})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_SRR5P_116RBIN_200D_CORE_29\\V20   FILES ${Z7A_SRR5_CORE_BMW_MID_V20})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V1      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V1})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V2      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V2})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V3      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V3})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V4      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V4})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V5      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V5})	
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V6      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V6})			
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V7      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V7})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V8      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V8})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V9      FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V9})	
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V10     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V10})	
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V11     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V11})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V12     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V12})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V13     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V13})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V14     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V14})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V15     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V15})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V16     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V16})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V17     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V17})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V18     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V18})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V19     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V19})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_116RBIN_CORE_20\\V20     FILES ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V20})						
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V1  FILES ${Z7A_SRR5_CORE_RNA_210_V1})	
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V2  FILES ${Z7A_SRR5_CORE_RNA_210_V2})	
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V3  FILES ${Z7A_SRR5_CORE_RNA_210_V3})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V4  FILES ${Z7A_SRR5_CORE_RNA_210_V4})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V5  FILES ${Z7A_SRR5_CORE_RNA_210_V5})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V7  FILES ${Z7A_SRR5_CORE_RNA_210_V7})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V8  FILES ${Z7A_SRR5_CORE_RNA_210_V8})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V9  FILES ${Z7A_SRR5_CORE_RNA_210_V9})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V11  FILES ${Z7A_SRR5_CORE_RNA_210_V11})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V12  FILES ${Z7A_SRR5_CORE_RNA_210_V12})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V13  FILES ${Z7A_SRR5_CORE_RNA_210_V13})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V14  FILES ${Z7A_SRR5_CORE_RNA_210_V14})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V15  FILES ${Z7A_SRR5_CORE_RNA_210_V15})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V16  FILES ${Z7A_SRR5_CORE_RNA_210_V16})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V17  FILES ${Z7A_SRR5_CORE_RNA_210_V17})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V18  FILES ${Z7A_SRR5_CORE_RNA_210_V18})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_210RBIN_CORE_21\\210\\V19  FILES ${Z7A_SRR5_CORE_RNA_210_V19})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_SAT_CORE_31\\V1		    FILES ${Z7A_SRR5_CORE_PSA_V1})
source_group(\\SRR5_CORE\\Z7A\\Z7A_Z7B_IPC_SRR5_SAT_CORE_31\\V2		    FILES ${Z7A_SRR5_CORE_PSA_V2})
source_group(\\SRR5_CORE\\Z7A\\MRR\\Z7A_Z7B_IPC_MRR_CORE_32\\V1     FILES ${Z7A_MRR_PSA_V1})
source_group(\\SRR5_CORE\\Z7A\\MRR\\Z7A_Z7B_IPC_MRR_CORE_32\\V2     FILES ${Z7A_MRR_PSA_V2})

#SRR5_CORE Z7B Source files
list(APPEND Z7BSOURCE "${SourceFiles}/SRR5_CORE/Z7B/Z7B_Logging.cpp"
						"${SourceFiles}/SRR5_CORE/Z7B/Z7B_Logging.h")		

#Grouping Z7B Source files
source_group(\\SRR5_CORE\\Z7B FILES ${Z7BSOURCE})		

#SRR5_CORE Z7B SRR5_CORE source file
list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V1	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V1/Z7B_SRR5_BMW_BPIL_Logging_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V1/Z7B_SRR5_BMW_BPIL_Logging_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V1/Z7B_BPIL_150D_64T_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V2	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V2/Z7B_SRR5_BMW_BPIL_Logging_V2.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V2/Z7B_SRR5_BMW_BPIL_Logging_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V2/Z7B_BPIL_150D_64T_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V3	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V3/Z7B_SRR5_BMW_BPIL_Logging_V3.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V3/Z7B_SRR5_BMW_BPIL_Logging_V3.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V3/Z7B_BPIL_150D_64T_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V4	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V4/Z7B_SRR5_BMW_BPIL_Logging_V4.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V4/Z7B_SRR5_BMW_BPIL_Logging_V4.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V4/Z7B_BPIL_150D_64T_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V5	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V5/Z7B_SRR5_BMW_BPIL_Logging_V5.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V5/Z7B_SRR5_BMW_BPIL_Logging_V5.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V5/Z7B_BPIL_150D_64T_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V6	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V6/Z7B_SRR5_BMW_BPIL_Logging_V6.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V6/Z7B_SRR5_BMW_BPIL_Logging_V6.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V6/Z7B_BPIL_150D_64T_CORE.h")										
										
list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V7	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V7/Z7B_SRR5_BMW_BPIL_Logging_V7.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V7/Z7B_SRR5_BMW_BPIL_Logging_V7.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V7/Z7B_BPIL_150D_64T_CORE.h")																	
										
list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V8	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V8/Z7B_SRR5_BMW_BPIL_Logging_V8.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V8/Z7B_SRR5_BMW_BPIL_Logging_V8.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V8/Z7B_BPIL_150D_64T_CORE.h")										

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V9	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V9/Z7B_SRR5_BMW_BPIL_Logging_V9.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V9/Z7B_SRR5_BMW_BPIL_Logging_V9.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V9/Z7B_BPIL_150D_64T_CORE.h")										

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V10	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V10/Z7B_SRR5_BMW_BPIL_Logging_V10.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V10/Z7B_SRR5_BMW_BPIL_Logging_V10.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V10/Z7B_BPIL_150D_64T_CORE.h")										

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V11	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V11/Z7B_SRR5_BMW_BPIL_Logging_V11.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V11/Z7B_SRR5_BMW_BPIL_Logging_V11.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V11/Z7B_BPIL_150D_64T_CORE.h")										

list(APPEND Z7B_SRR5_CORE_BMW_BPIL_V12	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V12/Z7B_SRR5_BMW_BPIL_Logging_V12.cpp"
			                            "${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V12/Z7B_SRR5_BMW_BPIL_Logging_V12.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_BPIL_150D_64T_CORE_45/V12/Z7B_BPIL_150D_64T_CORE.h")										
										
list(APPEND Z7B_SRR5_CORE_BMW_MID_V1	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V1/Z7B_SRR5_BMW_MID_Logging_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V1/Z7B_SRR5_BMW_MID_Logging_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V1/Z7B_SIDE_200D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_MID_V2	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V2/Z7B_SRR5_BMW_MID_Logging_V2.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V2/Z7B_SRR5_BMW_MID_Logging_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V2/Z7B_SIDE_200D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_MID_V3	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V3/Z7B_SRR5_BMW_MID_Logging_V3.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V3/Z7B_SRR5_BMW_MID_Logging_V3.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V3/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V4	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V4/Z7B_SRR5_BMW_MID_Logging_V4.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V4/Z7B_SRR5_BMW_MID_Logging_V4.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V4/Z7B_SIDE_200D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_MID_V5	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V5/Z7B_SRR5_BMW_MID_Logging_V5.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V5/Z7B_SRR5_BMW_MID_Logging_V5.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V5/Z7B_SIDE_200D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_MID_V6	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V6/Z7B_SRR5_BMW_MID_Logging_V6.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V6/Z7B_SRR5_BMW_MID_Logging_V6.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V6/Z7B_SIDE_200D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_BMW_MID_V7	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V7/Z7B_SRR5_BMW_MID_Logging_V7.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V7/Z7B_SRR5_BMW_MID_Logging_V7.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V7/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V8	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V8/Z7B_SRR5_BMW_MID_Logging_V8.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V8/Z7B_SRR5_BMW_MID_Logging_V8.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V8/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V9	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V9/Z7B_SRR5_BMW_MID_Logging_V9.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V9/Z7B_SRR5_BMW_MID_Logging_V9.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V9/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V10	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V10/Z7B_SRR5_BMW_MID_Logging_V10.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V10/Z7B_SRR5_BMW_MID_Logging_V10.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V10/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V11	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V11/Z7B_SRR5_BMW_MID_Logging_V11.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V11/Z7B_SRR5_BMW_MID_Logging_V11.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V11/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V12	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V12/Z7B_SRR5_BMW_MID_Logging_V12.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V12/Z7B_SRR5_BMW_MID_Logging_V12.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V12/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V13	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V13/Z7B_SRR5_BMW_MID_Logging_V13.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V13/Z7B_SRR5_BMW_MID_Logging_V13.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V13/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V14	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V14/Z7B_SRR5_BMW_MID_Logging_V14.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V14/Z7B_SRR5_BMW_MID_Logging_V14.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V14/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V15	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V15/Z7B_SRR5_BMW_MID_Logging_V15.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V15/Z7B_SRR5_BMW_MID_Logging_V15.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V15/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V16	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V16/Z7B_SRR5_BMW_MID_Logging_V16.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V16/Z7B_SRR5_BMW_MID_Logging_V16.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V16/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_BMW_MID_V17	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V17/Z7B_SRR5_BMW_MID_Logging_V17.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V17/Z7B_SRR5_BMW_MID_Logging_V17.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SIDE_200D_SAT_CORE_41/V17/Z7B_SIDE_200D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_GEELY_V1		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V1/Z7B_SRR5_GEELY_Logging_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V1/Z7B_SRR5_GEELY_Logging_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V1/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_GEELY_V2		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V2/Z7B_SRR5_GEELY_Logging_V2.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V2/Z7B_SRR5_GEELY_Logging_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V2/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_GEELY_V3		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V3/Z7B_SRR5_GEELY_Logging_V3.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V3/Z7B_SRR5_GEELY_Logging_V3.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V3/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_GEELY_V4		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V4/Z7B_SRR5_GEELY_Logging_V4.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V4/Z7B_SRR5_GEELY_Logging_V4.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V4/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_GEELY_V5		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V5/Z7B_SRR5_GEELY_Logging_V5.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V5/Z7B_SRR5_GEELY_Logging_V5.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V5/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_GEELY_V6		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V6/Z7B_SRR5_GEELY_Logging_V6.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V6/Z7B_SRR5_GEELY_Logging_V6.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V6/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_GEELY_V7		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V7/Z7B_SRR5_GEELY_Logging_V7.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V7/Z7B_SRR5_GEELY_Logging_V7.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V7/Z7B_FRNT_128D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_GEELY_V8		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V8/Z7B_SRR5_GEELY_Logging_V8.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V8/Z7B_SRR5_GEELY_Logging_V8.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V8/Z7B_FRNT_128D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_GEELY_V9		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V9/Z7B_SRR5_GEELY_Logging_V9.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V9/Z7B_SRR5_GEELY_Logging_V9.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V9/Z7B_FRNT_128D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_GEELY_V10		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V10/Z7B_SRR5_GEELY_Logging_V10.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V10/Z7B_SRR5_GEELY_Logging_V10.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V10/Z7B_FRNT_128D_SAT_CORE.h")

list(APPEND Z7B_SRR5_CORE_GEELY_V11		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V11/Z7B_SRR5_GEELY_Logging_V11.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V11/Z7B_SRR5_GEELY_Logging_V11.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_128D_SAT_CORE_42/V11/Z7B_FRNT_128D_SAT_CORE.h")
										
list(APPEND Z7B_SRR5_CORE_CHANGAN_V1	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V1/Z7B_SRR5_CHANGAN_Logging_V1.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V1/Z7B_SRR5_CHANGAN_Logging_V1.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V1/Z7B_FRNT_64SD_64FD_SAT_CORE.h")										

list(APPEND Z7B_SRR5_CORE_CHANGAN_V2	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V2/Z7B_SRR5_CHANGAN_Logging_V2.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V2/Z7B_SRR5_CHANGAN_Logging_V2.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V2/Z7B_FRNT_64SD_64FD_SAT_CORE.h")											

list(APPEND Z7B_SRR5_CORE_CHANGAN_V3	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V3/Z7B_SRR5_CHANGAN_Logging_V3.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V3/Z7B_SRR5_CHANGAN_Logging_V3.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V3/Z7B_FRNT_64SD_64FD_SAT_CORE.h")											

list(APPEND Z7B_SRR5_CORE_CHANGAN_V4	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V4/Z7B_SRR5_CHANGAN_Logging_V4.cpp"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V4/Z7B_SRR5_CHANGAN_Logging_V4.h"
										"${SourceFiles}/SRR5_CORE/Z7B/Z7B_FRNT_64SD_64FD_SAT_CORE_46/V4/Z7B_FRNT_64SD_64FD_SAT_CORE.h")											

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V1		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V1/Z7B_SRR5_Logging_V1.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V1/Z7B_SRR5_Logging_V1.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V1/Z7B_64SD_64FD_64TRK_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V2		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V2/Z7B_SRR5_Logging_V2.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V2/Z7B_SRR5_Logging_V2.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V2/Z7B_64SD_64FD_64TRK_STAND_CORE.h")							
													
list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V3		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V3/Z7B_SRR5_Logging_V3.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V3/Z7B_SRR5_Logging_V3.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V3/Z7B_64SD_64FD_64TRK_STAND_CORE.h")									
													
list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V4		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V4/Z7B_SRR5_Logging_V4.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V4/Z7B_SRR5_Logging_V4.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V4/Z7B_64SD_64FD_64TRK_STAND_CORE.h")
													
list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V5		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V5/Z7B_SRR5_Logging_V5.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V5/Z7B_SRR5_Logging_V5.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V5/Z7B_64SD_64FD_64TRK_STAND_CORE.h")				

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V6		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V6/Z7B_SRR5_Logging_V6.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V6/Z7B_SRR5_Logging_V6.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V6/Z7B_64SD_64FD_64TRK_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V7		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V7/Z7B_SRR5_Logging_V7.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V7/Z7B_SRR5_Logging_V7.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V7/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V8		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V8/Z7B_SRR5_Logging_V8.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V8/Z7B_SRR5_Logging_V8.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V8/Z7B_64SD_64FD_64TRK_STAND_CORE.h")	

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V9		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V9/Z7B_SRR5_Logging_V9.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V9/Z7B_SRR5_Logging_V9.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V9/Z7B_64SD_64FD_64TRK_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V10		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V10/Z7B_SRR5_Logging_V10.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V10/Z7B_SRR5_Logging_V10.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V10/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V11		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V11/Z7B_SRR5_Logging_V11.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V11/Z7B_SRR5_Logging_V11.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V11/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V12		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V12/Z7B_SRR5_Logging_V12.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V12/Z7B_SRR5_Logging_V12.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V12/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V13		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V13/Z7B_SRR5_Logging_V13.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V13/Z7B_SRR5_Logging_V13.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V13/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V14		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V14/Z7B_SRR5_Logging_V14.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V14/Z7B_SRR5_Logging_V14.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V14/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V15		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V15/Z7B_SRR5_Logging_V15.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V15/Z7B_SRR5_Logging_V15.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V15/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V16		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V16/Z7B_SRR5_Logging_V16.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V16/Z7B_SRR5_Logging_V16.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V16/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V17		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V17/Z7B_SRR5_Logging_V17.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V17/Z7B_SRR5_Logging_V17.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V17/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V18		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V18/Z7B_SRR5_Logging_V18.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V18/Z7B_SRR5_Logging_V18.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V18/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V19		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V19/Z7B_SRR5_Logging_V19.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V19/Z7B_SRR5_Logging_V19.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V19/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V20		"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V20/Z7B_SRR5_Logging_V20.cpp"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V20/Z7B_SRR5_Logging_V20.h"
													"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64TRK_STAND_CORE_43/V20/Z7B_64SD_64FD_64TRK_STAND_CORE.h")													

list(APPEND Z7B_SRR5_CORE_RNA_V1	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V1/Z7B_SRR5_RNA_Logging_V1.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V1/Z7B_SRR5_RNA_Logging_V1.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V1/Z7B_64SD_64T_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_RNA_V2	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V2/Z7B_SRR5_RNA_Logging_V2.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V2/Z7B_SRR5_RNA_Logging_V2.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V2/Z7B_64SD_64T_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_RNA_V3	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V3/Z7B_SRR5_RNA_Logging_V3.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V3/Z7B_SRR5_RNA_Logging_V3.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V3/Z7B_64SD_64T_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_RNA_V4	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V4/Z7B_SRR5_RNA_Logging_V4.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V4/Z7B_SRR5_RNA_Logging_V4.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V4/Z7B_64SD_64T_STAND_CORE.h")


list(APPEND Z7B_SRR5_CORE_RNA_V5	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V5/Z7B_SRR5_RNA_Logging_V5.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V5/Z7B_SRR5_RNA_Logging_V5.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V5/Z7B_64SD_64T_STAND_CORE.h")									


list(APPEND Z7B_SRR5_CORE_RNA_V6	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V6/Z7B_SRR5_RNA_Logging_V6.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V6/Z7B_SRR5_RNA_Logging_V6.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V6/Z7B_64SD_64T_STAND_CORE.h")
									
list(APPEND Z7B_SRR5_CORE_RNA_V7	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V7/Z7B_SRR5_RNA_Logging_V7.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V7/Z7B_SRR5_RNA_Logging_V7.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V7/Z7B_64SD_64T_STAND_CORE.h")									
list(APPEND Z7B_SRR5_CORE_RNA_V8	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V8/Z7B_SRR5_RNA_Logging_V8.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V8/Z7B_SRR5_RNA_Logging_V8.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V8/Z7B_64SD_64T_STAND_CORE.h")
									
list(APPEND Z7B_SRR5_CORE_RNA_V9	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V9/Z7B_SRR5_RNA_Logging_V9.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V9/Z7B_SRR5_RNA_Logging_V9.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V9/Z7B_64SD_64T_STAND_CORE.h")	

list(APPEND Z7B_SRR5_CORE_RNA_V10	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V10/Z7B_SRR5_RNA_Logging_V10.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V10/Z7B_SRR5_RNA_Logging_V10.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V10/Z7B_64SD_64T_STAND_CORE.h")									

list(APPEND Z7B_SRR5_CORE_RNA_V11	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V11/Z7B_SRR5_RNA_Logging_V11.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V11/Z7B_SRR5_RNA_Logging_V11.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V11/Z7B_64SD_64T_STAND_CORE.h")									

list(APPEND Z7B_SRR5_CORE_RNA_V12	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V12/Z7B_SRR5_RNA_Logging_V12.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V12/Z7B_SRR5_RNA_Logging_V12.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V12/Z7B_64SD_64T_STAND_CORE.h")

list(APPEND Z7B_SRR5_CORE_RNA_V13	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V13/Z7B_SRR5_RNA_Logging_V13.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V13/Z7B_SRR5_RNA_Logging_V13.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64T_STAND_CORE_44/V13/Z7B_64SD_64T_STAND_CORE.h")									
																		
list(APPEND Z7B_SRR5_CORE_PSA_V1	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SRR5_SAT_CORE_47/V1/Z7B_SRR5_PSA_Logging_V1.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SRR5_SAT_CORE_47/V1/Z7B_SRR5_PSA_Logging_V1.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SRR5_SAT_CORE_47/V1/Z7B_SRR5_SAT_CORE.h")									

list(APPEND Z7B_SRR5_CORE_PSA_V2	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SRR5_SAT_CORE_47/V2/Z7B_SRR5_PSA_Logging_V2.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SRR5_SAT_CORE_47/V2/Z7B_SRR5_PSA_Logging_V2.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_SRR5_SAT_CORE_47/V2/Z7B_SRR5_SAT_CORE.h")									

list(APPEND Z7B_MRR_PSA_V1			"${SourceFiles}/SRR5_CORE/Z7B/MRR/Z7B_MRR_SAT_CORE_48/V1/Z7B_MRR_PSA_Logging_V1.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/MRR/Z7B_MRR_SAT_CORE_48/V1/Z7B_MRR_PSA_Logging_V1.h"
									"${SourceFiles}/SRR5_CORE/Z7B/MRR/Z7B_MRR_SAT_CORE_48/V1/Z7B_MRR_SAT_CORE.h")								

list(APPEND Z7B_SRR5_CORE_HKMC_V1	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V1/Z7B_SRR5_HKMC_Logging_V1.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V1/Z7B_SRR5_HKMC_Logging_V1.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V1/Z7B_64SD_64FD_64OBJ_STAND_CORE.h")										

list(APPEND Z7B_SRR5_CORE_HKMC_V2	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V2/Z7B_SRR5_HKMC_Logging_V2.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V2/Z7B_SRR5_HKMC_Logging_V2.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V2/Z7B_64SD_64FD_64OBJ_STAND_CORE.h")										

list(APPEND Z7B_SRR5_CORE_HKMC_V3	"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V3/Z7B_SRR5_HKMC_Logging_V3.cpp"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V3/Z7B_SRR5_HKMC_Logging_V3.h"
									"${SourceFiles}/SRR5_CORE/Z7B/Z7B_64SD_64FD_64OBJ_STAND_CORE_49/V3/Z7B_64SD_64FD_64OBJ_STAND_CORE.h")										
						
#Grouping SRR5_CORE Z7B SRR5_CORE Source file
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V1			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V2			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V3			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V3})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V4			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V4})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V5			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V5})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V6			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V6})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V7			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V7})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V8			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V8})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V9			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V9})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V10			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V10})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V11			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V11})
source_group(\\SRR5_CORE\\Z7B\\Z7B_BPIL_150D_64T_CORE_45\\V12			FILES ${Z7B_SRR5_CORE_BMW_BPIL_V12})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V1			FILES ${Z7B_SRR5_CORE_BMW_MID_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V2			FILES ${Z7B_SRR5_CORE_BMW_MID_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V3			FILES ${Z7B_SRR5_CORE_BMW_MID_V3})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V4			FILES ${Z7B_SRR5_CORE_BMW_MID_V4})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V5			FILES ${Z7B_SRR5_CORE_BMW_MID_V5})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V6			FILES ${Z7B_SRR5_CORE_BMW_MID_V6})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V7			FILES ${Z7B_SRR5_CORE_BMW_MID_V7})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V8			FILES ${Z7B_SRR5_CORE_BMW_MID_V8})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V9			FILES ${Z7B_SRR5_CORE_BMW_MID_V9})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V10			FILES ${Z7B_SRR5_CORE_BMW_MID_V10})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V11			FILES ${Z7B_SRR5_CORE_BMW_MID_V11})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V12			FILES ${Z7B_SRR5_CORE_BMW_MID_V12})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V13			FILES ${Z7B_SRR5_CORE_BMW_MID_V13})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V14			FILES ${Z7B_SRR5_CORE_BMW_MID_V14})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V15			FILES ${Z7B_SRR5_CORE_BMW_MID_V15})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V16			FILES ${Z7B_SRR5_CORE_BMW_MID_V16})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SIDE_200D_SAT_CORE_41\\V17			FILES ${Z7B_SRR5_CORE_BMW_MID_V17})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V1				FILES ${Z7B_SRR5_CORE_GEELY_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V2				FILES ${Z7B_SRR5_CORE_GEELY_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V3				FILES ${Z7B_SRR5_CORE_GEELY_V3})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V4				FILES ${Z7B_SRR5_CORE_GEELY_V4})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V5				FILES ${Z7B_SRR5_CORE_GEELY_V5})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V6				FILES ${Z7B_SRR5_CORE_GEELY_V6})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V7				FILES ${Z7B_SRR5_CORE_GEELY_V7})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V8				FILES ${Z7B_SRR5_CORE_GEELY_V8})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V9				FILES ${Z7B_SRR5_CORE_GEELY_V9})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V10				FILES ${Z7B_SRR5_CORE_GEELY_V10})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_128D_SAT_CORE_42\\V11				FILES ${Z7B_SRR5_CORE_GEELY_V11})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_64SD_64FD_SAT_CORE_46\\V1			FILES ${Z7B_SRR5_CORE_CHANGAN_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_64SD_64FD_SAT_CORE_46\\V2			FILES ${Z7B_SRR5_CORE_CHANGAN_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_64SD_64FD_SAT_CORE_46\\V3			FILES ${Z7B_SRR5_CORE_CHANGAN_V3})
source_group(\\SRR5_CORE\\Z7B\\Z7B_FRNT_64SD_64FD_SAT_CORE_46\\V4			FILES ${Z7B_SRR5_CORE_CHANGAN_V4})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V1	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V2	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V2})	
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V3 	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V3})	
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V4	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V4})	
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V5	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V5})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V6	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V6})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V7	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V7})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V8	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V8})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V9	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V9})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V10	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V10})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V11	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V11})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V12	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V12})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V13	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V13})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V14	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V14})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V15	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V15})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V16	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V16})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V17	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V17})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V18	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V18})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V19	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V19})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64TRK_STAND_CORE_43\\V20	FILES ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V20})	
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V1				FILES ${Z7B_SRR5_CORE_RNA_V1})	
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V2				FILES ${Z7B_SRR5_CORE_RNA_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V3				FILES ${Z7B_SRR5_CORE_RNA_V3})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V4				FILES ${Z7B_SRR5_CORE_RNA_V4})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V5				FILES ${Z7B_SRR5_CORE_RNA_V5})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V6				FILES ${Z7B_SRR5_CORE_RNA_V6})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V7				FILES ${Z7B_SRR5_CORE_RNA_V7})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V8				FILES ${Z7B_SRR5_CORE_RNA_V8})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V9				FILES ${Z7B_SRR5_CORE_RNA_V9})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V10				FILES ${Z7B_SRR5_CORE_RNA_V10})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V11				FILES ${Z7B_SRR5_CORE_RNA_V11})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V12				FILES ${Z7B_SRR5_CORE_RNA_V12})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64T_STAND_CORE_44\\V13				FILES ${Z7B_SRR5_CORE_RNA_V13})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SRR5_SAT_CORE_47\\V1						FILES ${Z7B_SRR5_CORE_PSA_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_SRR5_SAT_CORE_47\\V2						FILES ${Z7B_SRR5_CORE_PSA_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64OBJ_STAND_CORE_49\\V1		FILES ${Z7B_SRR5_CORE_HKMC_V1})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64OBJ_STAND_CORE_49\\V2		FILES ${Z7B_SRR5_CORE_HKMC_V2})
source_group(\\SRR5_CORE\\Z7B\\Z7B_64SD_64FD_64OBJ_STAND_CORE_49\\V3		FILES ${Z7B_SRR5_CORE_HKMC_V3})
source_group(\\SRR5_CORE\\Z7B\\MRR\\Z7B_MRR_SAT_CORE_48\\V1					FILES ${Z7B_MRR_PSA_V1})

#SRR5_CORE Z7B_TRACKER_OUTPUT source file
list(APPEND Z7B_TRACKER_OUTPUT "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/Z7B_Tracker_Output.cpp"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/Z7B_Tracker_Output.h")						

list(APPEND Z7B_TRACKER_OUTPUT_V1 "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V1/z7b_Tracker_Output_Structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V1/Z7B_Tracker_Output_V1.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V1/Z7B_Tracker_Output_V1.h")
						
list(APPEND Z7B_TRACKER_OUTPUT_V2 "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V2/z7b_Tracker_Output_Structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V2/Z7B_Tracker_Output_V2.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V2/Z7B_Tracker_Output_V2.h")
						
list(APPEND Z7B_TRACKER_OUTPUT_V3 "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V3/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V3/Z7B_Tracker_Output_V3.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V3/Z7B_Tracker_Output_V3.h")

list(APPEND Z7B_TRACKER_OUTPUT_V4 "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V4/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V4/Z7B_Tracker_Output_V4.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V4/Z7B_Tracker_Output_V4.h")

list(APPEND Z7B_TRACKER_OUTPUT_V5 "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V5/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V5/Z7B_Tracker_Output_V5.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V5/Z7B_Tracker_Output_V5.h")

list(APPEND Z7B_TRACKER_OUTPUT_V6 "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V6/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V6/Z7B_Tracker_Output_V6.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V6/Z7B_Tracker_Output_V6.h")

list(APPEND Z7B_TRACKER_OUTPUT_V7  "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V7/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V7/Z7B_Tracker_Output_V7.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V7/Z7B_Tracker_Output_V7.h")
									
list(APPEND Z7B_TRACKER_OUTPUT_V8  "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V8/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V8/Z7B_Tracker_Output_V8.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V8/Z7B_Tracker_Output_V8.h")
									
list(APPEND Z7B_TRACKER_OUTPUT_V9  "${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V9/z7b_cust_tracker_output_structure.h"					
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V9/Z7B_Tracker_Output_V9.cpp"	
									"${SourceFiles}/SRR5_CORE/Z7B_TRACKER_OUTPUT/V9/Z7B_Tracker_Output_V9.h")
									
#Grouping Z7B Z7B_TRACKER_OUTPUT source file
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT FILES ${Z7B_TRACKER_OUTPUT})	
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V1 FILES ${Z7B_TRACKER_OUTPUT_V1})	
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V2 FILES ${Z7B_TRACKER_OUTPUT_V2})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V3 FILES ${Z7B_TRACKER_OUTPUT_V3})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V4 FILES ${Z7B_TRACKER_OUTPUT_V4})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V5 FILES ${Z7B_TRACKER_OUTPUT_V5})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V6 FILES ${Z7B_TRACKER_OUTPUT_V6})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V7 FILES ${Z7B_TRACKER_OUTPUT_V7})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V8 FILES ${Z7B_TRACKER_OUTPUT_V8})
source_group(\\SRR5_CORE\\Z7B_TRACKER_OUTPUT\\V9 FILES ${Z7B_TRACKER_OUTPUT_V9})


#SRR5_CORE DSPACE source file
list(APPEND DSPACE "${SourceFiles}/DSPACE/Dspace_Logging.cpp"					
				   "${SourceFiles}/DSPACE/Dspace_Logging.h")						

list(APPEND SCANIA_V7 "${SourceFiles}/DSPACE/SCANIA/V7/DSPACE_Input_Structure.h"					
					  "${SourceFiles}/DSPACE/SCANIA/V7/DSPACE_Logging_V7.cpp"	
					  "${SourceFiles}/DSPACE/SCANIA/V7/DSPACE_Logging_V7.h")

list(APPEND SCANIA_V8 "${SourceFiles}/DSPACE/SCANIA/V8/DSPACE_Input_Structure.h"					
					  "${SourceFiles}/DSPACE/SCANIA/V8/DSPACE_Logging_V8.cpp"	
					  "${SourceFiles}/DSPACE/SCANIA/V8/DSPACE_Logging_V8.h")
						

list(APPEND BMW_LOW_V2 "${SourceFiles}/DSPACE/BMW_LOW/V2/DSPACE_BMW_LOW_logging_structure.h"					
					   "${SourceFiles}/DSPACE/BMW_LOW/V2/DSPACE_BMW_LOW_Logging_V2.cpp"	
					   "${SourceFiles}/DSPACE/BMW_LOW/V2/DSPACE_BMW_LOW_Logging_V2.h")
					   

list(APPEND BMW_MID_V2 "${SourceFiles}/DSPACE/BMW_MID/V2/DSPACE_BMW_MID_logging_structure.h"					
					   "${SourceFiles}/DSPACE/BMW_MID/V2/DSPACE_BMW_MID_Logging_V2.cpp"	
					   "${SourceFiles}/DSPACE/BMW_MID/V2/DSPACE_BMW_MID_Logging_V2.h")
				
list(APPEND BMW_HIGH_V2 "${SourceFiles}/DSPACE/BMW_HIGH/V2/DSPACE_BMW_HIGH_logging_structure.h"					
					    "${SourceFiles}/DSPACE/BMW_HIGH/V2/DSPACE_BMW_HIGH_Logging_V2.cpp"	
					    "${SourceFiles}/DSPACE/BMW_HIGH/V2/DSPACE_BMW_HIGH_Logging_V2.h")

list(APPEND GEELY_V1 "${SourceFiles}/DSPACE/GEELY/V1/DSPACE_GEELY_logging_structure.h"					
					    "${SourceFiles}/DSPACE/GEELY/V1/DSPACE_GEELY_Logging_V1.cpp"	
					    "${SourceFiles}/DSPACE/GEELY/V1/DSPACE_GEELY_Logging_V1.h")
						
list(APPEND GEELY_V2 "${SourceFiles}/DSPACE/GEELY/V2/DSPACE_GEELY_logging_structure.h"					
					    "${SourceFiles}/DSPACE/GEELY/V2/DSPACE_GEELY_Logging_V2.cpp"	
					    "${SourceFiles}/DSPACE/GEELY/V2/DSPACE_GEELY_Logging_V2.h")
					   
					   
#Grouping DSPACE source file
source_group(\\DSPACE FILES ${DSPACE})	
source_group(\\DSPACE\\SCANIA\\V7 FILES ${SCANIA_V7})	
source_group(\\DSPACE\\SCANIA\\V8 FILES ${SCANIA_V8})		
source_group(\\DSPACE\\BMW_LOW\\V2 FILES ${BMW_LOW_V2})
source_group(\\DSPACE\\BMW_MID\\V2 FILES ${BMW_MID_V2})
source_group(\\DSPACE\\BMW_HIGH\\V2 FILES ${BMW_HIGH_V2})
source_group(\\DSPACE\\GEELY\\V1 FILES ${GEELY_V1})
source_group(\\DSPACE\\GEELY\\V2 FILES ${GEELY_V2})


# RADAR ECU Core 0 source file
list(APPEND ECU_Core0 	 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/ECU_C0_Logging.cpp"
						 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/ECU_C0_Logging.h")	

list(APPEND ECU_Core0_V4   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V4/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V4/ECU_CORE0_logging_V4.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V4/ECU_CORE0_logging_V4.h")
						   
list(APPEND ECU_Core0_V5   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V5/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V5/ECU_CORE0_logging_V5.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V5/ECU_CORE0_logging_V5.h")	
						   
list(APPEND ECU_Core0_V6   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V6/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V6/ECU_CORE0_logging_V6.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V6/ECU_CORE0_logging_V6.h")	
						   
list(APPEND ECU_Core0_V7   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V7/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V7/ECU_CORE0_logging_V7.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V7/ECU_CORE0_logging_V7.h")

list(APPEND ECU_Core0_V8   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V8/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V8/ECU_CORE0_logging_V8.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V8/ECU_CORE0_logging_V8.h")	
						   
list(APPEND ECU_Core0_V9   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V9/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V9/ECU_CORE0_logging_V9.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V9/ECU_CORE0_logging_V9.h")							   

list(APPEND ECU_Core0_V10   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V10/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V10/ECU_CORE0_logging_V10.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V10/ECU_CORE0_logging_V10.h")	

list(APPEND ECU_Core0_V11  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V11/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V11/ECU_CORE0_logging_V11.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V11/ECU_CORE0_logging_V11.h")	

list(APPEND ECU_Core0_V12  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V12/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V12/ECU_CORE0_logging_V12.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V12/ECU_CORE0_logging_V12.h")	

list(APPEND ECU_Core0_V13  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V13/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V13/ECU_CORE0_logging_V13.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V13/ECU_CORE0_logging_V13.h")	

list(APPEND ECU_Core0_V14  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V14/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V14/ECU_CORE0_logging_V14.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V14/ECU_CORE0_logging_V14.h")	

list(APPEND ECU_Core0_V15  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V15/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V15/ECU_CORE0_logging_V15.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V15/ECU_CORE0_logging_V15.h")	

list(APPEND ECU_Core0_V16  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V16/ECU_CORE0_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V16/ECU_CORE0_logging_V16.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE0_90/V16/ECU_CORE0_logging_V16.h")	

#Grouping RADARECU CORE 0 source file
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90 FILES ${ECU_Core0})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V4 FILES ${ECU_Core0_V4})	
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V5 FILES ${ECU_Core0_V5})	
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V6 FILES ${ECU_Core0_V6})				  
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V7 FILES ${ECU_Core0_V7})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V8 FILES ${ECU_Core0_V8})				  
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V9 FILES ${ECU_Core0_V9})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V10 FILES ${ECU_Core0_V10})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V11 FILES ${ECU_Core0_V11})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V12 FILES ${ECU_Core0_V12})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V13 FILES ${ECU_Core0_V13})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V14 FILES ${ECU_Core0_V14})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V15 FILES ${ECU_Core0_V15})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE0_90\\V16 FILES ${ECU_Core0_V16})		
# RADAR ECU Core 1 source file                                       
list(APPEND ECU_Core1 	 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/ECU_C1_Logging.cpp"
						 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/ECU_C1_Logging.h")	
                                                                     
list(APPEND ECU_Core1_V4   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V4/ECU_CORE1_logging_V4.cpp"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V4/ECU_CORE1_logging_structure.h"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V4/ECU_CORE1_logging_V4.h")

list(APPEND ECU_Core1_V5   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V5/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V5/ECU_CORE1_logging_V5.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V5/ECU_CORE1_logging_V5.h")
						   
list(APPEND ECU_Core1_V6   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V6/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V6/ECU_CORE1_logging_V6.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V6/ECU_CORE1_logging_V6.h")
						   
list(APPEND ECU_Core1_V7   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V7/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V7/ECU_CORE1_logging_V7.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V7/ECU_CORE1_logging_V7.h")
						   			   
list(APPEND ECU_Core1_V8   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V8/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V8/ECU_CORE1_logging_V8.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V8/ECU_CORE1_logging_V8.h")
						   
list(APPEND ECU_Core1_V9   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V9/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V9/ECU_CORE1_logging_V9.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V9/ECU_CORE1_logging_V9.h")

list(APPEND ECU_Core1_V10   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V10/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V10/ECU_CORE1_logging_V10.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V10/ECU_CORE1_logging_V10.h")

list(APPEND ECU_Core1_V11  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V11/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V11/ECU_CORE1_logging_V11.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V11/ECU_CORE1_logging_V11.h")

list(APPEND ECU_Core1_V12  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V12/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V12/ECU_CORE1_logging_V12.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V12/ECU_CORE1_logging_V12.h")

list(APPEND ECU_Core1_V13  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V13/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V13/ECU_CORE1_logging_V13.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V13/ECU_CORE1_logging_V13.h")

list(APPEND ECU_Core1_V14  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V14/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V14/ECU_CORE1_logging_V14.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V14/ECU_CORE1_logging_V14.h")

list(APPEND ECU_Core1_V15  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V15/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V15/ECU_CORE1_logging_V15.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V15/ECU_CORE1_logging_V15.h")

list(APPEND ECU_Core1_V16  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V16/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V16/ECU_CORE1_logging_V16.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V16/ECU_CORE1_logging_V16.h")

list(APPEND ECU_Core1_V17  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V17/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V17/ECU_CORE1_logging_V17.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V17/ECU_CORE1_logging_V17.h")

list(APPEND ECU_Core1_V18  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V18/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V18/ECU_CORE1_logging_V18.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V18/ECU_CORE1_logging_V18.h")

list(APPEND ECU_Core1_V19  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V19/ECU_CORE1_logging_structure.h"					
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V19/ECU_CORE1_logging_V19.cpp"	
						   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE1_91/V19/ECU_CORE1_logging_V19.h")
						   						   
#Grouping RADARECU CORE 1 source file
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91 FILES ${ECU_Core1})	
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V4 FILES ${ECU_Core1_V4})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V5 FILES ${ECU_Core1_V5})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V6 FILES ${ECU_Core1_V6})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V7 FILES ${ECU_Core1_V7})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V8 FILES ${ECU_Core1_V8})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V9 FILES ${ECU_Core1_V9})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V10 FILES ${ECU_Core1_V10})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V11 FILES ${ECU_Core1_V11})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V12 FILES ${ECU_Core1_V12})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V13 FILES ${ECU_Core1_V13})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V14 FILES ${ECU_Core1_V14})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V15 FILES ${ECU_Core1_V15})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V16 FILES ${ECU_Core1_V16})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V17 FILES ${ECU_Core1_V17})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V18 FILES ${ECU_Core1_V18})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE1_91\\V19 FILES ${ECU_Core1_V19})

# RADAR ECU Core 3 source file
list(APPEND ECU_Core3 	 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/ECU_C3_Logging.cpp"
						 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/ECU_C3_Logging.h")

list(APPEND ECU_Core3_V5  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V5/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V5/ECU_CORE3_logging_V5.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V5/ECU_CORE3_logging_V5.h")
						  
list(APPEND ECU_Core3_V6  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V6/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V6/ECU_CORE3_logging_V6.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V6/ECU_CORE3_logging_V6.h")

list(APPEND ECU_Core3_V7  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V7/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V7/ECU_CORE3_logging_V7.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V7/ECU_CORE3_logging_V7.h")	

list(APPEND ECU_Core3_V8  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V8/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V8/ECU_CORE3_logging_V8.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V8/ECU_CORE3_logging_V8.h")		

list(APPEND ECU_Core3_V9  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V9/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V9/ECU_CORE3_logging_V9.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V9/ECU_CORE3_logging_V9.h")	

list(APPEND ECU_Core3_V10  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V10/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V10/ECU_CORE3_logging_V10.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V10/ECU_CORE3_logging_V10.h")							  
						  
list(APPEND ECU_Core3_V11  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V11/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V11/ECU_CORE3_logging_V11.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V11/ECU_CORE3_logging_V11.h")

list(APPEND ECU_Core3_V12  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V12/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V12/ECU_CORE3_logging_V12.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V12/ECU_CORE3_logging_V12.h")
						  
list(APPEND ECU_Core3_V13  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V13/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V13/ECU_CORE3_logging_V13.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V13/ECU_CORE3_logging_V13.h")			

list(APPEND ECU_Core3_V14  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V14/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V14/ECU_CORE3_logging_V14.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V14/ECU_CORE3_logging_V14.h")	

list(APPEND ECU_Core3_V15  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V15/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V15/ECU_CORE3_logging_V15.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V15/ECU_CORE3_logging_V15.h")		

list(APPEND ECU_Core3_V16  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V16/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V16/ECU_CORE3_logging_V16.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V16/ECU_CORE3_logging_V16.h")		

list(APPEND ECU_Core3_V17  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V17/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V17/ECU_CORE3_logging_V17.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V17/ECU_CORE3_logging_V17.h")

list(APPEND ECU_Core3_V18  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V18/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V18/ECU_CORE3_logging_V18.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V18/ECU_CORE3_logging_V18.h")	

list(APPEND ECU_Core3_V19  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V19/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V19/ECU_CORE3_logging_V19.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V19/ECU_CORE3_logging_V19.h")						  

list(APPEND ECU_Core3_V20 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V20/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V20/ECU_CORE3_logging_V20.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V20/ECU_CORE3_logging_V20.h")						  

list(APPEND ECU_Core3_V21 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V21/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V21/ECU_CORE3_logging_V21.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V21/ECU_CORE3_logging_V21.h")						  

list(APPEND ECU_Core3_V22 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V22/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V22/ECU_CORE3_logging_V22.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V22/ECU_CORE3_logging_V22.h")						  

list(APPEND ECU_Core3_V23 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V23/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V23/ECU_CORE3_logging_V23.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V23/ECU_CORE3_logging_V23.h")						  

list(APPEND ECU_Core3_V24 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V24/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V24/ECU_CORE3_logging_V24.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V24/ECU_CORE3_logging_V24.h")						  

list(APPEND ECU_Core3_V25 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V25/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V25/ECU_CORE3_logging_V25.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V25/ECU_CORE3_logging_V25.h")						  

list(APPEND ECU_Core3_V26 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V26/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V26/ECU_CORE3_logging_V26.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V26/ECU_CORE3_logging_V26.h")						  

list(APPEND ECU_Core3_V27 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V27/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V27/ECU_CORE3_logging_V27.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V27/ECU_CORE3_logging_V27.h")						  

list(APPEND ECU_Core3_V28 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V28/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V28/ECU_CORE3_logging_V28.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V28/ECU_CORE3_logging_V28.h")						  

list(APPEND ECU_Core3_V29 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V29/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V29/ECU_CORE3_logging_V29.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V29/ECU_CORE3_logging_V29.h")						  

list(APPEND ECU_Core3_V30 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V30/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V30/ECU_CORE3_logging_V30.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V30/ECU_CORE3_logging_V30.h")						  

list(APPEND ECU_Core3_V31 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V31/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V31/ECU_CORE3_logging_V31.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V31/ECU_CORE3_logging_V31.h")						  

list(APPEND ECU_Core3_V32 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V32/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V32/ECU_CORE3_logging_V32.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V32/ECU_CORE3_logging_V32.h")						  

list(APPEND ECU_Core3_V33 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V33/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V33/ECU_CORE3_logging_V33.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V33/ECU_CORE3_logging_V33.h")						  

list(APPEND ECU_Core3_V34 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V34/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V34/ECU_CORE3_logging_V34.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V34/ECU_CORE3_logging_V34.h")						  

list(APPEND ECU_Core3_V35 "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V35/ECU_CORE3_logging_structure.h"					
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V35/ECU_CORE3_logging_V35.cpp"	
						  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_CORE3_92/V35/ECU_CORE3_logging_V35.h")						  
							  
#Grouping RADARECU CORE 0 source file
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92 FILES ${ECU_Core3})	
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V5 FILES ${ECU_Core3_V5})	
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V6 FILES ${ECU_Core3_V6})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V7 FILES ${ECU_Core3_V7})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V8 FILES ${ECU_Core3_V8})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V9 FILES ${ECU_Core3_V9})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V10 FILES ${ECU_Core3_V10})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V11 FILES ${ECU_Core3_V11})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V12 FILES ${ECU_Core3_V12})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V13 FILES ${ECU_Core3_V13})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V14 FILES ${ECU_Core3_V14})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V15 FILES ${ECU_Core3_V15})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V16 FILES ${ECU_Core3_V16})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V17 FILES ${ECU_Core3_V17})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V18 FILES ${ECU_Core3_V18})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V19 FILES ${ECU_Core3_V19})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V20 FILES ${ECU_Core3_V20})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V21 FILES ${ECU_Core3_V21})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V22 FILES ${ECU_Core3_V22})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V23 FILES ${ECU_Core3_V23})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V24 FILES ${ECU_Core3_V24})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V25 FILES ${ECU_Core3_V25})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V26 FILES ${ECU_Core3_V26})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V27 FILES ${ECU_Core3_V27})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V28 FILES ${ECU_Core3_V28})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V29 FILES ${ECU_Core3_V29})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V30 FILES ${ECU_Core3_V30})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V31 FILES ${ECU_Core3_V31})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V32 FILES ${ECU_Core3_V32})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V33 FILES ${ECU_Core3_V33})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V34 FILES ${ECU_Core3_V34})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_CORE3_92\\V35 FILES ${ECU_Core3_V35})

# RADAR ECU VRU_CLASSIFIER source file
list(APPEND ECU_VRU_CLASSIFIER 	"${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/ECU_VRU_CLASSIFIER_Logging.cpp"
								"${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/ECU_VRU_CLASSIFIER_Logging.h")

list(APPEND ECU_VRU_CLASSIFIER_V1  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V1/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V1/ECU_VRU_CLASSIFIER_logging_V1.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V1/ECU_VRU_CLASSIFIER_logging_V1.h")

list(APPEND ECU_VRU_CLASSIFIER_V2  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V2/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V2/ECU_VRU_CLASSIFIER_logging_V2.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V2/ECU_VRU_CLASSIFIER_logging_V2.h")

list(APPEND ECU_VRU_CLASSIFIER_V3  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V3/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V3/ECU_VRU_CLASSIFIER_logging_V3.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V3/ECU_VRU_CLASSIFIER_logging_V3.h")

list(APPEND ECU_VRU_CLASSIFIER_V4  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V4/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V4/ECU_VRU_CLASSIFIER_logging_V4.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V4/ECU_VRU_CLASSIFIER_logging_V4.h")

list(APPEND ECU_VRU_CLASSIFIER_V5  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V5/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V5/ECU_VRU_CLASSIFIER_logging_V5.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V5/ECU_VRU_CLASSIFIER_logging_V5.h")

list(APPEND ECU_VRU_CLASSIFIER_V6  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V6/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V6/ECU_VRU_CLASSIFIER_logging_V6.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V6/ECU_VRU_CLASSIFIER_logging_V6.h")

list(APPEND ECU_VRU_CLASSIFIER_V7  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V7/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V7/ECU_VRU_CLASSIFIER_logging_V7.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V7/ECU_VRU_CLASSIFIER_logging_V7.h")

list(APPEND ECU_VRU_CLASSIFIER_V8  "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V8/ECU_VRU_CLASSIFIER_logging_structure.h"					
						           "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V8/ECU_VRU_CLASSIFIER_logging_V8.cpp"	
								   "${SourceFiles}/RadarEcuStreams/BMW/RADAR_ECU_VRU_CLASSIFIER_96/V8/ECU_VRU_CLASSIFIER_logging_V8.h")

#Grouping RADARECU CORE 0 source file
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96	 FILES ${ECU_VRU_CLASSIFIER})	
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V1 FILES ${ECU_VRU_CLASSIFIER_V1})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V2 FILES ${ECU_VRU_CLASSIFIER_V2})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V3 FILES ${ECU_VRU_CLASSIFIER_V3})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V4 FILES ${ECU_VRU_CLASSIFIER_V4})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V5 FILES ${ECU_VRU_CLASSIFIER_V5})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V6 FILES ${ECU_VRU_CLASSIFIER_V6})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V7 FILES ${ECU_VRU_CLASSIFIER_V7})
source_group(\\RadarEcuStreams\\BMW\\RADAR_ECU_VRU_CLASSIFIER_96\\V8 FILES ${ECU_VRU_CLASSIFIER_V8})

#set the source
set(SOURCE_ALL ${COMMON_SOURCE} ${Z4CUSTOM} ${Z4CUSTOM_SRR5_BMW_BPIL_V1} ${Z4CUSTOM_SRR5_BMW_BPIL_V2} ${Z4CUSTOM_SRR5_BMW_BPIL_V3} ${Z4CUSTOM_SRR5_BMW_BPIL_V4} ${Z4CUSTOM_SRR5_BMW_BPIL_V5} ${Z4CUSTOM_SRR5_BMW_BPIL_V6} ${Z4CUSTOM_SRR5_BMW_BPIL_V7} ${Z4CUSTOM_SRR5_BMW_BPIL_V8} ${Z4CUSTOM_SRR5_BMW_LOW_V1} ${Z4CUSTOM_SRR5_BMW_LOW_V2} ${Z4CUSTOM_SRR5_BMW_LOW_V3} ${Z4CUSTOM_SRR5_BMW_LOW_V4} ${Z4CUSTOM_SRR5_BMW_LOW_V5} ${Z4CUSTOM_SRR5_BMW_LOW_V6} ${Z4CUSTOM_SRR5_BMW_LOW_V7} ${Z4CUSTOM_SRR5_BMW_LOW_V8} ${Z4CUSTOM_SRR5_BMW_LOW_V9} ${Z4CUSTOM_SRR5_BMW_LOW_V10} ${Z4CUSTOM_SRR5_BMW_LOW_V11} ${Z4CUSTOM_SRR5_BMW_LOW_V12} ${Z4CUSTOM_SRR5_BMW_LOW_V13} ${Z4CUSTOM_SRR5_BMW_LOW_V14} ${Z4CUSTOM_SRR5_BMW_LOW_V15} ${Z4CUSTOM_SRR5_BMW_LOW_V16} ${Z4CUSTOM_SRR5_BMW_LOW_V17} ${Z4CUSTOM_SRR5_BMW_LOW_V18} ${Z4CUSTOM_SRR5_BMW_LOW_V19} ${Z4CUSTOM_SRR5_BMW_LOW_V20} ${Z4CUSTOM_SRR5_BMW_LOW_V21} ${Z4CUSTOM_SRR5_BMW_LOW_V22} ${Z4CUSTOM_SRR5_BMW_MID_V1} ${Z4CUSTOM_SRR5_BMW_MID_V2} ${Z4CUSTOM_SRR5_BMW_MID_V3} ${Z4CUSTOM_SRR5_BMW_MID_V4} ${Z4CUSTOM_SRR5_BMW_MID_V5} ${Z4CUSTOM_SRR5_BMW_MID_V6} ${Z4CUSTOM_SRR5_BMW_MID_V7} ${Z4CUSTOM_SRR5_BMW_MID_V8} ${Z4CUSTOM_SRR5_BMW_MID_V9} ${Z4CUSTOM_SRR5_BMW_MID_V10} ${Z4CUSTOM_SRR5_BMW_MID_V11} ${Z4CUSTOM_SRR5_HKMC_SRR5_V1} ${Z4CUSTOM_SRR5_HKMC_SRR5_V2} ${Z4CUSTOM_SRR5_RNA_SUV_V1} ${Z4CUSTOM_SRR5_RNA_SUV_V2} ${Z4CUSTOM_SRR5_RNA_SUV_V3} ${Z4CUSTOM_SRR5_RNA_SUV_V4} ${Z4CUSTOM_SRR5_RNA_SUV_V5} ${Z4CUSTOM_SRR5_RNA_SUV_V6} ${Z4CUSTOM_SRR5_GEELY_V1} ${Z4CUSTOM_SRR5_GEELY_V2} ${Z4CUSTOM_SRR5_CHANGAN_SRR5_V1} ${Z4CUSTOM_SRR5_CHANGAN_SRR5_V2} ${Z4CUSTOM_SRR5_PSA_SRR5_V1} ${Z4CUSTOM_MRR_PSA_V3}
				${Z7BCUSTOM} ${Z7BCUSTOM_SRR5_BMW_BPIL_V1} ${Z7BCUSTOM_SRR5_BMW_BPIL_V2} ${Z7BCUSTOM_SRR5_BMW_BPIL_V3} ${Z7BCUSTOM_SRR5_BMW_BPIL_V4} ${Z7BCUSTOM_SRR5_BMW_BPIL_V5} ${Z7BCUSTOM_SRR5_BMW_BPIL_V6} ${Z7BCUSTOM_SRR5_BMW_LOW_V1} ${Z7BCUSTOM_SRR5_BMW_LOW_V2} ${Z7BCUSTOM_SRR5_BMW_LOW_V3} ${Z7BCUSTOM_SRR5_BMW_LOW_V4} ${Z7BCUSTOM_SRR5_BMW_LOW_V5} ${Z7BCUSTOM_SRR5_BMW_LOW_V6} ${Z7BCUSTOM_SRR5_BMW_LOW_V7} ${Z7BCUSTOM_SRR5_BMW_LOW_V8} ${Z7BCUSTOM_SRR5_BMW_LOW_V9} ${Z7BCUSTOM_SRR5_BMW_LOW_V11} ${Z7BCUSTOM_SRR5_BMW_LOW_V12} ${Z7BCUSTOM_SRR5_BMW_LOW_V13} ${Z7BCUSTOM_SRR5_BMW_LOW_V14} ${Z7BCUSTOM_SRR5_BMW_LOW_V15} ${Z7BCUSTOM_SRR5_BMW_LOW_V16} ${Z7BCUSTOM_SRR5_BMW_LOW_V17} ${Z7BCUSTOM_SRR5_BMW_MID_V1} ${Z7BCUSTOM_SRR5_BMW_MID_V2} ${Z7BCUSTOM_SRR5_BMW_MID_V3} ${Z7BCUSTOM_SRR5_BMW_MID_V4} ${Z7BCUSTOM_SRR5_BMW_MID_V5} ${Z7BCUSTOM_SRR5_BMW_MID_V6} ${Z7BCUSTOM_SRR5_BMW_MID_V7} ${Z7BCUSTOM_SRR5_BMW_MID_V8} ${Z7BCUSTOM_SRR5_HKMC_SRR5_V1} ${Z7BCUSTOM_SRR5_HKMC_SRR5_V2} ${Z7BCUSTOM_SRR5_HKMC_SRR5_V3} ${Z7BCUSTOM_SRR5_RNA_SUV_V1} ${Z7BCUSTOM_SRR5_GEELY_V1} ${Z7BCUSTOM_SRR5_RNA_SUV_V2} ${Z7BCUSTOM_SRR5_RNA_SUV_V3} ${Z7BCUSTOM_SRR5_RNA_SUV_V4} ${Z7BCUSTOM_SRR5_RNA_SUV_V5} ${Z7BCUSTOM_SRR5_RNA_SUV_V6} ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V1} ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V2} ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V3} ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V4} ${Z7BCUSTOM_SRR5_CHANGAN_SRR5_V5} ${Z7BCUSTOM_SRR5_GWM_SRR5_V1} ${Z7BCUSTOM_SRR5_PSA_SRR5_V1} ${Z7BCUSTOM_MRR_PSA_V1} 
				${CDC_SOURCE} ${CDC_MRR2_V1} ${CDC_SRR5_V4}
				${Z4SOURCE} ${Z4_SRR5_CORE_HKMC_V1} ${Z4_SRR5_CORE_RNA_V1} ${Z4_SRR5_CORE_RNA_V2} ${Z4_SRR5_CORE_RNA_V3} ${Z4_SRR5_CORE_RNA_V4} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V1} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V2} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V3} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V4} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V5} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V6} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V7} ${Z4_SRR5_CORE_HKMC_BMW_GEELY_CHANGAN_V8} ${Z4_SRR5_CORE_PSA_V1} ${Z4_MRR_PSA_V1} 
				${Z7ASOURCE} ${Z7A_SRR5_CORE_BMW_BPIL_V1} ${Z7A_SRR5_CORE_BMW_BPIL_V2} ${Z7A_SRR5_CORE_BMW_BPIL_V3} ${Z7A_SRR5_CORE_BMW_BPIL_V4} ${Z7A_SRR5_CORE_BMW_BPIL_V5} ${Z7A_SRR5_CORE_BMW_BPIL_V6} ${Z7A_SRR5_CORE_BMW_BPIL_V7} ${Z7A_SRR5_CORE_BMW_BPIL_V8} ${Z7A_SRR5_CORE_BMW_BPIL_V9} ${Z7A_SRR5_CORE_BMW_BPIL_V10} ${Z7A_SRR5_CORE_BMW_BPIL_V11} ${Z7A_SRR5_CORE_BMW_BPIL_V12} ${Z7A_SRR5_CORE_BMW_BPIL_V13} ${Z7A_SRR5_CORE_BMW_BPIL_V14} ${Z7A_SRR5_CORE_BMW_BPIL_V15} ${Z7A_SRR5_CORE_BMW_LOW_V1} ${Z7A_SRR5_CORE_BMW_LOW_V2} ${Z7A_SRR5_CORE_BMW_LOW_V3} ${Z7A_SRR5_CORE_BMW_LOW_V4} ${Z7A_SRR5_CORE_BMW_LOW_V5} ${Z7A_SRR5_CORE_BMW_LOW_V6} ${Z7A_SRR5_CORE_BMW_LOW_V7} ${Z7A_SRR5_CORE_BMW_LOW_V8} ${Z7A_SRR5_CORE_BMW_LOW_V9} ${Z7A_SRR5_CORE_BMW_LOW_V10} ${Z7A_SRR5_CORE_BMW_LOW_V11} ${Z7A_SRR5_CORE_BMW_LOW_V12} ${Z7A_SRR5_CORE_BMW_LOW_V13} ${Z7A_SRR5_CORE_BMW_LOW_V14} ${Z7A_SRR5_CORE_BMW_LOW_V15} ${Z7A_SRR5_CORE_BMW_LOW_V16} ${Z7A_SRR5_CORE_BMW_LOW_V17} ${Z7A_SRR5_CORE_BMW_LOW_V18} ${Z7A_SRR5_CORE_BMW_LOW_V19} ${Z7A_SRR5_CORE_BMW_LOW_V20} ${Z7A_SRR5_CORE_BMW_MID_V1} ${Z7A_SRR5_CORE_BMW_MID_V2} ${Z7A_SRR5_CORE_BMW_MID_V3} ${Z7A_SRR5_CORE_BMW_MID_V4} ${Z7A_SRR5_CORE_BMW_MID_V5} ${Z7A_SRR5_CORE_BMW_MID_V6} ${Z7A_SRR5_CORE_BMW_MID_V7} ${Z7A_SRR5_CORE_BMW_MID_V8} ${Z7A_SRR5_CORE_BMW_MID_V9} ${Z7A_SRR5_CORE_BMW_MID_V10} ${Z7A_SRR5_CORE_BMW_MID_V11} ${Z7A_SRR5_CORE_BMW_MID_V12} ${Z7A_SRR5_CORE_BMW_MID_V13} ${Z7A_SRR5_CORE_BMW_MID_V14} ${Z7A_SRR5_CORE_BMW_MID_V15} ${Z7A_SRR5_CORE_BMW_MID_V16} ${Z7A_SRR5_CORE_BMW_MID_V17} ${Z7A_SRR5_CORE_BMW_MID_V18} ${Z7A_SRR5_CORE_BMW_MID_V19} ${Z7A_SRR5_CORE_BMW_MID_V20}
				${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V1} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V2} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V3} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V4} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V5} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V6} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V7} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V8} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V9} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V10} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V11} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V12} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V13} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V14} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V15} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V16} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V17} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V18} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V19} ${Z7A_SRR5_CORE_HKMC_GEELY_CHANGAN_V20} 
				${Z7A_SRR5_CORE_RNA_210_V1} ${Z7A_SRR5_CORE_RNA_210_V2} ${Z7A_SRR5_CORE_RNA_210_V3} ${Z7A_SRR5_CORE_RNA_210_V4} ${Z7A_SRR5_CORE_RNA_210_V5} ${Z7A_SRR5_CORE_RNA_210_V7} ${Z7A_SRR5_CORE_RNA_210_V8} ${Z7A_SRR5_CORE_RNA_210_V9} ${Z7A_SRR5_CORE_RNA_210_V11}  ${Z7A_SRR5_CORE_RNA_210_V12} ${Z7A_SRR5_CORE_RNA_210_V13} ${Z7A_SRR5_CORE_RNA_210_V14} ${Z7A_SRR5_CORE_RNA_210_V15} ${Z7A_SRR5_CORE_RNA_210_V16} ${Z7A_SRR5_CORE_RNA_210_V17} ${Z7A_SRR5_CORE_RNA_210_V18} ${Z7A_SRR5_CORE_RNA_210_V19} ${Z7A_SRR5_CORE_PSA_V1} ${Z7A_SRR5_CORE_PSA_V2} ${Z7A_MRR_PSA_V1} ${Z7A_MRR_PSA_V2}
				${Z7BSOURCE} ${Z7B_SRR5_CORE_BMW_BPIL_V1} ${Z7B_SRR5_CORE_BMW_BPIL_V2} ${Z7B_SRR5_CORE_BMW_BPIL_V3} ${Z7B_SRR5_CORE_BMW_BPIL_V4} ${Z7B_SRR5_CORE_BMW_BPIL_V5} ${Z7B_SRR5_CORE_BMW_BPIL_V6} ${Z7B_SRR5_CORE_BMW_BPIL_V7} ${Z7B_SRR5_CORE_BMW_BPIL_V8} ${Z7B_SRR5_CORE_BMW_BPIL_V9} ${Z7B_SRR5_CORE_BMW_BPIL_V10} ${Z7B_SRR5_CORE_BMW_BPIL_V11} ${Z7B_SRR5_CORE_BMW_BPIL_V12} ${Z7B_SRR5_CORE_BMW_MID_V1} ${Z7B_SRR5_CORE_BMW_MID_V2} ${Z7B_SRR5_CORE_BMW_MID_V3} ${Z7B_SRR5_CORE_BMW_MID_V4} ${Z7B_SRR5_CORE_BMW_MID_V5} ${Z7B_SRR5_CORE_BMW_MID_V6} ${Z7B_SRR5_CORE_BMW_MID_V7} ${Z7B_SRR5_CORE_BMW_MID_V8} ${Z7B_SRR5_CORE_BMW_MID_V9} ${Z7B_SRR5_CORE_BMW_MID_V10} ${Z7B_SRR5_CORE_BMW_MID_V11} ${Z7B_SRR5_CORE_BMW_MID_V12} ${Z7B_SRR5_CORE_BMW_MID_V13} ${Z7B_SRR5_CORE_BMW_MID_V14} ${Z7B_SRR5_CORE_BMW_MID_V15} ${Z7B_SRR5_CORE_BMW_MID_V16} ${Z7B_SRR5_CORE_BMW_MID_V17} ${Z7B_SRR5_CORE_GEELY_V1} ${Z7B_SRR5_CORE_GEELY_V2} ${Z7B_SRR5_CORE_GEELY_V3} ${Z7B_SRR5_CORE_GEELY_V4} ${Z7B_SRR5_CORE_GEELY_V5} ${Z7B_SRR5_CORE_GEELY_V6} ${Z7B_SRR5_CORE_GEELY_V7} ${Z7B_SRR5_CORE_GEELY_V8} ${Z7B_SRR5_CORE_GEELY_V9} ${Z7B_SRR5_CORE_GEELY_V10} ${Z7B_SRR5_CORE_GEELY_V11} ${Z7B_SRR5_CORE_CHANGAN_V1} ${Z7B_SRR5_CORE_CHANGAN_V2} ${Z7B_SRR5_CORE_CHANGAN_V3} ${Z7B_SRR5_CORE_CHANGAN_V4} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V1} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V2} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V3} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V4} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V5} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V6} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V7} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V8} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V9} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V10} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V11} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V12} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V13} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V14} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V15} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V16} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V17} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V18} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V19} ${Z7B_SRR5_CORE_HKMC_BMW_LV_GWM_V20} ${Z7B_SRR5_CORE_RNA_V1} ${Z7B_SRR5_CORE_RNA_V2} ${Z7B_SRR5_CORE_RNA_V3} ${Z7B_SRR5_CORE_RNA_V4} ${Z7B_SRR5_CORE_RNA_V5} ${Z7B_SRR5_CORE_RNA_V6} ${Z7B_SRR5_CORE_RNA_V7} ${Z7B_SRR5_CORE_RNA_V8} ${Z7B_SRR5_CORE_RNA_V9}
				${Z7B_SRR5_CORE_RNA_V10} ${Z7B_SRR5_CORE_RNA_V11} ${Z7B_SRR5_CORE_RNA_V12} ${Z7B_SRR5_CORE_RNA_V13} ${Z7B_SRR5_CORE_PSA_V1} ${Z7B_SRR5_CORE_PSA_V2} ${Z7B_MRR_PSA_V1} ${Z7B_SRR5_CORE_HKMC_V1} ${Z7B_SRR5_CORE_HKMC_V2} ${Z7B_SRR5_CORE_HKMC_V3}
				${Z7B_TRACKER_OUTPUT} ${Z7B_TRACKER_OUTPUT_V1} ${Z7B_TRACKER_OUTPUT_V2} ${Z7B_TRACKER_OUTPUT_V3} ${Z7B_TRACKER_OUTPUT_V4} ${Z7B_TRACKER_OUTPUT_V5} ${Z7B_TRACKER_OUTPUT_V6} ${Z7B_TRACKER_OUTPUT_V7} ${Z7B_TRACKER_OUTPUT_V8} ${Z7B_TRACKER_OUTPUT_V9}
				${DSPACE} ${SCANIA_V7} ${SCANIA_V8} ${BMW_LOW_V2} ${BMW_MID_V2} ${BMW_HIGH_V2} ${GEELY_V1} ${GEELY_V2}
				${ECU_Core0} ${ECU_Core0_V4} ${ECU_Core0_V5} ${ECU_Core0_V6} ${ECU_Core0_V7} ${ECU_Core0_V8} ${ECU_Core0_V9} ${ECU_Core0_V10} ${ECU_Core0_V11} ${ECU_Core0_V12} ${ECU_Core0_V13} ${ECU_Core0_V14} ${ECU_Core0_V15} ${ECU_Core0_V16}
				${ECU_Core1} ${ECU_Core1_V4} ${ECU_Core1_V5} ${ECU_Core1_V6} ${ECU_Core1_V7} ${ECU_Core1_V8} ${ECU_Core1_V9} ${ECU_Core1_V10} ${ECU_Core1_V11} ${ECU_Core1_V12} ${ECU_Core1_V13} ${ECU_Core1_V14} ${ECU_Core1_V15} ${ECU_Core1_V16} ${ECU_Core1_V17} ${ECU_Core1_V18} ${ECU_Core1_V19}
				${ECU_Core3} ${ECU_Core3_V5} ${ECU_Core3_V6} ${ECU_Core3_V7} ${ECU_Core3_V8} ${ECU_Core3_V9} ${ECU_Core3_V10} ${ECU_Core3_V11} ${ECU_Core3_V12} ${ECU_Core3_V13} ${ECU_Core3_V14} ${ECU_Core3_V15} ${ECU_Core3_V16} ${ECU_Core3_V17} ${ECU_Core3_V18} ${ECU_Core3_V19} ${ECU_Core3_V20} ${ECU_Core3_V21} ${ECU_Core3_V22} ${ECU_Core3_V23} ${ECU_Core3_V24} ${ECU_Core3_V25} ${ECU_Core3_V26} ${ECU_Core3_V27} ${ECU_Core3_V28} ${ECU_Core3_V29} ${ECU_Core3_V30} ${ECU_Core3_V31} ${ECU_Core3_V32} ${ECU_Core3_V33} ${ECU_Core3_V34} ${ECU_Core3_V35} ${Calibration} ${usc} ${usc_Vp1} ${usc_Vp2} ${usc_Vp3} ${Vp1} ${Vp2}
				${ECU_VRU_CLASSIFIER} ${ECU_VRU_CLASSIFIER_V1} ${ECU_VRU_CLASSIFIER_V2} ${ECU_VRU_CLASSIFIER_V3} ${ECU_VRU_CLASSIFIER_V4} ${ECU_VRU_CLASSIFIER_V5} ${ECU_VRU_CLASSIFIER_V6} ${ECU_VRU_CLASSIFIER_V7} ${ECU_VRU_CLASSIFIER_V8}
				)
		
add_library (${PROJECTNAME} STATIC "")
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONHELPER} ${COMMONFUNCHEADERS} ${COMMONINC} ${COMMONHEADERS} ${COMMONINCAUDI})


target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR PC_RESIM)

target_link_libraries (${PROJECTNAME} DPH_RR_ADAS_LOGGER)

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

# Structure Alignment
	if(MSVC)
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
		set(CMAKE_CXX_FLAGS_RELEASE "/Od /Ob0")
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
	
	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)
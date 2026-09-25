# ###########################  ADVANCED RADAR SYSTEMS - GEN5
# ##############################################################################
# SUPPORTED VARIANTS - FLR4,SRR6,SRR6+ and RESIM PROJECT 
# Authors : Mandeep Singh (mandeep.singh1@aptiv.com)
# ##############################################################################

# CMakeLists.txt to :
#	copy the FRAMEWORK BINARIES to the /build and /Release Folder
#	copy the RADAR BINARIES to the /build and /Release Folder
#	set Debugging properties for Visual Studio

cmake_minimum_required(VERSION 3.18)

IF (MSVC)
	# setting x-link path for FW Dll
	set(MY_PUBLIC_FRAMEWORK_BIN "${OUTPUT_PATH}/../../../FW_Release")
	file(GLOB MY_PUBLIC_FRAMEWORK_DLL ${MY_PUBLIC_FRAMEWORK_BIN}/*.dll)
	file(GLOB MY_PUBLIC_RADAR_DLL ${MY_PUBLIC_FRAMEWORK_BIN}/../Radar_dlls/*.dll)
	file(GLOB MY_PUBLIC_RADAR_DLL ${MY_PUBLIC_FRAMEWORK_BIN}/../SM_dll/*.dll)
	
	# copy FW exe and dll to Debug folder
	message("[ INFO ] : Synchronizing FrameWork Binaries with x-link")
	file(COPY ${MY_PUBLIC_FRAMEWORK_BIN}/APT_SRR_RESIM.exe DESTINATION ${OUTPUT_PATH}/Debug)
	file(COPY ${MY_PUBLIC_FRAMEWORK_DLL} DESTINATION ${OUTPUT_PATH}/Debug)
	
	# copy FW exe and dll to Release folder
	file(COPY ${MY_PUBLIC_FRAMEWORK_BIN}/APT_SRR_RESIM.exe DESTINATION ${OUTPUT_PATH}/Release)
	file(COPY ${MY_PUBLIC_FRAMEWORK_DLL} DESTINATION ${OUTPUT_PATH}/Release)
	
	# copy radar dlls to Debug & Release folder
	message("[ INFO ] : Synchronizing Radar Binaries with x-link")
	file(COPY ${MY_PUBLIC_RADAR_DLL} DESTINATION ${OUTPUT_PATH}/Debug)
	file(COPY ${MY_PUBLIC_RADAR_DLL} DESTINATION ${OUTPUT_PATH}/Release)
	
	file(COPY ${MY_PUBLIC_SM_DLL} DESTINATION ${OUTPUT_PATH}/Debug)
	file(COPY ${MY_PUBLIC_SM_DLL} DESTINATION ${OUTPUT_PATH}/Release)
	
	# Update fmu based on newly generated radar dlls 
	message(" ---- Updating the fmu with latest dll ---- ")
	set(VV_ENGINE "${OUTPUT_PATH}/../../../LogicModel")
	execute_process(COMMAND python ${VV_ENGINE}/fmu_Windows.py)

	OPTION(MSVC_USER_SETTINGS "Generate visual studio user settings containing the debug settings" ON)
ELSE()
	# setting x-link path for FW Dll
	set(MY_PUBLIC_FRAMEWORK_BIN "${OUTPUT_PATH}/../../../../FW_Release")
	file(GLOB MY_PUBLIC_FRAMEWORK_DLL ${MY_PUBLIC_FRAMEWORK_BIN}/*.so)
	file(GLOB MY_PUBLIC_RADAR_DLL ${MY_PUBLIC_FRAMEWORK_BIN}/../Radar_dlls/*.so)
	file(GLOB MY_PUBLIC_RADAR_DLL ${MY_PUBLIC_FRAMEWORK_BIN}/../SM_dll/*.so)
	
	message("[ INFO ] : Synchronizing FrameWork Binaries with x-link")
	# copy FW exe and dll to Debug folder
	file(COPY ${MY_PUBLIC_FRAMEWORK_BIN}/APT_SRR_RESIM DESTINATION ${OUTPUT_PATH})
	file(COPY ${MY_PUBLIC_FRAMEWORK_DLL} DESTINATION ${OUTPUT_PATH})
	
	## copy FW exe and dll to Release folder
	file(COPY ${MY_PUBLIC_FRAMEWORK_BIN}/APT_SRR_RESIM DESTINATION ${OUTPUT_PATH})
	file(COPY ${MY_PUBLIC_FRAMEWORK_DLL} DESTINATION ${OUTPUT_PATH})
	
	# copy radar dlls to Debug & Release folder
	message("[ INFO ] : Synchronizing Radar Binaries with x-link")
	file(COPY ${MY_PUBLIC_RADAR_DLL} DESTINATION ${OUTPUT_PATH})
	file(COPY ${MY_PUBLIC_RADAR_DLL} DESTINATION ${OUTPUT_PATH})
	
	file(COPY ${MY_PUBLIC_SM_DLL} DESTINATION ${OUTPUT_PATH}/Debug)
	file(COPY ${MY_PUBLIC_SM_DLL} DESTINATION ${OUTPUT_PATH}/Release)
	
	# Update fmu based on newly generated radar dlls 
	message(" ---- Updating the fmu with latest dll ---- ")
	set(VV_ENGINE "${MY_PUBLIC_FRAMEWORK_BIN}/../LogicModel")
	execute_process(COMMAND python3 ${VV_ENGINE}/fmu_Linux.py)
	message(" ---- DONE ---- ")

ENDIF(MSVC)

# ******************************************** Important NOTE *****************************************
# OUTPUT_PATH can be updated as per customer Output file in DC_SIL_Library\CMake\CMakeLists.txt
# MY_PUBLIC_FRAMEWORK_BIN can be updated as per the existing x-link path in individual customer

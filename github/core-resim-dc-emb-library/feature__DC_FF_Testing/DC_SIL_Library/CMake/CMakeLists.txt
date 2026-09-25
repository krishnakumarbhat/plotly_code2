# For more info about this cmake project please consult the wiki page below
# http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=wiki-view&name=cmake&pid=2997.4224

##########################################################################################################
#	Cmake build command:
#		locate DC_SIL_Library\CMake\build and open command prompt to execute below commands based on Platform
#		Windows : 
#				VS 2014	:	cmake -G "Visual Studio 14 Win64" ..
#				VS 2019	:	cmake -G "Visual Studio 16 2019" ..
#		Linux	:
#				For Debug mode 				: cmake -DCMAKE_BUILD_TYPE=DEBUG ..
#				For Releame mode 			: cmake -DCMAKE_BUILD_TYPE=Release ..
#				For Linux compiling, run 	: make
# 
# Updated and added coments by Mandeep.singh1@aptiv.com
##########################################################################################################


CMAKE_MINIMUM_REQUIRED(VERSION 3.9.1) 	# setting minimum version required for CMake

# ******************* cmake-policy section ************
#CMake Policy mechanism is designed to help keep existing projects building as new versions of CMake introduce changes in behaviour.
#Each new policy (behavioral change) is given an identifier of the form CMP<NNNN> where <NNNN> is an integer index.
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0011 NEW)

add_compile_definitions("_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING")

string(TIMESTAMP build_timestamp "%Y-%m-%d %H:%M:%S")
message("[INFO] : DC_Lib build date and time : " ${build_timestamp})
add_definitions(-DBUILD_INFO="${build_timestamp}")

if(${TRACKER_TYPE} STREQUAL "SRR")
	add_compile_definitions("TRACKER_SRR")
elseif(${TRACKER_TYPE} STREQUAL "MRR")
	add_compile_definitions("TRACKER_MRR")
endif()
message("[INFO] : Tracker selected as ${TRACKER_TYPE}")

if(${CUST} STREQUAL "CEER")
	message("[INFO] : DC_Lib build for CUSTOMER : " ${CUST})
	add_compile_definitions("${CUST}")
endif()

# Specify cmake to link project to the main customer solution
SET(ONLY_FF "ALL")


#create project
PROJECT (${CUST}_DC_RESIM)		# setting project name
set(CMAKE_VERBOSE_MAKEFILE ON)

#if(NOT CMAKE_CROSSCOMPILING)
	option(RESIMULATION "Build Resimulation code" ON)
#endif()


if(NOT CMAKE_CROSSCOMPILING)
	option(PC_RESIM "Enable PC_RESIM" ON)
endif(NOT CMAKE_CROSSCOMPILING)

# Put this flag on, so it is possible to put the new targets in subdirectories
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



# Set compiler Flag
SET_TARGET_PROPERTIES(PROPERTIES LINKER_LANGUAGE CXX)

message("[INFO] : checking OS configuration")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
	message("[INFO] : 64 bit OS")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x32")
	message("[INFO] : 32 bit OS")
endif()

# Set CMake Install Prefix
SET(CMAKE_INSTALL_PREFIX ${OUT_DIR_BIN}	
	CACHE PATH "Foo install prefix" FORCE
)

if(MSVC)
	add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)
	add_compile_definitions($<$<CONFIG:Release>:NDEBUG>)
	add_compile_options("/Zp2") #For Windows by deafault it should be 2-byte padding so this is enabled
endif(MSVC)

if(UNIX)
add_compile_options("$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
add_compile_options("-fPIC")
	if(BUILD_FOR_32_BIT)
		add_compile_options("-m32")
	endif(BUILD_FOR_32_BIT)
#add_compile_options("-fpack-struct=2") #For Linux it is removed as deafault padding is working now
add_compile_options("-fgnu89-inline")
add_compile_options("-fno-gnu-unique")

endif(UNIX)

# Set environmental variable for customer
SET(CUSTOMER "CEER" CACHE STRING "Customer name for this build")
SET(CUSTOMER_PROGRAM "RADAR_ECU")

# Set CMake environmental variables
SET(DEPENDENCIES_ROOT_PATH 	"${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "")

# run python script for Trackerlib update for xtrk 
find_package(Python3 REQUIRED COMPONENTS Interpreter)
# Execute the Python script
set(PYTHON_SCRIPT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/update.py")
execute_process(
        COMMAND ${Python3_EXECUTABLE} ${PYTHON_SCRIPT_PATH}
		RESULT_VARIABLE SCRIPT_RETURN_CODE
    )
if(NOT SCRIPT_RETURN_CODE EQUAL 0)
	message(FATAL_ERROR "Python Script Failed to Update")
else()
	message(STATUS "[XTRK]: TrackerLib Files Updated")
endif()

# Setting environmental variable for customer subdirectories
set(Source_path "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set(TRACKERLIB_IFACE_VARIANT_PATH			"${Source_path}/Application/F360Tracker/Wrappers/trackerLib_qnx_variant_A/iface_variant_A")
set(RSPP_IFACE_VARIANT_PATH					"${Source_path}/Application/F360Tracker/Wrappers/rspp_iface_variant_A/include")


set(MDFLOG_PROJ "${CMAKE_CURRENT_SOURCE_DIR}/../SIL_INT/APTV_DC_RESIM/CoreLibraryProjects/mdf_log")
set(ECU_SYNC_PROJ "${CMAKE_CURRENT_SOURCE_DIR}/../SIL_INT/APTV_DC_RESIM/CoreLibraryProjects/ecu_sync")
set(CORE_LIB_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/../SIL_INT/APTV_DC_RESIM/CoreLibraryProjects")
option(BINWRITER_MOCK "Binwriter is not active, only mock .h files are provided" OFF)
set(COMMON_INC "${CMAKE_CURRENT_SOURCE_DIR}/../SIL_INT/APTV_DC_RESIM/CommonFiles")
set(BINWRITER_PATH "${Source_path}/DC_SIL_Library/SIL_INT/DC_SIL_SOURCE/SIL_CORE/AS_BIN_WRITER" CACHE PATH "Path to bin writer library")
set(BINWRITER_LIBRARY_TYPE STATIC CACHE STRING "Type of library to build")
set(SIL_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/../Sil_Executable")
set(CURR_VARIANT "${SIL_EXECUTABLE}/RECU_dll/${CUST}")

if(${TRACKER_TYPE} STREQUAL "SRR")
	if(UNIX)
		if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
			set(OUTPUT_PATH "${CURR_VARIANT}/SRR/Debug")
		elseif (${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
			set(OUTPUT_PATH "${CURR_VARIANT}/SRR/Release")
		endif()
	else()
		set(OUTPUT_PATH "${CURR_VARIANT}/SRR")
	endif()
elseif(${TRACKER_TYPE} STREQUAL "MRR")
	if(UNIX)
		if(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
			set(OUTPUT_PATH "${CURR_VARIANT}/MRR/Debug")
		elseif(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
			set(OUTPUT_PATH "${CURR_VARIANT}/MRR/Release")
		endif()
	else()
		set(OUTPUT_PATH "${CURR_VARIANT}/MRR")
	endif()
endif()
	

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

add_subdirectory("${Source_path}/Application/F360Tracker/Wrappers" "${CMAKE_CURRENT_BINARY_DIR}/F360Tracker")
add_subdirectory("${Source_path}/Application/FeatureFunctions"  	"${CMAKE_CURRENT_BINARY_DIR}/FeatureFunctions")
add_subdirectory("${Source_path}/DC_SIL_Library/SIL_INT/DC_SIL_SOURCE/SIL_CORE/CMake"	 	"${CMAKE_CURRENT_BINARY_DIR}/DC_RESIM")
add_subdirectory("${Source_path}/DC_SIL_Library/SIL_INT/APTV_DC_RESIM/ApplicationProjects/DC_RESIM/DC_SIL_LIB/CMake"	 	"${CMAKE_CURRENT_BINARY_DIR}/DC_SIL_LIB")
add_subdirectory("${BINWRITER_PATH}/CMake" "${CMAKE_CURRENT_BINARY_DIR}/AS-bin-writer-lib")
add_subdirectory("${Source_path}/DC_SIL_Library/SIL_INT/APTV_DC_RESIM/CoreLibraryProjects/mdf_log/CMake" "${CMAKE_CURRENT_BINARY_DIR}/MDFLog")
add_subdirectory("${Source_path}/DC_SIL_Library/SIL_INT/APTV_DC_RESIM/CoreLibraryProjects/ecu_sync/CMake" "${CMAKE_CURRENT_BINARY_DIR}/Ecu_Sync")

if (MSVC)
	option(MSVC_USER_SETTINGS "Generate visual studio user settings containing the debug settings" ON)
	if(${TRACKER_TYPE} STREQUAL "SRR")
		set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "SRR_DC_SIL_LIB")
		if(${MSVC_USER_SETTINGS})
			set(MSVC_COMMAND_DEBUG "../../../SIL_Executable/RECU_dll/CEER/SRR/Debug/APT_SRR_RESIM.exe" CACHE STRING "The command to run for debugging in VS")
			set(MSVC_COMMAND_ARGUMENTS_DEBUG "../../../SIL_Executable/Config/SIL_Engine_Config.xml ../../../SIL_Executable/Config/SIL_Input.txt" CACHE STRING "The command arguments for the debug command")
			configure_file(UserTemplate.user ${CMAKE_CURRENT_BINARY_DIR}/DC_SIL_LIB/SRR_DC_SIL_LIB.vcxproj.user @ONLY)
		endif()
	elseif(${TRACKER_TYPE} STREQUAL "MRR")
		set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "MRR_DC_SIL_LIB")
		if(${MSVC_USER_SETTINGS})
			set(MSVC_COMMAND_DEBUG "../../../SIL_Executable/RECU_dll/CEER/MRR/Debug/APT_SRR_RESIM.exe" CACHE STRING "The command to run for debugging in VS")
			set(MSVC_COMMAND_ARGUMENTS_DEBUG "../../../SIL_Executable/Config/SIL_Engine_Config.xml ../../../SIL_Executable/Config/SIL_Input.txt" CACHE STRING "The command arguments for the debug command")
			configure_file(UserTemplate.user ${CMAKE_CURRENT_BINARY_DIR}/DC_SIL_LIB/MRR_DC_SIL_LIB.vcxproj.user @ONLY)
		endif()
	endif()
endif()

# calling SIL_Executable CMAKELists.txt to copy FW and RADAR_DLL binaries.
add_subdirectory("${SIL_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Executable")


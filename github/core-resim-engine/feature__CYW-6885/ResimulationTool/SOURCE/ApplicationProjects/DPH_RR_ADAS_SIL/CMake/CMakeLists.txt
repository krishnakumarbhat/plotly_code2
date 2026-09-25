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

if("${buildConfigToUpper}" STREQUAL "SIL")
set(PROJECTNAME  "DPH_RR_ADAS_SIL")
endif()

project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})
link_directories("${CORE_LIB_PROJECT}/CCA_ViGEM/bin/lib")

#include directories

set(COMMONINC "${COMMON_INC}/inc")
set(COMMONPLUGIN "${COMMON_INC}/plugin")

set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")
set(COMMON_SYM "${COMMON_INC}/sym")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONBMWSP25 "${COMMON_INC}/BMWSP25_includes")
set(DVLMESSAGE "${CORE_LIB_PROJECT}/dvlMessage/inc")

set(BOOST_INC "${APP_PROJECT}/BOOST/Include")


#include headers
set(SourceFiles "${DPH_RR_ADAS_SIL_PROJ}/")
set(SOURCE_ALL "${SourceFiles}cdc_shared.h")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_calib_handler.h")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_sil.h")
list(APPEND SOURCE_ALL "${SourceFiles}sil_demux.h")
list(APPEND SOURCE_ALL "${SourceFiles}cdcWrite.h")
list(APPEND SOURCE_ALL "${SourceFiles}embLibName.h")
list(APPEND SOURCE_ALL "${SourceFiles}SRR3_Internal_Data_Logger.h")
list(APPEND SOURCE_ALL "${SourceFiles}Udp_record_custom.h")
list(APPEND SOURCE_ALL "${SourceFiles}CustomSIL/AUDI/dph_rr_adas_audi_sil.h")
list(APPEND SOURCE_ALL "${SourceFiles}SRR3_SIL_API.h")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_badlog_identifier.h")
list(APPEND SOURCE_ALL "${COMMON}/IRadarStream_Lib_Loader.h")
list(APPEND SOURCE_ALL "${COMMON}/IRadarStream.h")
list(APPEND SOURCE_ALL "${COMMON}/Z_Logging.h")
#list(APPEND SOURCE_ALL "${CORE_LIB_PROJECT}/CCA_ViGEM/inc/vgm_cca_log.h")
#list(APPEND SOURCE_ALL "${CORE_LIB_PROJECT}/CCA_Vpcap/inc/vgm_cca_vpcap.h")


#include custom specific
list(APPEND SOURCE_ALL "${DVLMESSAGE}/dvl_message.h")



#include sources
list(APPEND SOURCE_ALL "${COMMONINC}/fixmac.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")
list(APPEND SOURCE_ALL "${COMMON_INC}/Utility/commonUtils.h")

list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_calib_handler.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_sil.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}sil_demux.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}cdcWrite.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}embLibName.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}SRR3_Internal_Data_Logger.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}dph_rr_adas_badlog_identifier.cpp")
#list(APPEND SOURCE_ALL "${CORE_LIB_PROJECT}/CCA_ViGEM/src/vgm_cca_log.cpp")
#list(APPEND SOURCE_ALL "${CORE_LIB_PROJECT}/CCA_Vpcap/src/vgm_cca_vpcap.cpp")

#custom specific sources
#group source Audi files
list(APPEND AUDI_inc_files "${COMMONINC}/AUDI/rte.h")
list(APPEND AUDI_inc_files "${COMMONINC}/AUDI/rte_type.h")
list(APPEND AUDI_inc_files "${COMMONINC}/AUDI/Rte_Type_FXRPP_Z060.h")
list(APPEND AUDI_inc_files "${COMMONINC}/AUDI/sil_srr3_debug_types.h")
list(APPEND AUDI_inc_files "${COMMONINC}/AUDI/sil_srr3_interface_common.h")
list(APPEND AUDI_inc_files "${COMMONINC}/AUDI/srr3_decoder_interface.h")
source_group(\\CustomSIL\\AUDI\\inc FILES ${AUDI_inc_files})

list(APPEND AUDI_sym_files "${COMMON_SYM}/AUDI/srr3_audi_symbol_10.h")
list(APPEND AUDI_sym_files "${COMMON_SYM}/AUDI/srr3_audi_symbol_6.h")
list(APPEND AUDI_sym_files "${COMMON_SYM}/AUDI/srr3_audi_symbol_7.h")
list(APPEND AUDI_sym_files "${COMMON_SYM}/AUDI/srr3_audi_symbol_8.h")
list(APPEND AUDI_sym_files "${COMMON_SYM}/AUDI/srr3_audi_symbol_9.h")
source_group(\\CustomSIL\\AUDI\\sym FILES ${AUDI_sym_files})

list(APPEND AUDI_SRC "${SourceFiles}/CustomSIL/AUDI/dph_rr_adas_audi_sil.cpp")
list(APPEND AUDI_SRC "${SourceFiles}/CustomSIL/AUDI/dph_rr_adas_audi_sil.h")

source_group(\\CustomSIL\\AUDI FILES ${AUDI_SRC})

#group source Geely files
list(APPEND Geely "${COMMON_SYM}/GEELY/srr3_geely_symbol.h")
source_group(\\CustomSIL\\GEELY FILES ${Geely})

#group source GWM files
list(APPEND GWM "${COMMON_SYM}/GWM/srr3_gwm_symbol.h")
source_group(\\CustomSIL\\GWM FILES ${GWM})

#group source HKMC2 files
list(APPEND HKMC2 "${COMMON_SYM}/HKMC2/srr3_hkmc2_symbol.h")
source_group(\\CustomSIL\\HKMC2 FILES ${HKMC2})

#group source JLR files
list(APPEND JLR "${COMMON_SYM}/JLR/srr3_jlr_symbol.h")
source_group(\\CustomSIL\\JLR\\Ext FILES ${JLR})

#group source SCANIA files
list(APPEND SCANIA "${COMMON_SYM}/SCANIA/srr3_scania_symbol.h")
source_group(\\CustomSIL\\SCANIA FILES ${SCANIA})

#group source BMW files
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/canmsg_map.cpp")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/dph_rr_adas_bmw_sil.cpp")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/fas_can_gateway.c")
list(APPEND BMW_files "${SourceFiles}FAS_OUT.cpp")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/pcan_gateway.c")
list(APPEND BMW_files "${SourceFiles}Rte_Stub_BMW.cpp")
list(APPEND BMW_files "${SourceFiles}SystemStateMachineType_Stub.cpp")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/vcan_msg.c")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/canmsg_map1.h")
source_group(\\CustomSIL\\BMW\\SourceFiles FILES ${BMW_files})

#group source BMW can source files
list(APPEND BMW_files "${SourceFiles}ext_util.c")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_can_calib.cpp")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_can_db.cpp")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_can_db.h")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_can_db_util.cpp")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_can_db_util.h")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_data_ptr.h")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_fasl_latch_ext.cpp")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_fasr_latch_ext.cpp")
list(APPEND BMW_files "${SourceFiles}srr3_bmw_pcan_latch_ext.cpp")
source_group(\\CustomSIL\\BMW\\SourceFiles\\can FILES ${BMW_files})

#group source BMW headers
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/dph_rr_adas_bmw_sil.h")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/fas_can_gateway.h")
list(APPEND BMW_files "${SourceFiles}CustomSIL/BMW/pcan_gateway.h")

source_group(\\CustomSIL\\BMW\\HeaderFiles FILES ${BMW_files})

add_library(${PROJECTNAME} SHARED "")
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
						${INC_COMMONFILES} 
						${COMMONPLUGIN} 
						${AUDI_INC} 
						${BMW_INC} 
						${BMW_INCFUNHEADER} 
						${GEELY_INC} 
						${GWM_INC} 
						${HKMC2_INC} 
						${JLR_INC} 
						${PCR_INC} 
						${SCANIA_INC} 
						${COMMONHEADERS} 
						${COMMON_SYM} 
						${AUDI_SYM} 
						${BMW_SYM} ${GEELY_SYM} ${GWM_SYM} ${HKMC2_SYM} ${JLR_SYM} ${PCR_SYM} ${SCANIA_SYM} ${COMMONUTILITY} ${COMMONHELPER} ${DVLMESSAGE} ${BOOST_INC} ${COMMONBMWSP25})


target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} 
									${AUDI_inc_files} 
									${AUDI_sym_files} 
									${AUDI_files} 
									${Geely} 
									${GWM} 
									${JLR} 
									${SCANIA} 
									${BMW_files}
									${AUDI_SRC}
									)


if("${buildConfigToUpper}" STREQUAL "SIL")
	if(MSVC)
		target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR SRR3_SIL_EXPORTS CORE_SIL _WIN_PLATFORM _MBCS MULTITHR WIN32_LEAN_AND_MEAN
													WIN32_EXTRA_LEAN)
	else()
		target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR SRR3_SIL_EXPORTS CORE_SIL _MBCS MULTITHR)
	endif()
endif()	


#get_target_property(src ${PROJECTNAME} SOURCES)
#message("Sil source value:${src}")

if(MSVC)
set(ADDITIONAL_LIB 	
                    Blf		
					DPH_RR_ADAS_LOGGER 
					CrossPlatform
					#CCA_ViGEM
					mudp_decoder_calib
					DPH_RR_ADAS_Config
					UDP_Transmitter
					Timing_Profile
					)
elseif(UNIX)
set(ADDITIONAL_LIB 			
                    Blf		
					DPH_RR_ADAS_LOGGER 
					CrossPlatform
					CCA_ViGEM
					CCA_Vpcap
					mudp_decoder_calib
					DPH_RR_ADAS_Config
					UDP_Transmitter
					Mdf_Log
					Timing_Profile
					)
endif()
			
if(MSVC)
	target_link_libraries(${PROJECTNAME} ${ADDITIONAL_LIB})
	#target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Release")
	elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS19/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS19/lib/windows/${CONF_DIR}/Release")
	else()
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST/lib/windows/${CONF_DIR}/Release")
	endif()
else()
	set(BOOST_LIB_DIR_UNIX_DEBUG "${APP_PROJECT}/BOOST/lib/linux/${CONF_DIR}/Debug")
	set(BOOST_LIB_DIR_UNIX_RELEASE "${APP_PROJECT}/BOOST/lib/linux/${CONF_DIR}/Release")
endif()


string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TO_UPPER)
message("Config : ${CMAKE_BUILD_TYPE_TO_UPPER}")

if(MSVC)
if(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
	if(CONF_DIR STREQUAL "x64")
		if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
			target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_system-vc140-mt-gd-1_65_1.lib
																)
        elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		   target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc1429-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc1429-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc1429-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc1429-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_system-vc1429-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_system-vc141-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc141-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc141-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc141-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc141-mt-gd-1_65_1.lib
																)
		else()
			target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_system-vc110-mt-gd-1_65_1.lib
																)
		endif()
	elseif(CONF_DIR STREQUAL "x86")	
									
		target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc110-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_system-vc110-mt-gd-1_65_1.lib
																)
	
	endif(CONF_DIR STREQUAL "x64")
elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE" OR ${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELWITHDEBINFO")
	if(${CONF_DIR} STREQUAL "x64")
	  if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		  target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}    ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc140-mt-1_65_1.lib
																)
	 elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
	      target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}    ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc1429-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc141-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc141-mt-1_65_1.lib
																)
	 else()
	 target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc110-mt-1_65_1.lib
																)	
																  									    
	endif()
	elseif(${CONF_DIR} STREQUAL "x86")
		
		target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc110-mt-1_65_1.lib
																)
			
	endif(${CONF_DIR} STREQUAL "x64")
	
endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
else(MSVC)
	target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB})
	
	if(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")

	   if(${CONF_DIR} STREQUAL "x64")
			target_link_libraries(${PROJECTNAME}  ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}  ${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_chrono-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_date_time-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_regex-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_system-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_thread-gcc-mt-d-1_65_1.a
																				)
	   elseif(${CONF_DIR} STREQUAL "x86")
			target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_chrono-gcc-mt-d-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_date_time-gcc-mt-d-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_regex-gcc-mt-d-1_65_1.a
																			    ${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_system-gcc-mt-d-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_thread-gcc-mt-d-1_65_1.a
																				)
	   endif(${CONF_DIR} STREQUAL "x64")
	
	elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE" OR ${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELWITHDEBINFO")
	   if(${CONF_DIR} STREQUAL "x64")
			target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_chrono-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_date_time-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_regex-gcc-mt-1_65_1.a
																			    ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_system-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_thread-gcc-mt-1_65_1.a
																				)
	   elseif(${CONF_DIR} STREQUAL "x86")
			target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_chrono-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_date_time-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_regex-gcc-mt-1_65_1.a
																			    ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_system-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_thread-gcc-mt-1_65_1.a
																				)
	   endif(${CONF_DIR} STREQUAL "x64")
	endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
endif(MSVC)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
											    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
											    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()		
endif(MSVC)

if(UNIX)
	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
	add_compile_options("-fPIC")
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
endif(UNIX)
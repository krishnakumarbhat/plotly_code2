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

set(PROJECTNAME  "mdf_udpData_Proc")
project(${PROJECTNAME})

set(SourceFiles "${MDF4_DECODER_PROJ}/")
link_directories(${SourceFiles}/../output)


#Linking directories
link_directories(${OUTPUT_PATH})
#Adding required common header files
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONSYM "${COMMON_INC}/sym")
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers")
set(COMMONPLUGIN "${COMMON_INC}/plugin")
set(COMMONUTIL "${COMMON_INC}/Utility")
set(BOOST_INC "${APP_PROJECT}/BOOST/Include")

#including sources and headers

list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginInterface.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/VerboseLogUtil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/cust_opt.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")
list(APPEND HEADERS "${COMMONINC}/Resim_Config.h")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.h")

#adding the sources specific to customer
set(SourceFiles "${MDF4_DECODER_PROJ}/")
list(APPEND SOURCE_ALL "${SourceFiles}/mdf_udp_Data_Proc.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/Converter_Config.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/mdf_udp_Data_Proc.h")
list(APPEND SOURCE_ALL "${SourceFiles}/mdf_udp_Interface.cpp")
list(APPEND HEADERS "${SourceFiles}/mdf_udp_Interface.h")
list(APPEND HEADERS "${SourceFiles}/MDF4_Decoder_version.h")
list(APPEND HEADERS "${SourceFiles}/SRR3_SIL_API.h")
list(APPEND HEADERS "${SourceFiles}/stdafx.cpp")
list(APPEND HEADERS "${SourceFiles}/stdafx.h")
list(APPEND HEADERS "${SourceFiles}/targetver.h")

add_executable (${PROJECTNAME} "") 
#set(BOOST_INC "${BORDNET_DECODER_PROJ}/../BOOST/Include")
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

if("${buildConfigToUpper}" STREQUAL "SIL")
target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR PC_RESIM)
endif()

set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

target_link_libraries(${PROJECTNAME} ccalib_mdf_x64_4-7-6)

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
						${COMMONHEADERS} 
						${COMMONHELPER} 
						${COMMONUDPHEADER} 
						${COMMONPLUGIN}
						${COMMONUTIL}
						${BOOST_INC}
						)

set(ADDITIONAL_LIB 
					DPH_RR_ADAS_LOGGER 
					CrossPlatform 
					Video_Decoder
					Mdf_Log
					)

if(MSVC)
	target_link_libraries(${PROJECTNAME} ${ADDITIONAL_LIB})
	#target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Release")
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
		if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
			target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_DEBUG}/libboost_date_time-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_regex-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_chrono-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_thread-vc140-mt-gd-1_65_1.lib
																${BOOST_LIB_DIR_DEBUG}/libboost_system-vc140-mt-gd-1_65_1.lib
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
elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE")
	if(CONF_DIR STREQUAL "x64")
	  if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		  target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}    ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc140-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc140-mt-1_65_1.lib
																)
	 else()
	 target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc110-mt-1_65_1.lib
																)	
																  									    
	endif()
	elseif(CONF_DIR STREQUAL "x86")
		
		target_link_libraries(${PROJECTNAME}  ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_RELEASE}/libboost_date_time-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_regex-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_thread-vc110-mt-1_65_1.lib
															    ${BOOST_LIB_DIR_RELEASE}/libboost_chrono-vc110-mt-1_65_1.lib
																${BOOST_LIB_DIR_RELEASE}/libboost_system-vc110-mt-1_65_1.lib
																)
			
	endif(CONF_DIR STREQUAL "x64")
	
endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
else(MSVC)
	target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB})
	
	if(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")

	   if(CONF_DIR STREQUAL "x64")
			target_link_libraries(${PROJECTNAME}  ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}  ${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_chrono-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_date_time-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_regex-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_system-gcc-mt-d-1_65_1.a
																					${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_thread-gcc-mt-d-1_65_1.a
																				)
	   elseif(CONF_DIR STREQUAL "x86")
			target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_chrono-gcc-mt-d-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_date_time-gcc-mt-d-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_regex-gcc-mt-d-1_65_1.a
																			    ${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_system-gcc-mt-d-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_DEBUG}/libboost_thread-gcc-mt-d-1_65_1.a
																				)
	   endif(CONF_DIR STREQUAL "x64")
	
	elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE")
	   if(CONF_DIR STREQUAL "x64")
			target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_chrono-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_date_time-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_regex-gcc-mt-1_65_1.a
																			    ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_system-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_thread-gcc-mt-1_65_1.a
																				)
	   elseif(CONF_DIR STREQUAL "x86")
			target_link_libraries(${PROJECTNAME} ${CMAKE_DL_LIBS} ${ADDITIONAL_LIB}   ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_chrono-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_date_time-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_regex-gcc-mt-1_65_1.a
																			    ${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_system-gcc-mt-1_65_1.a
																				${BOOST_LIB_DIR_UNIX_RELEASE}/libboost_thread-gcc-mt-1_65_1.a
																				)
	   endif(CONF_DIR STREQUAL "x64")
	endif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "DEBUG")
endif(MSVC)					
if(MSVC)
	target_link_libraries (${PROJECTNAME}  ${ADDITIONAL_LIB})
	
else()
	set(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")
	target_link_libraries (${PROJECTNAME}  ${ADDITIONAL_LIB} ${CMAKE_DL_LIBS}) #${CMAKE_DL_LIBS} for dlopen and dlclose lib for Linux build
endif()

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
	if(MSVC)
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp4")
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)
target_link_libraries (${PROJECTNAME}  ${CMAKE_DL_LIBS})
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

cmake_minimum_required(VERSION 4.0.3)

#Set Project Name
set(PROJECTNAME  "SRR_HiL_Exec")
project(${PROJECTNAME})


#Linking directories
link_directories(${OUTPUT_PATH})
if(MSVC)
# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/../../../../")
# set path for Common includes
set(COMMON_INC "${SOURCE_PROJECT}/CommonFiles")
message("${COMMON_INC}")
# set path for Application
set(APP_PROJECT "${SOURCE_PROJECT}/ApplicationProjects")

# set path for core library projects
set(CORE_LIB_PROJECT "${SOURCE_PROJECT}/CoreLibraryProjects")

set(SRR3HIL_EXE_PROJ "${APP_PROJECT}/SRR3_Resimulator/SRR3_HiL_Exec")
else()
endif()
#Setting common include headers
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers")
set(COMMONPLUGIN "${COMMON_INC}/plugin")
set(COMMON_THREADS "${COMMON_INC}/thread")
set(COMMONHEADER "${COMMON_INC}/CommonHeaders")
set(BOOSTINC "${APP_PROJECT}/BOOST/Include")
#set(MDF4_INCLUDEDIR "${FFMPEG_PROJ}/aptiv_mdf4")
#set(ZLIB_INCLUDEDIR "${FFMPEG_PROJ}/zlib_install_64/include")
#set(ZLIB_DIR "${FFMPEG_PROJ}/zlib_install_64/install_64/lib")

#including sources and headers
set(SOURCES "${SRR3HIL_EXE_PROJ}")
list(APPEND HEADERS "${COMMONINC}/common.h")
list(APPEND HEADERS "${COMMONINC}/XCPSeedKey.h")
list(APPEND HEADERS "${COMMONINC}/HiLBaseStream.h")
list(APPEND HEADERS "${COMMONHEADER}/Read_Config.h")
list(APPEND HEADERS "${COMMONHEADER}/types.h")

list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiL_OH_Prioratize.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLController.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLExecutive.h")
#list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLConcurrentQueue.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLFixedSizeQueue.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLOfflineStream.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLOnlineStream.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/srr3_hil_exec.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/hil_command_req_resp_stream.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/hil_communication_stream.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/fort.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/fort.hpp")


list(APPEND SOURCE_ALL "${COMMONHELPER}/Calc_CRC.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/Compute.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")
list(APPEND SOURCE_ALL "${COMMONHELPER}/Cust_sel.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")

list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/hil_pack/hil_audi_pack.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/HiL_OH_Prioratize.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/HiLBaseStream.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/HiLController.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/HiLExecutive.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/HiLOfflineStream.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/HiLOnlineStream.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/InitFencesDefaultValues.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/InitOHDefaultValues.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/InitTrackerDefaultValues.cpp")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/srr3_hil_exec.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/ThBase.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/ThBase.h")

list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/fort.c")

list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/../HiLInputs/pugixml/pugiconfig.hpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/../HiLInputs/pugixml/pugixml.hpp")

if(MSVC)
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/SyncManager.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/EventPosix.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/EventPosix.h")
else()
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/EventPosix.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/EventPosix.h")
endif()
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/Read_Config.cpp")

if(MSVC)
	list(APPEND SOURCE_ALL "${SOURCES}/dllmain.cpp")
endif()
#Creating static lib
add_library(${PROJECTNAME} SHARED "")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	SET(CMAKE_CXX_FLAGS -pthread)
endif()

#Link source and headers to target

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

if(MSVC)
else()
set (CMAKE_C_FLAGS "-fPIC")
set (CMAKE_CXX_FLAGS "-fPIC")
endif()

	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()


#find_file(ZLIB_LIB_STATIC
#	NAMES zlibstatic.lib 
#	HINTS ${ZLIB_DIR})

#find_file(ZLIB_LIB
#	NAMES zlib.lib
#	HINTS ${ZLIB_DIR})


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)
if(MSVC)
target_compile_definitions(${PROJECTNAME} PRIVATE SRR3_HIL_EXPORTS
													PC_RESIM
													HIL_MODE
													ENABLE_CUSTHDR
													WIN32_LEAN_AND_MEAN
													WIN32_EXTRA_LEAN
													_CRT_SECURE_NO_WARNINGS
													-D_SCL_SECURE_NO_WARNINGS)
else()	
target_compile_definitions(${PROJECTNAME} PRIVATE SRR3_HIL_EXPORTS
													PC_RESIM
													HIL_MODE
													ENABLE_CUSTHDR)
endif()	
					
if(MSVC)
target_link_libraries(${PROJECTNAME} 	#aptiv_mdf4
										fort_tabular
										CrossPlatform
										Mdf_Log
										CCA_ViGEM
										#radar_stream_decoder
										FDX_Protocol)
#target_link_libraries(${PROJECTNAME}  wsock32 ws2_32 FOUNDATION_INTERFACE Poco::Foundation Poco::Net )
else()
target_link_libraries(${PROJECTNAME}    #aptiv_mdf4
										fort_tabular
										CrossPlatform
										#radar_stream_decoder
										Mdf_Log
										CCA_ViGEM)
endif(MSVC)			


if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM)
endif(MSVC)	

set(PUGI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../HiLInputs/pugixml/binaries")
if(MSVC)									
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../HiLInputs/pugixml/binaries/windows")
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/VS15/Debug/")
		find_library(pugixml_release pugixml HINTS "${LIB_PATH}/VS15/Release/")
		
		target_link_libraries(${PROJECTNAME} debug "${pugixml_debug}"
											 optimized "${pugixml_release}"
											 )
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
        find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/VS19/Debug/")
		find_library(pugixml_release pugixml HINTS "${LIB_PATH}/VS19/Release/")
		
		target_link_libraries(${PROJECTNAME} debug "${pugixml_debug}"
											 optimized "${pugixml_release}"
											 )
	else()
		find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/VS12/Debug/")
		find_library(pugixml_release pugixml HINTS "${LIB_PATH}/VS12/Release/")
		
		target_link_libraries(${PROJECTNAME} debug "${pugixml_debug}"
											 optimized "${pugixml_release}"
											 )
	endif()

else()
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../HiLInputs/pugixml/binaries/linux")
find_library(pugixml_debug libpugixml.a HINTS "${LIB_PATH}/Debug/")
find_library(pugixml_release libpugixml.a HINTS "${LIB_PATH}/Release/")


target_link_libraries(${PROJECTNAME} debug "${pugixml_debug}"
                                                                         optimized "${pugixml_release}"
                                                                         )
endif()
set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

if(MSVC)
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													${COMMON_THREADS}
													${FDX_HEADER}
													${BOOSTINC}
													${COMMONHEADER}
													#${MDF4_INCLUDEDIR}
													#${ZLIB_INCLUDEDIR}
													)
else()
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													${COMMON_THREADS}
													${BOOSTINC}
													${COMMONHEADER}
													#${MDF4_INCLUDEDIR}
													#${ZLIB_INCLUDEDIR}
													)
endif()


#set(ADDITIONAL_LIB ${ZLIB_LIB_STATIC} ${ZLIB_LIB} )

#target_link_libraries(${PROJECTNAME} ${ADDITIONAL_LIB})

#set(ZLIB_INCLUDE_DIR "${FFMPEG_PROJ}/zlib_install_64/install_64/include")
#set(ZLIB_LIBRARY "${FFMPEG_PROJ}/zlib_install_64/install_64/lib")
##find_package("ZLIB")
#find_package(ZLIB REQUIRED)

#if (NOT ZLIB_FOUND)
#    return()
#endif()

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else(MSVC)
target_compile_features(${PROJECTNAME} PRIVATE cxx_std_17)
endif(MSVC)

if(MSVC)
	#target_link_libraries(${PROJECTNAME} ${ADDITIONAL_LIB})
	#target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Release")
	elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS19/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS19/lib/windows/${CONF_DIR}/Release")
		#message("BOOST_LIB_DIR_DEBUG PATH " ${BOOST_LIB_DIR_DEBUG} )
	else()
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST/lib/windows/${CONF_DIR}/Release")
	endif()
else()
	set(BOOST_LIB_DIR_UNIX_DEBUG "${APP_PROJECT}/BOOST/lib/linux/${CONF_DIR}/Debug")
	set(BOOST_LIB_DIR_UNIX_RELEASE "${APP_PROJECT}/BOOST/lib/linux/${CONF_DIR}/Release")
endif()

if(MSVC)	
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM _HAS_STD_BYTE=0) 
	
	# /Zi - Create debugging information
	# /Od - Disable optimization
	# /MTd - Statically link the multithreaded debug version of the CRT
	# /MDd - Dynamically link the multithreaded debug version of the CRT
	# /RTC1 - Run time checks
	# /DNDEBUG - Disables asserts
	# /MT - Statically link the multithreaded release version of the CRT
	# /MD - Dynamically link the multithreaded release version of the CRT
	# /O2 - Optimize for speed
	# /Oy - Enable frame pointer omission (FPO) (otherwise CMake will automatically turn it off)
	# /GL - Link time code generation (whole program optimization)
	# /Gy - Function-level linking
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MT /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")

	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MTd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")

	#set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "/Zp2")

	STRING(REPLACE "/O2" "/Od" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
	STRING(REPLACE "/Ob2" "/Ob0" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
                                        
endif(MSVC)

#BOOST Lib add_library
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
elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE")
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
	
	elseif(${CMAKE_BUILD_TYPE_TO_UPPER} STREQUAL "RELEASE")
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




# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH PULSE WFMO) 
	else()
		#target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
	target_compile_definitions(${PROJECTNAME} PRIVATE PULSE WFMO)
else()
	target_compile_definitions(${PROJECTNAME} PRIVATE PULSE WFMO) 
endif(MSVC)



if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



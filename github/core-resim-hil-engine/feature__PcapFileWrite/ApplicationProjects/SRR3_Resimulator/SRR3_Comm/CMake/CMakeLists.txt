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
set(PROJECTNAME  "srr_comm")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#Setting common include headers
set(COMMON_THREADS "${COMMON_INC}/thread")
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers")
set(COMMONPLUGIN "${COMMON_INC}/plugin")
set(RIO_SOCKET "${SOURCES}/Shared")
set(BOOSTINC "${APP_PROJECT}/BOOST/Include")

message("common thread ${COMMON_INC}")

#including sources and headers
set(SOURCES "${SRR3COMM_PROJ}")
list(APPEND SOURCE_ALL "${SOURCES}/srr3_comm.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/ThBase.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/ThTimeOut.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/resource.h")
list(APPEND SOURCE_ALL "${SOURCES}/srr3_comm.h")
list(APPEND SOURCE_ALL "${SOURCES}/ThTimeOut.h")
list(APPEND SOURCE_ALL "${SOURCES}/ThBase.h")


if(MSVC)
	list(APPEND SOURCE_ALL "${SOURCES}/dllmain.cpp")
	list(APPEND SOURCE_ALL "${SOURCES}/Shared/Shared.h")
	list(APPEND SOURCE_ALL "${SOURCES}/Shared/Constants.h")
	list(APPEND SOURCE_ALL "${SOURCES}/EventPosix.cpp")
	list(APPEND SOURCE_ALL "${SOURCES}/EventPosix.h")
#BOOST Sockets
	list(APPEND SOURCE_ALL "${SOURCES}/asio_socket.cpp")
	list(APPEND SOURCE_ALL "${SOURCES}/asio_socket.h")
	list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn.h")
	list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn.cpp")
	#list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn_Raw_Sock.h")
	#list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn_Raw_Sock.cpp")
else()
	list(APPEND SOURCE_ALL "${SOURCES}/EventPosix.cpp")
	list(APPEND SOURCE_ALL "${SOURCES}/EventPosix.h")
	#list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn_Raw_Sock.h")
	#list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn_Raw_Sock.cpp")
	list(APPEND SOURCE_ALL "${SOURCES}/asio_socket.cpp")
	list(APPEND SOURCE_ALL "${SOURCES}/asio_socket.h")
	list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn.h")
	list(APPEND SOURCE_ALL "${SOURCES}/UdpIpConn.cpp")
endif()
#Creating static lib
add_library(${PROJECTNAME} SHARED "")

#Link source and headers to target
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

target_compile_definitions(${PROJECTNAME} PRIVATE SRR3_COMM_EXPORTS)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

endif()
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													#${COMMON_THREADS}
													${RIO_SOCKET}
													${BOOSTINC}
													)

if(MSVC)	
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM)

	#target_link_libraries(${PROJECTNAME} PRIVATE wsock32 ws2_32 FOUNDATION_INTERFACE Poco::Foundation Poco::Net )
	
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
 
endif(MSVC)

if(MSVC)
	target_link_libraries(${PROJECTNAME} ${ADDITIONAL_LIB})
	#target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS15/lib/windows/${CONF_DIR}/Release")
	elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		set(BOOST_LIB_DIR_DEBUG "${APP_PROJECT}/BOOST_VS19/lib/windows/${CONF_DIR}/Debug")
		set(BOOST_LIB_DIR_RELEASE "${APP_PROJECT}/BOOST_VS19/lib/windows/${CONF_DIR}/Release")
		message("BOOST_LIB_DIR_DEBUG PATH " ${BOOST_LIB_DIR_DEBUG} )
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
		#target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH PULSE WFMO _ITERATOR_DEBUG_LEVEL=0) 
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH PULSE WFMO BOOST_NO_LONG_LONG) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN)
		target_compile_definitions(${PROJECTNAME} PRIVATE PULSE WFMO)
else()
	target_compile_definitions(${PROJECTNAME} PRIVATE PULSE WFMO)
endif(MSVC)
if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



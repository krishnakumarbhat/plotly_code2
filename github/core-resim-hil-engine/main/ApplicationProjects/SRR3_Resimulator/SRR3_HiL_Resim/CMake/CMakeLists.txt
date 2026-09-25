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
set(PROJECTNAME  "SRR_HiL_Resim")
project(${PROJECTNAME})

#Linking directories
link_directories(${OUTPUT_PATH})

#Setting common include headers
set(COMMONINC "${COMMON_INC}/inc")
set(COMMONHELPER "${COMMON_INC}/helper")
set(COMMONHEADER "${COMMON_INC}/CommonHeaders")
set(COMMONPLUGIN "${COMMON_INC}/plugin")
set(COMMONTHREADS "${COMMON_INC}/thread")
set(BOOSTINC "${APP_PROJECT}/BOOST/Include")
if(MSVC)
set(FDX_HEADER "${FDX_PROJ}")
else()
endif()

#including sources and headers
set(SOURCES "${SRR3HIL_RESIM_PROJ}")
list(APPEND HEADERS "${COMMONHELPER}/CFileParsing.h")
list(APPEND HEADERS "${COMMONINC}/SRR3_HIL_API.h")
list(APPEND HEADERS "${COMMONHEADER}/Read_Config.h")
#list(APPEND HEADERS "${FDX_HEADER}/FDXRadarSensorData.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/CFileParsing.cpp")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/Cust_sel.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")
list(APPEND SOURCE_ALL "${COMMONINC}/dspace_hil_input.h")
list(APPEND SOURCE_ALL "${ETHERNETGRABBER_PROJ}/networkconfig.h")
list(APPEND SOURCE_ALL "${ETHERNETGRABBER_PROJ}/PacketGrabber.h")
if(MSVC)
else()
list(APPEND HEADERS "${COMMON_INC}/Utility/commonUtils.h")
endif()

list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_FL.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_FR.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_RL.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_RR.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_FC.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_RC.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_BPIL_LC.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/CallBackProcs_BPIL_RC.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/srr3_hil_resimulator.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/srr3_hil_resimulator.h")
list(APPEND SOURCE_ALL "${SOURCES}/DataQ.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/DataQ.h")
list(APPEND SOURCE_ALL "${SOURCES}/ThBase.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/ThBase.h")
list(APPEND SOURCE_ALL "${SOURCES}/ThMudpGrab.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/ThMudpGrab.h")
list(APPEND SOURCE_ALL "${SOURCES}/HiLConcurrentQueue.h")
list(APPEND SOURCE_ALL "${SOURCES}/HiLFixedSizeQueue.h")
if(MSVC)
else()
list(APPEND SOURCE_ALL "${SOURCES}/EventPosix.cpp")
list(APPEND SOURCE_ALL "${SOURCES}/EventPosix.h")
endif()
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/Read_Config.cpp")

#Creating executable lib
add_executable(${PROJECTNAME} "")

#Link source and headers to target
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

if(MSVC)
target_link_libraries(${PROJECTNAME} PRIVATE EthernetGrabber
												winmm.lib
												shlwapi.lib
												IniConfig
												CrossPlatform
												Mdf_Log
												#radar_stream_decoder
												FDX_Protocol)
else()
target_link_libraries(${PROJECTNAME} PRIVATE EthernetGrabber
												#/ winmm.lib
												#/ shlwapi.lib
												IniConfig
												CrossPlatform
												Mdf_Log
												#radar_stream_decoder
												 ${CMAKE_DL_LIBS})
endif()

set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../HiLInputs/pugixml/binaries")
find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/${CONF_DIR}")
find_library(pugixml_release pugixml HINTS "${LIB_PATH}/${CONF_DIR}")


target_link_libraries(${PROJECTNAME} PRIVATE debug "${pugixml_debug}"
									 optimized "${pugixml_release}"
									 )


set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)

	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

	STRING(REPLACE "-O2" "-Od" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

endif()

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else(MSVC)
target_compile_features(${PROJECTNAME} PRIVATE cxx_std_17)
endif(MSVC)

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)
if(MSVC)
target_compile_definitions(${PROJECTNAME} PRIVATE HIL_MODE
													ENABLE_CUSTHDR
													_CRT_SECURE_NO_WARNINGS
													-D_SCL_SECURE_NO_WARNINGS
													WIN32_LEAN_AND_MEAN
													WIN32_EXTRA_LEAN)
else()
target_compile_definitions(${PROJECTNAME} PRIVATE HIL_MODE
													ENABLE_CUSTHDR)
endif() 
if(MSVC)
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONHEADER} 
													${COMMONPLUGIN} 
													${COMMONTHREADS}
													${FDX_HEADER}
													${BOOSTINC})
													
else()
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONHEADER} 
													${COMMONPLUGIN} 
													${COMMONTHREADS}
													${BOOSTINC})
endif()


if(MSVC)
else()													
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	SET(CMAKE_CXX_FLAGS -pthread)
	set(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")
endif() 
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

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN)

else()
	target_compile_definitions(${PROJECTNAME} PRIVATE PULSE WFMO)
endif(MSVC)
if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



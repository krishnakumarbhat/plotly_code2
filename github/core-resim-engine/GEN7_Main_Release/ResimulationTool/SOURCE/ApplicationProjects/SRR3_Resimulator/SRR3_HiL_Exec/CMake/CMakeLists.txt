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

cmake_minimum_required(VERSION 3.2)

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

#including sources and headers
set(SOURCES "${SRR3HIL_EXE_PROJ}")
list(APPEND HEADERS "${COMMONINC}/common.h")
list(APPEND HEADERS "${COMMONINC}/XCPSeedKey.h")
list(APPEND HEADERS "${COMMONINC}/HiLBaseStream.h")
list(APPEND HEADERS "${COMMONHEADER}/Read_Config.h")

list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiL_OH_Prioratize.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLController.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLExecutive.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLOfflineStream.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/HiLOnlineStream.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/srr3_hil_exec.h")
list(APPEND HEADERS "${SRR3HIL_EXE_PROJ}/hil_command_req_resp_stream.h")

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
if(MSVC)
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/SyncManager.cpp")
else()
endif()
list(APPEND SOURCE_ALL "${COMMON_THREADS}/ThBase.cpp")
list(APPEND SOURCE_ALL "${SRR3HIL_EXE_PROJ}/Read_Config.cpp")

if(MSVC)
	list(APPEND SOURCE_ALL "${SOURCES}/dllmain.cpp")
endif()
#Creating static lib
add_library(${PROJECTNAME} SHARED "")

#Link source and headers to target

target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)
if(MSVC)
else()
set (CMAKE_C_FLAGS "-fPIC")
set (CMAKE_CXX_FLAGS "-fPIC")
endif()

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
													WIN32_EXTRA_LEAN)
else()	
target_compile_definitions(${PROJECTNAME} PRIVATE SRR3_HIL_EXPORTS
													PC_RESIM
													HIL_MODE
													ENABLE_CUSTHDR)
endif()	
					
if(MSVC)
target_link_libraries(${PROJECTNAME} 	CrossPlatform
										Mdf_Log
										#radar_stream_decoder
										FDX_Protocol)
else()
target_link_libraries(${PROJECTNAME}    CrossPlatform
										#radar_stream_decoder
										Mdf_Log)
endif()			

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

if(MSVC)
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													${COMMON_THREADS}
													${FDX_HEADER}
													${BOOSTINC})
else()
#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
													${COMMONHELPER} 
													${COMMONUDPHEADER} 
													${COMMONPLUGIN} 
													${COMMON_THREADS}
													${BOOSTINC})
endif()

if(MSVC)	
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
	
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

	set_target_properties(${PROJECTNAME} PROPERTIES
                                        COMPILE_FLAGS "/Zp2"
                                        )
endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
	
endif(MSVC)

if(UNIX)
	target_compile_options(${PROJECTNAME} PRIVATE -g -g3 -gdwarf-2)
endif(UNIX)



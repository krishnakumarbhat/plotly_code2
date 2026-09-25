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

set(PROJECTNAME  "APT_SRR_RESIM")
project(${PROJECTNAME})

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
set(SourceFiles "${APT_SRR_RESIM_PROJ}/")
set(SOURCE_ALL "${SourceFiles}/bmw_output_trace.c")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")
list(APPEND SOURCE_ALL "${COMMONSYM}/srr3_tracker_symbol.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")

#adding the sources specific to customer
if("${buildConfigToUpper}" STREQUAL "SIL")
	list(APPEND SOURCE_ALL "${SourceFiles}/dph_rr_adas_resim.cpp")
	list(APPEND HEADERS "${SourceFiles}/dph_rr_adas_resim.h")
endif()
list(APPEND SOURCE_ALL "${SourceFiles}/Error_Logging_Status.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/matData.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/Buspecidlookuptable.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/bmw_output_trace.c")
list(APPEND HEADERS "${SourceFiles}/apt_srr_resim_version.h")
list(APPEND HEADERS "${SourceFiles}/dph_rr_adas_resim.h")
list(APPEND HEADERS "${SourceFiles}/Error_Logging_Status.h")
list(APPEND HEADERS "${SourceFiles}/Resource.rc")

add_executable (${PROJECTNAME} "") 
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

if("${buildConfigToUpper}" STREQUAL "SIL")
target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR PC_RESIM MULTITHR)
endif()

set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

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
					DPH_RR_ADAS_Config 
					SRR_MUDP_Receive
					Mdf_Log
					Timing_Profile
					mudp_decoder_calib
					)
			
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
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN
													) 
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)

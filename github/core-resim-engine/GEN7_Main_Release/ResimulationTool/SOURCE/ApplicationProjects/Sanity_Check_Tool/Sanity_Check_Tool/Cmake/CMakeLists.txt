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

set(PROJECTNAME  "sanity_check_tool")
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

#including sources and headers
set(SourceFiles "${Sanity_Check_Tool_PROJ}/")

list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/cust_opt.cpp")
list(APPEND SOURCE_ALL "${COMMONSYM}/srr3_tracker_symbol.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")

#adding the sources specific to customer
set(SourceFiles "${Sanity_Check_Tool_PROJ}/") 
list(APPEND SOURCE_ALL "${SourceFiles}/sanity_check_tool.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/sanity_check_tool_options.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/PackUDPLogging.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/MUDP_Data_Extracter_Config.cpp")
list(APPEND HEADERS "${SourceFiles}/hil_embedded_utility.h")
list(APPEND HEADERS "${SourceFiles}/PackUDPLogging.h")
list(APPEND HEADERS "${SourceFiles}/sanity_check_tool_options.h")

add_executable (${PROJECTNAME} "") 
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

if("${buildConfigToUpper}" STREQUAL "SIL")
target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR PC_RESIM)
endif()

set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
						${COMMONHEADERS} 
						${COMMONHELPER} 
						${COMMONUDPHEADER} 
						${COMMONPLUGIN}
						${COMMONUTIL}
						)

set(ADDITIONAL_LIB 
					DPH_RR_ADAS_LOGGER 
					CrossPlatform                         
					srr3_decoder_dph
					radar_stream_decoder
					Mdf_Log
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
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)
target_link_libraries (${PROJECTNAME}  ${CMAKE_DL_LIBS}) 
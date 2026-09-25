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

set(PROJECTNAME  "ORCAS_MDF_Cutter_Tool")
project(${PROJECTNAME})

set(SourceFiles "${ORCAS_MDF_Cutter_Tool_PROJ}/")
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
#list(APPEND SOURCE_ALL "${COMMONHELPER}/VerboseLogUtil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.h")
list(APPEND SOURCE_ALL "${COMMONHELPER}/genutil.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/cust_opt.cpp")
list(APPEND SOURCE_ALL "${COMMONHELPER}/compute.c")
list(APPEND HEADERS "${COMMONINC}/Resim_Config.h")
list(APPEND SOURCE_ALL "${COMMONPLUGIN}/PluginLoader.h")

#adding the sources specific to customer
set(SourceFiles "${ORCAS_MDF_Cutter_Tool_PROJ}/")
#list(APPEND SOURCE_ALL "${SourceFiles}/mdf_udp_Data_Proc.cpp")
#list(APPEND SOURCE_ALL "${SourceFiles}/Converter_Config.cpp")
#list(APPEND SOURCE_ALL "${SourceFiles}/mdf_udp_Data_Proc.h")
list(APPEND SOURCE_ALL "${SourceFiles}/mdf_udp_Interface.cpp")
list(APPEND HEADERS "${SourceFiles}/mdf_udp_Interface.h")
#list(APPEND HEADERS "${SourceFiles}/MDF4_Decoder_version.h")
list(APPEND HEADERS "${SourceFiles}/SRR3_SIL_API.h")
list(APPEND SOURCE_ALL "${SourceFiles}/stdafx.cpp")
list(APPEND SOURCE_ALL "${SourceFiles}/ORCAS_MDF_Cutter_Tool.cpp")
list(APPEND HEADERS "${SourceFiles}/stdafx.h")
list(APPEND HEADERS "${SourceFiles}/targetver.h")


add_executable (${PROJECTNAME} "") 
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL} ${HEADERS})

add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)

if("${buildConfigToUpper}" STREQUAL "SIL")
target_compile_definitions(${PROJECTNAME} PRIVATE ENABLE_CUSTHDR PC_RESIM)
endif()

set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

#target_link_libraries(${PROJECTNAME} ccalib_mdf_x64_2-1-3)

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
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN ORCAS_CUTTER_TOOL) 
	endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)
target_link_libraries (${PROJECTNAME}  ${CMAKE_DL_LIBS}) 
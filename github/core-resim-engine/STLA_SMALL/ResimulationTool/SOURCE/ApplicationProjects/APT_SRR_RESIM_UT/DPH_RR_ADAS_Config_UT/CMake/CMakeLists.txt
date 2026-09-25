 #Build Steps
message(STATUS "******************DPH_RR_ADAS_Config_UT*******************")
set(PROJECTNAME  "DPH_RR_ADAS_Config_UT")
#newly added 1
add_executable(${PROJECTNAME} ${CMAKE_CURRENT_LIST_FILE})
# Set the CMake MSVC Runtime Library to MultiThreaded Debug (/MTd) 
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") 
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
#newly added 1
# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/../../../../../SOURCE")
set(COMMON_INC "${SOURCE_PROJECT}/CommonFiles")
set(APP_PROJECT "${SOURCE_PROJECT}/ApplicationProjects")
set(CORE_LIB_PROJECT "${SOURCE_PROJECT}/CoreLibraryProjects")
set(COMMON "${SOURCE_PROJECT}/Common")
set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/output_UT/${CMAKE_VS_PLATFORM_NAME}")

#1 end
#dph_rr_adas_config.cpp
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/dph_rr_adas_config.cpp")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/CommonFiles/CommonHeaders/dph_rr_adas_config.h")

list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/CommonFiles/inc/pugiconfig.hpp")
list(APPEND SOURCE_ALL "${SOURCE_PROJECT}/CommonFiles/inc/pugixml.hpp")

list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/AUDI/dph_rr_adas_audi_config.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/AUDI/dph_rr_adas_audi_config.h")


list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/BMW/dph_rr_adas_bmw_config.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/BMW/dph_rr_adas_bmw_config.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/src/dph_rr_adas_config_mock_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_Config_UT/inc/dph_rr_adas_config_UT.hpp")

#add_library(${PROJECTNAME} STATIC "")
#target_link_libraries(${PROJECTNAME} gtest_main gmock_main)


target_include_directories(${PROJECTNAME} PRIVATE ${HEADERS}) 
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
						${CMAKE_CURRENT_SOURCE_DIR}
						${COMMONHEADERS} 
						${COMMONHELPER} 
						${COMMONUDPHEADER} 
						${COMMONPLUGIN}
						${COMMONUTIL}
						${COMMOM_INC}
						${COMMOMINC}
						${BOOST_INC}
						${HEADERS}
						${UT_INC}
						${SOURCE_PROJECT}
						${COMMON}
						#${CORE_LIB_PROJECT}# for future usage
						)
set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)


set (CMAKE_CXX_STANDARD 11)


target_link_libraries(${PROJECTNAME} ${pugixml_debug} gtest_main gmock_main)

add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
option(COVERAGE "Option description" OFF) 
message("coverage Config Type is : ${COVERAGE}")
if(COVERAGE)
    execute_process(COMMAND OpenCppCoverage --modules=DPH_RR_ADAS_Config --excluded_sources c:\\users\\5l1niv\\ --excluded_sources f:\\dd\\ --excluded_sources c:\\boost* --excluded_sources C:\\Program* --excluded_sources googletest -- ./output_UT/x64/Debug/DPH_RR_ADAS_RESIM_UT.exe --export_type=html)
endif(COVERAGE)


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	#target_link_libraries (${PROJECTNAME} ${SOURCES} )

	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 

#	target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MD /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")
    target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Debug>:/MDd /Zi /Od /RTC1 ${CRT_FLAG_DEBUG}>")	
	
endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		#target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
		target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM WIN32_LEAN_AND_MEAN
	 												WIN32_EXTRA_LEAN)
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)


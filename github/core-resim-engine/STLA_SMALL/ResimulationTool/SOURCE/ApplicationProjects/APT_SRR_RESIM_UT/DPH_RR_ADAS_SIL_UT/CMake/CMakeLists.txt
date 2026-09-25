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
message(STATUS "**************DPH_RR_ADAS_SIL_UT**************")
set(PROJECTNAME  "DPH_RR_ADAS_SIL_UT")
project(${PROJECTNAME})
add_executable(${PROJECTNAME} ${CMAKE_CURRENT_LIST_FILE})
# Set the CMake MSVC Runtime Library to MultiThreaded Debug (/MTd) 
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") 
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
#Linking directories

#Adding required common header files
# Setting Source projects
set(SOURCE_PROJECT "${CMAKE_CURRENT_LIST_DIR}/../../../../../SOURCE")
set(COMMON_INC "${SOURCE_PROJECT}/CommonFiles")
set(APP_PROJECT "${SOURCE_PROJECT}/ApplicationProjects")
set(CORE_LIB_PROJECT "${SOURCE_PROJECT}/CoreLibraryProjects")
set(COMMON "${SOURCE_PROJECT}/Common")
set(COMMONINC "${COMMON_INC}/inc/")
set(COMMONSYM "${COMMON_INC}/sym/")
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders/")
set(COMMONHELPER "${COMMON_INC}/helper/")
set(COMMONUDPHEADER "${COMMON_INC}/udp_headers/")
set(COMMONPLUGIN "${COMMON_INC}/plugin/")
set(COMMONUTIL "${COMMON_INC}/Utility/")
set(BOOST_INC "${APP_PROJECT}/BOOST/Include/")
set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/output_UT/${CMAKE_VS_PLATFORM_NAME}")
set(CUSTOM_CONFIG "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/")
set(UT_INC "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/inc/")
set(SRC_INC "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.h")

list(APPEND HEADERS "${COMMONINC}/guard.h")
list(APPEND HEADERS "${COMMONUTIL}/commonUtils.h")
list(APPEND HEADERS "${COMMONHEADERS}/dph_rr_adas_logger.h")
list(APPEND HEADERS "${COMMONHEADERS}/types.h")
list(APPEND HEADERS "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.h")

list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.h")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_SIL/SRR3_Internal_Data_Logger.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/src/SRR3_Internal_Data_Logger_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_SIL_UT/inc/SRR3_Internal_Data_Logger_UT.hpp")

target_include_directories(${PROJECTNAME} PRIVATE ${HEADERS}) 
target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} 
						${CMAKE_CURRENT_SOURCE_DIR}
						${COMMONHEADERS} 
						${COMMONHELPER} 
						${COMMONUDPHEADER} 
						${COMMONPLUGIN}
						${COMMONUTIL}
						${BOOST_INC}
						${HEADERS}
						${UT_INC}
						${SRC_INC}
						)
set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)


set (CMAKE_CXX_STANDARD 11)

# include(FetchContent)
# FetchContent_Declare(
  # googletest
  # # Specify the commit you depend on and update it regularly.
  # URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
target_link_libraries(${PROJECTNAME} gtest_main gmock_main)

add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
option(COVERAGE "Option description" OFF) 
message("coverage Config Type is : ${COVERAGE}")
if(COVERAGE)
    execute_process(COMMAND OpenCppCoverage --modules=DPH_RR_ADAS_SIL --excluded_sources c:\\users\\5l1niv\\ --excluded_sources f:\\dd\\ --excluded_sources c:\\boost* --excluded_sources C:\\Program* --excluded_sources googletest -- ./output_UT/x64/Debug/DPH_RR_ADAS_SIL_UT.exe --export_type=html)
endif(COVERAGE)


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
	#target_link_libraries (${PROJECTNAME} ${SOURCES} )

	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS _WIN_PLATFORM) 

	#target_compile_options(${PROJECTNAME} PRIVATE "$<$<CONFIG:Release>:/MD /DNDEBUG /O2 /Oy /GL /Gy ${CRT_FLAG_RELEASE}>")
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


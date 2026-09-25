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
message(STATUS "**************DPH_RR_ADAS_LOGGER_UT**************")
set(PROJECTNAME "DPH_RR_ADAS_LOGGER_UT")
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
set(OUTPUT_PATH "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/output_UT/${CMAKE_VS_PLATFORM_NAME}")
set(CUSTOM_CONFIG "${APP_PROJECT}/DPH_RR_ADAS_Config/CustomConfig/")


set(UT_INC "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/inc/")
set(SOURCE_ALL "${CORE_LIB_PROJECT}/CrossPlatform/commonUtils.cpp")
list(APPEND HEADERS "${COMMONINC}/guard.h")
list(APPEND HEADERS "${COMMONUTIL}/commonUtils.h")
list(APPEND HEADERS "${COMMONHEADERS}/dph_rr_adas_logger.h")
list(APPEND HEADERS "${COMMONHEADERS}/types.h")

list(APPEND HEADERS "${COMMONHEADERS}/dph_rr_adas_logger_lib_loader.h")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_LOGGER/dph_rr_adas_logger_lib_loader.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/src/dph_rr_adas_logger_lib_loader_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/src/dph_rr_adas_logger_mock_UT.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/inc/dph_rr_adas_logger_UT.hpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/APT_SRR_RESIM_UT/DPH_RR_ADAS_LOGGER_UT/inc/dph_rr_adas_logger_lib_loader_UT.hpp")
#list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_Config/dph_rr_adas_config.cpp")
list(APPEND SOURCE_ALL "${APP_PROJECT}/DPH_RR_ADAS_LOGGER/dph_rr_adas_logger.cpp")



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
						)
set_target_properties(${PROJECTNAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)


set (CMAKE_CXX_STANDARD 11)

target_link_libraries(${PROJECTNAME} gtest_main gmock_main)

add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
option(COVERAGE "Option description" OFF) 
message("coverage Config Type is : ${COVERAGE}")
if(COVERAGE)
    execute_process(COMMAND OpenCppCoverage --modules=DPH_RR_ADAS_LOGGER --excluded_sources c:\\users\\5l1niv\\ --excluded_sources f:\\dd\\ --excluded_sources c:\\boost* --excluded_sources C:\\Program* --excluded_sources googletest -- ./output_UT/x64/Debug/dph_rr_adas_logger_ut.exe --export_type=html)
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


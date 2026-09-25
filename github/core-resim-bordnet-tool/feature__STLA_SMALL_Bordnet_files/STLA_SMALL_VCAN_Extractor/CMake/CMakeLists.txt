##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "STLA_SMALL_VCAN_Extractor")
project(${project_name})

set(STLA_SMALL_VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(STLA_SMALL_VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(UTILITY "${CMAKE_CURRENT_LIST_DIR}/../../Utility")
set(STLA_SMALL_SRR7P_VCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SMALL_SRR7P_VCAN_RL_Decoder")
set(STLA_SMALL_SRR7P_VCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SMALL_SRR7P_VCAN_RR_Decoder")
set(STLA_SMALL_FLR7_VCAN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../STLA_SMALL_FLR7_VCAN_Decoder")

set(SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SMALL_SRR7P_VcanDecoderRL.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SMALL_SRR7P_VcanDecoderRL.cpp")

list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SMALL_SRR7P_VcanDecoderRR.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SMALL_SRR7P_VcanDecoderRR.cpp")


list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Include/STLA_SMALL_FLR7_VcanDecoderFC.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Source/STLA_SMALL_FLR7_VcanDecoderFC.cpp")


list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Source/STLA_SMALL_SRR7P_VcanRLBase.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Source/STLA_SMALL_SRR7P_VcanRLBase_V2.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Source/STLA_SMALL_SRR7P_VcanRLBase_V7.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Source/STLA_SMALL_SRR7P_VcanRLBase_V8.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Source/STLA_SMALL_SRR7P_VcanRLBase_V9.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Source/STLA_SMALLPcanHDF_RL_V9.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanBaseRL.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanBaseRL_V2.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanBaseRL_V7.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanBaseRL_V8.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanBaseRL_V9.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanRLBase.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanRLBase_V2.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanRLBase_V7.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanRLBase_V8.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALL_SRR7P_VcanRLBase_V9.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include/STLA_SMALLPcanHDF_RL_V9.h")

list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Source/STLA_SMALL_SRR7P_VcanRRBase.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Source/STLA_SMALL_SRR7P_VcanRRBase_V2.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Source/STLA_SMALL_SRR7P_VcanRRBase_V7.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Source/STLA_SMALL_SRR7P_VcanRRBase_V8.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Source/STLA_SMALL_SRR7P_VcanRRBase_V9.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Source/STLA_SMALLPcanHDF_RR_V9.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanBaseRR.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanBaseRR_V2.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanBaseRR_V7.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanBaseRR_V8.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanBaseRR_V9.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanRRBase.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanRRBase_V2.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanRRBase_V7.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanRRBase_V8.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALL_SRR7P_VcanRRBase_V9.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include/STLA_SMALLPcanHDF_RR_V9.h")


list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Source/STLA_SMALL_FLR7_VcanFCBase.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Source/STLA_SMALL_FLR7_VcanFCBase_V2.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Source/STLA_SMALL_FLR7_VcanFCBase_V7.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Source/STLA_SMALL_FLR7_VcanFCBase_V8.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Source/STLA_SMALL_FLR7_VcanFCBase_V9.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Source/STLA_SMALLPcanHDF_FC_V9.cpp")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanBaseFC.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanBaseFC_V2.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanBaseFC_V7.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanBaseFC_V8.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanBaseFC_V9.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanFCBase.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanFCBase_V2.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanFCBase_V7.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanFCBase_V8.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALL_FLR7_VcanFCBase_V9.h")
list(APPEND SOURCES "${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include/STLA_SMALLPcanHDF_FC_V9.h")

source_group(TREE ${STLA_SMALL_VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries(${target_name} PRIVATE Mdf_Log)
target_link_libraries(${target_name} PRIVATE CrossPlatform)
target_link_libraries(${target_name} PRIVATE Utility)

target_include_directories(${target_name} PUBLIC ${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_SRR7P_VCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/STLA_SMALL_FLR7_VCAN_FC/Include)


target_include_directories(${target_name} PUBLIC ${STLA_SMALL_VCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${STLA_SMALL_VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${STLA_SMALL_SRR7P_VCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${STLA_SMALL_SRR7P_VCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${STLA_SMALL_FLR7_VCAN_INTERFACEFILES})

target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)
target_include_directories(${target_name} PRIVATE ${UTILITY})
target_include_directories(${target_name} PUBLIC ${COMMON_INC}/HDF/include)

if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${project_name} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)

# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)


if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)

# ======================== HDF5 Integration =========================
if(MSVC)

	find_library(hdf_libs hdf5 hdf5_cpp HINTS "${HDF5_LIBRARIES}/${CMAKE_BUILD_TYPE}")
	target_link_libraries(${target_name} PRIVATE ${hdf_libs})
	message(STATUS "HDF5 include directories: ${HDF5_INCLUDE_DIRS}")
	message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")
	#find_file(HDF_DLL NAMES hdf5.dll hdf5_cpp.dll HINTS "${HDF5_LIBRARIES}/DLL")
else()
	target_link_libraries(${target_name} PUBLIC dl)

	find_package(HDF5 QUIET COMPONENTS C CXX)
	if(HDF5_FOUND)
		if(HDF5_INCLUDE_DIRS)
			target_include_directories(${target_name} PUBLIC ${HDF5_INCLUDE_DIRS})
		endif()

		if(TARGET HDF5::HDF5)
			target_link_libraries(${target_name} PUBLIC HDF5::HDF5)
		elseif(TARGET HDF5::CXX)
			target_link_libraries(${target_name} PUBLIC HDF5::CXX)
			if(TARGET HDF5::C)
				target_link_libraries(${target_name} PUBLIC HDF5::C)
			endif()
		else()
			set(hdf_libs ${HDF5_CXX_LIBRARIES} ${HDF5_LIBRARIES})
			target_link_libraries(${target_name} PUBLIC ${hdf_libs})
		endif()
	else()
		find_library(hdf_cpp_lib NAMES hdf5_serial_cpp hdf5_cpp libhdf5_serial_cpp.so libhdf5_cpp.so HINTS /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /usr/local/lib)
		find_library(hdf_c_lib NAMES hdf5_serial hdf5 libhdf5_serial.so libhdf5.so HINTS /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /usr/local/lib)
		if(NOT hdf_cpp_lib OR NOT hdf_c_lib)
			message(FATAL_ERROR "Missing HDF5 libraries on Linux. Install libhdf5-dev or ensure hdf5_serial_cpp/hdf5_cpp and hdf5_serial/hdf5 are discoverable.")
		endif()
		target_link_libraries(${target_name} PUBLIC ${hdf_cpp_lib} ${hdf_c_lib})
	endif()

endif(MSVC)
if(UNIX)
	find_path(HDF5_SYSTEM_INCLUDE_DIR NAMES H5Cpp.h PATHS /usr/include/hdf5/serial /usr/local/include/hdf5/serial /usr/include /usr/local/include)
	if(HDF5_SYSTEM_INCLUDE_DIR)
		target_include_directories(${target_name} PUBLIC ${HDF5_SYSTEM_INCLUDE_DIR})
	endif()
endif()

if(MSVC)
  #add_custom_command(
    #TARGET ${project_name} POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy 
      #${HDF_DLL} 
     #$<TARGET_FILE_DIR:${project_name}>)
endif()
	
# ======================== TARGET SOURCES =========================
target_sources(${target_name} PRIVATE ${SOURCES})

# ======================== OUTPUT PATHS =========================
if(OUTPUT_PATH)
set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${project_name} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)


# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${target_name} PRIVATE _MBCS _WIN_PLATFORM) 
endif(MSVC)


if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)
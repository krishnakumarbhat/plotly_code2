##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "MCIP_PCAN_Extractor")
project(${project_name})

set(MCIP_PCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(MCIP_PCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(UTILITY "${CMAKE_CURRENT_LIST_DIR}/../../Utility")

set(MCIP_PCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../MCIP_PCAN_FL_Decoder")
set(MCIP_PCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../MCIP_PCAN_FR_Decoder")
set(MCIP_PCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../MCIP_PCAN_RL_Decoder")
set(MCIP_PCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../MCIP_PCAN_RR_Decoder")
set(MCIP_PCAN_FLR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../MCIP_PCAN_FLR_Decoder")
set(MCIP_AUTOGEN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../Auto_Gen_Files")

#FL
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Include/MCIPPcanDecoderFL.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Source/MCIPPcanDecoderFL.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Source/MCIPPcanFLBase_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include/MCIPPcanBaseFL_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include/MCIPPcanFLBase_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Source/MCIPPcanHDF_FL_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include/MCIPPcanHDF_FL_V2.h")

#FL V4
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Source/MCIPPcanFLBase_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include/MCIPPcanBaseFL_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include/MCIPPcanFLBase_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Source/MCIPPcanHDF_FL_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include/MCIPPcanHDF_FL_V4.h")

#FR
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Include/MCIPPcanDecoderFR.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Source/MCIPPcanDecoderFR.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Source/MCIPPcanFRBase_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include/MCIPPcanBaseFR_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include/MCIPPcanFRBase_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Source/MCIPPcanHDF_FR_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include/MCIPPcanHDF_FR_V2.h")

#FR V4
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Source/MCIPPcanFRBase_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include/MCIPPcanBaseFR_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include/MCIPPcanFRBase_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Source/MCIPPcanHDF_FR_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include/MCIPPcanHDF_FR_V4.h")

#RL
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Include/MCIPPcanDecoderRL.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Source/MCIPPcanDecoderRL.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Source/MCIPPcanRLBase_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include/MCIPPcanBaseRL_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include/MCIPPcanRLBase_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Source/MCIPPcanHDF_RL_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include/MCIPPcanHDF_RL_V2.h")

#RL V4
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Source/MCIPPcanRLBase_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include/MCIPPcanBaseRL_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include/MCIPPcanRLBase_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Source/MCIPPcanHDF_RL_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include/MCIPPcanHDF_RL_V4.h")

#RR
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Include/MCIPPcanDecoderRR.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Source/MCIPPcanDecoderRR.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Source/MCIPPcanRRBase_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include/MCIPPcanBaseRR_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include/MCIPPcanRRBase_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Source/MCIPPcanHDF_RR_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include/MCIPPcanHDF_RR_V2.h")

#RR V4
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Source/MCIPPcanRRBase_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include/MCIPPcanBaseRR_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include/MCIPPcanRRBase_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Source/MCIPPcanHDF_RR_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include/MCIPPcanHDF_RR_V4.h")

#FLR
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Include/MCIPPcanDecoderFLR.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/Source/MCIPPcanDecoderFLR.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Source/MCIPPcanFLRBase_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include/MCIPPcanBaseFLR_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include/MCIPPcanFLRBase_V2.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Source/MCIPPcanHDF_FLR_V2.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include/MCIPPcanHDF_FLR_V2.h")

#FLR V4
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Source/MCIPPcanFLRBase_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include/MCIPPcanBaseFLR_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include/MCIPPcanFLRBase_V4.h")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Source/MCIPPcanHDF_FLR_V4.cpp")
list(APPEND SOURCES "${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include/MCIPPcanHDF_FLR_V4.h")


source_group(TREE ${MCIP_PCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries(${target_name} PRIVATE Mdf_Log)
target_link_libraries(${target_name} PRIVATE CrossPlatform)
target_link_libraries(${target_name} PRIVATE Utility)


target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FL/Include)
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_PROJECT_ROOT_PATH}/MCIP_PCAN_FLR/Include)


target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_FL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${MCIP_PCAN_FLR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${MCIP_AUTOGEN_INTERFACEFILES/MCIP_V2/Include})


target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)
target_include_directories(${target_name} PRIVATE ${UTILITY})
target_include_directories(${target_name} PUBLIC ${COMMON_INC}/HDF/include)

# ======================== HDF5 Integration =========================
if(MSVC)

	find_library(hdf_c_lib NAMES hdf5 HINTS "${HDF5_LIBRARIES}/${CMAKE_BUILD_TYPE}")
	find_library(hdf_cpp_lib NAMES hdf5_cpp HINTS "${HDF5_LIBRARIES}/${CMAKE_BUILD_TYPE}")
	if(NOT hdf_c_lib OR NOT hdf_cpp_lib)
		message(FATAL_ERROR "Missing vendored HDF5 libraries in ${HDF5_LIBRARIES}/${CMAKE_BUILD_TYPE}.")
	endif()
	target_link_libraries(${target_name} PRIVATE ${hdf_cpp_lib} ${hdf_c_lib})
	message(STATUS "HDF5 include directories: ${HDF5_INCLUDE_DIRS}")
	message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")
	#find_file(HDF_DLL NAMES hdf5.dll hdf5_cpp.dll HINTS "${HDF5_LIBRARIES}/DLL")
else()
	target_link_libraries(${target_name} PUBLIC dl)

	find_library(vendored_hdf_cpp_lib NAMES hdf5_cpp libhdf5_cpp.so HINTS "${HDF5_ROOT}/linux/lib" NO_DEFAULT_PATH)
	find_library(vendored_hdf_c_lib NAMES hdf5 libhdf5.so HINTS "${HDF5_ROOT}/linux/lib" NO_DEFAULT_PATH)
	if(vendored_hdf_cpp_lib AND vendored_hdf_c_lib)
		target_link_libraries(${target_name} PUBLIC ${vendored_hdf_cpp_lib} ${vendored_hdf_c_lib})
	else()

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
	endif()

endif(MSVC)

if(UNIX)
	find_path(HDF5_SYSTEM_INCLUDE_DIR NAMES H5Cpp.h PATHS /usr/include/hdf5/serial /usr/local/include/hdf5/serial /usr/include /usr/local/include)
	if(HDF5_SYSTEM_INCLUDE_DIR)
		target_include_directories(${target_name} PUBLIC ${HDF5_SYSTEM_INCLUDE_DIR})
	endif()
endif()

# ======================== LINK LIBRARIES =========================
target_link_libraries(${target_name} PRIVATE Mdf_Log)
target_link_libraries(${target_name} PRIVATE CrossPlatform)

#if(MSVC)
#  add_custom_command(
#    TARGET ${project_name} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy 
#      ${HDF_DLL} 
#      $<TARGET_FILE_DIR:${project_name}>)
#endif()
	
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
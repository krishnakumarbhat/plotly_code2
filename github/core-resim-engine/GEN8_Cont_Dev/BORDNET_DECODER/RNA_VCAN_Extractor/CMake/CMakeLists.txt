##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "RNA_VCAN_Extractor")
project(${project_name})

set(RNA_VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(RNA_VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(RNA_VCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../RNA_VCAN_FL_Decoder")
set(RNA_VCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../RNA_VCAN_FR_Decoder")
set(RNA_VCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../RNA_VCAN_RL_Decoder")
set(RNA_VCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../RNA_VCAN_RR_Decoder")
set(RNA_RT_RANGE_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../RNA_RT_RANGE_Decoder")



set(SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Include/RNAVcanDecoderFL.h")

#RT_RANGE
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_RT_RANGE/Source/RNA_HDF_RT_RANGE.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_RT_RANGE/Include/RNA_HDF_RT_Range.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_RT_RANGE/Source/RNAVcanRTRangeBase.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_RT_RANGE/Include/RNAVcanBaseRTRange.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_RT_RANGE/Include/RNAVcanRTRangeBase.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Include/RNAVcanDecoderRTRange.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Source/RNAVcanDecoderRTRange.cpp")



list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Source/RNAVcanDecoderFL.cpp")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Include/RNAVcanDecoderFR.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Source/RNAVcanDecoderFR.cpp")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Include/RNAVcanDecoderRL.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Source/RNAVcanDecoderRL.cpp")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Include/RNAVcanDecoderRR.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/Source/RNAVcanDecoderRR.cpp")


list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Source/RNAVcanFLBase.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanBaseFL.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanFLBase.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Source/RNAVcanFRBase.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanBaseFR.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanFRBase.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Source/RNAVcanRLBase.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanBaseRL.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanRLBase.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Source/RNAVcanRRBase.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanBaseRR.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanRRBase.h")

#RNA_V1

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Source/RNAVcanFLBase_V1.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Source/RNAVcanHDF_FL.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanBaseFL_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanFLBase_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanHDF_FL.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Source/RNAVcanFRBase_V1.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Source/RNAVcanHDF_FR.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanBaseFR_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanFRBase_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanHDF_FR.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Source/RNAVcanRLBase_V1.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Source/RNAVcanHDF_RL.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanBaseRL_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanRLBase_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanHDF_RL.h")


list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Source/RNAVcanRRBase_V1.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Source/RNAVcanHDF_RR.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanBaseRR_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanRRBase_V1.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanHDF_RR.h")

#RNA_V2

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Source/RNAVcanFLBase_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Source/RNAVcanHDF_FL.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Source/RNAVcanHDF_FL_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanBaseFL_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanFLBase_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanHDF_FL.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include/RNAVcanHDF_FL_V2.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Source/RNAVcanFRBase_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Source/RNAVcanHDF_FR.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Source/RNAVcanHDF_FR_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanBaseFR_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanFRBase_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanHDF_FR.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include/RNAVcanHDF_FR_V2.h")

list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Source/RNAVcanRLBase_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Source/RNAVcanHDF_RL.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Source/RNAVcanHDF_RL_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanBaseRL_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanRLBase_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanHDF_RL.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include/RNAVcanHDF_RL_V2.h")


list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Source/RNAVcanRRBase_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Source/RNAVcanHDF_RR.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Source/RNAVcanHDF_RR_V2.cpp")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanBaseRR_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanRRBase_V2.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanHDF_RR.h")
list(APPEND SOURCES "${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include/RNAVcanHDF_RR_V2.h")

source_group(TREE ${RNA_VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FL/Include)
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_VCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_PROJECT_ROOT_PATH}/RNA_RT_RANGE/Include)


target_include_directories(${target_name} PUBLIC ${RNA_VCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_FL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${RNA_VCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${RNA_RT_RANGE_INTERFACEFILES})

target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

# ======================== HDF5 Integration =========================
if(MSVC)

	find_library(hdf_libs hdf5 hdf5_cpp HINTS "${HDF5_LIBRARIES}/${CMAKE_BUILD_TYPE}")
	target_link_libraries(${target_name} "${hdf_libs}")
	message(STATUS "HDF5 include directories: ${HDF5_INCLUDE_DIRS}")
	message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")
	find_file(HDF_DLL NAMES hdf5.dll hdf5_cpp.dll HINTS "${HDF5_LIBRARIES}/DLL")
else()
	target_link_libraries(${target_name} dl)
	
	find_library(hdf_libs NAMES libhdf5.so libhdf5_cpp.so HINTS "${HDF5_LIBRARIES}/../linux/lib")

endif(MSVC)
target_link_libraries(${target_name} "${hdf_libs}")


 # If manual paths are required, uncomment and modify the below lines:
target_include_directories(${target_name} PUBLIC "../../Common_Headers/HDF/include")

# ======================== LINK LIBRARIES =========================
target_link_libraries(${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

if(MSVC)
  add_custom_command(
    TARGET ${project_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
      ${HDF_DLL} 
      $<TARGET_FILE_DIR:${project_name}>)
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
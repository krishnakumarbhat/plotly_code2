##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "CEER_PCAN_Extractor")
project(${project_name})

set(CEER_PCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(CEER_PCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(CEER_PCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../CEER_PCAN_FL_Decoder")
set(CEER_PCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../CEER_PCAN_FR_Decoder")
set(CEER_PCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../CEER_PCAN_RL_Decoder")
set(CEER_PCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../CEER_PCAN_RR_Decoder")


list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Include/CEERPcanDecoderFL.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Source/CEERPcanDecoderFL.cpp")

list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FL/Source/CEERPcanFLBase.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FL/Include/CEERPcanBaseFL.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FL/Include/CEERPcanFLBase.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FL/Source/CEERPcanHDF_FL.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FL/Include/CEERPcanHDF_FL.h")

list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Include/CEERPcanDecoderFR.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Source/CEERPcanDecoderFR.cpp")

list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FR/Source/CEERPcanFRBase.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FR/Include/CEERPcanBaseFR.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FR/Include/CEERPcanFRBase.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FR/Source/CEERPcanHDF_FR.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FR/Include/CEERPcanHDF_FR.h")


list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Include/CEERPcanDecoderRL.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Source/CEERPcanDecoderRL.cpp")

list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RL/Source/CEERPcanRLBase.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RL/Include/CEERPcanBaseRL.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RL/Include/CEERPcanRLBase.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RL/Source/CEERPcanHDF_RL.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RL/Include/CEERPcanHDF_RL.h")

list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Include/CEERPcanDecoderRR.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/Source/CEERPcanDecoderRR.cpp")

list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RR/Source/CEERPcanRRBase.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RR/Include/CEERPcanBaseRR.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RR/Include/CEERPcanRRBase.h")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RR/Source/CEERPcanHDF_RR.cpp")
list(APPEND SOURCES "${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RR/Include/CEERPcanHDF_RR.h")



source_group(TREE ${CEER_PCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)


target_include_directories(${target_name} PUBLIC ${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FL/Include)
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_PROJECT_ROOT_PATH}/CEER_PCAN_RR/Include)


target_include_directories(${target_name} PUBLIC ${CEER_PCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_FL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${CEER_PCAN_RR_INTERFACEFILES})


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
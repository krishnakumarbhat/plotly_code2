##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "GPO_GEN8_PCAN_Extractor")
project(${project_name})

set(GPO_GEN8_PCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(GPO_GEN8_PCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(UTILITY "${CMAKE_CURRENT_LIST_DIR}/../../Utility")
set(GPO_GEN8_PCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../GPO_GEN8_PCAN_FR_Decoder")
set(GPO_GEN8_PCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../GPO_GEN8_PCAN_FL_Decoder")
set(GPO_GEN8_PCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../GPO_GEN8_PCAN_RL_Decoder")
set(GPO_GEN8_PCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../GPO_GEN8_PCAN_RR_Decoder")
set(GPO_GEN8_PCAN_FLR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../GPO_GEN8_PCAN_FLR_Decoder")
set(GPO_GEN8_AUTOGEN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../Auto_Gen_Files")

#FR
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Include/GPO_GEN8_PcanDecoderFR.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Source/GPO_GEN8_PcanDecoderFR.cpp")
#FR V1_1
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FR/Source/GPO_GEN8_PcanHDF_FR_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FR/Include/GPO_GEN8_PcanHDF_FR_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FR/Source/GPO_GEN8_PcanFRBase_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FR/Include/GPO_GEN8_PcanBaseFR_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FR/Include/GPO_GEN8_PcanFRBase_V1_0.h")

#FL
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Include/GPO_GEN8_PcanDecoderFL.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Source/GPO_GEN8_PcanDecoderFL.cpp")
#FL V1_1
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FL/Source/GPO_GEN8_PcanHDF_FL_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FL/Include/GPO_GEN8_PcanHDF_FL_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FL/Source/GPO_GEN8_PcanFLBase_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FL/Include/GPO_GEN8_PcanBaseFL_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FL/Include/GPO_GEN8_PcanFLBase_V1_0.h")

#RR
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Include/GPO_GEN8_PcanDecoderRR.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Source/GPO_GEN8_PcanDecoderRR.cpp")
#RR V1_1
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RR/Source/GPO_GEN8_PcanHDF_RR_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RR/Include/GPO_GEN8_PcanHDF_RR_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RR/Source/GPO_GEN8_PcanRRBase_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RR/Include/GPO_GEN8_PcanBaseRR_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RR/Include/GPO_GEN8_PcanRRBase_V1_0.h")

#RL
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Include/GPO_GEN8_PcanDecoderRL.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Source/GPO_GEN8_PcanDecoderRL.cpp")
#RL V1_1
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RL/Source/GPO_GEN8_PcanHDF_RL_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RL/Include/GPO_GEN8_PcanHDF_RL_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RL/Source/GPO_GEN8_PcanRLBase_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RL/Include/GPO_GEN8_PcanBaseRL_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RL/Include/GPO_GEN8_PcanRLBase_V1_0.h")

#FLR
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Include/GPO_GEN8_PcanDecoderFLR.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Source/GPO_GEN8_PcanDecoderFLR.cpp")
#FLR V1_1
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FLR/Source/GPO_GEN8_PcanHDF_FLR_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FLR/Include/GPO_GEN8_PcanHDF_FLR_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FLR/Source/GPO_GEN8_PcanFLRBase_V1_0.cpp")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FLR/Include/GPO_GEN8_PcanBaseFLR_V1_0.h")
list(APPEND SOURCES "${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FLR/Include/GPO_GEN8_PcanFLRBase_V1_0.h")



source_group(TREE ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)
target_link_libraries(${target_name} Utility)


target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FL/Include)
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/GPO_GEN8_PCAN_FLR/Include)


target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_FL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_PCAN_FLR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${GPO_GEN8_AUTOGEN_INTERFACEFILES/GPO_GEN8_V1_0/Include})


target_sources(${target_name} PRIVATE ${SOURCES})

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)
target_include_directories(${target_name} PRIVATE ${UTILITY})

# ======================== HDF5 Integration =========================
if(MSVC)

	find_library(hdf_libs hdf5 hdf5_cpp HINTS "${HDF5_LIBRARIES}/${CMAKE_BUILD_TYPE}")
	target_link_libraries(${target_name} "${hdf_libs}")
	message(STATUS "HDF5 include directories: ${HDF5_INCLUDE_DIRS}")
	message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")
	#find_file(HDF_DLL NAMES hdf5.dll hdf5_cpp.dll HINTS "${HDF5_LIBRARIES}/DLL")
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
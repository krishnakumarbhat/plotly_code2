##########################
### Project definition ###
##########################
#CMAKE_TRATON_EXTRACTOR###
cmake_minimum_required(VERSION 4.0)

set(project_name  "TRATON_SRR6P_VCAN_Extractor")
project(${project_name})

set(TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(TRATON_SRR6P_VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_PATH "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF")


set(TRATON_SRR6P_VCAN_FC_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../TRATON_SRR6P_VCAN_FC_Decoder")
set(TRATON_SRR6P_VCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../TRATON_SRR6P_VCAN_RL_Decoder")
set(TRATON_SRR6P_VCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../TRATON_SRR6P_VCAN_RR_Decoder")

set(SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/TRATONVcanDecoderRL.h")
	#======================================C1_3==========================

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/TRATONVcanRLBase_C1_3.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/TRATONVcanBaseRL_C1_3.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Source/TRATONVcanRLBase_C1_3.cpp")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/HDF_DataHandler_RL.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Source/HDF_DataHandler_RL.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/HDF_DataHandler_RR.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Source/HDF_DataHandler_RR.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/TRATONVcanRRBase_C1_3.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/TRATONVcanBaseRR_C1_3.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Source/TRATONVcanRRBase_C1_3.cpp")

	#======================================C1_2==========================

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Include/TRATONVcanBaseFC_C1_2.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Include/TRATONVcanFCBase_C1_2.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Source/TRATONVcanFCBase_C1_2.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/TRATONVcanRLBase_C1_2.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/TRATONVcanBaseRL_C1_2.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Source/TRATONVcanRLBase_C1_2.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/TRATONVcanRRBase_C1_2.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/TRATONVcanBaseRR_C1_2.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Source/TRATONVcanRRBase_C1_2.cpp")

	#======================================TRATON===========================

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Source/TRATONVcanRRBase.cpp")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/TRATONVcanDecoderRL.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/TRATONVcanDecoderRR.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/TRATONVcanDecoderRR.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include/TRATONVcanDecoderFC.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Source/TRATONVcanDecoderFC.cpp")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Source/TRATONVcanRLBase.cpp")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/TRATONVcanBaseRL.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include/TRATONVcanRLBase.h")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/TRATONVcanBaseRR.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include/TRATONVcanRRBase.h")

list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Source/TRATONVcanFCBase.cpp")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Include/TRATONVcanBaseFC.h")
list(APPEND SOURCES "${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Include/TRATONVcanFCBase.h")


source_group(TREE ${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

# ======================== INCLUDE DIRECTORIES =========================
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_FC/Include)
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/TRATON_VCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_PROJECT_ROOT_PATH}/Include)
target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_FC_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${TRATON_SRR6P_VCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)


 # If manual paths are required, uncomment and modify the below lines:
target_include_directories(${target_name} PUBLIC "../../Common_Headers/HDF/include")

# ======================== LINK LIBRARIES =========================
target_link_libraries(${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)

# ======================== HDF5 Integration =========================
if(MSVC)
  find_file(HDF_DLL NAMES hdf5.dll HINTS "${HDF5_PATH}/windows/lib/DLL")
  find_file(HDF_CPP_DLL NAMES hdf5_cpp.dll HINTS "${HDF5_PATH}/windows/lib/DLL")
  find_file(HDF_LIB NAMES hdf5.lib HINTS "${HDF5_PATH}/windows/lib/${CMAKE_BUILD_TYPE}")
  find_file(HDF_CPP_LIB NAMES hdf5_cpp.lib HINTS "${HDF5_PATH}/windows/lib/${CMAKE_BUILD_TYPE}")
  file(MAKE_DIRECTORY ${OUTPUT_PATH})
  file(COPY ${HDF_DLL} ${HDF_CPP_DLL} ${HDF_LIB} ${HDF_CPP_LIB} DESTINATION ${OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
  find_library(hdf_libs hdf5 hdf5_cpp HINTS "${HDF5_PATH}/windows/lib/${CMAKE_BUILD_TYPE}")
  target_link_libraries(${target_name} "${hdf_libs}")
else()
  find_file(HDF_SO NAMES libhdf5.so HINTS "${HDF5_PATH}/linux/lib")
  find_file(HDF_CPP_SO NAMES libhdf5_cpp.so HINTS "${HDF5_PATH}/linux/lib")
  file(MAKE_DIRECTORY ${OUTPUT_PATH})
  file(COPY ${HDF_SO} ${HDF_CPP_SO} DESTINATION ${OUTPUT_PATH})
  target_link_libraries(${target_name} "${OUTPUT_PATH}/libhdf5_cpp.so") 
  target_link_libraries(${target_name} "${OUTPUT_PATH}/libhdf5.so")
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

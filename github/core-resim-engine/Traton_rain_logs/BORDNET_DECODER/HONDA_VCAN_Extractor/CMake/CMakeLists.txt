##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "HONDA_VCAN_Extractor")
project(${project_name})

set(HONDA_VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(HONDA_VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_PATH "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF")
set(HONDA_VCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_VCAN_RL_Decoder")
set(HONDA_VCAN_RR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_VCAN_RR_Decoder")
set(HONDA_VCAN_FR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_VCAN_FR_Decoder")
set(HONDA_VCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../HONDA_VCAN_FL_Decoder")

set(SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Include/HondaVcanDecoderRL.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Source/HondaVcanDecoderRL.cpp")



list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Include/HondaVcanDecoderRR.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Source/HondaVcanDecoderRR.cpp")
 
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Include/HondaVcanDecoderFR.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Source/HondaVcanDecoderFR.cpp")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Include/HondaVcanDecoderFL.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/Source/HondaVcanDecoderFL.cpp")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Source/HondaVcanRLBase.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Include/HondaVcanBaseRL.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Include/HondaVcanRLBase.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Source/HondaVcanRRBase.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Include/HondaVcanBaseRR.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Include/HondaVcanRRBase.h")
 
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Source/HondeVcanFRBase.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Include/HondaVcanBaseFR.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Include/HondaVcanFRBase.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Source/HondeVcanFLBase.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Include/HondaVcanBaseFL.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Include/HondaVcanFLBase.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Source/HondeVcanFLBase_V12.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Include/HondaVcanBaseFL_V12.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Include/HondaVcanFLBase_V12.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Source/HDF_Honda_FL.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Include/HDF_Honda_Header_FL.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Source/HDF_Honda_FR.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Include/HDF_Honda_Header_FR.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Source/HDF_Honda_RL.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Include/HDF_Honda_Header_RL.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Source/HDF_Honda_RR.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Include/HDF_Honda_Header_RR.h")


list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Source/HondeVcanFRBase_V12.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Include/HondaVcanBaseFR_V12.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Include/HondaVcanFRBase_V12.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Source/HondaVcanRRBase_V12.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Include/HondaVcanBaseRR_V12.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Include/HondaVcanRRBase_V12.h")

list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Source/HondaVcanRLBase_V12.cpp")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Include/HondaVcanBaseRL_V12.h")
list(APPEND SOURCES "${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Include/HondaVcanRLBase_V12.h")

source_group(TREE ${HONDA_VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
#target_link_libraries (${target_name} Mdf_Log)
#target_link_libraries(${target_name} CrossPlatform)

target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_RR/Include)
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FR/Include)
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_PROJECT_ROOT_PATH}/HONDA_VCAN_FL/Include)


target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_RR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_FR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${HONDA_VCAN_FL_INTERFACEFILES})



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
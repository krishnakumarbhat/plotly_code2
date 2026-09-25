##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "AL_PCAN_Extractor")
project(${project_name})

set(AL_PCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(AL_PCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(UTILITY "${CMAKE_CURRENT_LIST_DIR}/../../Utility")
set(AL_PCAN_FL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../AL_PCAN_FL_Decoder")
set(AL_PCAN_RL_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../AL_PCAN_RL_Decoder")
set(AL_PCAN_FLR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../AL_PCAN_FLR_Decoder")
set(AL_AUTOGEN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../Auto_Gen_Files")

#FL
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/Include/ALPcanDecoderFL.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/Source/ALPcanDecoderFL.cpp")
#FL V1_1
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FL/Source/ALPcanHDF_FL_V1_1.cpp")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FL/Include/ALPcanHDF_FL_V1_1.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FL/Source/ALPcanFLBase_V1_1.cpp")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FL/Include/ALPcanBaseFL_V1_1.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FL/Include/ALPcanFLBase_V1_1.h")

#RL
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/Include/ALPcanDecoderRL.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/Source/ALPcanDecoderRL.cpp")
#RL V1_1
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_RL/Source/ALPcanHDF_RL_V1_1.cpp")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_RL/Include/ALPcanHDF_RL_V1_1.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_RL/Source/ALPcanRLBase_V1_1.cpp")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_RL/Include/ALPcanBaseRL_V1_1.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_RL/Include/ALPcanRLBase_V1_1.h")

#FLR
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/Include/ALPcanDecoderFLR.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/Source/ALPcanDecoderFLR.cpp")
#FLR V1_1
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FLR/Source/ALPcanHDF_FLR_V1_1.cpp")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FLR/Include/ALPcanHDF_FLR_V1_1.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FLR/Source/ALPcanFLRBase_V1_1.cpp")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FLR/Include/ALPcanBaseFLR_V1_1.h")
list(APPEND SOURCES "${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FLR/Include/ALPcanFLRBase_V1_1.h")

source_group(TREE ${AL_PCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)
target_link_libraries(${target_name} Utility)


target_include_directories(${target_name} PUBLIC ${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FL/Include)
target_include_directories(${target_name} PUBLIC ${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_RL/Include)
target_include_directories(${target_name} PUBLIC ${AL_PCAN_PROJECT_ROOT_PATH}/AL_PCAN_FLR/Include)


target_include_directories(${target_name} PUBLIC ${AL_PCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${AL_PCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${AL_PCAN_FL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${AL_PCAN_RL_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${AL_PCAN_FLR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${AL_AUTOGEN_INTERFACEFILES/AL_V1_1/Include})


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
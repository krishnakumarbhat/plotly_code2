##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(project_name  "AL_VCAN_Extractor")
project(${project_name})

set(AL_VCAN_PROJECT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(AL_VCAN_COMMON_STRUCTURES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Structures")
set(HDF5_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../../Common_Headers/HDF/lib")
set(UTILITY "${CMAKE_CURRENT_LIST_DIR}/../../Utility")
set(AL_VCAN_FLR_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../AL_VCAN_FLR_Decoder")
set(AL_AUTOGEN_INTERFACEFILES "${CMAKE_CURRENT_LIST_DIR}/../../Auto_Gen_Files")

#FLR
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/Include/ALVCANDecoderFLR.h")
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/Source/ALVcanDecoderFLR.cpp")
#FLR V1_1
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/AL_VCAN_FLR/Source/ALVcanHDF_FLR_V1_1.cpp")
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/AL_VCAN_FLR/Include/ALVCANHDF_FLR_V1_1.h")
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/AL_VCAN_FLR/Source/ALVcanFLRBase_V1_1.cpp")
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/AL_VCAN_FLR/Include/ALVCANBaseFLR_V1_1.h")
list(APPEND SOURCES "${AL_VCAN_PROJECT_ROOT_PATH}/AL_VCAN_FLR/Include/ALVCANFLRBase_V1_1.h")

source_group(TREE ${AL_VCAN_PROJECT_ROOT_PATH} FILES ${SOURCES})

set(target_name ${project_name})
add_library(${target_name} STATIC "")

#link libraries
target_link_libraries (${target_name} Mdf_Log)
target_link_libraries(${target_name} CrossPlatform)
target_link_libraries(${target_name} Utility)

target_include_directories(${target_name} PUBLIC ${AL_VCAN_PROJECT_ROOT_PATH}/AL_VCAN_FLR/Include)


target_include_directories(${target_name} PUBLIC ${AL_VCAN_PROJECT_ROOT_PATH}/Include)

target_include_directories(${target_name} PUBLIC ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${AL_VCAN_COMMON_STRUCTURES})
target_include_directories(${target_name} PUBLIC ${AL_VCAN_FLR_INTERFACEFILES})
target_include_directories(${target_name} PUBLIC ${AL_AUTOGEN_INTERFACEFILES/AL_VCAN_V1_1/Include})


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
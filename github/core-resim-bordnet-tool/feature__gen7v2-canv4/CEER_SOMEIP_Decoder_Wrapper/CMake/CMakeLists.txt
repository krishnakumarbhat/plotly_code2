cmake_minimum_required(VERSION 4.0)

set(project_name "CEER_SOMEIP_Decoder_Wrapper")
project(${project_name})

set(target_name ${project_name})

add_library(${target_name} STATIC "")


set(PRJ_ROOT_PATH "${CEER_SOMEIP_Decoder_Wrapper_PROJ}")
set(SOURCE_PATH "${PRJ_ROOT_PATH}/Source")
set(INCLUDE_PATH "${PRJ_ROOT_PATH}/Include")

set(INCLUDE_INTERFACE_PATH "${PRJ_ROOT_PATH}/../CEER_SRR7P_SOMEIP_Decoder")
#set(INCLUDE_INTERFACE_PATH "${PRJ_ROOT_PATH}/../CEER_IPFFAR_SOMEIP_Decoder")
set(INCLUDE_INTERFACE_PATH "${PRJ_ROOT_PATH}/../CEER_FLR7_SOMEIP_Decoder")
set(INCLUDE_INTERFACE_PATH "${PRJ_ROOT_PATH}/../Utility")
set(COMMON_STRUCTURES_PATH "${PRJ_ROOT_PATH}/../Common_Structures")

list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_Decoder.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_FRL7_SOMEIP_Decoder.cpp")
#list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_Decoder.cpp")
#list(APPEND SOURCE_FILES "${SOURCE_PATH}/Utility.cpp")


list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIPBase.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_V2.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_FRL7_SOMEIPBase.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_FRL7_SOMEIP_V2.cpp")
#list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_IPFFAR_SOMEIPBase.cpp")
#list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_V2.cpp")

list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_HDF_V2.cpp")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIPBase.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_FRL7_SOMEIPBase.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_IPFFAR_SOMEIPBase.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Utility.h")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_Decoder.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_FRL7_SOMEIP_Decoder.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_Decoder.h")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_HDF_V2.h")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_V2.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_FRL7_SOMEIP_V2.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_V2.h")

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)

target_sources(${target_name} PRIVATE ${SOURCE_FILES})
target_include_directories(${target_name} PRIVATE ${INCLUDE_PATH})
target_include_directories(${target_name} PRIVATE ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${INCLUDE_INTERFACE_PATH})
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
target_include_directories(${target_name} PUBLIC ${COMMON_STRUCTURES_PATH})

target_link_libraries(${target_name} CrossPlatform)
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

if(MSVC)
	target_compile_definitions(${target_name} PRIVATE  _WIN_PLATFORM) 
endif(MSVC)
if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)
	
	add_definitions(${CMAKE_CFLAGS} "-fPIC")

	#For Debugging in Linux...
	target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
	
	

	find_package (Threads)
	target_link_libraries (${target_name} ${CMAKE_THREAD_LIBS_INIT})

endif(UNIX)


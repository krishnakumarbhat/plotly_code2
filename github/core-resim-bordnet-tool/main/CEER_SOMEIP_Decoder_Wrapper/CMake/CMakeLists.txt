cmake_minimum_required(VERSION 4.0)

set(project_name "CEER_SOMEIP_Decoder_Wrapper")
project(${project_name})

set(target_name ${project_name})

add_library(${target_name} STATIC "")


set(PRJ_ROOT_PATH "${CEER_SOMEIP_Decoder_Wrapper_PROJ}")
set(SOURCE_PATH "${PRJ_ROOT_PATH}/Source")
set(INCLUDE_PATH "${PRJ_ROOT_PATH}/Include")
set(HDF5_ROOT "${PRJ_ROOT_PATH}/../Common_Headers/HDF")
set(HDF5_LIBRARIES "${HDF5_ROOT}/lib")

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
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIPBase_V2_0.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_V2.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_V2_0.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_FRL7_SOMEIPBase.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_FRL7_SOMEIP_V2.cpp")
#list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_IPFFAR_SOMEIPBase.cpp")
#list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_V2.cpp")

list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_HDF_V2.cpp")
list(APPEND SOURCE_FILES "${SOURCE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_HDF_V2_0.cpp")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIPBase.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIPBase_V2_0.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_FRL7_SOMEIPBase.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_IPFFAR_SOMEIPBase.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Utility.h")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_Decoder.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_FRL7_SOMEIP_Decoder.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_Decoder.h")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_HDF_V2.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_HDF_V2_0.h")

list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_V2.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_SRR7P_SOMEIP_V2_0.h")
list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_FRL7_SOMEIP_V2.h")
#list(APPEND SOURCE_FILES "${INCLUDE_PATH}/Wrapper_CEER_IPFFAR_SOMEIP_V2.h")

#link include directories
target_include_directories(${target_name} PUBLIC ${COMMON_INC})
target_include_directories(${target_name} PRIVATE ${COMMON_INC}/pugixml)
target_include_directories(${target_name} PUBLIC ${COMMON_INC}/HDF/include)

target_sources(${target_name} PRIVATE ${SOURCE_FILES})
target_include_directories(${target_name} PRIVATE ${INCLUDE_PATH})
target_include_directories(${target_name} PRIVATE ${BORDNET_DECODER_PROJ})
target_include_directories(${target_name} PUBLIC ${INCLUDE_INTERFACE_PATH})
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
target_include_directories(${target_name} PUBLIC ${COMMON_STRUCTURES_PATH})

target_link_libraries(${target_name} PRIVATE CrossPlatform)
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
	target_link_libraries (${target_name} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

endif(UNIX)


# Build steps:
# Step1 cmake -G "Visual Studio 14 Win64" ..
# Step2
# Release Mode	:	cmake --build . --target ALL_BUILD --config Release
# Debug Mode		:	cmake --build . --target ALL_BUILD --config Debug

# #########################
# ## Project definition ###
# #########################
cmake_minimum_required(VERSION 3.10)
string(ASCII 27 ESCAPE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_MODE "srr_dc" CACHE STRING "Build mode: srr_dc or mrr_dc")

# project name
project(DC_RESIM)

if(${BUILD_MODE} STREQUAL "mrr_dc")
	set(PROJECTNAME "MRR_DC_SIL_LIB")
	set(DCINFO "[MRR DC INFO]")
elseif(${BUILD_MODE} STREQUAL "srr_dc")
	set(PROJECTNAME "SRR_DC_SIL_LIB")
	set(DCINFO "[SRR DC INFO]")
endif()

set(SG_VARIANT "PLATFORM_UNLIMITED" CACHE STRING "Stationary geometry variant")
set(F360_TRACKER_VARIANT "A" CACHE STRING "F360 Tracker/ROT variant")

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-conversion")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wno-error=pedantic -Wno-error=missing-field-initializers")
endif()

set_target_properties(PROPERTIES LINKER_LANGUAGE CXX)

# Enable position-independent code for Linux shared library builds
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# run python script for Trackerlib update for xtrk
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Execute the Python script
set(PYTHON_SCRIPT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/update.py")
execute_process(
	COMMAND ${Python3_EXECUTABLE} ${PYTHON_SCRIPT_PATH}
	RESULT_VARIABLE SCRIPT_RETURN_CODE
)

if(NOT SCRIPT_RETURN_CODE EQUAL 0)
	message(FATAL_ERROR "Python Script Failed to Update")
else()
	message(STATUS "[XTRK]: TrackerLib Files Updated")
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/pugixml-1.10 pugixml)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/AS_BIN_WRITER/CMake AS-bin-writer-lib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/rspp rspp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/ocg/src ocg)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/sg_stationary_geometry sg_stationary_geometry)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/F360TrackerLib F360TrackerLib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/VSE_Core VSE_Core)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/OLP_Core OLP_Core)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions FeatureFunctions)

# to speed up the incremental build, created a library out of smc parameters autogen files then using it for linking.
add_subdirectory(sil_smc/sil_autogen)
add_subdirectory(sil_udp_streams)

# Set target name
set(target_name ${PROJECTNAME})

# Set OS types
if(MSVC)
	set(OS_FOLDER_TYPE "WINDOWS")
else()
	set(OS_FOLDER_TYPE "LINUX")
	set(CMAKE_CXX_FLAGS "-fPIC")
endif()

# Set build folder type
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	set(BUILD_FOLDER_TYPE "Release")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	set(BUILD_FOLDER_TYPE "Debug")
endif()

# FOR SFL integration
# Define macro Feature_Building_Kit_USE_GENERIC_PA_INTERFACE in ${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/CMakeLists.txt
# and set option Feature_Building_Kit_USE_GENERIC_PA_INTERFACE to ON
# to enable the generic PA interface
# set Feature_Building_Kit_PA_OBJ_NUMBER_OF_OBJECTS as 250

# Provide all the include folders
if(MSVC)
	set(HDF_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/HDF/HDF_Windows/lib")
	set(hdf_PROJ "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/HDF/HDF_Windows")
	set(HDF5_INC "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/HDF/HDF_Windows/include")
	set(DGPS_INC "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/DGPS/Include")
	set(DGPS_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/DGPS/lib")
else()
	set(HDF_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/HDF/HDF_Linux/lib")
	set(hdf_PROJ "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/HDF/HDF_Linux")
	set(HDF5_INC "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/HDF/HDF_Linux/include")
	set(DGPS_INC "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/DGPS/Include")
	set(DGPS_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/DGPS/lib")
endif()

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers)

list(APPEND highfive "${hdf_PROJ}/highfive")
list(APPEND highfive "${hdf_PROJ}/highfive/H5PropertyList.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Reference.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Selection.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Utility.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Version.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Version.hpp.in")
list(APPEND highfive "${hdf_PROJ}/highfive/half_float.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/highfive.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/span.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/xtensor.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/boost.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/eigen.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Attribute.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5DataSet.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5DataSpace.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5DataType.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Easy.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Exception.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5File.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Group.hpp")
list(APPEND highfive "${hdf_PROJ}/highfive/H5Object.hpp")
list(APPEND highfive "${HDF5_INC}/hdf5.h")
list(APPEND highfive "${HDF5_INC}/H5Tpublic.h")

# Provide all the .h files
set(INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/sil_wrapper.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_input/sil_input.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_output/sil_output.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_output/debug_file_write.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/Tracker_VariantA_Wrapper.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/olp_wrapper.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/sfl_wrapper.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_debug_file/HDF_Trace.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_debug_file/Event_Logger.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_config_read/dc_read_config.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/stream_handler.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/stream_header.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/rot_sae_object_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/rot_iso_object_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/rot_processed_detection_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/vse_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/sfl_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/gt_input_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/sg_stream.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_smc/SMCParameters.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_smc/DCEnums.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_smc/DCMacros.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_smc/DCSensorType.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_smc/DCCustomer.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_dgps/DGPS_Decoder.h)
list(APPEND INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/AS_BIN_WRITER/ports/visualC12/as-bin-writer-lib_export.h)

# Provide all the .cpp files
set(SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/sil_wrapper.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_input/sil_input.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_output/sil_output.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_output/debug_file_write.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/Tracker_VariantA_Wrapper.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/olp_wrapper.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_wrappers/sfl_wrapper.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_debug_file/HDF_Trace.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_debug_file/Event_Logger.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_config_read/dc_read_config.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_smc/SMCParameters.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/stream_handler.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/ROT_ISOobject_stream.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/ROT_SAEobject_stream.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/ROT_ProcessedDetection_stream.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/VSE_stream.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/SFL_stream.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/GT_Input_stream.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_dgps/DGPS_Decoder.cpp)
list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5/SG_stream.cpp)

# ###############################
# ## Setting dependencies     ###
# ###############################

# Create shared library, list the source files and headers
add_library(${PROJECTNAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/sil_wrappers
	${CMAKE_CURRENT_LIST_DIR}/sil_input
	${CMAKE_CURRENT_LIST_DIR}/sil_output
	${CMAKE_CURRENT_LIST_DIR}/sil_debug_file
	${CMAKE_CURRENT_LIST_DIR}/sil_udp_streams/RECU
	${CMAKE_CURRENT_LIST_DIR}/sil_dgps
	${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/rspp/include
	${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/VSE_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/core
	${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/Logging
	${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/Types
	${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/OLP_Core/includes
	${CMAKE_CURRENT_LIST_DIR}/sil_engine_headers
	${CMAKE_CURRENT_LIST_DIR}/sil_smc
	${CMAKE_CURRENT_LIST_DIR}/sil_smc/sil_autogen
	${CMAKE_CURRENT_LIST_DIR}/sil_error_codes
	${CMAKE_CURRENT_LIST_DIR}/sil_config_read
	${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/inc
	${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/AS_BIN_WRITER
	${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/AS_BIN_WRITER/ports/visualC12
	${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/pugixml-1.10/src
	${CMAKE_CURRENT_LIST_DIR}/sil_component_streams/A5
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CED/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CED/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CED/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CED/Calibration/CED_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CED/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CTA/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CTA/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CTA/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CTA/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/CTA/Calibration/CTA_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LCDA/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LCDA/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LCDA/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LCDA/Calibration/LCDA_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LCDA/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LTB/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LTB/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LTB/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LTB/Calibration/LTB_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/LTB/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Math_Library/development/dependencies
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Math_Library/ml_core/include
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Math_Library/ml_core/include/legacy
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/PT/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/PT/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/PT/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/PT/Calibration/PT_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/PT/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/RECW/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/RECW/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/RECW/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/RECW/Calibration/RECW_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/RECW/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/SCW/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/SCW/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/SCW/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/SCW/Calibration/SCW_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/SCW/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/TA/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/TA/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/TA/Source
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/TA/Calibration/TA_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/TA/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Calibration/ESA_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Customer_Adapter
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Customer_Adapter/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/ESA/Calibration/Generic
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Calibration_Tool/c_src
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Calibration/Feature_Building_Kit_Core
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface/boundaries
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface/debug_writer
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface/fill_pa_data
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface/host_trail
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface/index_lookup
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Interface/object_ageing
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Platform_Abstraction_Layer
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Platform_Abstraction_Layer/Generic/inc/data_ports
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Platform_Abstraction_Layer/Generic/inc
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/interpolation
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/reference_point
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/trajectory_prediction
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/validation
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/zone
	${CMAKE_CURRENT_LIST_DIR}/../Application/FeatureFunctions/Mock_Files/SIL_Library
	${hdf_PROJ}
	${HDF5_INC}
	${highfive}
	${DGPS_INC}
)

target_link_libraries(${PROJECTNAME}
	PUBLIC
	pugixml
	F360-Tracker
	rspp
	occupancy_grid
	stationary_geometries
	VSE_Core
	autogen_smc
	recu_streams
	OLP_Core
	LCDA
	CTA
	TA
	CED
	RECW
	PT
	LTB
	ESA
	SCW
	AS-bin-writer-lib
	Feature_Building_Kit
)

if(MSVC)
	target_link_libraries(${PROJECTNAME}
		PUBLIC
		${HDF_LIB_DIR}/zlib-static.lib
		${HDF_LIB_DIR}/hdf5.lib
		${HDF_LIB_DIR}/hdf5_cpp.lib
		${HDF_LIB_DIR}/hdf5_hl.lib
		${HDF_LIB_DIR}/hdf5_hl_cpp.lib
		${HDF_LIB_DIR}/hdf5_java.lib
		${HDF_LIB_DIR}/hdf5_tools.lib
		${DGPS_LIB_DIR}/SMValidation.lib
		${DGPS_LIB_DIR}/SensorModel2.lib
	)
else()
	target_link_libraries(${PROJECTNAME}
		PUBLIC
		${HDF_LIB_DIR}/libhdf5.so
		${DGPS_LIB_DIR}/libSMValidation.so
		${DGPS_LIB_DIR}/libSensorModel2.so
	)
endif()

if(UNIX)
	target_link_options(${PROJECTNAME} PRIVATE "-Wl,--exclude-libs,	AS-bin-writer-lib.a")
endif()

target_compile_definitions(${PROJECTNAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

if(${BUILD_MODE} STREQUAL "mrr_dc")
	target_compile_definitions(${PROJECTNAME} PRIVATE MRR_DC)
elseif(${BUILD_MODE} STREQUAL "srr_dc")
	target_compile_definitions(${PROJECTNAME} PRIVATE SRR_DC)
endif()

target_compile_definitions(${PROJECTNAME} PRIVATE DomainController="${PROJECTNAME}")
target_compile_definitions(${PROJECTNAME} PRIVATE STD_DC_STRING_INFO="${DCINFO}")

if(MSVC)
	message("[WARNING]: Below preprocessor directors are used to suppress the compiler warnings")
	message("\t1. WINDOWS_IGNORE_PACKING_MISMATCH")
	message("\t2. _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING")
	message("\t3. _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING")
	target_compile_definitions(${PROJECTNAME} PRIVATE WINDOWS_IGNORE_PACKING_MISMATCH)
	target_compile_definitions(${PROJECTNAME} PRIVATE _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING)
	target_compile_definitions(${PROJECTNAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()

find_package(Python COMPONENTS Interpreter Development REQUIRED)

add_custom_command(TARGET ${PROJECTNAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECTNAME}>
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/sil_executables/fw_dlls" "$<TARGET_FILE_DIR:${PROJECTNAME}>"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${DGPS_LIB_DIR}/libSensorModel2.so"
	"$<TARGET_FILE_DIR:${PROJECTNAME}>/libSensorModel2.so"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/DGPS/lib/OSI_library.obj"
	"$<TARGET_FILE_DIR:${PROJECTNAME}>/OSI_library.obj"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_LIST_DIR}/sil_ext_libs/DGPS/lib/OSI_library.mtl"
	"$<TARGET_FILE_DIR:${PROJECTNAME}>/OSI_library.mtl"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_LIST_DIR}/sil_executables/dgps_config/Bumper_Angular_Error.csv"
	"$<TARGET_FILE_DIR:${PROJECTNAME}>/Bumper_Angular_Error.csv"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${DGPS_LIB_DIR}/SensorModel2.pdb"
	"$<TARGET_FILE_DIR:${PROJECTNAME}>/SensorModel2.pdb"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${DGPS_LIB_DIR}/SMValidation.lib"
	"$<TARGET_FILE_DIR:${PROJECTNAME}>/SMValidation.lib"
	COMMAND ${Python3_EXECUTABLE}
	${CMAKE_CURRENT_LIST_DIR}/version.py
	--tracker "${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/core/f360_tracker_version.h"
	--ocg "${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/ocg/src/iface/ocg_version.h"
	--olp "${CMAKE_CURRENT_LIST_DIR}/../Application/F360Tracker/OLP_Core/includes/olp_iface.h"
	--out $<TARGET_FILE_DIR:${PROJECTNAME}>/versions.txt
)
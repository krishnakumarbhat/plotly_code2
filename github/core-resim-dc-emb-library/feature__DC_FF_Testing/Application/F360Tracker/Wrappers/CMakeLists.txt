cmake_minimum_required(VERSION 3.20)
project(sp_25_tracker_wrapper VERSION 0.1.1)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_FOLDER "SP25 Tracker Wrapper")
add_library(sp_25_tracker_wrapper)

# Set default install folder to cmake build folder
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(SG_MOCK_EXTERNAL_INCLUDES ON CACHE BOOL "Used to enable reuse.h for sg")

# add install target
install(TARGETS sp_25_tracker_wrapper)

if (MSVC)
    add_compile_options(
        "$<$<CONFIG:DEBUG>:/MDd>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/MD>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/Gy>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/Oi>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/GL>" # taken from raw VS
        /sdl- # taken from raw VS
        /W4 # taken from raw VS
        #/WX # taken from raw VS
        /Gm- # taken from raw VS
    )

    add_link_options(
        "$<$<CONFIG:RELEASE>:/LTCG>" # taken from raw VS
    )

    add_compile_definitions(
        _SCL_SECURE_NO_WARNINGS # taken from raw VS
        _CRT_SECURE_NO_WARNINGS # taken from raw VS
        $<$<CONFIG:DEBUG>:_DEBUG>
    )
endif()


add_subdirectory("../F360TrackerLib" "${CMAKE_CURRENT_BINARY_DIR}/F360TrackerLib")
add_subdirectory("../rspp" "${CMAKE_CURRENT_BINARY_DIR}/rspp")
add_subdirectory("../VSE_Core" "${CMAKE_CURRENT_BINARY_DIR}/VSE_Core")
add_subdirectory("../sg_stationary_geometry" "${CMAKE_CURRENT_BINARY_DIR}/sg_stationary_geometry")
add_subdirectory("../olp/OLP_Core" "${CMAKE_CURRENT_BINARY_DIR}/olp")
add_subdirectory("../ocg/src" "${CMAKE_CURRENT_BINARY_DIR}/ocg")

target_link_libraries(
   sp_25_tracker_wrapper
        PRIVATE VSE_Core
        PRIVATE F360-Tracker
        PRIVATE rspp
        PRIVATE UDP_LOGGING_IFACE
        PRIVATE DC_RESIM_IFACE
        PRIVATE ${CUST}_DC_RESIM
		PRIVATE stationary_geometries
		PRIVATE occupancy_grid
		PRIVATE OLP_Core
)
            
target_include_directories(
   sp_25_tracker_wrapper 
   PRIVATE
      common/include
      ${RSPP_IFACE_VARIANT_PATH}
      ${TRACKERLIB_IFACE_VARIANT_PATH}/include
      ${TRACKERLIB_IFACE_VARIANT_PATH}
      ${TRACKERLIB_IFACE_VARIANT_PATH}/Types
      ${TRACKERLIB_IFACE_VARIANT_PATH}/core
      ${TRACKERLIB_IFACE_VARIANT_PATH}/Logging
      common/include/output_adaptation
	  olp/include
	  sfl_wrapper
	  sg/core/inc
	  sg/core/modules/calibrations/iface
	  sg/core/modules/calibrations/inc
	  sg/core/modules/common/iface
	  sg/core/modules/contour_downselection/iface
	  sg/core/modules/contour_initialization/iface
	  sg/core/modules/contour_initialization/inc
	  sg/core/modules/dc_dummy_generator/iface
	  sg/core/modules/detection_clustering/iface
	  sg/core/modules/detection_clustering/inc
	  sg/core/modules/detection_processing/iface
	  sg/core/modules/detection_processing/inc
	  sg/core/modules/drivability_classification/iface
	  sg/core/modules/drivability_classification/inc
	  sg/core/modules/dummy_generator/iface
	  sg/core/modules/host/iface
	  sg/core/modules/math/iface
	  sg/core/modules/math/iface/geometry
	  sg/core/modules/measurement_association/iface
	  sg/core/modules/measurement_association/inc
	  sg/core/modules/measurement_update/iface
	  sg/core/modules/measurement_update/inc
	  sg/core/modules/postprocessing/iface
	  sg/core/modules/postprocessing/inc
	  sg/core/modules/time_update/iface
	  sg/core/modules/time_update/inc
)

if(${TRACKER_TYPE} STREQUAL "SRR")
	target_sources(
	sp_25_tracker_wrapper 
		PRIVATE
			common/include/SRR_Wrapper_F360Tracker.h
			common/source/SRR_Wrapper_F360Tracker.cpp
			)
elseif(${TRACKER_TYPE} STREQUAL "MRR")
         target_sources(
   sp_25_tracker_wrapper 
      PRIVATE
         Common/include/MRR_Wrapper_F360Tracker.h
         Common/source/MRR_Wrapper_F360Tracker.cpp
         )
endif()

target_sources(
   sp_25_tracker_wrapper 
      PRIVATE
         common/include/output_adaptation/f360_tracker_globals.h
         common/include/TrackerIAL.h
         common/include/fixmac.h
         ${TRACKERLIB_IFACE_VARIANT_PATH}/Logging/tracker_f360_internal_logging.h
         common/include/output_adaptation/Tracker_OAL.h
         common/include/AS_bin_writer_F360_tracker_stub.h
         common/source/AS_bin_writer_F360_tracker_stub.cpp
		 sfl_wrapper/ff_sil_wrapper.h
)

target_include_directories(
    occupancy_grid_iface
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sg_stationary_geometry/src/iface
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sg_stationary_geometry/src/iface/types
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sg_stationary_geometry/src/iface/types/contour
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sg_stationary_geometry/src/iface/types/dump)
if(NOT CMAKE_CROSSCOMPILING)
    target_compile_definitions(sp_25_tracker_wrapper PRIVATE -DPC_RESIM)
endif(NOT CMAKE_CROSSCOMPILING)

if(MSVC)
    target_compile_definitions(sp_25_tracker_wrapper PRIVATE _XKEYCHECK_H)
endif(MSVC)

# Set up preprocessor
set(VARIANT_TRACKER "f360_variant_A")
set(RSPP_VARIANT_TRACKER "rspp_variant_A")

add_definitions(-Df360_variant_A=${VARIANT_TRACKER})
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_TRACKER})

# arrange folder/files in solution to reflect the folder structure
# source_group_by_folder(Component_Resim_F360_Tracker F360TrackerLib)
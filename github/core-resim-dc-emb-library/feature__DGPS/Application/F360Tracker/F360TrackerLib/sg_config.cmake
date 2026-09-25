# Configuration for stationary geometries interface
include(../../utilities/cmake/SetupTrackerVariants.cmake)
if(NOT DEFINED RSPP_VARIANT_NS_STR)
   setup_rspp_variant(../sg_stationary_geometry/sg_repository/src/core/modules/rspp/iface RSPP_VARIANT_NS_STR)
endif()

if(NOT TARGET stationary_geometries_iface)  
    set(SG_IFACE_ONLY ON)
    add_subdirectory(../sg_stationary_geometry sg_stationary_geometry)
endif()

message("SG: Used variant -> ${SG_VARIANT}")

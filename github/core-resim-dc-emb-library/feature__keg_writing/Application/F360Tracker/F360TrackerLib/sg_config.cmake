# Configuration for occupancy grid interface
if(NOT TARGET stationary_geometries_iface)  
    set(SG_IFACE_ONLY ON)
    add_subdirectory(../sg_stationary_geometry sg_stationary_geometry)
endif()

message("SG: Used variant -> ${SG_VARIANT}")

# Configuration for occupancy grid interface
if(NOT TARGET occupancy_grid_iface)
   set(OCG_VARIANT_NAME "FLR4px1_v1" CACHE STRING "Project name to determine which variant to include")
   set(OCG_USE_INTERNAL_INCLUDES false)
   set_property(CACHE OCG_VARIANT_NAME PROPERTY STRINGS FLR4px1_v1 FLR4x1_v1 FLR7x1_v1 SRR7px2_v1)
   
   add_subdirectory(../ocg/src/iface ocg_iface)
endif()

target_link_libraries(F360-Tracker PUBLIC occupancy_grid_iface)

message("OCG: Used variant -> ${OCG_VARIANT_NAME}")

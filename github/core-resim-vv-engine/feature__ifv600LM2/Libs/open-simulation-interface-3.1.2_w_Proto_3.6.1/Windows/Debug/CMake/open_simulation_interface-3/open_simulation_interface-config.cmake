# Compute paths
get_filename_component(OPEN_SIMULATION_INTERFACE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(OPEN_SIMULATION_INTERFACE_INCLUDE_DIRS "C:/Work/OSI_3_1_2/osi_3_1_2_binaries/Debug/include/osi3;C:/Work/OSI_3_1_2/install/include")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET OPEN_SIMULATION_INTERFACE AND NOT OPEN_SIMULATION_INTERFACE_BINARY_DIR)
    include("${OPEN_SIMULATION_INTERFACE_CMAKE_DIR}/open_simulation_interface_targets.cmake")
endif()

# These are IMPORTED targets created by open_simulation_interface_targets.cmake
set(OPEN_SIMULATION_INTERFACE_LIBRARIES open_simulation_interface)

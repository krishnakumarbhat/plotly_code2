# Compute paths
get_filename_component(OPEN_SIMULATION_INTERFACE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(OPEN_SIMULATION_INTERFACE_INCLUDE_DIRS "C:/Project/Softwares/OSI/osi_3.2.0/include/osi3;C:/Project/Softwares/Protobuf/windows_build_protobuf_3_6_1/protobuf-3.6.1/src")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET OPEN_SIMULATION_INTERFACE AND NOT OPEN_SIMULATION_INTERFACE_BINARY_DIR)
    include("${OPEN_SIMULATION_INTERFACE_CMAKE_DIR}/open_simulation_interface_targets.cmake")
endif()

# These are IMPORTED targets created by open_simulation_interface_targets.cmake
set(OPEN_SIMULATION_INTERFACE_LIBRARIES open_simulation_interface)

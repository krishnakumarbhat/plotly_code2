# Compute paths
get_filename_component(OPEN_SIMULATION_INTERFACE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(OPEN_SIMULATION_INTERFACE_INCLUDE_DIRS "/home/aptiv/work/Jira_Attachment/DUJ-500/output/open-simulation-interface-3.1.2_w_Proto_3.6.1/Release/include/osi3;/home/aptiv/work/Jira_Attachment/DUJ-500/output/protobuf-3.6.1/Release/include")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET OPEN_SIMULATION_INTERFACE AND NOT OPEN_SIMULATION_INTERFACE_BINARY_DIR)
    include("${OPEN_SIMULATION_INTERFACE_CMAKE_DIR}/open_simulation_interface_targets.cmake")
endif()

# These are IMPORTED targets created by open_simulation_interface_targets.cmake
set(OPEN_SIMULATION_INTERFACE_LIBRARIES open_simulation_interface)

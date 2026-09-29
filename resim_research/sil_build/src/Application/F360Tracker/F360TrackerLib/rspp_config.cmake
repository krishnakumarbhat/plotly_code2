# Configuration for RSPP interface
add_subdirectory(../rspp/include rspp_iface)

# Set up preprocessor
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_NS_STR})

# Add interface directories
target_include_directories(F360-Tracker PUBLIC ../rspp/include)

add_library(rspp_iface INTERFACE)

target_include_directories(
    rspp_iface
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../rspp/include
)

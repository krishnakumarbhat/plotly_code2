# Configuration for RSPP interface
add_subdirectory(../rspp/include rspp_iface)

# Set up preprocessor
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_NS_STR})

# Add interface directories
target_include_directories(F360-Tracker PUBLIC ../rspp/include)

##########################################################################
# Generic setup
##########################################################################
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/install_dir")
endif()

##########################################################################
# Interface files
##########################################################################
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.22.0") 
    get_target_property(iface_binary_dir stationary_geometries_iface BINARY_DIR)
    file(GLOB_RECURSE iface_generated_files CONFIGURE_DEPENDS "${iface_binary_dir}/*.h")

    get_target_property(iface_source_dir stationary_geometries_iface SOURCE_DIR)
    file(GLOB_RECURSE iface_files CONFIGURE_DEPENDS "${iface_source_dir}/*.h")

    get_target_property(rspp_iface_sources SG-Rspp-iface SOURCES)

    install(
        FILES ${iface_files} ${iface_generated_files}
        DESTINATION "sg/iface"
    )
    install(
        FILES ${rspp_iface_sources}
        DESTINATION "sg/rspp_iface"
    )
else()
    get_target_property(iface_dirs stationary_geometries_iface INTERFACE_INCLUDE_DIRECTORIES)

    set(iface_files)

    foreach(dir IN LISTS iface_dirs)
        string(REPLACE "$<BUILD_INTERFACE:" "" dir "${dir}")
        string(REPLACE ">" "" dir "${dir}")
        file(GLOB_RECURSE header_files "${dir}/*.h")
        foreach(header_file IN LISTS header_files)
            list(APPEND iface_files ${header_file})
        endforeach()
    endforeach()

    list(REMOVE_DUPLICATES iface_files)
    
    install(
        FILES ${iface_files}
        DESTINATION "sg/iface"
    )

    get_target_property(rspp_iface_dir SG-Rspp-iface INTERFACE_INCLUDE_DIRECTORIES)
    string(REPLACE "$<BUILD_INTERFACE:" "" rspp_iface_dir "${rspp_iface_dir}")
    string(REPLACE ">" "" rspp_iface_dir "${rspp_iface_dir}")
    file(GLOB rspp_header_files "${rspp_iface_dir}/*.h")
    install(
        FILES ${rspp_header_files}
        DESTINATION "sg/rspp_iface"
    )
endif()



##########################################################################
# Library file
##########################################################################
if(NOT SG_IFACE_ONLY)
    install(
        TARGETS stationary_geometries
        DESTINATION "sg/lib"
    )
endif()

##########################################################################
# Documentation
##########################################################################
install(
    FILES "docs/integration_guide.md"
    DESTINATION "sg/doc"
)
install(
    FILES "tests/sg_library_integration/integration_module/main.cpp"
    DESTINATION "sg/doc"
    RENAME integration_example.cpp
)

##########################################################################
# CMake file for installed library
##########################################################################
configure_file("tools/cmake/install_CMakeLists.txt.in" "${CMAKE_CURRENT_BINARY_DIR}/install_CMakeLists.txt" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/install_CMakeLists.txt"
    DESTINATION "sg"
    RENAME CMakeLists.txt
)
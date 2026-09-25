get_target_property(iface_install_files stationary_geometries_iface SOURCES)
get_target_property(iface_source_dir stationary_geometries_iface SOURCE_DIR)
list(TRANSFORM iface_install_files PREPEND "${iface_source_dir}/")

install(FILES ${iface_install_files} DESTINATION sg/iface)
install(TARGETS stationary_geometries DESTINATION sg/lib)
install(FILES docs/integration_guide.md DESTINATION sg/doc)
install(FILES tools/cmake/release_CMakeLists.txt DESTINATION sg RENAME CMakeLists.txt)
install(FILES tests/sg_library_integration/integration_module/main.cpp DESTINATION sg/doc RENAME integration_example.cpp)
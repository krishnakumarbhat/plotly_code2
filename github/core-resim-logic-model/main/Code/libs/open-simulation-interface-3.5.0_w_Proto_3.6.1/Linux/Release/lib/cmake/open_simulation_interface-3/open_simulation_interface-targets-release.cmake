#----------------------------------------------------------------
# Generated CMake target import file for configuration "RELEASE".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "open_simulation_interface::open_simulation_interface_static" for configuration "RELEASE"
set_property(TARGET open_simulation_interface::open_simulation_interface_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(open_simulation_interface::open_simulation_interface_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/osi3/libopen_simulation_interface_static.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS open_simulation_interface::open_simulation_interface_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_open_simulation_interface::open_simulation_interface_static "${_IMPORT_PREFIX}/lib/osi3/libopen_simulation_interface_static.a" )

# Import target "open_simulation_interface::open_simulation_interface_pic" for configuration "RELEASE"
set_property(TARGET open_simulation_interface::open_simulation_interface_pic APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(open_simulation_interface::open_simulation_interface_pic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/osi3/libopen_simulation_interface_pic.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS open_simulation_interface::open_simulation_interface_pic )
list(APPEND _IMPORT_CHECK_FILES_FOR_open_simulation_interface::open_simulation_interface_pic "${_IMPORT_PREFIX}/lib/osi3/libopen_simulation_interface_pic.a" )

# Import target "open_simulation_interface::open_simulation_interface" for configuration "RELEASE"
set_property(TARGET open_simulation_interface::open_simulation_interface APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(open_simulation_interface::open_simulation_interface PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/osi3/libopen_simulation_interface.so.3.5.0"
  IMPORTED_SONAME_RELEASE "libopen_simulation_interface.so.3.5.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS open_simulation_interface::open_simulation_interface )
list(APPEND _IMPORT_CHECK_FILES_FOR_open_simulation_interface::open_simulation_interface "${_IMPORT_PREFIX}/lib/osi3/libopen_simulation_interface.so.3.5.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

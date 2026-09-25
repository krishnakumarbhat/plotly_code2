#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "open_simulation_interface_static" for configuration "Debug"
set_property(TARGET open_simulation_interface_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(open_simulation_interface_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface_static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS open_simulation_interface_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_open_simulation_interface_static "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface_static.lib" )

# Import target "open_simulation_interface_pic" for configuration "Debug"
set_property(TARGET open_simulation_interface_pic APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(open_simulation_interface_pic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface_pic.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS open_simulation_interface_pic )
list(APPEND _IMPORT_CHECK_FILES_FOR_open_simulation_interface_pic "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface_pic.lib" )

# Import target "open_simulation_interface" for configuration "Debug"
set_property(TARGET open_simulation_interface APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(open_simulation_interface PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS open_simulation_interface )
list(APPEND _IMPORT_CHECK_FILES_FOR_open_simulation_interface "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface.lib" "${_IMPORT_PREFIX}/lib/osi3/open_simulation_interface.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was open_simulation_interface-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)
find_dependency(Protobuf)

if(NOT TARGET open_simulation_interface AND NOT open_simulation_interface_BINARY_DIR)
  set_and_check(OPEN_SIMULATION_INTERFACE_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include/osi3")
  set(OPEN_SIMULATION_INTERFACE_LIBRARIES "open_simulation_interface")
  include("${CMAKE_CURRENT_LIST_DIR}/open_simulation_interface_targets.cmake")
endif()

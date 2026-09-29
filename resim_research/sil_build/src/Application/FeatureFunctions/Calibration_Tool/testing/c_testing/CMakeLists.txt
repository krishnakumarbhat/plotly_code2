# ##############################################################################
# REQUIRED BLOCK minimum version, project, target NOTE CT_CALTOOL_TESTING must
# be a unique, constant Module/Library name
# ##############################################################################
cmake_minimum_required(VERSION 3.14)

project(CT_CALTOOL_TESTING)

set(CT_CALTOOL_GTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
option(MODULE_UNIT_TESTS "Include unit tests into build" OFF)

# define directories
if(NOT CT_CALTOOL_TESTING_PATH)
  set(CT_CALTOOL_TESTING_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif(NOT CT_CALTOOL_TESTING_PATH)

# PLEASE NOTE: This block needs to stay above the add_subdirectory(...) calls to
# have any effect on the targets. Furthermore targets can extend/overwrite those
# setting here using target_compile_options(...).
if(MSVC)
  # For MSVC we override the default compiler settings We use /Wall for C code,
  # but disable the following warnings: warning 4668 about undefined
  # preprocessor macros, warning 4820 about byte padding
  set(CMAKE_C_FLAGS "/Wall /wd4668 /wd4820")
  # We use /W4 for C++ code
  set(CMAKE_CXX_FLAGS "/W4 /EHsc")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_C_FLAGS
      "-Wall -Wextra -pedantic -Wshadow -Wconversion ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wshadow ${CMAKE_CXX_FLAGS}")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_C_FLAGS
      "-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wdeclaration-after-statement ${CMAKE_C_FLAGS}"
  )
  set(CMAKE_CXX_FLAGS
      "-Wall -Wextra -Wpedantic -Wshadow -Wextra-semi ${CMAKE_CXX_FLAGS}")
endif()

# Add CT_CALTOOL module for testing
add_subdirectory(${CT_CALTOOL_TESTING_PATH}/../c_src c_src)

# Include files for testing suite
target_include_directories(
  CT_CALTOOL PRIVATE ${CT_CALTOOL_TESTING_PATH}/example_files/Core)
target_sources( CT_CALTOOL PRIVATE
    ${CT_CALTOOL_TESTING_PATH}/example_files/Core/cf_core_calibration_t.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Core/cf_core_calibration.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Core/cf_core_calibration_print_functions.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Core/cf_core_calibration_check.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Core/cf_core_calibration_check.c
    )
target_include_directories(
  CT_CALTOOL PRIVATE ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A)
target_sources( CT_CALTOOL PRIVATE
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_core_calibration.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_customer_calibration.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_customer_calibration.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_customer_calibration_check.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_customer_calibration_check.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_customer_calibration_print_functions.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_customer_calibration_t.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_public_calibration.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_public_calibration.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_public_calibration_check.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_public_calibration_check.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_public_calibration_print_functions.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_public_calibration_t.h
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_update_calibration.c
    ${CT_CALTOOL_TESTING_PATH}/example_files/Customer_A/cf_update_calibration.h
    )
    
target_include_directories(CT_CALTOOL
                           PUBLIC ${CT_CALTOOL_TESTING_PATH}/Mock_Files)
target_sources(CT_CALTOOL PRIVATE ${CT_CALTOOL_TESTING_PATH}/Mock_Files/reuse.h)

if(${MODULE_UNIT_TESTS})
  add_executable(CT_CALTOOL_TESTING ${CMAKE_CURRENT_LIST_FILE})

  # Set start-up project for VS Studio
  if(MSVC)
    set_property(DIRECTORY ${CT_CALTOOL_ROOT_PATH} PROPERTY VS_STARTUP_PROJECT
                                                            CT_CALTOOL_TESTING)
  endif()

  # Include googletest
  if(CT_CALTOOL_GTEST_PATH AND (EXISTS ${CT_CALTOOL_GTEST_PATH}))
    if(MSVC)
      if(${MSVC_TOOLSET_VERSION} LESS 140)
        message(
          FATAL_ERROR
            "Visual Studio Version too old for Unit-Test build! \nUse Visual Studio 14 2015 or later."
        )
      endif()
    endif()
    # gtest_force_shared_crt (see
    # https://github.com/google/googletest/blob/master/googletest/README.md) By
    # default, new Visual Studio projects link the C runtimes dynamically but
    # Google Test links them statically. This will generate an error that looks
    # something like the following: gtest.lib(gtest-all.obj) : error LNK2038:
    # mismatch detected for 'RuntimeLibrary': value 'MTd_StaticDebug' doesn't
    # match value 'MDd_DynamicDebug' in main.obj
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "Always use msvcrt.dll" FORCE)
    set(CMAKE_FOLDER "googletest")
    add_subdirectory("${CT_CALTOOL_GTEST_PATH}"
                     "${CMAKE_CURRENT_BINARY_DIR}/googletest")

    target_link_libraries(CT_CALTOOL_TESTING CT_CALTOOL gtest)
  endif()

  target_sources(
    CT_CALTOOL_TESTING
    PRIVATE ${CT_CALTOOL_TESTING_PATH}/c_testing/ct_reverse_array_test.cpp)
  target_sources(
    CT_CALTOOL_TESTING
    PRIVATE ${CT_CALTOOL_TESTING_PATH}/c_testing/ct_reverse_array_test.hpp)
  target_sources(
    CT_CALTOOL_TESTING
    PRIVATE ${CT_CALTOOL_TESTING_PATH}/c_testing/c_testing_main.cpp)
  # Install target
  add_definitions(-D_VARIADIC_MAX=10)
  install(TARGETS CT_CALTOOL_TESTING)

else()

  add_library(CT_CALTOOL_TESTING ${CMAKE_CURRENT_LIST_FILE})
  set_target_properties(CT_CALTOOL_TESTING PROPERTIES LINKER_LANGUAGE CXX)
endif()

message(STATUS "##############################################")
message(STATUS "######## CMake Configuration Summary #########")
message(STATUS "##############################################")
message(STATUS "Compiler in use:        ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler Flags (C):     ${CMAKE_C_FLAGS}")
message(STATUS "Compiler Flags (C++):   ${CMAKE_CXX_FLAGS}")
if(NOT MSVC)
  message(STATUS "Installation directory: ${CMAKE_INSTALL_PREFIX}")
endif()
message(STATUS "##############################################")

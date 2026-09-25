cmake_minimum_required(VERSION 3.16)

project(Occupancy_Grid_GTest)

# ==================================
# Gtest setup
# ==================================
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

# Prepare setup for code coverage build
set(CMAKE_C_FLAGS_COVERAGE --coverage)
set(CMAKE_CXX_FLAGS_COVERAGE --coverage)

set(CMAKE_FOLDER ${CMAKE_FOLDER}/GTest)
# We use the CI_ADDITIONAL_SETUP variable for all additional setup specifically required for Jenkins.
# the following block skip cloning GoogleTest from github in case "CI_ADDITIONAL_SETUP" is "ON".
# Assuming that you already have a googletest localy with its path populated in GTEST_PATH environment variable.
if(NOT CI_ADDITIONAL_SETUP)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
else()
  add_subdirectory($ENV{GTEST_PATH} googletest)
  include(GoogleTest)
endif()
set(CMAKE_FOLDER "")

# ==================================
# Enable external type definitions
# ==================================
set(OCG_USE_INTERNAL_INCLUDES ON)
set(OCG_RSPP_VARIANT_NAME "rspp_variant_A")

# ==================================
# OCG Core
# ==================================
# set(CMAKE_FOLDER "Occupancy grid/core")
# add_subdirectory("./../../src" ocg_core)

# ==================================
# Tests setup
# ==================================
set(CMAKE_FOLDER "Tests")
add_subdirectory(tests)
set(CMAKE_FOLDER "")

# ==================================
# Other
# ==================================
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

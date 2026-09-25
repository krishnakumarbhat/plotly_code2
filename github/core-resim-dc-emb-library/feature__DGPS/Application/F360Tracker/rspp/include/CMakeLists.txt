cmake_minimum_required(VERSION 3.16)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities/cmake/SetupTrackerVariants.cmake)

setup_rspp_variant(${CMAKE_CURRENT_SOURCE_DIR} RSPP_VARIANT_NS_STR)
set(RSPP_VARIANT_NS_STR "${RSPP_VARIANT_NS_STR}" PARENT_SCOPE)


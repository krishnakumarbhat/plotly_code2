cmake_minimum_required(VERSION 3.16)

add_library(wrapper_gtest INTERFACE)

target_include_directories(
   wrapper_gtest
      INTERFACE
         ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
   wrapper_gtest
      INTERFACE
         sg_check_size.h
         wrapper_gtest.h
)

target_link_libraries(
    wrapper_gtest
        INTERFACE GTest::gtest_main
        INTERFACE GTest::gmock
)
cmake_minimum_required(VERSION 3.16)

add_executable(embedded_list_ut)

target_sources(
   embedded_list_ut
      PRIVATE
         ut_embedded_list.cpp
         ut_embedded_list_swap.cpp
         ut_embedded_list_sort.cpp
         ut_embedded_list_iterator.cpp
         ut_embedded_list_shared_memory.cpp
         ut_embedded_list_memory_leak.cpp
         ut_embedded_list_operator.cpp
         ut_embedded_list_constructors.cpp
         ut_embedded_list_reassignment.cpp
         ut_embedded_list_algorithm.cpp
         ut_embedded_list_move.cpp
         ut_embedded_list_helpers.h
         ut_embedded_list_const_iterator.cpp
)

target_link_libraries(
   embedded_list_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
)

include(GoogleTest)
gtest_discover_tests(embedded_list_ut)
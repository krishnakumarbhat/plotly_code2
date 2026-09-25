cmake_minimum_required(VERSION 3.16)

add_library(timer_resim)

set_target_properties(timer_resim PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(
   timer_resim
      PUBLIC iface
      PRIVATE src
)

target_sources(
   timer_resim
      PRIVATE
         iface/timer_resim.h
         src/timer_resim.cpp
)

target_link_libraries(
   timer_resim
      PUBLIC stationary_geometries_iface
)

get_target_property(src_files timer_resim SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})

get_target_property(timer_resim_iface_install_files timer_resim SOURCES)
install(FILES ${timer_resim_iface_install_files} DESTINATION timer_resim/iface)

install(
    TARGETS
        timer_resim
    DESTINATION
        timer_resim/lib
)
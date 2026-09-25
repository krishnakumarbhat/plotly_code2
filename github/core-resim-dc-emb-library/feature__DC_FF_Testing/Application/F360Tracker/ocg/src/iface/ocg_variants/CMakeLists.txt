cmake_minimum_required(VERSION 3.16)

add_library(ocg_constants INTERFACE)

if(${OCG_VARIANT_NAME} STREQUAL "FLR4px1_v1")
   target_include_directories(ocg_constants INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FLR4px1_v1/)

   target_sources(
      ocg_constants
         INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/FLR4px1_v1/ocg_constants.h
   )

elseif(${OCG_VARIANT_NAME} STREQUAL "FLR4x1_v1")
   target_include_directories(ocg_constants INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FLR4x1_v1/)

   target_sources(
   ocg_constants
      INTERFACE
         ${CMAKE_CURRENT_LIST_DIR}/FLR4x1_v1/ocg_constants.h
   )
   
 elseif(${OCG_VARIANT_NAME} STREQUAL "FLR4p_plt_x1_v1")
   target_include_directories(ocg_constants INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FLR4p_plt_x1_v1/)

   target_sources(
   ocg_constants
      INTERFACE
         ${CMAKE_CURRENT_LIST_DIR}/FLR4p_plt_x1_v1/ocg_constants.h
   )

elseif(${OCG_VARIANT_NAME} STREQUAL "FLR7x1_v1")
   target_include_directories(ocg_constants INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FLR7x1_v1/)

   target_sources(
   ocg_constants
      INTERFACE
         ${CMAKE_CURRENT_LIST_DIR}/FLR7x1_v1/ocg_constants.h
   )
elseif(${OCG_VARIANT_NAME} STREQUAL "SRR7px2_v1")
   target_include_directories(ocg_constants INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/SRR7px2_v1/)

   target_sources(
   ocg_constants
      INTERFACE
         ${CMAKE_CURRENT_LIST_DIR}/SRR7px2_v1/ocg_constants.h
   )
endif()


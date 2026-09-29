target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_input_t.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_output_t.h)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_pre_run.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_post_run.c)

target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_rna_sweet400_debug_interface.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_rna_sweet400_debug_interface.c)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_rna_sweet400_debug_writer.h)
target_sources(LCDA PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lcda_rna_sweet400_debug_writer.c)

if(NOT TARGET GDSRTracker)
   include(${LCDA_ROOT_PATH}/Mock_Files/RNA_SWEET400/Mock_Files.cmake)
endif()

# This variable specifies the project specific feature folder (used for automatic make file generation)
set(LCDA_make_alias "RR_Z2/RR_Z2_CUSTOMER/Feature_Functions/FF_LCDA")

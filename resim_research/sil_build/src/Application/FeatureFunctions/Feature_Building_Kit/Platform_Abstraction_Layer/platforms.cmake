# REQUIRED BLOCK minimum version, project, target NOTE please use VERSION 3.14 or above
cmake_minimum_required(VERSION 3.14)

include(${CMAKE_CURRENT_LIST_DIR}/All_Valid_PA_Options.cmake)

if(Feature_Building_Kit_USE_GENERIC_PA_INTERFACE)
   include(${CMAKE_CURRENT_LIST_DIR}/Generic/Generic.cmake)
   target_compile_definitions(Feature_Building_Kit PRIVATE "PA_Generic")
else()
   if("${Feature_Building_Kit_PROJECT_VARIANT}" IN_LIST PA_GDSR_Consumer)
      include(${CMAKE_CURRENT_LIST_DIR}/gdsr/gdsr.cmake)
      target_compile_definitions(Feature_Building_Kit PRIVATE "PA_GDSR")
   elseif("${Feature_Building_Kit_PROJECT_VARIANT}" IN_LIST PA_F360_Consumer)
      include(${CMAKE_CURRENT_LIST_DIR}/f360/f360.cmake)
      target_compile_definitions(Feature_Building_Kit PRIVATE "PA_F360")
   elseif("${Feature_Building_Kit_PROJECT_VARIANT}" IN_LIST PA_U360_Consumer)
      include(${CMAKE_CURRENT_LIST_DIR}/u360/u360.cmake)
      target_compile_definitions(Feature_Building_Kit PRIVATE "PA_U360")
   else()
      message(FATAL_ERROR "Platform abstraction is not set up for ${Feature_Building_Kit_PROJECT_VARIANT}")
   endif()
endif()

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# definition for getter function of context selection. This enables features to provide a single interface independent
# of the tracker variant
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_selector.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_selector.c)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/pa_data.h)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/pa_shared_types.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pa_mock_functions.h)

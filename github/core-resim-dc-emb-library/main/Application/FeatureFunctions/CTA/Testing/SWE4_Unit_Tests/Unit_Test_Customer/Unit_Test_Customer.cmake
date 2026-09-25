target_sources(CTA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CTA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(CTA PUBLIC "-DCTA_UNIT_TEST")

# In case of customer build the cmake file shall be loaded. If its a generic build, then the generic pre and postrun
# does not need to be tested, since only the Core is important then.
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${CTA_PROJECT_VARIANT})
   include(${CMAKE_CURRENT_LIST_DIR}/${CTA_PROJECT_VARIANT}/UT_${CTA_PROJECT_VARIANT}.cmake)
else()
   if(NOT ${CTA_PROJECT_VARIANT} MATCHES Generic)
      message(FATAL_ERROR "Unit Test Build for the specified customer is not set up.")
   endif()
endif()

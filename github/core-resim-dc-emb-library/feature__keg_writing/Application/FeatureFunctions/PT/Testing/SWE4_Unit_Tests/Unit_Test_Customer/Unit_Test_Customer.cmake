target_sources(PT_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# In case of customer build the cmake file shall be loaded. If its a generic build, then the generic pre and postrun
# does not need to be tested, since only the Core is important then.
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${PT_PROJECT_VARIANT})
   include(${CMAKE_CURRENT_LIST_DIR}/${PT_PROJECT_VARIANT}/UT_${PT_PROJECT_VARIANT}.cmake)
else()
   if(NOT ${PT_PROJECT_VARIANT} MATCHES Generic)
      message(
         FATAL_ERROR
            "Unit Test Build for the specified customer is currently not set up. For Path Tracking no real customer adapter exists. So use the generic adapter"
      )
   endif()
endif()

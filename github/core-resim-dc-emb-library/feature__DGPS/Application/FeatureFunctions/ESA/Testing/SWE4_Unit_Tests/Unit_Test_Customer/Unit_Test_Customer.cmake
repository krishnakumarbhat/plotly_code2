target_sources(ESA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(ESA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${ESA_PROJECT_VARIANT})
   include(${CMAKE_CURRENT_LIST_DIR}/${ESA_PROJECT_VARIANT}/UT_${ESA_PROJECT_VARIANT}.cmake)
else()
   message(FATAL_ERROR "Unit Test Build for the specified customer is not set up.")
endif()

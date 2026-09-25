target_sources(CTA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CTA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(CTA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(CTA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/cta_post_run_test.cpp)
target_sources(CTA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/cta_pre_run_test.cpp)

target_include_directories(CTA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(CTA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/cta_post_run_test.hpp)
target_sources(CTA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/cta_pre_run_test.hpp)

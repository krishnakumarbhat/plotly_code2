target_sources(TA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(TA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(TA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(TA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ta_post_run_test.cpp)
target_sources(TA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ta_pre_run_test.cpp)

target_include_directories(TA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(TA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ta_post_run_test.hpp)
target_sources(TA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ta_pre_run_test.hpp)

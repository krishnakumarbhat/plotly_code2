target_sources(RECW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(RECW_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(RECW_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(RECW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/recw_post_run_test.cpp)
target_sources(RECW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/recw_pre_run_test.cpp)

target_include_directories(RECW_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(RECW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/recw_post_run_test.hpp)
target_sources(RECW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/recw_pre_run_test.hpp)

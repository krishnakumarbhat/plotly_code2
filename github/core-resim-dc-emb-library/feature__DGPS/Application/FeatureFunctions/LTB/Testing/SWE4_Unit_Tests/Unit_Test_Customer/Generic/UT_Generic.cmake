target_sources(LTB_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LTB_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(LTB_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(LTB_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ltb_post_run_test.cpp)

target_include_directories(LTB_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(LTB_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ltb_post_run_test.hpp)

target_sources(LTB_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ltb_pre_run_test.cpp)
target_sources(LTB_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ltb_pre_run_test.hpp)

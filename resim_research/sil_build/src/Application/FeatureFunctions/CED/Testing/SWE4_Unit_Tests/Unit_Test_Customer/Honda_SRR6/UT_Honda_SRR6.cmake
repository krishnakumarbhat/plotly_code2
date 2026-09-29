target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CED_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(CED_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_post_run_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_pre_run_test.cpp)

target_include_directories(CED_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_post_run_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_pre_run_test.hpp)

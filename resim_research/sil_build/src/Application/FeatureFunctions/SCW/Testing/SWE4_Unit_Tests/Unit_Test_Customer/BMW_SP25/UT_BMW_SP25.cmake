target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(SCW_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(SCW_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/scw_post_run_test.cpp)
target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/scw_pre_run_test.cpp)
target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/scw_state_machine_test.cpp)

target_include_directories(SCW_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/scw_post_run_test.hpp)
target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/scw_pre_run_test.hpp)
target_sources(SCW_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/scw_state_machine_test.hpp)

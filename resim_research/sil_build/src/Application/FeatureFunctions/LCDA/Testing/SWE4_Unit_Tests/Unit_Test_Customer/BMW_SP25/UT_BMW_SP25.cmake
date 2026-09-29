target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(LCDA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(LCDA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/lane_model_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/lane_model_camera_data_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/lcda_post_run_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/lcda_pre_run_test.cpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/lcda_state_machine_test.cpp)

target_include_directories(LCDA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/lane_model_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/lane_model_camera_data_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/lcda_post_run_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/lcda_pre_run_test.hpp)
target_sources(LCDA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/lcda_state_machine_test.hpp)

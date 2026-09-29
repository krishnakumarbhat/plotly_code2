target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(CED_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(CED_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_pre_run_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_post_run_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_state_machine_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_post_run_boardnet_mirror_led_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_post_run_boardnet_seat_doors_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_post_run_boardnet_interior_light_test.cpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/ced_post_run_boardnet_optical_elements_test.cpp)

target_include_directories(CED_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_pre_run_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_post_run_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_state_machine_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_post_run_boardnet_mirror_led_test.hpp)
target_sources(CED_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_post_run_boardnet_seat_doors_test.hpp)
target_sources(CED_Unit_Test
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_post_run_boardnet_interior_light_test.hpp)
target_sources(CED_Unit_Test
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/ced_post_run_boardnet_optical_elements_test.hpp)

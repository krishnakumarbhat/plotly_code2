target_sources(ESA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(ESA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(ESA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(ESA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/esa_post_run_test.cpp)
target_sources(ESA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/esa_pre_run_test.cpp)

target_include_directories(ESA_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(ESA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/esa_post_run_test.hpp)
target_sources(ESA_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/esa_pre_run_test.hpp)

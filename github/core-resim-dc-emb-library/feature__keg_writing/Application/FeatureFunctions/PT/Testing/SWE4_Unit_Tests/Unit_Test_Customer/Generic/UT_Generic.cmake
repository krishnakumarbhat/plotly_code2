target_sources(PT_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)
target_sources(PT_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Source/pt_constants_selection_test.cpp)

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)
target_sources(PT_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Test_Classes/pt_constants_selection_test.hpp)

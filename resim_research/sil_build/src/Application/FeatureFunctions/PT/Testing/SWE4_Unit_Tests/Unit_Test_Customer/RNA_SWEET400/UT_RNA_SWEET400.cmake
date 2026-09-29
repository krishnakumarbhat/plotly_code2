target_sources(PT_Unit_Test PRIVATE ${CMAKE_CURRENT_LIST_FILE})

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Source)

target_include_directories(PT_Unit_Test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Test_Classes)

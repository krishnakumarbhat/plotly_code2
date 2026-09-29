#add_subdirectory(unit_test unit_test)

add_subdirectory(static_analysis static_analysis)

INCLUDE(math_library_generate_mak)
math_library_generate_mak()

# Configure gcov-based coverage for the safety_handler component

# Apply coverage-friendly compiler and linker flags

target_compile_options(safety_handler PUBLIC -ftest-coverage -fprofile-arcs -fno-exceptions -fno-inline)
add_definitions(-DNDEBUG)
set_target_properties(safety_handler PROPERTIES LINK_FLAGS "-lgcov --coverage")

set_target_properties(safety_handler-UT PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(safety_handler-QT PROPERTIES LINK_FLAGS "-lgcov --coverage")

target_compile_options(safety_handler-UT PRIVATE -ftest-coverage -fprofile-arcs -fno-inline)
target_compile_options(safety_handler-QT PRIVATE -ftest-coverage -fprofile-arcs -fno-inline)

set(gcovr_exclude_pattern "(.*/unittest)|(.*/testing/unit_testing/cpputest.*)|(.*/qualification_testing.*)")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage)
find_program(GCOVR_BIN gcovr)
get_property(SH_OBJ_DIR TARGET safety_handler PROPERTY BINARY_DIR)

add_custom_target(safety_handler-coverage
   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}/.. --object-directory ${SH_OBJ_DIR}
   --html --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_report_safety_handler.html
   --exclude="${gcovr_exclude_pattern}" --delete
   --gcov-ignore-parse-errors="suspicious_hits.warn_once_per_file"

   DEPENDS safety_handler-coverage_RunTests
)

add_custom_target(safety_handler-coverage_summary
   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}/.. --object-directory ${SH_OBJ_DIR}
   --html -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_safety_handler.html
   --exclude="${gcovr_exclude_pattern}"
   --gcov-ignore-parse-errors="suspicious_hits.warn_once_per_file"

   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}/.. --object-directory ${SH_OBJ_DIR}
   --xml -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_safety_handler.xml
   --exclude="${gcovr_exclude_pattern}"
   --gcov-ignore-parse-errors="suspicious_hits.warn_once_per_file"

   DEPENDS safety_handler-coverage_RunTests
)

add_custom_target(safety_handler-coverage_RunTests
   COMMAND
   $<TARGET_FILE:safety_handler-UT>
)

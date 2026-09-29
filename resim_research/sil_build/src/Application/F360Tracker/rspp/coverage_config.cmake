
# Compile flags and definitions need to be global in order to affect implementations in header files
target_compile_options(rspp PUBLIC -ftest-coverage -fprofile-arcs -fno-exceptions -fno-inline)
# Add NDEBUG to get rid of assert() branches. Should find a different way of doing this later.
add_definitions(-DNDEBUG)

set_target_properties(rspp PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(rspp-UT PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(rspp-QT PROPERTIES LINK_FLAGS "-lgcov --coverage")

set(gcovr_exclude_pattern "(.*/unittest)|(.*/testing/unit_testing/cpputest.*)|(.*/qualification_testing.*)")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage)
set(GCOVR_BIN ${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/coverage/gcovr)
get_property(OBJ_DIR TARGET rspp PROPERTY BINARY_DIR)

# Create coverage target
add_custom_target(rspp-coverage COMMAND
   python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_report_rspp.html
   --exclude="${gcovr_exclude_pattern}" --delete

   DEPENDS rspp-coverage_RunTests
)

# Create coverage target
add_custom_target(rspp-coverage_summary
   COMMAND python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_rspp.html
   --exclude="${gcovr_exclude_pattern}"

   COMMAND python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --xml -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_rspp.xml
   --exclude="${gcovr_exclude_pattern}"

   DEPENDS rspp-coverage_RunTests
)

# Make sure the UT executable runs before collecting coverage
add_custom_target(rspp-coverage_RunTests
   COMMAND
   $<TARGET_FILE:rspp-UT> &&
   $<TARGET_FILE:rspp-QT>
)

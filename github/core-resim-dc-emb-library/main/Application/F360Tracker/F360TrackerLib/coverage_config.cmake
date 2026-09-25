
# Compile flags and definitions need to be global in order to affect implementations in header files
target_compile_options(F360-Tracker PUBLIC -ftest-coverage -fprofile-arcs -fno-exceptions -fno-inline)
# Add NDEBUG to get rid of assert() branches. Should find a different way of doing this later.
add_definitions(-DNDEBUG)

# Update properties for existing targets
set_target_properties(F360-Tracker PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(F360-Tracker-UT PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(F360-Tracker-QT PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(F360-Tracker-IT PROPERTIES LINK_FLAGS "-lgcov --coverage")
set_target_properties(F360-Tracker-IT-call-order PROPERTIES LINK_FLAGS "-lgcov --coverage")

set(gcovr_exclude_pattern "(tests/)|(.*/unittest)|(.*/testing/unit_testing/cpputest.*)"
                          "?|(.*/ut-frameworks.*)|(.*/*System.*)|(.*/*F360Tracker_Wrapper.*)"
                          "?|(.*/*Output_Adaptation.*)|(.*/*F360_CSLPTrackerPCDLL.*)"
                          "?|(.*/*unified_output.source.*)|(.*/integration_testing.*)"
                          "?|(.*/qualification_testing.*)|(.*/f360_tracker.cpp)|(.*/VSE_Core.*)"
                          "?|(.*/cv_trailer_estimator.*)")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage)
set(GCOVR_BIN ${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/coverage/gcovr)
get_property(OBJ_DIR TARGET F360-Tracker PROPERTY BINARY_DIR)

# Create coverage target
add_custom_target(F360-Tracker-coverage COMMAND
   python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_report.html
   --exclude="${gcovr_exclude_pattern}" --delete

   DEPENDS F360-Tracker-coverage-RunTests
)

# Create coverage target cv trailer
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer)
set(gcovr_exclude_pattern_cv_trailer "(.*/unittest)")

set(gcovr_filter_pattern_cv_trailer "(.*/f360_cvt.*)")
add_custom_target(F360-Tracker-coverage_cv-trailer COMMAND
   python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer/coverage_report_cv_trailer.html
   --filter="${gcovr_filter_pattern_cv_trailer}"
   --exclude="${gcovr_exclude_pattern_cv_trailer}"
   --delete

   DEPENDS F360-Tracker-coverage-RunTests
)

# Create coverage target F360-Tracker-coverage_summary
add_custom_target(F360-Tracker-coverage_summary 
   COMMAND python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage.html
   --exclude="${gcovr_exclude_pattern}"

   COMMAND python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --xml -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage.xml
   --exclude="${gcovr_exclude_pattern}"

   DEPENDS F360-Tracker-coverage-RunTests
)

# Create coverage target F360-Tracker-coverage_cv_trailer_summary
add_custom_target(F360-Tracker-coverage_cv_trailer_summary 
   COMMAND python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer/coverage_report_cv_trailer.html
   --filter="${gcovr_filter_pattern_cv_trailer}" 
   --exclude="${gcovr_exclude_pattern_cv_trailer}"

   COMMAND python ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --xml -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer/coverage_report_cv_trailer.xml
   --filter="${gcovr_filter_pattern_cv_trailer}" 
   --exclude="${gcovr_exclude_pattern_cv_trailer}"

   DEPENDS F360-Tracker-coverage-RunTests
)

# Aggregate target to generate both summary reports in a single make invocation.
add_custom_target(F360-Tracker-coverage_all_summary
   DEPENDS
   F360-Tracker-coverage_summary
   F360-Tracker-coverage_cv_trailer_summary
)

# Make sure the UT executable runs before collecting coverage
add_custom_target(F360-Tracker-coverage-RunTests
   COMMAND
   $<TARGET_FILE:F360-Tracker-UT> &&
   $<TARGET_FILE:F360-Tracker-QT>
)

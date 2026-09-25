
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
                          "?|(.*/safety_handler/.*)")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage)
find_program(GCOVR_BIN gcovr)
get_property(OBJ_DIR TARGET F360-Tracker PROPERTY BINARY_DIR)

# Write cleanup script to remove zero-coverage gcda/gcno from the library target.
# When UT source files #include library source files, both the library and UT targets
# compile the same code but with different branch structures. gcovr merges both
# datasets, inflating the total branch count with phantom uncovered branches.
# Removing the library's empty (0% coverage) gcda/gcno prevents this merge artifact.
# Update 20260616: This script was found to be too aggressive and remove files that
# should be included in coverage. The root cause is that some files were compiled twice
# due to how they were included in the unittest (HZC-312). These included files has now
# been addressed and the clean up script is not needed.
# set(CLEANUP_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/cleanup_zero_coverage.sh")
# file(WRITE ${CLEANUP_SCRIPT} [=[
# #!/bin/bash
# LIB_DIR="$1"
# BUILD_DIR="$2"
# for gcda in $(find "$LIB_DIR" -name "*.gcda" 2>/dev/null); do
#     dir=$(dirname "$gcda")
#     base=$(basename "$gcda")
#     output=$(cd "$dir" && gcov -n "$base" 2>/dev/null)
#     has_nonzero=$(echo "$output" | grep "Lines executed:" | grep -v "0.00%")
#     if [ -z "$has_nonzero" ] && echo "$output" | grep -q "Lines executed:"; then
#         gcno="${gcda%.gcda}.gcno"
#         rm -f "$gcda" "$gcno"
#     fi
# done
# find "$LIB_DIR" -name "*.gcno" | while read f; do
#     gcda="${f%.gcno}.gcda"
#     if [ ! -f "$gcda" ]; then rm -f "$f"; fi
# done
# ]=])

# Cleanup target: runs after tests but before coverage collection
# add_custom_target(F360-Tracker-coverage-cleanup
#    COMMAND bash ${CLEANUP_SCRIPT} ${OBJ_DIR}/CMakeFiles/F360-Tracker.dir ${OBJ_DIR}
#    DEPENDS F360-Tracker-coverage-RunTests
# )

# Generate an intermediate JSON coverage file once (with parallel gcov via -j).
# All summary targets read from this JSON instead of re-invoking gcov, reducing
# the expensive "Reading coverage data" phase from 4 serial passes to 1.
add_custom_target(F360-Tracker-coverage-json
   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --json -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_data.json
   --gcov-ignore-parse-errors="suspicious_hits.warn_once_per_file"
   --gcov-ignore-errors=no_working_dir_found
   -j
   DEPENDS F360-Tracker-coverage-RunTests
)

# Create coverage target
add_custom_target(F360-Tracker-coverage COMMAND
   ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_report.html
   --exclude="${gcovr_exclude_pattern}" --delete
   --gcov-ignore-parse-errors="suspicious_hits.warn_once_per_file"
   --gcov-ignore-errors=no_working_dir_found

   DEPENDS F360-Tracker-coverage-RunTests
)

# Create coverage target cv trailer
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer)
set(gcovr_exclude_pattern_cv_trailer "(.*/unittest)")

set(gcovr_filter_pattern_cv_trailer "(.*/cv_trailer_estimator.*)")
add_custom_target(F360-Tracker-coverage_cv-trailer COMMAND
   ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} --object-directory ${OBJ_DIR}
   --html --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer/coverage_report_cv_trailer.html
   --filter="${gcovr_filter_pattern_cv_trailer}"
   --exclude="${gcovr_exclude_pattern_cv_trailer}"
   --delete
   --gcov-ignore-parse-errors="suspicious_hits.warn_once_per_file"
   --gcov-ignore-errors=no_working_dir_found

   DEPENDS F360-Tracker-coverage-RunTests
)

# Create coverage target F360-Tracker-coverage_summary
# Reads from the pre-built JSON (no gcov re-invocation, fast).
add_custom_target(F360-Tracker-coverage_summary
   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}
   --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_data.json
   --html -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage.html
   --exclude="${gcovr_exclude_pattern}"

   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}
   --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_data.json
   --xml -o ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage.xml
   --exclude="${gcovr_exclude_pattern}"

   DEPENDS F360-Tracker-coverage-json
)

# Create coverage target F360-Tracker-coverage_cv_trailer_summary
# Reads from the pre-built JSON (no gcov re-invocation, fast).
add_custom_target(F360-Tracker-coverage_cv_trailer_summary
   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}
   --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_data.json
   --html -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer/coverage_report_cv_trailer.html
   --filter="${gcovr_filter_pattern_cv_trailer}"
   --exclude="${gcovr_exclude_pattern_cv_trailer}"

   COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR}
   --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage_data.json
   --xml -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_cv_trailer/coverage_report_cv_trailer.xml
   --filter="${gcovr_filter_pattern_cv_trailer}"
   --exclude="${gcovr_exclude_pattern_cv_trailer}"

   DEPENDS F360-Tracker-coverage-json
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

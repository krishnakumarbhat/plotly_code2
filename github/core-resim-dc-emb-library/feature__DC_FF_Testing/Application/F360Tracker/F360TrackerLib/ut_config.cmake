
macro(add_unittest_template aNAME aPATH)
   #copy template file and change name
   set(template_filename f360_unittest_template.cpp)
   set(template_path ${CMAKE_CURRENT_SOURCE_DIR}/../../testing/unit_testing/cpputest)
   file(COPY ${template_path}/${template_filename} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/${aPATH})
   file(RENAME ${aPATH}/${template_filename} ${aPATH}/${aNAME}_unittest.cpp)

   #patch file by finding keyword and replace it
   file(READ ${aPATH}/${aNAME}_unittest.cpp FILE_CONTENT)
   string(REGEX REPLACE "{modulename}" "${aNAME}" MODIFIED_FILE_CONTENT "${FILE_CONTENT}")
   file(WRITE ${aPATH}/${aNAME}_unittest.cpp "${MODIFIED_FILE_CONTENT}")
endmacro()

# Collect all UT sources and add empty _unittest.cpp files where necessary
foreach(source_file ${SRC})
   #MESSAGE("Working with file " ${source_file})
   get_filename_component(source_directory ${source_file} PATH)
   get_filename_component(parent_directory ${source_directory} PATH)
   # Add exception for cv_trailer estimator folder
   if (NOT ${parent_directory} MATCHES "/cv_trailer_estimator")
   get_filename_component(name_without_extension ${source_file} NAME_WE)
   if(${source_file} MATCHES ".cpp" AND NOT ${source_file} MATCHES "xtrk")
      set(utest_src ${name_without_extension}_unittest.cpp)
      if(${source_directory} MATCHES "/source")
         set(testfile  ${CMAKE_CURRENT_SOURCE_DIR}/${source_directory}/../unittest/${utest_src})
         set(testsrc ${source_directory}/../unittest)
      else()
         ## Special handling for files not in subfolders
         set(testfile  ${CMAKE_CURRENT_SOURCE_DIR}/${source_directory}/unittest/${utest_src})
         set(testsrc ${source_directory}/unittest)
      endif()

      if(NOT EXISTS ${testfile})
         #Create a template unittest file
         MESSAGE("Adding ${testfile}.")
         add_unittest_template(${name_without_extension} ${testsrc})
      endif()
   endif()
   endif()#endif for cv_trailer_estimator check
endforeach()

# Add CppUTest
add_subdirectory(../../utilities/ut-frameworks/cpputest cpputest)

# Collect utility functions source files
set(UTIL_SRC
   ../../testing/unit_testing/cpputest/app_main.cpp
   Fusion360/unittest_stubs/f360_data_generator.cpp
   Fusion360/unittest_stubs/f360_set_variant.cpp
   Fusion360/clustering/unittest/utility_functions/f360_clustering_data_generator.cpp
   Fusion360/cluster_grouping/unittest/utility_functions/f360_cluster_grouping_data_generator.cpp
   Fusion360/occlusion/unittest/utilities/f360_occlusion_ut_helpers.cpp
   Fusion360/static_environment/unittest/utility_functions/f360_lsc_data_generator.cpp
)

SET(SM_UT_LIST
   # Add unit tests
   StateManager/unittest/f360_input_diagnostics_unittest.cpp
   StateManager/unittest/f360_output_diagnostics_unittest.cpp
   StateManager/unittest/f360_safety_control_logic_unittest.cpp
   StateManager/unittest/State_Manager_unittest.cpp
   # Also add mocks
   StateManager/unittest/stubs/f360_input_diagnostics_mock.cpp
   StateManager/unittest/stubs/f360_output_diagnostics_mock.cpp
   StateManager/unittest/stubs/SafetyLogicMock.cpp
   StateManager/unittest/stubs/TrackerMock.cpp
)

SET(SM_QT_LIST
   # qt
   StateManager/unittest/f360_safety_control_logic_qualtest_unittest.cpp
   StateManager/unittest/f360_output_diagnostics_qualtest_unittest.cpp
   StateManager/unittest/f360_input_diagnostics_qualtest_unittest.cpp
   # mocks
   StateManager/unittest/stubs/f360_input_diagnostics_mock.cpp
   StateManager/unittest/stubs/f360_output_diagnostics_mock.cpp
   StateManager/unittest/stubs/SafetyLogicMock.cpp
   StateManager/unittest/stubs/TrackerMock.cpp
)

file(GLOB_RECURSE GLOB_UTEST_SRC_LIST
   "*_unittest.cpp"
)

# Go through all found unittest files and remove those that are named "qualtest"
foreach(UTEST_SRC_FILE ${GLOB_UTEST_SRC_LIST})
   if(NOT ${UTEST_SRC_FILE} MATCHES "qualtest")
      # Current files does not have qualtest in the filename, so add it to UTEST_SRC_LIST
      set(UTEST_SRC_LIST ${UTEST_SRC_LIST} ${UTEST_SRC_FILE})

      # Check if the unittest file has a corresponding source file, alert user if not.
      get_filename_component(name_without_extension ${UTEST_SRC_FILE} NAME_WE)
      string(REPLACE "_unittest" "" name_without_extension ${name_without_extension})
      if(NOT "${SRC} ${SM_SRC}" MATCHES ${name_without_extension})
        message("UT without source: " ${UTEST_SRC_FILE})
      endif()
   # else()
      # message("removing " ${UTEST_SRC_FILE})
   endif()
endforeach()

# Collect qualtests
foreach(SRC_DIR ${SRC_DIRS})
   file(GLOB SRC_LIST "${SRC_DIR}/unittest/*_qualtest_unittest.cpp")
   set(QUALTEST_SRC_LIST ${QUALTEST_SRC_LIST} ${SRC_LIST})
endforeach()

# Collect integtests
file(GLOB INTEGTEST_SRC_LIST
    "integration_testing/integtest/*_integtest.cpp"
    "integration_testing/integtest/sanity_checks/*_integtest.cpp"
    "integration_testing/integtest/input_init/source/*.cpp"
)

# Set compile options for subsequent targets
if (MSVC)
   add_compile_options(/MP -D_USE_MATH_DEFINES)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   add_compile_options(-Wall -Wextra -pedantic -Werror -ggdb -O0 -Wno-class-memaccess)
endif()

# Unit Tests
add_executable(F360-Tracker-UT ${UTEST_SRC_LIST} ${SM_UT_LIST} ${UTIL_SRC})
target_link_libraries(F360-Tracker-UT CppUTest CppUTestExt F360-Tracker)

# Qualification Tests
add_executable(F360-Tracker-QT ${QUALTEST_SRC_LIST} ${SM_QT_LIST} ${UTIL_SRC})
target_link_libraries(F360-Tracker-QT CppUTest CppUTestExt F360-Tracker)

# Integration tests
add_executable(F360-Tracker-IT ${INTEGTEST_SRC_LIST}
   ../../testing/unit_testing/cpputest/app_main.cpp
   )
target_link_libraries(F360-Tracker-IT CppUTest CppUTestExt F360-Tracker)

# Additional integrationt tests
add_executable(F360-Tracker-IT-call-order
   integration_testing/integtest/F360_module_calling_order.cpp
   ../../testing/unit_testing/cpputest/app_main.cpp
   )
target_link_libraries(F360-Tracker-IT-call-order CppUTest CppUTestExt F360-Tracker)

# Disable some warnings in some files 
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set_property(SOURCE integration_testing/integtest/F360_module_calling_order.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-parameter")
   set_property(SOURCE StateManager/unittest/stubs/f360_input_diagnostics_mock.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-parameter")
   set_property(SOURCE StateManager/unittest/stubs/f360_output_diagnostics_mock.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-parameter")
   set_property(SOURCE StateManager/unittest/stubs/SafetyLogicMock.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-parameter")
   set_property(SOURCE StateManager/unittest/stubs/TrackerMock.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-parameter")
endif()

# Add include directories for testing targets
foreach(SRC_DIR ${SRC_DIRS})
   target_include_directories(F360-Tracker-UT PRIVATE ${SRC_DIR}/include)
   target_include_directories(F360-Tracker-UT PRIVATE ${SRC_DIR})
   target_include_directories(F360-Tracker-QT PRIVATE ${SRC_DIR}/include)
   target_include_directories(F360-Tracker-IT PRIVATE ${SRC_DIR}/include)
   target_include_directories(F360-Tracker-IT-call-order PRIVATE ${SRC_DIR}/include)
endforeach()

target_include_directories(F360-Tracker-UT PRIVATE
   Fusion360
   Fusion360/cv_trailer_estimator/_sharedutils
   Fusion360/cv_trailer_estimator/calc_norm_vec
   Fusion360/cv_trailer_estimator/calc_trail_lon_vel
   Fusion360/cv_trailer_estimator/gen_rnd_idx_vecs
   Fusion360/cv_trailer_estimator/ransac
   Fusion360/cv_trailer_estimator/discrete_deriv
   Fusion360/cv_trailer_estimator/gen_rnd_idx_pair
   Fusion360/cv_trailer_estimator/line_estimation
   Fusion360/cv_trailer_estimator/n_inliers
   Fusion360/cv_trailer_estimator/one_trailer_ekf
   Fusion360/cv_trailer_estimator/two_trailer_ekf
   Fusion360/cv_trailer_estimator/length_filter
   Fusion360/cv_trailer_estimator
   Fusion360/include
   Fusion360/cluster_grouping/unittest/utility_functions
   Fusion360/clustering/unittest/utility_functions
   Fusion360/initialize_tracks/unittest/utility_functions
   Fusion360/occlusion/unittest/utilities
   Fusion360/static_environment/unittest/utility_functions
   Fusion360/unittest_stubs
   StateManager/unittest/stubs
   ../sg_stationary_geometry/src/iface
   ../sg_stationary_geometry/src/iface/types
   ../sg_stationary_geometry/src/iface/types/dump
   ../sg_stationary_geometry/src/iface/types/contour
   ../../utilities/ut-frameworks/cpputest/include
   )

target_include_directories(F360-Tracker-QT PRIVATE
   Fusion360
   Fusion360/include
   Fusion360/cv_trailer_estimator/_sharedutils
   Fusion360/cv_trailer_estimator/calc_norm_vec
   Fusion360/cv_trailer_estimator/calc_trail_lon_vel
   Fusion360/cv_trailer_estimator/gen_rnd_idx_vecs
   Fusion360/cv_trailer_estimator/ransac
   Fusion360/cv_trailer_estimator/discrete_deriv
   Fusion360/cv_trailer_estimator/gen_rnd_idx_pair
   Fusion360/cv_trailer_estimator/line_estimation
   Fusion360/cv_trailer_estimator/n_inliers
   Fusion360/cv_trailer_estimator/one_trailer_ekf
   Fusion360/cv_trailer_estimator/two_trailer_ekf
   Fusion360/cv_trailer_estimator/length_filter
   Fusion360/cv_trailer_estimator
   Fusion360/cluster_grouping/unittest/utility_functions
   Fusion360/clustering/unittest/utility_functions
   Fusion360/initialize_tracks/unittest/utility_functions
   Fusion360/occlusion/unittest/utilities
   Fusion360/static_environment/unittest/utility_functions
   Fusion360/unittest_stubs
   StateManager/unittest/stubs
   ../../utilities/ut-frameworks/cpputest/include
   ../sg_stationary_geometry/src/iface
   ../sg_stationary_geometry/src/iface/types
   ../sg_stationary_geometry/src/iface/types/dump
   ../sg_stationary_geometry/src/iface/types/contour
   ../../utilities/ut-frameworks/cpputest/include
   )

target_include_directories(F360-Tracker-IT PRIVATE
   Fusion360
   Fusion360/include
   Fusion360/unittest_stubs
   integration_testing/integtest
   integration_testing/integtest/input_init/include
   ../../utilities/ut-frameworks/cpputest/include
   ../../testing/unit_testing/type_check
   ../sg_stationary_geometry/src/iface
   ../sg_stationary_geometry/src/iface/types
   ../sg_stationary_geometry/src/iface/types/dump
   ../sg_stationary_geometry/src/iface/types/contour
   ../../utilities/ut-frameworks/cpputest/include
   )

target_include_directories(F360-Tracker-IT-call-order PRIVATE
   Fusion360
   Fusion360/include
   Fusion360/unittest_stubs
   integration_testing/integtest
   integration_testing/integtest/input_init/include
   ../../utilities/ut-frameworks/cpputest/include
   ../../testing/unit_testing/type_check
   ../sg_stationary_geometry/src/iface
   ../sg_stationary_geometry/src/iface/types
   ../sg_stationary_geometry/src/iface/types/dump
   ../sg_stationary_geometry/src/iface/types/contour
   ../../utilities/ut-frameworks/cpputest/include
   )

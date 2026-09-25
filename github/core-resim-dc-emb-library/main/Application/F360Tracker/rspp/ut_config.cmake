macro(add_unittest_template aNAME aPATH)
   #copy template file and change name
   set(template_filename rspp_unittest_template.cpp)
   set(template_path ${CMAKE_CURRENT_SOURCE_DIR}/../../testing/unit_testing/cpputest)
   file(COPY ${template_path}/${template_filename} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/${aPATH})
   file(RENAME ${aPATH}/${template_filename} ${aPATH}/${aNAME}_unittest.cpp)

   #patch file by finding keyword and replace it
   file(READ ${aPATH}/${aNAME}_unittest.cpp FILE_CONTENT)
   string(REGEX REPLACE "{modulename}" "${aNAME}" MODIFIED_FILE_CONTENT "${FILE_CONTENT}")
   file(WRITE ${aPATH}/${aNAME}_unittest.cpp "${MODIFIED_FILE_CONTENT}")
endmacro()

get_target_property(rspp_sources rspp SOURCES)

# Collect all UT sources and crate template empty files if necessary
foreach(source_file ${rspp_sources})
   get_filename_component(source_directory ${source_file} PATH)
   get_filename_component(name_without_extension ${source_file} NAME_WE)
   if(${source_file} MATCHES ".cpp")
      set(utest_src ${name_without_extension}_unittest.cpp)
      set(testfile  ${CMAKE_CURRENT_SOURCE_DIR}/${source_directory}/../unittest/${utest_src})
      set(testsrc ${source_directory}/../unittest)
      
      if(NOT EXISTS ${testfile})
         #Create a template unittest file
         MESSAGE("Adding ${testfile}.")
         add_unittest_template(${name_without_extension} ${testsrc})
      endif()
   endif()
endforeach()

add_subdirectory(../../utilities/ut-frameworks/cpputest cpputest)

file(GLOB_RECURSE GLOB_UTEST_SRC_LIST
   "*_unittest.cpp"
)

foreach(UTEST_SRC_FILE ${GLOB_UTEST_SRC_LIST})
   if(NOT ${UTEST_SRC_FILE} MATCHES "qualtest")
      # Current files does not have qualtest in the filename, so add it to UTEST_SRC_LIST
      set(UTEST_SRC_LIST ${UTEST_SRC_LIST} ${UTEST_SRC_FILE})

      # Check if the unittest file has a corresponding source file, alert user if not.
      get_filename_component(name_without_extension ${UTEST_SRC_FILE} NAME_WE)
      string(REPLACE "_unittest" "" name_without_extension ${name_without_extension})
      if(NOT "${rspp_sources}" MATCHES ${name_without_extension})
        message("UT without source: " ${UTEST_SRC_FILE})
      endif()
   endif()
endforeach()

# ==============================================================================
# Collect all QT sources
# ==============================================================================
get_target_property(rspp_source_dir rspp SOURCE_DIR)
file(GLOB SRC_LIST "${rspp_source_dir}/unittest/*_qualtest_unittest.cpp")
set(QUALTEST_SRC_LIST ${SRC_LIST})

# ==============================================================================
# Setup targets
# ==============================================================================
add_executable(rspp-UT ${UTEST_SRC_LIST} ../../testing/unit_testing/cpputest/app_main.cpp)
add_executable(rspp-QT ${QUALTEST_SRC_LIST} ../../testing/unit_testing/cpputest/app_main.cpp)

target_include_directories(rspp-UT PRIVATE
   ../../utilities/ut-frameworks/cpputest/include
   include
   source
)

target_include_directories(rspp-QT PRIVATE
   ../../utilities/ut-frameworks/cpputest/include
   include
   source
)

target_link_libraries(rspp-UT rspp CppUTest CppUTestExt)
target_link_libraries(rspp-QT rspp CppUTest CppUTestExt)

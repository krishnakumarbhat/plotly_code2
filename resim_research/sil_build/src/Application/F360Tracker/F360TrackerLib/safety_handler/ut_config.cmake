
macro(add_unittest_template aNAME aPATH)
   #copy template file and change name
   set(template_filename f360_unittest_template.cpp)
   set(template_path ${CMAKE_CURRENT_SOURCE_DIR}/../../../testing/unit_testing/cpputest)
   file(COPY ${template_path}/${template_filename} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/${aPATH})
   file(RENAME ${aPATH}/${template_filename} ${aPATH}/${aNAME}_unittest.cpp)

   #patch file by finding keyword and replace it
   file(READ ${aPATH}/${aNAME}_unittest.cpp FILE_CONTENT)
   string(REGEX REPLACE "{modulename}" "${aNAME}" MODIFIED_FILE_CONTENT "${FILE_CONTENT}")
   file(WRITE ${aPATH}/${aNAME}_unittest.cpp "${MODIFIED_FILE_CONTENT}")
endmacro()

# Collect all UT sources and add empty _unittest.cpp files where necessary
foreach(source_file ${SAFETY_HANDLER_SRC})
   #MESSAGE("Working with file " ${source_file})
   get_filename_component(source_directory ${source_file} PATH)
   get_filename_component(parent_directory ${source_directory} PATH)
   get_filename_component(name_without_extension ${source_file} NAME_WE)
   if(${source_file} MATCHES ".cpp" )
      set(utest_src ${name_without_extension}_unittest.cpp)
      if(${source_directory} MATCHES "source")
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
endforeach()

# Add CppUTest
# add_subdirectory(../../utilities/ut-frameworks/cpputest cpputest)

# Collect utility functions source files
set(UTIL_SRC
   ../../../testing/unit_testing/cpputest/app_main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/unittest/utility_functions/sh_unittest_support.cpp
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
      if(NOT "${SAFETY_HANDLER_SRC}" MATCHES ${name_without_extension})
        message("UT without source: " ${UTEST_SRC_FILE})
      endif()
   endif()
endforeach()

# Collect qualtests
file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/unittest/*_qualtest_unittest.cpp")
set(QUALTEST_SRC_LIST ${SRC_LIST})

# Unit Tests
add_executable(safety_handler-UT ${UTEST_SRC_LIST} ${UTIL_SRC})
target_link_libraries(safety_handler-UT CppUTest CppUTestExt safety_handler)

# Qualification Tests
add_executable(safety_handler-QT ${QUALTEST_SRC_LIST} ${UTIL_SRC})
target_link_libraries(safety_handler-QT CppUTest CppUTestExt safety_handler)


# Add include directories for testing targets

target_include_directories(safety_handler-UT PRIVATE
                          ../../../utilities/ut-frameworks/cpputest/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/unittest/utility_functions
                          ${CMAKE_CURRENT_SOURCE_DIR}/source
                          ${CMAKE_CURRENT_SOURCE_DIR}/iface)

target_include_directories(safety_handler-QT PRIVATE
                           ../../../utilities/ut-frameworks/cpputest/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/unittest/utility_functions
                           ${CMAKE_CURRENT_SOURCE_DIR}/source
                           ${CMAKE_CURRENT_SOURCE_DIR}/iface)
#Name of module - needs to be changed.
set(MODULE_NAME
   "SharedTrackerAPI"
)
#Path where module folder is - needs to be changed.
set(MODULE_PATH
    "sw"
)

#Path to module root -
set(MODULE_ROOT
   ${MODULE_PATH}/F360TrackerLib/SharedTrackerAPI/Types
)

#Path to stub files -
set(STUBS_ROOT
)

#Add paths to include files needed
include_directories(
   ${MODULE_ROOT}
   ${MODULE_PATH}/F360TrackerLib/SharedTrackerAPI/Types
)

message("Adding tests from " ${CMAKE_CURRENT_SOURCE_DIR})

#Add source files that shall be tested
set(SRC
   ${MODULE_ROOT}/f360_radar_sensor.h
)

#Add additional stub files to be built if any.
set(STUB_SRC
)


unset(UTEST_SRC_LIST)

#
#Loop to generate corresponding template test file if not already there
#
#MESSAGE("Module=" ${MODULE_ROOT})
foreach(filename IN LISTS SRC)
#   MESSAGE("Working with file=" ${filename} )
   get_filename_component(name_without_extension ${filename} NAME_WE)
   set(utest_src ${name_without_extension}_unittest.cpp)
   set(testfile  ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_ROOT}/unittest/${utest_src})
   if(NOT EXISTS ${testfile})
       #Create a template unittest file
       MESSAGE("${testfile} doesn't exist.")
       add_unittest_template(${name_without_extension} ${MODULE_ROOT}/unittest)
   endif()
   set(UTEST_SRC_LIST
      ${UTEST_SRC_LIST}
      ${MODULE_ROOT}/unittest/${utest_src}
   )
endforeach(filename)

add_cppu_test(${MODULE_NAME}_test
   ${UTEST_SRC_LIST}
   ${SRC}
   ${STUB_SRC}
)

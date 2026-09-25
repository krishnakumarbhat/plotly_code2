#Name of module - needs to be changed.
set(MODULE_NAME
   "State_Manager"
)
#Path where module folder is - needs to be changed.
set(MODULE_PATH
    "sw"
) 

#Path to module root - 
set(MODULE_ROOT 
   ${MODULE_PATH}/F360TrackerLib/StateManager
)

#Path to stub files -
set(STUBS_ROOT 
   ${MODULE_ROOT}/unittest/stubs
)

#Add paths to include files needed
include_directories(
   ${MODULE_ROOT}/include
   ${MODULE_PATH}/F360TrackerLib/SharedTrackerAPI
   ${MODULE_PATH}/F360TrackerLib/SharedTrackerAPI/core
   ${MODULE_PATH}/F360TrackerLib/SharedTrackerAPI/Logging
   ${MODULE_PATH}/F360TrackerLib/SharedTrackerAPI/Types
   ${MODULE_ROOT}/unittest/stubs
)

message("Adding tests from " ${CMAKE_CURRENT_SOURCE_DIR})

#Add source files that shall be tested
set(SRC 
   ${MODULE_ROOT}/source/State_Manager.cpp
   ${MODULE_ROOT}/source/f360_input_diagnostics.cpp
   ${MODULE_ROOT}/source/f360_output_diagnostics.cpp
   ${MODULE_ROOT}/source/f360_safety_control_logic.cpp
)

#Add additional stub files to be built if any.
set(STUB_SRC
    ${MODULE_ROOT}/unittest/stubs/f360_input_diagnostics_mock.h
    ${MODULE_ROOT}/unittest/stubs/f360_output_diagnostics_mock.h
    ${MODULE_ROOT}/unittest/stubs/SafetyLogicMock.cpp
    ${MODULE_ROOT}/unittest/stubs/TrackerMock.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_angle.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_point.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_bounding_box.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_math_func.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_line.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_vector.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_polygon.cpp
    ${MODULE_PATH}/F360TrackerLib/Fusion360/common/source/f360_norm_heading_angle.cpp
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

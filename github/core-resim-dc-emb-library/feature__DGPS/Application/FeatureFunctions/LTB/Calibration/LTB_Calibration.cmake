set(LTB_CAL_ROOT_DIR "${LTB_ROOT_PATH}/Calibration")
set(LTB_CAL_CUSTOMER_DIR "${LTB_CAL_ROOT_DIR}/${LTB_PROJECT_VARIANT}")
set(LTB_SECTION_COMPATIBILITY "LTB_Core")
set(LTB_CAL_HEADER_FILE_DIR "${LTB_CAL_ROOT_DIR}/${LTB_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(LTB_GENERATE_CAL_FILES "Generate cal files for LTB" OFF)
if(LTB_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${LTB_CAL_ROOT_DIR}/LTB_Core/Calibration_Tool_for_LTB.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(LTB PUBLIC ${LTB_CAL_HEADER_FILE_DIR})
target_sources(LTB PRIVATE ${LTB_CAL_HEADER_FILE_DIR}/ltb_cal.xml)
target_sources(LTB PRIVATE ${LTB_CAL_HEADER_FILE_DIR}/ltb_core_calibration.h)
target_sources(LTB PRIVATE ${LTB_CAL_HEADER_FILE_DIR}/ltb_core_calibration_t.h)
target_sources(LTB PRIVATE ${LTB_CAL_HEADER_FILE_DIR}/ltb_core_calibration_print_functions.c)
target_sources(LTB PRIVATE ${LTB_CAL_HEADER_FILE_DIR}/ltb_core_calibration_check.h)
target_sources(LTB PRIVATE ${LTB_CAL_HEADER_FILE_DIR}/ltb_core_calibration_check.c)

target_include_directories(LTB PUBLIC ${LTB_CAL_CUSTOMER_DIR})
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_core_calibration.c)

target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_customer_calibration.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_customer_calibration_print_functions.c)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_customer_calibration_t.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_customer_calibration_check.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_customer_calibration_check.c)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_customer_calibration.c)

target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_public_calibration.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_public_calibration_print_functions.c)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_public_calibration_t.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_public_calibration_check.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_public_calibration_check.c)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_public_calibration.c)

target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_update_calibration.h)
target_sources(LTB PRIVATE ${LTB_CAL_CUSTOMER_DIR}/ltb_update_calibration.c)

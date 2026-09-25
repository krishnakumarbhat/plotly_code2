set(PT_CAL_ROOT_DIR "${PT_ROOT_PATH}/Calibration")
set(PT_CAL_CUSTOMER_DIR "${PT_CAL_ROOT_DIR}/${PT_PROJECT_VARIANT}")
set(PT_SECTION_COMPATIBILITY "PT_Core")
set(PT_CAL_HEADER_FILE_DIR "${PT_CAL_ROOT_DIR}/${PT_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(PT_GENERATE_CAL_FILES "Generate cal files for PT" OFF)
if(PT_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${PT_CAL_ROOT_DIR}/PT_Core/Calibration_Tool_for_PT.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(PT PUBLIC ${PT_CAL_HEADER_FILE_DIR})
target_sources(PT PRIVATE ${PT_CAL_HEADER_FILE_DIR}/pt_cal.xml)
target_sources(PT PRIVATE ${PT_CAL_HEADER_FILE_DIR}/pt_core_calibration.h)
target_sources(PT PRIVATE ${PT_CAL_HEADER_FILE_DIR}/pt_core_calibration_t.h)
target_sources(PT PRIVATE ${PT_CAL_HEADER_FILE_DIR}/pt_core_calibration_print_functions.c)
target_sources(PT PRIVATE ${PT_CAL_HEADER_FILE_DIR}/pt_core_calibration_check.h)
target_sources(PT PRIVATE ${PT_CAL_HEADER_FILE_DIR}/pt_core_calibration_check.c)


target_include_directories(PT PUBLIC ${PT_CAL_CUSTOMER_DIR})
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_core_calibration.c)

target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_customer_calibration.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_customer_calibration_print_functions.c)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_customer_calibration_t.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_customer_calibration_check.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_customer_calibration_check.c)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_customer_calibration.c)

target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_public_calibration.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_public_calibration_print_functions.c)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_public_calibration_t.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_public_calibration_check.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_public_calibration_check.c)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_public_calibration.c)

target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_update_calibration.h)
target_sources(PT PRIVATE ${PT_CAL_CUSTOMER_DIR}/pt_update_calibration.c)

set(ESA_CAL_ROOT_DIR "${ESA_ROOT_PATH}/Calibration")
set(ESA_CAL_CUSTOMER_DIR "${ESA_CAL_ROOT_DIR}/${ESA_PROJECT_VARIANT}")
set(ESA_SECTION_COMPATIBILITY "ESA_Core")
set(ESA_CAL_HEADER_FILE_DIR "${ESA_CAL_ROOT_DIR}/${ESA_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(ESA_GENERATE_CAL_FILES "Generate cal files for ESA" OFF)
if(ESA_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${ESA_CAL_ROOT_DIR}/ESA_Core/Calibration_Tool_for_ESA.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(ESA PUBLIC ${ESA_CAL_HEADER_FILE_DIR})
target_sources(ESA PRIVATE ${ESA_CAL_HEADER_FILE_DIR}/esa_cal.xml)
target_sources(ESA PRIVATE ${ESA_CAL_HEADER_FILE_DIR}/esa_core_calibration.h)
target_sources(ESA PRIVATE ${ESA_CAL_HEADER_FILE_DIR}/esa_core_calibration_t.h)
target_sources(ESA PRIVATE ${ESA_CAL_HEADER_FILE_DIR}/esa_core_calibration_print_functions.c)
target_sources(ESA PRIVATE ${ESA_CAL_HEADER_FILE_DIR}/esa_core_calibration_check.h)
target_sources(ESA PRIVATE ${ESA_CAL_HEADER_FILE_DIR}/esa_core_calibration_check.c)


target_include_directories(ESA PUBLIC ${ESA_CAL_CUSTOMER_DIR})
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_core_calibration.c)

target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_customer_calibration.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_customer_calibration_print_functions.c)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_customer_calibration_t.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_customer_calibration_check.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_customer_calibration_check.c)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_customer_calibration.c)

target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_public_calibration.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_public_calibration_print_functions.c)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_public_calibration_t.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_public_calibration_check.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_public_calibration_check.c)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_public_calibration.c)

target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_update_calibration.h)
target_sources(ESA PRIVATE ${ESA_CAL_CUSTOMER_DIR}/esa_update_calibration.c)

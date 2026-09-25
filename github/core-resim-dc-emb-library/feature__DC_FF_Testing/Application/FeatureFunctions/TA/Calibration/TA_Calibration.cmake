set(TA_CAL_ROOT_DIR "${TA_ROOT_PATH}/Calibration")
set(TA_CAL_CUSTOMER_DIR "${TA_CAL_ROOT_DIR}/${TA_PROJECT_VARIANT}")
set(TA_SECTION_COMPATIBILITY "TA_Core")
set(TA_CAL_HEADER_FILE_DIR "${TA_CAL_ROOT_DIR}/${TA_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(TA_GENERATE_CAL_FILES "Generate cal files for TA" OFF)
if(TA_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${TA_CAL_ROOT_DIR}/TA_Core/Calibration_Tool_for_TA.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(TA PUBLIC ${TA_CAL_HEADER_FILE_DIR})
target_sources(TA PRIVATE ${TA_CAL_HEADER_FILE_DIR}/ta_cal.xml)
target_sources(TA PRIVATE ${TA_CAL_HEADER_FILE_DIR}/ta_core_calibration.h)
target_sources(TA PRIVATE ${TA_CAL_HEADER_FILE_DIR}/ta_core_calibration_t.h)
target_sources(TA PRIVATE ${TA_CAL_HEADER_FILE_DIR}/ta_core_calibration_print_functions.c)
target_sources(TA PRIVATE ${TA_CAL_HEADER_FILE_DIR}/ta_core_calibration_check.h)
target_sources(TA PRIVATE ${TA_CAL_HEADER_FILE_DIR}/ta_core_calibration_check.c)

target_include_directories(TA PUBLIC ${TA_CAL_CUSTOMER_DIR})
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_core_calibration.c)

target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_customer_calibration.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_customer_calibration_print_functions.c)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_customer_calibration_t.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_customer_calibration_check.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_customer_calibration_check.c)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_customer_calibration.c)

target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_public_calibration.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_public_calibration_print_functions.c)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_public_calibration_t.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_public_calibration_check.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_public_calibration_check.c)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_public_calibration.c)

target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_update_calibration.h)
target_sources(TA PRIVATE ${TA_CAL_CUSTOMER_DIR}/ta_update_calibration.c)

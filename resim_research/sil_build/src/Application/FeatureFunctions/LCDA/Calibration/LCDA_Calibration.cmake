set(LCDA_CAL_ROOT_DIR "${LCDA_ROOT_PATH}/Calibration")
set(LCDA_CAL_CUSTOMER_DIR "${LCDA_CAL_ROOT_DIR}/${LCDA_PROJECT_VARIANT}")
set(LCDA_SECTION_COMPATIBILITY "LCDA_Core")
set(LCDA_CAL_HEADER_FILE_DIR "${LCDA_CAL_ROOT_DIR}/${LCDA_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(LCDA_GENERATE_CAL_FILES "Generate cal files for LCDA" OFF)
if(LCDA_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${LCDA_CAL_ROOT_DIR}/LCDA_Core/Calibration_Tool_for_LCDA.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(LCDA PUBLIC ${LCDA_CAL_HEADER_FILE_DIR})
target_sources(LCDA PRIVATE ${LCDA_CAL_HEADER_FILE_DIR}/lcda_cal.xml)
target_sources(LCDA PRIVATE ${LCDA_CAL_HEADER_FILE_DIR}/lcda_core_calibration.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_HEADER_FILE_DIR}/lcda_core_calibration_t.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_HEADER_FILE_DIR}/lcda_core_calibration_print_functions.c)
target_sources(LCDA PRIVATE ${LCDA_CAL_HEADER_FILE_DIR}/lcda_core_calibration_check.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_HEADER_FILE_DIR}/lcda_core_calibration_check.c)


target_include_directories(LCDA PUBLIC ${LCDA_CAL_CUSTOMER_DIR})
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_core_calibration.c)

target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_customer_calibration.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_customer_calibration_print_functions.c)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_customer_calibration_t.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_customer_calibration_check.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_customer_calibration_check.c)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_customer_calibration.c)

target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_public_calibration.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_public_calibration_print_functions.c)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_public_calibration_t.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_public_calibration_check.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_public_calibration_check.c)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_public_calibration.c)

target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_update_calibration.h)
target_sources(LCDA PRIVATE ${LCDA_CAL_CUSTOMER_DIR}/lcda_update_calibration.c)

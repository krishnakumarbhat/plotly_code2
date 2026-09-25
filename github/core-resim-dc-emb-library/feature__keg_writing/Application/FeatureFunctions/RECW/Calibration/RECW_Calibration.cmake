set(RECW_CAL_ROOT_DIR "${RECW_ROOT_PATH}/Calibration")
set(RECW_CAL_CUSTOMER_DIR "${RECW_CAL_ROOT_DIR}/${RECW_PROJECT_VARIANT}")
set(RECW_SECTION_COMPATIBILITY "RECW_Core")
set(RECW_CAL_HEADER_FILE_DIR "${RECW_CAL_ROOT_DIR}/${RECW_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(RECW_GENERATE_CAL_FILES "Generate cal files for RECW" OFF)
if(RECW_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${RECW_CAL_ROOT_DIR}/RECW_Core/Calibration_Tool_for_RECW.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(RECW PUBLIC ${RECW_CAL_HEADER_FILE_DIR})
target_sources(RECW PRIVATE ${RECW_CAL_HEADER_FILE_DIR}/recw_cal.xml)
target_sources(RECW PRIVATE ${RECW_CAL_HEADER_FILE_DIR}/recw_core_calibration.h)
target_sources(RECW PRIVATE ${RECW_CAL_HEADER_FILE_DIR}/recw_core_calibration_t.h)
target_sources(RECW PRIVATE ${RECW_CAL_HEADER_FILE_DIR}/recw_core_calibration_print_functions.c)
target_sources(RECW PRIVATE ${RECW_CAL_HEADER_FILE_DIR}/recw_core_calibration_check.h)
target_sources(RECW PRIVATE ${RECW_CAL_HEADER_FILE_DIR}/recw_core_calibration_check.c)


target_include_directories(RECW PUBLIC ${RECW_CAL_CUSTOMER_DIR})
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_core_calibration.c)

target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_customer_calibration.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_customer_calibration_print_functions.c)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_customer_calibration_t.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_customer_calibration_check.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_customer_calibration_check.c)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_customer_calibration.c)

target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_public_calibration.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_public_calibration_print_functions.c)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_public_calibration_t.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_public_calibration_check.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_public_calibration_check.c)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_public_calibration.c)

target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_update_calibration.h)
target_sources(RECW PRIVATE ${RECW_CAL_CUSTOMER_DIR}/recw_update_calibration.c)

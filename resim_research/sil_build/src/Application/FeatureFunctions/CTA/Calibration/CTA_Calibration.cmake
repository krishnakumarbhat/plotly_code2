set(CTA_CAL_ROOT_DIR "${CTA_ROOT_PATH}/Calibration")
set(CTA_CAL_CUSTOMER_DIR "${CTA_CAL_ROOT_DIR}/${CTA_PROJECT_VARIANT}")
set(CTA_SECTION_COMPATIBILITY "CTA_Core")
set(CTA_CAL_HEADER_FILE_DIR "${CTA_CAL_ROOT_DIR}/${CTA_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(CTA_GENERATE_CAL_FILES "Generate cal files for CTA" OFF)
if(CTA_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${CTA_CAL_ROOT_DIR}/CTA_Core/Calibration_Tool_for_CTA.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(CTA PUBLIC ${CTA_CAL_HEADER_FILE_DIR})
target_sources(CTA PRIVATE ${CTA_CAL_HEADER_FILE_DIR}/cta_cal.xml)
target_sources(CTA PRIVATE ${CTA_CAL_HEADER_FILE_DIR}/cta_core_calibration.h)
target_sources(CTA PRIVATE ${CTA_CAL_HEADER_FILE_DIR}/cta_core_calibration_t.h)
target_sources(CTA PRIVATE ${CTA_CAL_HEADER_FILE_DIR}/cta_core_calibration_print_functions.c)
target_sources(CTA PRIVATE ${CTA_CAL_HEADER_FILE_DIR}/cta_core_calibration_check.h)
target_sources(CTA PRIVATE ${CTA_CAL_HEADER_FILE_DIR}/cta_core_calibration_check.c)

target_include_directories(CTA PUBLIC ${CTA_CAL_CUSTOMER_DIR})
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_core_calibration.c)

target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_customer_calibration.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_customer_calibration_print_functions.c)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_customer_calibration_t.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_customer_calibration_check.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_customer_calibration_check.c)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_customer_calibration.c)

target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_public_calibration.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_public_calibration_print_functions.c)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_public_calibration_t.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_public_calibration_check.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_public_calibration_check.c)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_public_calibration.c)

target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_update_calibration.h)
target_sources(CTA PRIVATE ${CTA_CAL_CUSTOMER_DIR}/cta_update_calibration.c)

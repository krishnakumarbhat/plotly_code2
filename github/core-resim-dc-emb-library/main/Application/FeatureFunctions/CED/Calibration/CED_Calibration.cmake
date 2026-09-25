set(CED_CAL_ROOT_DIR "${CED_ROOT_PATH}/Calibration")
set(CED_CAL_CUSTOMER_DIR "${CED_CAL_ROOT_DIR}/${CED_PROJECT_VARIANT}")
set(CED_SECTION_COMPATIBILITY "CED_Core")
set(CED_CAL_HEADER_FILE_DIR "${CED_CAL_ROOT_DIR}/${CED_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(CED_GENERATE_CAL_FILES "Generate cal files for CED" OFF)
if(CED_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${CED_CAL_ROOT_DIR}/CED_Core/Calibration_Tool_for_CED.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(CED PUBLIC ${CED_CAL_HEADER_FILE_DIR})
target_sources(CED PRIVATE ${CED_CAL_HEADER_FILE_DIR}/ced_cal.xml)
target_sources(CED PRIVATE ${CED_CAL_HEADER_FILE_DIR}/ced_core_calibration.h)
target_sources(CED PRIVATE ${CED_CAL_HEADER_FILE_DIR}/ced_core_calibration_t.h)
target_sources(CED PRIVATE ${CED_CAL_HEADER_FILE_DIR}/ced_core_calibration_print_functions.c)
target_sources(CED PRIVATE ${CED_CAL_HEADER_FILE_DIR}/ced_core_calibration_check.h)
target_sources(CED PRIVATE ${CED_CAL_HEADER_FILE_DIR}/ced_core_calibration_check.c)


target_include_directories(CED PUBLIC ${CED_CAL_CUSTOMER_DIR})
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_core_calibration.c)

target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_customer_calibration.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_customer_calibration_print_functions.c)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_customer_calibration_t.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_customer_calibration_check.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_customer_calibration_check.c)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_customer_calibration.c)

target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_public_calibration.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_public_calibration_print_functions.c)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_public_calibration_t.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_public_calibration_check.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_public_calibration_check.c)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_public_calibration.c)

target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_update_calibration.h)
target_sources(CED PRIVATE ${CED_CAL_CUSTOMER_DIR}/ced_update_calibration.c)
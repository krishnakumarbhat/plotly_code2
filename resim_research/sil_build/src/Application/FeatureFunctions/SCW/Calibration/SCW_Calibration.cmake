set(SCW_CAL_ROOT_DIR "${SCW_ROOT_PATH}/Calibration")
set(SCW_CAL_CUSTOMER_DIR "${SCW_CAL_ROOT_DIR}/${SCW_PROJECT_VARIANT}")
set(SCW_SECTION_COMPATIBILITY "SCW_Core")
set(SCW_CAL_HEADER_FILE_DIR "${SCW_CAL_ROOT_DIR}/${SCW_SECTION_COMPATIBILITY}")

# Generate the calibrations for the customer if defined
option(SCW_GENERATE_CAL_FILES "Generate cal files for SCW" OFF)
if(SCW_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(COMMAND cmd /c ${SCW_CAL_ROOT_DIR}/SCW_Core/Calibration_Tool_for_SCW.bat)
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(SCW PUBLIC ${SCW_CAL_HEADER_FILE_DIR})
target_sources(SCW PRIVATE ${SCW_CAL_HEADER_FILE_DIR}/scw_cal.xml)
target_sources(SCW PRIVATE ${SCW_CAL_HEADER_FILE_DIR}/scw_core_calibration.h)
target_sources(SCW PRIVATE ${SCW_CAL_HEADER_FILE_DIR}/scw_core_calibration_t.h)
target_sources(SCW PRIVATE ${SCW_CAL_HEADER_FILE_DIR}/scw_core_calibration_print_functions.c)
target_sources(SCW PRIVATE ${SCW_CAL_HEADER_FILE_DIR}/scw_core_calibration_check.h)
target_sources(SCW PRIVATE ${SCW_CAL_HEADER_FILE_DIR}/scw_core_calibration_check.c)


target_include_directories(SCW PUBLIC ${SCW_CAL_CUSTOMER_DIR})
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_core_calibration.c)

target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_customer_calibration.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_customer_calibration_print_functions.c)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_customer_calibration_t.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_customer_calibration_check.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_customer_calibration_check.c)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_customer_calibration.c)

target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_public_calibration.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_public_calibration_print_functions.c)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_public_calibration_t.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_public_calibration_check.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_public_calibration_check.c)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_public_calibration.c)

target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_update_calibration.h)
target_sources(SCW PRIVATE ${SCW_CAL_CUSTOMER_DIR}/scw_update_calibration.c)

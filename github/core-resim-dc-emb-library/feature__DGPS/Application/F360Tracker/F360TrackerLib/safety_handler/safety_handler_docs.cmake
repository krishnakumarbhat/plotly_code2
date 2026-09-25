# Set path to config directory
set(common_conf_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../DevOps/tools/ut-frameworks/common-configs)

# Determine tracker version
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/../SharedTrackerAPI/core/f360_tracker_version.h
  TRACKER_VERSIONS REGEX "Tracker_Version_")

foreach(TRACKER_VERSION ${TRACKER_VERSIONS})
  if(${TRACKER_VERSION} MATCHES "Major")
    string(REGEX REPLACE ".*= *([0-9]+).*" "\\1" TRACKER_VERSION_MAJOR "${TRACKER_VERSION}")
  elseif(${TRACKER_VERSION} MATCHES "Minor")
    string(REGEX REPLACE ".*= *([0-9]+).*" "\\1" TRACKER_VERSION_MINOR "${TRACKER_VERSION}")
  elseif(${TRACKER_VERSION} MATCHES "Patch")
    string(REGEX REPLACE ".*= *([0-9]+).*" "\\1" TRACKER_VERSION_PATCH "${TRACKER_VERSION}")
  endif()
endforeach()

# Create extended project information for Doxygen
if(DEFINED SW_TAG_HEX)
    set(MODULE_COMMIT_HASH "${SW_TAG_HEX}")
else()
    set(MODULE_COMMIT_HASH "Unknown")
endif()

if(DEFINED BUILD_DATE)
    set(PROJECT_BUILD_DATETIME "${BUILD_DATE}")
else()
    string(TIMESTAMP PROJECT_BUILD_DATETIME "%Y-%m-%d %H:%M:%S")
endif()

if(DEFINED TESTER)
    set(PROJECT_TESTER "${TESTER}")
else()
   set(PROJECT_TESTER "Unknown")
endif()

set (MODULE_INTEGRITY_LEVEL "ASIL-B") # used by the UTS report generation

set(TRACKER_MODULE_NAME "SafetyHandler" CACHE STRING "Tracker module name") # for UTS Doxygen generation
set(SAFETY_HANDLER_UTR_MODULE_NAME "SafetyHandler" CACHE STRING "Tracker module utr name") # for UTR processing, defined here to make sure the module name consistency
set(PROJECT_VERSION_DOCSTRING "${TRACKER_VERSION_MAJOR}.${TRACKER_VERSION_MINOR}.${TRACKER_VERSION_PATCH}" CACHE STRING "Version docstring")

# Find packages and programs
find_package(Doxygen)
find_package(PythonInterp)
find_program(LIBXSLT_XSLTPROC_EXECUTABLE xsltproc)
find_program(FOP_EXECUTABLE fop)

if(NOT DOXYGEN_FOUND)
  message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()

# UTS
if(EXISTS ${common_conf_dir}/Doxyfile-uts.in)
  set(doxyfile_in ${common_conf_dir}/Doxyfile-uts.in)
  set(uts_layout ${common_conf_dir}/uts-DoxygenLayout.xml)
  set(doxyfile  ${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/uts/Doxyfile)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/uts")
  set(DOXYGEN_INPUT "${CMAKE_CURRENT_SOURCE_DIR}")

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(uts_safetyhandler
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating UTS documentation with Doxygen"
    VERBATIM)

  add_custom_command(TARGET uts_safetyhandler POST_BUILD
    COMMAND make
    WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/uts
    COMMENT "Generating pdf version of UTS")

  add_custom_command(TARGET uts_safetyhandler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/uts/refman.pdf
    ${CMAKE_BINARY_DIR}/doc/SafetyHandler_UTS.pdf
    COMMENT "Rename output file")

else()
  message(WARNING "uts-Doxyfile.in is missing. UTS generation")
endif()

# UTR
if(EXISTS ${common_conf_dir}/junit2pdf-utr.xsl)

  configure_file(${common_conf_dir}/junit2pdf-utr.xsl  ${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/utr/junit2pdf-utr.xsl @ONLY)

  add_custom_target(utr_safetyhandler_junit
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/utr/junit-results
  )

  add_custom_command(TARGET utr_safetyhandler_junit POST_BUILD
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc/${SAFETY_HANDLER_UTR_MODULE_NAME}/utr/junit-results
    COMMAND $<TARGET_FILE:safety_handler-UT>
    -ojunit
    COMMAND ${CMAKE_COMMAND} -E remove -f cpputest_.xml
    && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake
    && ${LIBXSLT_XSLTPROC_EXECUTABLE}
    --output ../combined-results.xml
    ${common_conf_dir}/combine-junit.xsl junit-index.xml
  )

  add_custom_target(utr_safetyhandler
    COMMAND ${CMAKE_COMMAND} -E echo "UTR PDF generation moved to bash script."
    DEPENDS utr_safetyhandler_junit
  )

else()
  message(WARNING "Files are missing. Skipping UTR generation")
endif()
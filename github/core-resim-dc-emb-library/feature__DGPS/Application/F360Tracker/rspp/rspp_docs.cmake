# Set path to config directory
set(common_conf_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../DevOps/tools/ut-frameworks/common-configs)

# Determine component version
  file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/rspp_version.h
    RSPP_VERSIONS REGEX "RSPP_Version_")

  foreach(RSPP_VERSION ${RSPP_VERSIONS})
    if(${RSPP_VERSION} MATCHES "Major")
      string(REGEX REPLACE ".*= *([0-9]+).*" "\\1" RSPP_VERSIONS_MAJOR "${RSPP_VERSION}")
    elseif(${RSPP_VERSION} MATCHES "Minor")
      string(REGEX REPLACE ".*= *([0-9]+).*" "\\1" RSPP_VERSIONS_MINOR "${RSPP_VERSION}")
    elseif(${RSPP_VERSION} MATCHES "Patch")
      string(REGEX REPLACE ".*= *([0-9]+).*" "\\1" RSPP_VERSIONS_PATCH "${RSPP_VERSION}")
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

set (MODULE_INTEGRITY_LEVEL "ASIL-B")

set(TRACKER_MODULE_NAME "RSPP" CACHE STRING "Tracker module name")
set(PROJECT_VERSION_DOCSTRING "${RSPP_VERSIONS_MAJOR}.${RSPP_VERSIONS_MINOR}.${RSPP_VERSIONS_PATCH}" CACHE STRING "Version docstring")

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
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/uts/Doxyfile)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/uts")
  set(DOXYGEN_INPUT "${CMAKE_CURRENT_SOURCE_DIR}")

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(rspp-uts
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating UTS documentation with Doxygen"
    VERBATIM)

  add_custom_command(TARGET rspp-uts POST_BUILD
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/uts
    COMMENT "Generating pdf version of UTS")

  add_custom_command(TARGET rspp-uts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/uts/refman.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/doc/RSPP_UTS.pdf
    COMMENT "Rename output file")

else()
  message(WARNING "uts-Doxyfile.in is missing. UTS generation")
endif()

if(FALSE)
  # ITS
  if(EXISTS ${common_conf_dir}/Doxyfile-its.in)
    set(doxyfile_in ${common_conf_dir}/Doxyfile-its.in)
    set(layout ${common_conf_dir}/uts-DoxygenLayout.xml)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/its/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(its
      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating ITS documentation with Doxygen"
      VERBATIM)

    add_custom_command(TARGET its POST_BUILD
      COMMAND make
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/its
      COMMENT "Generating pdf version of ITS")

    add_custom_command(TARGET its POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E rename
      ${CMAKE_CURRENT_BINARY_DIR}/doc/its/refman.pdf
      ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_ITS.pdf
      COMMENT "Rename output file")

  else()
    message(WARNING "its-Doxyfile.in is missing. Skipping API documentation generation")
  endif()
endif()

# QTS
if(EXISTS ${common_conf_dir}/Doxyfile-qts.in)
  set(doxyfile_in ${common_conf_dir}/Doxyfile-qts.in)
  set(layout ${common_conf_dir}/uts-DoxygenLayout.xml)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/qts/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(rspp-qts
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating QTS documentation with Doxygen"
    VERBATIM)

  add_custom_command(TARGET rspp-qts POST_BUILD
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/qts
    COMMENT "Generating pdf version of QTS")

  add_custom_command(TARGET rspp-qts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${CMAKE_CURRENT_BINARY_DIR}/doc/qts/refman.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/doc/RSPP_QTS.pdf
    COMMENT "Rename output file")

else()
  message(WARNING "Doxyfile-qts.in is missing. QTS documentation generation")
endif()

# UTR
if(EXISTS ${common_conf_dir}/junit2pdf-utr.xsl)

  configure_file(${common_conf_dir}/junit2pdf-utr.xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/junit2pdf-utr.xsl @ONLY)

  add_custom_target(rspp-utr_junit
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/utr/junit-results
  )

  add_custom_command(TARGET rspp-utr_junit POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/${TRACKER_MODULE_NAME}/utr/junit-results
    COMMAND $<TARGET_FILE:rspp-UT> -o junit
    && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake

    && ${LIBXSLT_XSLTPROC_EXECUTABLE}
    --output ../combined-results.xml
    ${common_conf_dir}/combine-junit.xsl junit-index.xml
  )

  add_custom_target(rspp-utr
    DEPENDS rspp-qts rspp-qtr_junit
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr
    COMMAND ${CMAKE_COMMAND} -E echo "RSPP-QTR PDF generation moved to bash script."
    DEPENDS rspp-utr_junit
  )

else()
  message(WARNING "Files are missing. Skipping UTR generation")
endif()

if(FALSE)
  # ITR
  if(EXISTS ${common_conf_dir}/junit2pdf-itr.xsl)

    configure_file(${common_conf_dir}/junit2pdf-itr.xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit2pdf-itr.xsl @ONLY)

    add_custom_target(rspp-itr_junit
      COMMAND ${CMAKE_COMMAND} -E
      make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit-results
    )

    add_custom_command(TARGET rspp-itr_junit POST_BUILD
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit-results
      COMMAND $<TARGET_FILE:rspp-IT> -o junit
      && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake

      && ${LIBXSLT_XSLTPROC_EXECUTABLE}
      --output ../combined-results.xml
      ${common_conf_dir}/combine-junit.xsl junit-index.xml
    )

    add_custom_target(rspp-itr
      COMMAND ${FOP_EXECUTABLE}
      -xml ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/combined-results.xml
      -xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit2pdf-itr.xsl
      -pdf ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_ITR.pdf
      DEPENDS rspp-itr_junit
    )

  else()
    message(WARNING "Files are missing. Skipping ITR generation")
  endif()
endif()

# QTR
if(EXISTS ${common_conf_dir}/junit2pdf-qtr.xsl)

  configure_file(${common_conf_dir}/junit2pdf-qtr.xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit2pdf-qtr.xsl @ONLY)

  add_custom_target(rspp-qtr_junit
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit-results
  )

  add_custom_command(TARGET rspp-qtr_junit POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit-results
    COMMAND $<TARGET_FILE:rspp-QT> -o junit
    && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake

    && ${LIBXSLT_XSLTPROC_EXECUTABLE}
    --output ../combined-results.xml
    ${common_conf_dir}/combine-junit.xsl junit-index.xml
  )

  add_custom_target(rspp-qtr
    COMMAND ${CMAKE_COMMAND} -E echo "RSPP-QTR PDF generation moved to bash script."
    # COMMAND ${FOP_EXECUTABLE}
    # -xml ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/combined-results.xml
    # -xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit2pdf-qtr.xsl
    # -pdf ${CMAKE_CURRENT_BINARY_DIR}/doc/RSPP_QTR.pdf
    DEPENDS rspp-qtr_junit
  )

else()
  message(WARNING "Files are missing. Skipping QTR generation")
endif()

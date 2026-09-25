# Set path to config directory
set(common_conf_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/ut-frameworks/common-configs)

# Determine tracker version
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/SharedTrackerAPI/core/f360_tracker_version.h
  TRACKER_VERSIONS REGEX "Tracker_Version_")

foreach(TRACKER_VERSION ${TRACKER_VERSIONS})
  if(${TRACKER_VERSION} MATCHES "Major")
    string(REGEX MATCH "[0-9]+" TRACKER_VERSION_MAJOR ${TRACKER_VERSION})
  elseif(${TRACKER_VERSION} MATCHES "Minor")
    string(REGEX MATCH "[0-9]+" TRACKER_VERSION_MINOR ${TRACKER_VERSION})
  elseif(${TRACKER_VERSION} MATCHES "Patch")
    string(REGEX MATCH "[0-9]+" TRACKER_VERSION_PATCH ${TRACKER_VERSION})
  endif()
endforeach()

set(PROJECT_VERSION_DOCSTRING "Version ${TRACKER_VERSION_MAJOR}.${TRACKER_VERSION_MINOR}.${TRACKER_VERSION_PATCH}")

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
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/uts/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(uts
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating UTS documentation with Doxygen"
    VERBATIM)

  add_custom_command(TARGET uts POST_BUILD
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/uts
    COMMENT "Generating pdf version of UTS")

  add_custom_command(TARGET uts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${CMAKE_CURRENT_BINARY_DIR}/doc/uts/refman.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_UTS.pdf
    COMMENT "Rename output file")

else()
  message(WARNING "uts-Doxyfile.in is missing. UTS generation")
endif()

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

# QTS
if(EXISTS ${common_conf_dir}/Doxyfile-qts.in)
  set(doxyfile_in ${common_conf_dir}/Doxyfile-qts.in)
  set(layout ${common_conf_dir}/uts-DoxygenLayout.xml)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/qts/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(qts
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating QTS documentation with Doxygen"
    VERBATIM)

  add_custom_command(TARGET qts POST_BUILD
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/qts
    COMMENT "Generating pdf version of QTS")

  add_custom_command(TARGET qts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${CMAKE_CURRENT_BINARY_DIR}/doc/qts/refman.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_QTS.pdf
    COMMENT "Rename output file")

else()
  message(WARNING "Doxyfile-qts.in is missing. QTS documentation generation")
endif()

# UTR
if(EXISTS ${common_conf_dir}/junit2pdf-utr.xsl)

  configure_file(${common_conf_dir}/junit2pdf-utr.xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/utr/junit2pdf-utr.xsl @ONLY)

  add_custom_target(utr_junit
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc/utr/junit-results
  )

  add_custom_command(TARGET utr_junit POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/utr/junit-results
    COMMAND $<TARGET_FILE:F360-Tracker-UT> -o junit
    && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake

    && ${LIBXSLT_XSLTPROC_EXECUTABLE}
    --output ../combined-results.xml
    ${common_conf_dir}/combine-junit.xsl junit-index.xml
  )

  add_custom_target(utr
    COMMAND ${FOP_EXECUTABLE}
    -xml ${CMAKE_CURRENT_BINARY_DIR}/doc/utr/combined-results.xml
    -xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/utr/junit2pdf-utr.xsl
    -pdf ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_UTR.pdf
    DEPENDS utr_junit
  )

else()
  message(WARNING "Files are missing. Skipping UTR generation")
endif()

# ITR
if(EXISTS ${common_conf_dir}/junit2pdf-itr.xsl)

  configure_file(${common_conf_dir}/junit2pdf-itr.xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit2pdf-itr.xsl @ONLY)

  add_custom_target(itr_junit
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit-results
  )

  add_custom_command(TARGET itr_junit POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit-results
    COMMAND $<TARGET_FILE:F360-Tracker-IT> -o junit
    && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake

    && ${LIBXSLT_XSLTPROC_EXECUTABLE}
    --output ../combined-results.xml
    ${common_conf_dir}/combine-junit.xsl junit-index.xml
  )

  add_custom_target(itr
    COMMAND ${FOP_EXECUTABLE}
    -xml ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/combined-results.xml
    -xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/itr/junit2pdf-itr.xsl
    -pdf ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_ITR.pdf
    DEPENDS itr_junit
  )

else()
  message(WARNING "Files are missing. Skipping ITR generation")
endif()

# QTR
if(EXISTS ${common_conf_dir}/junit2pdf-qtr.xsl)

  configure_file(${common_conf_dir}/junit2pdf-qtr.xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit2pdf-qtr.xsl @ONLY)

  add_custom_target(qtr_junit
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit-results
  )

  add_custom_command(TARGET qtr_junit POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit-results
    COMMAND $<TARGET_FILE:F360-Tracker-QT> -o junit
    && ${CMAKE_COMMAND} -P ${common_conf_dir}/create_index.cmake

    && ${LIBXSLT_XSLTPROC_EXECUTABLE}
    --output ../combined-results.xml
    ${common_conf_dir}/combine-junit.xsl junit-index.xml
  )

  add_custom_target(qtr
    COMMAND ${FOP_EXECUTABLE}
    -xml ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/combined-results.xml
    -xsl ${CMAKE_CURRENT_BINARY_DIR}/doc/qtr/junit2pdf-qtr.xsl
    -pdf ${CMAKE_CURRENT_BINARY_DIR}/doc/F360-Tracker_QTR.pdf
    DEPENDS qtr_junit
  )

else()
  message(WARNING "Files are missing. Skipping QTR generation")
endif()

# Function: setup_unit_tests_report_target
# Description: setup unit tests report target that generates report in HTML format.
# Dependencies: xsltproc
function(setup_unit_tests_report_target)
    if(UNIX)    
        find_program(CMD_XSLTPROC xsltproc)
        if(CMD_XSLTPROC)
            add_report_generation_target(${CMD_XSLTPROC})
        else()
            message(WARNING "xsltproc is not found. Install it e.g. 'sudo apt install xsltproc'. Until that report generation is not supported.")
        endif()
    else()
        message(WARNING "UT report generation is only supported for UNIX.")
    endif()
endfunction()

# Function: add_report_generation_target
# Description: add target that generates UT report.
# Parameters: 
#    [in] xsltproc_command - command to the xsltproc program
function(add_report_generation_target xsltproc_command)
    set(CONVERSION_XSL_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/ctest_gtest_to_html.xsl)
    set(REPORT_XML_FILE     ${CMAKE_BINARY_DIR}/ut_report.xml)
    set(REPORT_HTML_FILE    ${CMAKE_BINARY_DIR}/ut_report.html)

    include(ProcessorCount)
    ProcessorCount(num_of_processors)

    add_custom_target(
        cmd_generate_ut_report
            COMMENT "Generating UT report"
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --parallel ${num_of_processors}
            COMMAND ctest --test-dir ${CMAKE_BINARY_DIR} --output-on-failure --parallel ${num_of_processors} --output-junit ${REPORT_XML_FILE} || echo "Continue report generation even if any test is failed"
            COMMAND ${xsltproc_command} ${CONVERSION_XSL_FILE} ${REPORT_XML_FILE} > ${REPORT_HTML_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "UT report: ${REPORT_HTML_FILE}"
            VERBATIM
    )
endfunction()
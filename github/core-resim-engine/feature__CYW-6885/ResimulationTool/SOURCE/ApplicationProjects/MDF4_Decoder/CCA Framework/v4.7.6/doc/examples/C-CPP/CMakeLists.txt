# Example file for the CCA-Framework-MDF: Demonstrates how to create a cmake file for projects using the CCA-Framework-MDF.
#
# @copyright            Copyright (c) 2017-2025 ViGEM GmbH. All rights reserved.
#
# @file                 CMakeLists.txt
#
# @date                 2025-03-13
# @version              4.7.6
# @license              This example code from ViGEM GmbH has been provided
# for example and illustration purposes only.
#
# NOTE: THIS CODE IS PROVIDED FROM VIGEM GMBH "AS IS", WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTY. VIGEM GMBH SHALL ONLY BE LIABLE WITHOUT LIMITATION UNDER THE
# GERMAN PRODUCT ACT (PRODUKTHAFTUNGSGESETZ). IN NO EVENT SHALL VIGEM GMBH BE
# LIABLE FOR ANY CLAIMS, DAMAGES OR OTHER LIABILITIES ARISING OUT OF THE USE OF
# THIS CODE, UNLESS VIGEM GMBH IS RESPONSIBLE FOR SUCH CLAIMS, DAMAGES OR OTHER
# LIABILITIES DUE TO INTENT OR GROSS NEGLIGENCE ON ITS PART.
#
# The rights of use the code are based on the terms of:
#   - the contract between the user and ViGEM GmbH,
#   - the General Terms and Conditions for Software of ViGEM GmbH.
#     NOTE: SECTION 6.2 OF THE GENERAL TERMS AND CONDITIONS FOR SOFTWARE OF
#           VIGEM GMBH DOES NOT APPLY FOR CODES PROVIDED FROM VIGEM GMBH FOR
#           EXAMPLE AND ILLUSTRATION PURPOSES.
# In addition shall apply supplementary:
#   - the General Terms and Conditions of ViGEM GmbH
#
# These additional applicable provisions can be sent to you by postal mail on
# request and can be viewed as electronic documents on our homepage and our
# customer portal at www.ViGEM.de.

cmake_minimum_required(VERSION 3.0)
project(cca-framework-examples C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../")
find_package(CCAFrameworkMDF REQUIRED)

macro(cpp_example name)
    add_executable(${name} ${name}/${name}.cpp)
    target_link_libraries(${name} CCAFrameworkMDF)
endmacro()

cpp_example(cca-convert)
cpp_example(cca-device)
cpp_example(cca-device-fw-update)
cpp_example(cca-download)
cpp_example(cca-readfile)
cpp_example(cca-session)
cpp_example(cca-state)
cpp_example(cca-stream)
cpp_example(cca-xcpmessage)
cpp_example(cca-can_message)
cpp_example(cca-mipi)
cpp_example(cca-property)
cpp_example(cca-datasync)
cpp_example(cca-getconfiguration)
cpp_example(cca-setconfiguration)
cpp_example(cca-events)
cpp_example(cca-sorter)
cpp_example(cca-ipcamInfo)
cpp_example(cca-jumpToTimestamp)
cpp_example(cca-IndexingOldFile)
cpp_example(cca-indexedFileWriter)
cpp_example(cca-camera_frame_indexing)
cpp_example(cca-start_stop_recording)
cpp_example(cca-set_current_time)

add_executable(cca-device-c cca-device-c/cca-device.c)
target_link_libraries(cca-device-c CCAFrameworkMDF)

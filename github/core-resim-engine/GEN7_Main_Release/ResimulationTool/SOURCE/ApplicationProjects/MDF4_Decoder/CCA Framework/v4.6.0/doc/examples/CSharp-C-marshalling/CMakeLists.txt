# Example file for the CCA-Framework-MDF: Demonstrates how to create a cmake file for C# projects under Windows using the CCA-Framework-MDF.
#
# @copyright            Copyright (c) 2017-2024 ViGEM GmbH. All rights reserved.
#
# @file                 CMakeLists.txt
#
# @date                 2024-09-05
# @version              4.6.0
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
project(cca-framework-examples CXX CSharp)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../")
find_package(CCAFrameworkMDF REQUIRED)

macro(csharp_example name)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${name}/CcaLib.cs.in ${CMAKE_CURRENT_BINARY_DIR}/${name}/CcaLib.cs)
    add_executable(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name}/CcaLib.cs ${CMAKE_CURRENT_SOURCE_DIR}/${name}/Program.cs)
    target_compile_options(${name} PUBLIC "/unsafe") # necessary to work with memory pointer returned by the C API
endmacro()

csharp_example(cca-interop-file)
csharp_example(cca-interop-devicestate)

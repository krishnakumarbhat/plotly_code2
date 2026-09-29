@mainpage Main Page
@tableofcontents

@section overview Overview

If you are tasked with writing code for the customer adapter of a Side Radar Feature (SRF) we strongly encourage you
to take a few minutes for going through the best practices guide and getting familiar with the current coding guidelines and best practices.

@subsection c_guideline Coding Guideline (for C)
Please take a look at the APTIV Coding Standard for detailed information.\n 
[Link to Coding Standards](http://sdt52.usinkok.northamerica.delphiauto.net/wiki/index.php/GSCP:DesignStandards#Coding_Standards)

@subsection review_checklist Coding Review Checklist
Please also take a look at the coding review checklist as it contains most of the software quality checks for this project.\n 
[Link to Coding Review Checklist](http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=wiki-view&name=SWE4_Code_Review_Checklist&pid=4211.9414)

@section naming_convention Naming Convention
- The naming convention for the SRF Codebase does not deviate from the conventions defined in the APTIV Coding Standard. Please refer to chapter 4.4 in @ref c_guideline to see how to name variables, functions, etc.

- File names should be all lowercase and delimited with underscores. The feature abbreviation should be the prefix for every file (e.g. `ta_object_filter.c` or `ced_factory.h`).
Files that mostly define a new type should have the `*_t.h` suffix in the file name. Every file should include a description and copywrite notice and header files need an include-guard.
@code
#ifndef FILE_NAME_H
#define FILE_NAME_H

/**
 * @file file_name.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Brief description of file content goes here.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */
@endcode \n 

- Boolean-type variables should start with a `f_` prefix (for flag) and Boolean-type functions should be named like a question that can be answered true/false or yes/no, like `Is_Object_Behind_Guardrail()`.

@section code_formatting Code Formatting
- For code style or formatting the SRF Codebase also follows the Coding Standards closely. You can find more information in chapter 4.5. in @ref c_guideline

- Some of the style rules can be applied automatically by using clang-format and the SRF provided [clang-format file](/Plastic/SRFSCRUM_Side_Radar_Features/.clang-format) \n 
Visual Studio 2017 or later will auto-detect this file in a project’s file path.

@section general_best General Best Practices
- Avoid the use of magic numbers or keep it to a minimum. Please create defines or enums for your constant values. The defined values have to be in parentheses, like  `#define TEST_VALUE (123)`.
- Please comment your code and use `/* comment */`-style line comments.

@section doxygen_convention Doxygen Conventions
In this project the convention for doxygen is to always include 
- a brief description
- the return value
- a reference to the related SRD, SAD and SDD requirements
- the verification criteria for the corresponding SDD requirement

Additionally the parameters should be described in-line.
@code
/**
 * @brief This function calls the core algorithm
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
void Feature_Core_Run(Feature_Core_Output_T *p_feature_core_output /**< Feature Core Output */,
                      const Feature_Core_Input_T *p_feature_core_input /**< Feature Core Input */,
                      const Feature_Calibration_T *p_feature_cal /**< Feature Calibration */);
@endcode \n 

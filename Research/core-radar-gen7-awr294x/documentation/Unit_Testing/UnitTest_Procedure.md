# ALL ABOUT UNIT TEST PROCEDURE
-------------------------
# Table of contents
- [ALL ABOUT UNIT TEST PROCEDURE](#all-about-unit-test-procedure)
- [Table of contents](#table-of-contents)
  - [1. Introduction](#1-introduction)
  - [2. Tools Required](#2-tools-required)
  - [3. UT Framework](#3-ut-framework)
      - [3.1. Creation of New Test Files](#31-creation-of-new-test-files)
      - [3.2. Format of the stub functions](#32-format-of-the-stub-functions)
      - [3.3. Bazel dependencies](#33-bazel-dependencies)
      - [3.4.Test Case Creation](#34test-case-creation)
      - [3.5. Method of invoking and running the tests](#35-method-of-invoking-and-running-the-tests)
      - [3.6. Coverage Reports](#36-coverage-reports)
- [4. Debugging Testcase](#4-debugging-testcase)
  - [4.1. configuring VS code for debugging](#41-configuring-vs-code-for-debugging)
  - [4.2. Automating build process before debugging](#42-automating-build-process-before-debugging)
- [5. File Revision History](#5-file-revision-history)
-------------------------
## 1. Introduction
Unit testing is a software testing method by which individual units of source code or components of a software are tested. It is done during the development(Coding phase) of an application by the developers.

• It helps to fix bugs early in the development cycle and save costs.

• It helps the developers to understand the testing code base and enables them to make changes quickly.

• It serves as project documentation.

• It helps with code re-use. Migrate  both your code and your tests to your new projects. Tweak the code until the tests run again

-------------------------
## 2. Tools Required
The below tools are required for unit test to comapare the code changes, to build, invoke and for generating the reports.

• Visual Studio code
• Bazel
• Gcovr
• Beyond compare

-------------------------
## 3. UT Framework
To understand the existing UT framework, it is important to understand about the below points.

• Creation of New Test Files

• Format of the stub functions

• Bazel dependencies

• Method of invoking and running the tests

• Coverage Reports

-------------------------
#### 3.1. Creation of New Test Files
For example the module path given to perform UT is as follows :
**ADVRADAR_AWR294X\software\app\mss\sensor_pos**
The module which needs to be tested is **sensor_position.c** as shown in the below picture.

![NewTestFolderCreation](NewTestFolderCreation.PNG)

Following the above path which is mentioned, create the test folder and it should contain the below files. Inside the test folder, the name of the files can be given as <file_name>_fake

• dd_sensor_pos_fake.cc
• dd_sensor_pos_fake.h
• dd_sensor_pos_unit_test.cc
• BUILD

![NewTestFilesCreation](NewTestFilesCreation.PNG)

-------------------------
#### 3.2. Format of the stub functions

After the creation of new test files, The file **dd_sensor_pos_fake.cc** consists of definition of the fake/stub function as shown below.

The syntax will be **DEFINE_FAKE_VALUE_FUNC(Return type, Function name, Data types of the Function parameters)**. In the below picture, the syntax for definition of the  different types of stub/fake functions are mentioned below.

• Function which has only return type with no arguments.

• Functions which has only arguments with void return type.

• Functions with no return type and no arguments.

• Functions which has return type and has arguments.

![Defining ' Stubs](Defining_Stubs.PNG)

The file **dd_sensor_pos_fake.h** includes the declaration of fake/stub functions as shown in below picture.
The syntax will be **DECLARE_FAKE_VALUE_FUNC (Return type, Function name, Data types of the Function parameters)**. Also include all the header file dependencies in this file.
The **dd_sensor_pos_unit_test.cc** is a file or script where test cases can be written for each functions, which is implemented in source code sensor_position.c.

![Declaring ' Stubs](Declaring_Stubs.PNG)

-------------------------
#### 3.3. Bazel dependencies
In the below picture, the Build file which includes all the dependencies path of source file i,e sensor_position.c.

![BuildFile](BuildFile.PNG)

**cc_library**

All header files that are used in the build must be declared in the hdrs or srcs of cc_* rules.
The syntax for creating the cc_library along with all the arguments as mentioned below.

**cc_library(name, deps, srcs, data, hdrs, alwayslink, compatible_with, copts, defines, deprecation, distribs, exec_compatible_with, exec_properties, features, implementation_deps, include_prefix, includes, licenses, linkopts, linkstamp, linkstatic, local_defines, nocopts, restricted_to, strip_include_prefix, tags, target_compatible_with, testonly, textual_hdrs, toolchains, visibility, win_def_file)**

For cc_library rules, headers in hdrs comprise the public interface of the library and can be directly included both from the files in hdrs and srcs of the library itself as well as from files in hdrs and srcs of cc_* rules that list the library in their deps.

Headers in srcs must only be directly included from the files in hdrs and srcs of the library itself. The inclusion checking rules only apply to direct inclusions.Technically, the compilation of a .cc file may transitively include any header file in the hdrs or srcs in any cc_library in the transitive deps closure. It should not include indirectly dependent header files in hdrs or scrs in any cc_library. This is roughly the same decision as between public and private visibility in programming languages.

The below picture shows clearly how the dependencies module paths should be added in cc_library for unit test.

![CClibrary_Dependencies](CClibrary_Dependencies.PNG)

**cc_test**

 cc_test rules do not have an exported interface, so they also do not have a hdrs attribute. All headers that belong to the binary or test directly should be listed in the srcs.

The syntax for creating the cc_test along with all the arguments as mentioned below.

**cc_test(name, deps, srcs, data, additional_linker_inputs, args, compatible_with, copts, defines, deprecation, distribs, env, env_inherit, exec_compatible_with, exec_properties, features, flaky, includes, licenses, linkopts, linkstatic, local, local_defines, malloc, nocopts, restricted_to, shard_count, size, stamp, tags, target_compatible_with, testonly, timeout, toolchains, visibility, win_def_file)**

In the above syntax, the arguments of cc_test which includes the below attributes.

**name** : It is required and unique name for this target.

**deps** : Here the list of other libraries to be linked in to the binary targets and these can be cc_library or objc_library targets.

**scrs** : The list of C and C++ files that are processed to create the target. These are C/C++ source and header files, either non-generated (source code) or generated. All .cc, .c, and .cpp files will be compiled. These might be generated files. A .h file will not be compiled, but will be available for inclusion by sources in this rule.

**includes** : List of include dirs to be added to the compile line. Headers must be added to srcs or hdrs, otherwise they will not be available to dependent rules when compilation is sandboxed (the default).

**linkstatic** : For cc_binary and cc_test: link the binary in static mode. For cc_library.linkstatic,
by default this option is **on** for cc_binary and **off** for the rest.

The linkstatic attribute has a different meaning if used on a cc_library() rule. For a C++ library, linkstatic=True indicates that only static linking is allowed. linkstatic=False does not prevent static libraries from being created. The attribute is meant to control the creation of dynamic libraries.

The below picture shows clearly how the dependencies module paths should be added in cc_test for unit test.

![CCtest_Dependencies](CCtest_Dependencies.PNG)

 For more information we can refer the link **https://bazel.build/reference/be/c-cpp#cc_test**.

-------------------------
#### 3.4.Test Case Creation
The file **dd_sensor_pos_unit_test.cc** is a script where test cases can be written.
This file should include the #include<gtest/gtest.h> and the unit test header file **#include “fff.h”**.
Creating the Test suite definition is shown below in the picture :

![CreationOfFixtureClass](CreationOfFixtureClass.PNG)

Here **Setup()** and **Teardown()** functions are called for each test case or for whole testsuit. Googletest does not reuse the same test fixture object across multiple tests. For each TEST_F, googletest will create a fresh test fixture object, immediately call SetUp(), run the test body, call TearDown(), and then delete the test fixture object.

**Test Fixtures:** Using the Same Data Configuration for Multiple Tests
For example the function **Read_Sensor_Position_From_Pins** need to be tested as show below.

![FunctionForTest](FunctionForTest.PNG)

After analysis of the function implementation, the TEST_F can be written as show in the picture below.

![TestCaseforFunction](TestCaseforFunction.PNG)

In TEST_F, the first parameter will be the class which you have created above and second parameter will be the suitable name of test case what functionality does.

-------------------------
#### 3.5. Method of invoking and running the tests

The tests are invoked through these commands :

• Saves the state of all googletest flags.

• Creates a test fixture object for the first test.

• Initializes it via SetUp().

• Runs the test on the fixture object.

• Cleans up the fixture via TearDown().

• Deletes the fixture.

• Restores the state of all googletest flags.

• Repeats the above steps for the next test, until all tests have run.

To run the Test cases for particular module which needs to be tested, the following commands need to be run.

In general :
**bazel test --test_output=all //:mingw_tests**

For the module sensor_position.c :
**bazel test --test_output=all //software/app/mss/sensor_pos/test:unit_tests**

The above command will change according to the module path and If we provide the correct path of the module which we are testing, it will build all the test cases with pass or fail result as show below.

![RunningTestCases](RunningTestCases.PNG)

The above picture shows the number of test cases passed, If in case any interruption occurred while running test cases, it will throw an error with failed test case.

Once all the Test cases are passed then coverage reports can be generated by running the below command.

In general :
**bazel run //:coverage**

For the module sensor_position.c :
**bazel run //software/app/mss/sensor_pos/test:coverage**

![GeneratingCovReports](GeneratingCovReports.PNG)

After running the command copy the path and see the coverage.html in any browser to check the number of functions, branches and MCDC conditions which are covered with test cases written.

-------------------------
#### 3.6. Coverage Reports

The below picture shows the coverage reports and number of Functions which covered. It also shows the number of lines and branches covered along with the boolean effectiveness.

![Coverage](Coverage.PNG)

In the below picture, It can be seen that lines of code covered and which are the branches covered along with uncovered lines in detail.

![CoveredLines](CoveredLines.PNG)

# 4. Debugging Testcase
## 4.1. configuring VS code for debugging
1. While building UT, add debug flag in command line as shown in below command.
**bazel test //software/app/common/calibrations/test:unit_tests  --copt="-g" --copt="-O0"**
**'-g'**: enables generating debug symbols along with executable.
**'-O0'**: disable all compiler optimization.
2. In VS code in left panel open run and debug (press ctrl+shift+D) click on create launch.json file
![run_and_debug](run_and_debug.png)
3. Add following lines of code in json file
```
{
    "version": "0.0.1",
    "configurations": [
        {
            "name": "Unit_Tests",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/PATH/TO/EXECUTABLE",
            "stopAtEntry": false,
            "MIMode": "gdb",
            "cwd" : "${workspaceFolder}",
            "miDebuggerPath": "${workspaceFolder}/bazel-advradar_awr294x/external/mingw64_10_0_0_rev0/bin/gdb.exe"
        },
    ],
}
```
```"program": "${workspaceFolder}/PATH/TO/EXECUTABLE"``` is relative path from project repository to executable generated after building test.
```"miDebuggerPath": "${workspaceFolder}/bazel-advradar_awr294x/external/mingw64_10_0_0_rev0/bin/gdb.exe"``` is the path to gdb.exe from mingw. (this folder becomes available only after building unit test via bazel)

![build_result](Build_results.png)
4. To debug the test, first build and make sure the path to executable is correct.
5. Add breakpoints to lines in vs code
6. In run and debug panel click on play button to start debugging (press F5). this will start debugging session
![debugging_view](debugging_view.png)

## 4.2. Automating build process before debugging
we can configure VS code to build our unit test before running debugger, these steps are optional but makes debugging easier as it will automatically build out UT before stating debugger. we need to create a task to build the UT.
1. In down menu bar select *terminal* then *Configure tasks*
2. Select option *create task.json from template* then select option *other*, it will create a task.json file in .vscode folder
3. Copy following code into file

{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Unit_test",
            "type": "shell",
            "command": "bazel test //software/app/common/calibrations/test:unit_tests  --copt='-g' --copt='-O0'",
        }
    ]
}

where
```"command": "bazel test //software/app/common/calibrations/test:unit_tests  --copt='-g' --copt='-O0'",``` is the command which will compile our UT. this command will run before everytime debugger is invoked.
1. In launch.json file add following line in our configuration.
```"preLaunchTask": "Unit_test",``` this will cause build command to run before invoking debugger.
```
{
    "version": "0.0.1",
    "configurations": [
        {
            "name": "Unit_Tests",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/bazel-bin/software/app/mss/mmic/test/dd_mmic_unit_test.exe",
            "stopAtEntry": false,
            "MIMode": "gdb",
            "cwd" : "${workspaceFolder}",
            "miDebuggerPath": "${workspaceFolder}/bazel-advradar_awr294x/external/mingw64_10_0_0_rev0/bin/gdb.exe",
            "preLaunchTask": "Unit_test",
        },
    ],
}
```
# 5. File Revision History
|Rev|Date|NetId|Name|SCR|
|-|-|-|-|-|
|0.1|20-Dec-2022| h0fx3n| Suma Patil| DDR-1904|
|0.2|15-Mar-2023| cpdhup| Shubham Adgaonkar| DDR-1994|

# Gen8 iND13400 Repository

[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

This repository stores the Gen8 code for the indie iND13400 microcontroller

[TOC]

## First Steps (repo_init.py)

### Overview

A script called repo_init.py is stored at the base of this repository.
It should be called each time this repository is cloned.

### What it does

 1. Verifies a compliant version of Python is used & downloads required python packages
 2. Installs required pre-commit hooks for this repository. These are verified via CI
 3. .netrc - creates/updates a .netrc with required credentials for building the project (See [.netrc Credentials](#.netrc-credentials))

### Requirements

Python v3.7 or greater should be supported.  v3.10 is recommended since this is what the scripts are tested with.

### .netrc Credentials

Credentials to various tools are required as part of the build and setup processes. The .netrc file is used to provide those credentials to those processes.

repo_init.py *may* ask you to provide your username and API Key / password to populate the .netrc file. This file is stored locally in your machine's Home folder.

It is recommended to use an API key instead of your password otherwise your raw password will be stored in the .netrc file on this machine.
API keys can be generated from the Web GUIs of each individual tool.
See the [Adv Active Safety SW/SYS Git Gerrit Wiki](https://tinyurl.com/advSysSwGitGerritWikiApi) for instructions on generating API keys for the various tools.
**You will also need to run this script whenever your password updates (Once per user per machine) unless you use API keys**

### How to Run

To run the script, open a command prompt and run the following from the root of the repository:

    python repo_init.py


## Committing changes to the Gerrit Repository

Our Gerrit repository is guarded against changes that do not pass a set of Jenkins checks.
You will not be able to submit your changes if any of them fail.
Jenkins will write a comment back to Gerrit when any of them fail.
It is your job to read this comment and take action based on it.
You can also utilize the Jenkins logs via the link Jenkins writes in the comment for further analysis and debug.
If you feel that Jenkins failed due to an unknown error or you need help deciphering the Jenkins logs, please contact the CICD team.

Below are the different Jenkins checks that are run as part of our CICD setup.

### Verified
This checks that changes that follow the formatting standards identified by a team of your peers, guided by Aptiv's coding standards.
When commiting your changes, a set of scripts will run (installed by running repo_init.py above) which should automatically format your code, and fix/flag some potential problems.
These checks will then be run again as part of the "Verified" Jenkins job to ensure that you have the pre-commit checks in place.
This is to remove the unnecessary burden on developers to format their code in a standard way, and to make sure all our code is fomratted in the same method.

If your change fails the "Verified" check, there are 2 potential issues:

1. Your code did not pass all the pre-commit checks
2. An unknown error occured and the CICD team needs to check what happened

If it is one of the first option, it is your job to fix the issues, but a comment written back to your Gerrit review by Jenkins will help.
If you have code that you think should *not* be required to follow the autoformatting standards, please reach out to the CICD team.
They can determine if it is a valid request and assist in excluding the files.

###  Build
This verifies that the code is buildable for the default build configuration for *each* variant.
If your change fails the "Build" check, there are 2 potential issues:

1. Your code did not build for one of the variants
2. An unknown error occured and the CICD team needs to check what happened

If it is one of the first option, it is your job to fix the issues. The Jenkins build logs can assist you in this process and a comment will be written back to your Gerrit review with a link to the job for your review.

### Coverity
This verifies that the code is static analysis compliant for the default build configuration for *each* variant.
Core Radar has specified a set of rules to check against (Coverity High & Medium, MISRA Mandatory & Required).
If your C code is not compliant, you will get a -1 Coverity vote, and will need to fix the issues written back to Gerrit in the comments.

If your change fails the "Coverity" check, there are 3 potential issues:

1. Your code did not build for one of the variants
2. Your code does not meet the static analysis standards
3. An unknown error occured and the CICD team needs to check what happened

If it is one of the first or second option, it is your job to fix the issues. The Jenkins build logs can assist you in this process and a comment will be written back to your Gerrit review with a link to the job for your review.

### Unit Test
This verifies that all unit tests compile and pass.

If your change fails the "Unit-Test" check, there are 2 potential issues:

1. Some of the unit tests are either not passing or not compiling successfully
2. An unknown error occured and the CICD team needs to check what happened

If it is the first option, it is your job to fix the issues. The Jenkins build logs can assist you in this process and a comment will be written back to your Gerrit review with a link to the job for your review.

### MCDC Coverage
This verifies that the project coverage requirements are met via unit testing.

If your change fails the "MCDC-Coverage" check, there are 3 potential issues:

1. Some of the unit tests are either not passing or not compiling successfully. The Unit-Test flag should also fail
2. The Coverage requirements are not met
3. An unknown error occured and the CICD team needs to check what happened

If it is one of the first or second option, it is your job to fix the issues. The Jenkins build logs can assist you in this process and a comment will be written back to your Gerrit review with a link to the job for your review.

### Smoke Test
This verifies that some automated runtime tests pass as expected.

If your change fails the "Smoke-Test" check, there are 2 potential issues:

1. Some of the smoke tests are failing.
2. An unknown error occured and the CICD team needs to check what happened

If it is the first option, it is your job to fix the issues. The Jenkins build logs can assist you in this process and a comment will be written back to your Gerrit review with a link to the job for your review.


## Building the Gen8 code
### Bazel
Bazel is used to build the code. Bazelisk is a wrapper around Bazel that ensures we all use the same Bazel version.
Bazel is a very powerful build tool and has many functionalities that may benefit the software development process (including dependency maps, build trees, etc.). More information can be found at https://bazel.build.

A detailed live demonstration was recorded on Bazel. You can find the link to the video at https://web.microsoftstream.com/embed/channel/04ec5a53-afff-4221-ba76-0a7d0dd50ed6?app=microsoftteams&sort=undefined&l=en-us#

If you do not have access to this link, request access to the Adv Active Safety SW/SYS Team in Microsoft Teams.

### Building the Code
The following steps can be used to build the Gen 8 code.
All commands should be run from the root of the repository.

    Gen8:
    bazelisk build //:gen8
    bazelisk build //:gen8 --override_repository=spbb="absolute\path\in\local\machine"

    bazelisk build //:gen8 --config=use_test_pattern_gen  (run this with MemAccess in Trace32 set to Denied and Disabled all Break points)

    //To test the IDM and DC Compensation , build with adc loggin capability and mars pattern generator.
    //Enable idm and disable IDM from mipi_ifc.c file and run this build....and check for dc offset gone and IDM stats
    bazelisk build //:gen8 --@spbb_cfg//:enable_adc_logging=True --config=configure_MARS_test_pattern --override_repository=spbb="C:\Users\mjc2z6\git_repos\g8_bb_clean_nextCI\Core_Radar_Gen8_iND13400_Signal_Processing"

    //To test the Range processing with Test pattern generator so that you can plot the
    //logged ADC data build with this command:
    bazelisk build //:gen8 --config=use_test_pattern_gen --@spbb_cfg//:enable_adc_logging=True
    (run this with MemAccess in Trace32 set to Denied and Disabled all Break points)
    //Then change the xcp_log_chirp_num to anything between 0-1019
    //Var.set XCP_AdcData_Buffer.log_adc_command.xcp_log_chirp_num = 1019
    //Var.draw XCP_AdcData_Buffer.xcp_adc_data_log

### Conditional Builds
Our builds have optional conditional builds to enable/disable certain parts of the code.
While production intent configurations should be done with the variant flags, these other conditional builds are used for debugging/testing.
The following conditional build flags are supported. They can be added to the command line to build with them.

Short and long form of conditional builds are defiend in .bazelrc as flag_alias configurations.
Only one form of the flag needs to be defined on the command line when building.

#### FPGA vs ASIC configuration
Used to set the option for FPGA or ASIC configurations. The default selection is for the ASIC configuration. This option is specifically added for the BBE configuration.

```
--@build_config//:asic_fpga=fpga
--@build_config//:asic_fpga=asic
--bbe_asic_fpga=fpga
--bbe_asic_fpga=asic
```

#### Enable XCP ADC logging
Enable/disable the ADC logging of a chirp of data to system memory for XCP consumption. The default selection is disabled.

```
--@spbb_cfg//:enable_adc_logging=True
--@spbb_cfg//:enable_adc_logging=False
```

#### Enable timing output for chirps
Enable/disable the logging of the timing related to the chirp processing. Includes the first chirp timing, the max of the intermediate chirps, and the last chirp timing information.

```
--//software/bbe32/src:enable_chirp_timing=True
--//software/bbe32/src:enable_chirp_timing=False

--enable_chirp_profiling=True
--enable_chirp_profiling=False
```

### Stack Monitering (Stack Monitor for R52)
    --disable_stack_monitor_r52
    --//software/r52/r52_stack:disable_stack_monitor_r52
Disable Run Time stack monitering. By default Stack monitering is ENABLED.


## Building/Running Unit Tests using Google Test Framework

### Running Unit Tests

   The following steps can be used to build the code for unit tests.

   From the root directory of the repository, run:

    bazelisk test //:all_unit_tests --test_output=all

   This will build and run ALL unit tests listed in the all_unit_tests test_suite

    bazelisk test //:tests_bbe --config=bbe_ut
   This will build and run ALL unit tests listed in tests_bbe test_suite

   It is also possible to target single units for testing:

    bazelisk test //software/app/common/test:version_unit_test --test_output=all

   Bazel is a very good incremental build tool, therefore cleaning should not be necessary.
   However, if you wish to clean out the build cache, you can do so by running:

    bazelisk clean

### Generating Coverage Reports

   The following steps can be used to generate the coverage report.  It is not required to run the tests before generating coverage.  Any of these commands will tell you where they put their build output in the console log.

   From the root directory of the repository, run:

    bazelisk build //coverage:report

   This will create a report for ALL unit tests listed in the coverage_summary build target name 'coverage'.  It is also possible to generate coverage for individual targets:

    bazelisk build //coverage:versions

   Check the `coverage/BUILD` file for the list of all coverage targets, and the respective thresholds for passing in CI.


## Building compile_commands.json for TiCS analysis

   The following step can be used to generate the compile_commands.json to the main workspace. TiCS expectes this file to be there during the analysis.

   From the root directory of the repository, run:

    bazel run //:compiledb

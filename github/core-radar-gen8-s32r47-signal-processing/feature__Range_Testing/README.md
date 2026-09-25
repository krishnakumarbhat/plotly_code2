# Advanced Engineering - Gen 8 FLR8HD Signal Processing Repository
This repository contains the signal processing building block source code for Gen 8 FLR8HD (NXP based).

These building blocks are targeted for Gen 8 FLR8HD and PCRESIM. The blocks here
should be reconfigurable for the embedded usage and generic C code.

## First Steps (repo_init.py)
A script called repo_init.py is stored at the base of this repository.
It should be called each time this repository is run to download/install repo specific tools as well as ensure that some setup steps are completed.

Python v3.7 or greater should be supported.  v3.10 is recommended since this is what the scripts are tested with.

This script *may* ask you to provide your username and API Key / password to populate a .netrc file. This file is stored locally in your machine's Home folder.

It is recommended to use an API key instead of your password otherwise your raw password will be stored in the .netrc file on this machine.
API keys can be generated from the Web GUIs of each individual tool.
See the Adv Active Safety SW/SYS Git Gerrit Wiki (https://tinyurl.com/advSysSwGitGerritWikiApi) for instructions on generating API keys for the various tools.

**You will also need to run this script whenever your password updates (Once per user per machine) unless you use API keys**

To run the script, open a command prompt and run:

   python repo_init.py

## Bazel
Bazel is used to build the code. Bazelisk is a wrapper around Bazel that ensures we all use the same Bazel version.
Bazel is a very powerful build tool and has many functionalities that may benefit the software development process (including dependency maps, build trees, etc.). More information can be found at https://bazel.build.

A detailed live demonstration was recorded on Bazel. You can find the link to the video at https://web.microsoftstream.com/embed/channel/04ec5a53-afff-4221-ba76-0a7d0dd50ed6?app=microsoftteams&sort=undefined&l=en-us#

If you do not have access to this link, request access to the Adv Active Safety SW/SYS Team in Microsoft Teams.

Bazel is a very good incremental build tool, therefore cleaning should not be necessary.
However, if you wish to clean out the build cache, you can do so by running:

   ```
   bazelisk clean
   ```
# Building the Code
*TBD - NEEDS UPDATED*
The integration of the building blocks into an application is TBD.

However, all the building blocks can be built for unit testing.
Some of the building blocks will also implement testing on the hardware utilizing the Unity framework. This testing is for development purposes only, and will not be used to validate coverage of tests!

## Build options
There are switches that can be used to test different configurations:

1. --//tools/bazel/toolchains/bbe32:bbe_optimization (or alias --bbe_opt) can be used to select the optimization level for the BBE.

```
--//tools/bazel/toolchains/bbe32:bbe_optimization=O0
--//tools/bazel/toolchains/bbe32:bbe_optimization=O1
--//tools/bazel/toolchains/bbe32:bbe_optimization=O2
--//tools/bazel/toolchains/bbe32:bbe_optimization=O3
--bbe_opt=O0
--bbe_opt=O1
--bbe_opt=O2
--bbe_opt=O3
```

## Conditional Build Arguments
*TBD - NOT YET IMPLEMENTED*

# Example Simulation Application
As a means to provide a common entry point, an example test simulation app is provided. This allows simulation of underlying BB code using the Xtensa provided ISS (Instruction Set Simulator). The simulation can also be used to enable output of profile information. See the examples in the internal/bbe32 BUILD file for the use of the "run_profiler" tag to enable the output of the profile information.

To run the example application in the simulator, use one of the following build examples.

   Example:
   bazelisk build //internal/bbe32:example_test_app_sim_show_summary
   bazelisk build //internal/bbe32:example_test_app_sim

# Example Embedded Application using the Diagnostic Tool framework on the EVB
As a means to provide a common entry point, an example test embedded app is provided for the EVB. This allows testing of underlying BB code using the NXP-provided Diagnostics Tool framework on the EVB.

To run the example application on the NXP EVB, use the following build example, and follow the notes in the matching BUILD file.

   Example:
   bazelisk build //internal/evb_diag_tool/examples/base:example_base_test
   bazelisk build //internal/evb_diag_tool/examples/testtype_profile_spt:example_testtype_profile_spt
   bazelisk build //internal/evb_diag_tool/examples/testtype_profile:example_testtype_profile

## Module Specific Tests

The following profiling tests are provided for running on the EVB:
bazelisk build //modules/range/test/evb_diag_tool_profile_spt:range_profile_spt
bazelisk build //modules/doppler/test/evb_diag_tool_profile_spt:doppler_profile_spt

To BUILD Range Module:
For SINGLE SPT: bazelisk.exe build -c dbg //modules/range/imp/src/nxp:Range_SPT_Single
For DUAL SPT: bazelisk.exe build -c dbg //modules/range/imp/src/nxp:Range_SPT

*TBD - MORE TO BE IMPLEMENTED*

The example application can be used as a reference to create simulations for individual modules. The module should provide the implementation for the test in the following interface:
   int bbe_test_entry_point()
   {
      return 0;
   }
For First pass test use:
    bazelisk build //modules/first_pass/test/unit_tests:fp_example_test_app_sim_show_summary --verbose_failures

## Building and testing using the Unity framework
   *TBD - NOT YET IMPLEMENTED*
   Use of the Unity test framework allows building a set of tests that can be loaded onto the DSP and executed. This setup requires a console output to get readable test results. The implementation is currently only confirmed to work on the AWR2944 evaluation kit with the CCS programming enabled. Eventually, this framework will allow running on the APTIV hardware as well, but this is TBD.

   The outputs of the build commands here can be loaded into the DSP and executed. The test assertion outputs are output to the CIO interface, which is displayed in the console of CCS.

   ### TBD - Add Modules Here If Unity is implemented

## Building/Running Unit Tests using Google Test Framework
   *TBD - NOT YET IMPLEMENTED*

### Adding unit tests
   *TBD - NOT YET IMPLEMENTED*

   When adding unit tests, add a specific test using the cc_test option.

   For each module, there should also be a test_suite  added, to simplify calling and running ALL unit tests for the module and adding the results to a coverage test output.

   After adding the test_suite to a specific module and getting passing results for all unit tests, then add the module to the test suite at the root of this repository. This can be done by updating the test_suite in that root BUILD file.

   NOTE: When adding unit tests with FFF, the unit test case must explicitly call DEFINE_FFF_GLOBALS. This can be done in one of two ways:
   1. Include ti_peripherals_fff.cc in the build by including the dependency "@spbb//common/test/mocks:ti_peripherals_mock_lib"
   2. In one of the .cc files used for the unit test, call DEFINE_FFF_GLOBALS. An example is available in ti_peripherals_fff.cc.

## Running coverage test
   *TBD - NOT YET IMPLEMENTED*

   To run just the tests for all modules:

   ```
   bazelisk test //:tests
   ```

   #### NOTE:
   Windows runs into long path issues with coverage reports.  Please create a `user.bazelrc` file in the root of the repo and add these lines to it:

   startup --output_base=C:/bzl
   build:gcovr --@bazel_platform//quality/ut:tmp_dir=C:/tmp

   The exact paths do not need to be exact.  The goal is to make them as short as possible.

   To run the coverage tests and generate html for all modules:

   ```
   bazelisk build //:coverage_report --config=spbb --config gcovr
   ```

## Committing changes to the Gerrit Repository
Our Gerrit repository is guarded against changes that may break the build.
You will not be able to submit your changes if any build is broken by them.

It is also guarded against changes that do not match the formatting standards identified by a team of your peers, guided by Aptiv's coding standards.
When committing your changes, a set of scripts will run (installed by running repo_init.py above) which should automatically format your code, and flag some potential problems.
These checks will then be run again as part of the "Verified" Jenkins job to ensure that you have the pre-commit checks in place.
This is to remove the unnecessary burden on developers to format their code in a standard way, and to make sure all our code is formatted in the same method.

If your change fails the "Verified" check, there are 3 potential issues:
1. Your code did not build
2. Your code did not pass all the pre-commit checks
3. An unknown error occured and the CICD team needs to check what happened

If it is one of the first 2 options, it is your job to fix the issues.
If you have code that you think should *not* be required to follow the autoformatting standards, please reach out to the CICD team.
They can determine if it is a valid request and assist in excluding the files.

# Porting to application
When porting to the application, the versioning scheme is described below. An automated email should be delivered that contains the contents of the http_archive to include the BBs into the application.

The signal processing provides multiple building block implementations, but all build time configuration is provided by the application through a Bazel local repository. An example of this local repository is provided in the appl_cfg/bb_cfg folder, and *MUST* be named bb_cfg in the application WORKSPACE file. The bb_cfg repository configuration can also be seen in the root WORKSPACE file of this repository.

The application must define this local_repository reference that is used by this building block. The local repository must be a standalone folder with an appropriate BUILD file and a WORKSPACE file. The WORKSPACE file can be empty, but must exist for Bazel to recognize the folder as a repository.

A similar thing is done in reverse for items the signal processing requires the application to provide. The application *MUST* provide a local repository named bb_include for these items. The items must match the items in the appl_cfg/bb_include folder.

1. @bb_cfg
   - Provides the build time configurations for the modules in cc_library() spbb_cfg_h which provides spbb_cfg.h.
2. @bb_include
   - Provides the build time items for the modules in cc_library() spbb_include_h which provides spbb_include.h.

# PCRESIM
   *TBD - NOT YET IMPLEMENTED*

# Versioning
   *TBD - NOT YET IMPLEMENTED*

  Releases are automatically tagged and tracked by a Jenkins job that uploads the tagged code to JFrog. To make a new release version two files must be updated as detailed here.

  1. changelog.md
    i. This file contains the version and revision of the code releases. This file should be updated to include notes about the release - what is changed in the released version of the code. This might require checking the git log to appropriately capture any changes made since the last version of the repository was tagged.
  2. version.yaml
    i. This file contains the name of the component (SPBB) which does not change, a version number and a revision number.

  The version number is the same format we were using before, just specified as X.Y with revision Z in the version.yaml file.

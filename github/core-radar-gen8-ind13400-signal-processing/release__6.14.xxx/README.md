# Core Radar - Gen8 Chandra Signal Processing Repository
This repository contains the signal processing building block source code for Gen8 Chandra (Indie Chandra+BBE based).

These building blocks are targeted for Gen8 and PCRESIM. The blocks here
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
The integration of the building blocks into an application is TBD.

However, all the building blocks can be built for unit testing.
Some of the building blocks will also implement testing on the hardware utilizing the Unity framework. This testing is for development purposes only, and will not be used to validate coverage of tests!

## Build options
In the SPBB repository, the default build enables additional options through the .bazelrc file:

1. ENABLE_HELPERS_SFR_DEBUG - Enables the use of the SFR pointers in Trace32 to allow for easier debugging. These can also be viewed through the PER use.
1. MIPI_HELPERS_DEBUG_ENABLE - Enables the debug mode APIs for the MIPI helper interface. This allows configuring the test pattern generator or the debug port mode of the MIPI ping/pong buffers.

There are switches that can be used to test different configurations:

1. --@build_config//:asic_fpga (or alias--bbe_asic_fpga) can be used to set the option for FPGA or ASIC configurations. The default selection is for the ASIC configuration.

```
--@build_config//:asic_fpga=fpga
--@build_config//:asic_fpga=asic
--bbe_asic_fpga=fpga
--bbe_asic_fpga=asic
```

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

1. --//modules/helpers/imp:profile_timing=ns (or alias --bbe_profile_timing) can be used to select microseconds or nanoseconds for profiling structures.

```
--//modules/helpers/imp:profile_timing=us
--//modules/helpers/imp:profile_timing=ns
--bbe_profile_timing=us
--bbe_profile_timing=ns
```

# Example Application
As a means to provide a common entry point, an example test app is provided. This allows simulation of underlying BB code using the Xtensa provided ISS (Instruction Set Simulator). The simulation can also be used to enable output of profile information. See the examples in the internal/bbe32 BUILD file for the use of the "run_profiler" tag to enable the output of the profile information.

## Module Specific Tests
The example application can be used as a reference to create simulations for individual modules. The module should provide the implementation for the test in the following interface:
    int bbe_test_entry_point()
    {
        return 0;
    }

To run the example application in the simulator, use one of the following build examples.

## Building and testing using A0 image
       Example:
       (To build .elf for running on Simulator for BBE of LUNA FPGA)
       bazelisk build //internal/bbe32:example_test_app_sim_show_summary
       bazelisk build //internal/bbe32:example_test_app_sim
       bazelisk build //internal/bbe32:example_luna_test_app

## baseline BBE build
       (To build .elf for running on the Target EVB, load it via trace32 for xtensa)
       bazelisk build //internal/bbe32:example_luna_test_app_emb --bbe_asic_fpga=fpga
       bazelisk build //internal/bbe32:example_luna_test_app_emb --bbe_asic_fpga=asic

## Mipi related
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_test --bbe_asic_fpga=fpga
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_idm_test --bbe_asic_fpga=fpga
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_test --bbe_asic_fpga=asic
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_idm_test --bbe_asic_fpga=asic

## SysMem related
       bazelisk build //modules/helpers/test/embedded/sysmem:fito_test
       bazelisk build //modules/helpers/test/embedded/sysmem:profiling_tests
       bazelisk build //modules/helpers/test/embedded/sysmem:idma_test --bbe_opt=O2 --bbe_profile_timing=ns

## RA and Range proc related
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=False

       (To test range processing for lookA, B, C, D)
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookA
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookB
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookC
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookD

## AA and First pass Related
       bazelisk build //modules/helpers/test/embedded/aux_acc:example_luna_test_app_emb
       bazelisk build //modules/rdd_first_pass_process/test:example_luna_test_app_emb --//modules/rdd_first_pass_process/test/testdata:test_select=bbe_en_local_max
       bazelisk build //modules/rdd_first_pass_process/test:example_luna_test_app_emb --//modules/rdd_first_pass_process/test/testdata:test_select=aa_en_local_max
       bazelisk build //modules/rdd_first_pass_process/test:example_luna_test_app_emb
       bazelisk build //modules/rdd_first_pass_process/test:3d_map_fun_test_emb

## Doppler proc related
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb
       bazelisk build //modules/doppler_process/test/unit_test/simulator:transpose_ops_sim

## Second pass and Sw BW related
       bazelisk build //modules/second_pass/test/sim:simulate_functional_test
       bazelisk build //modules/sweep_bw/test/sim:simulate_functional_test
       bazelisk build //modules/cdc/test:cdc_functional_test_emb

## REST and Non-REST Doppler processing related
       (To Build the doppler non-REST processing test cases)
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin3
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin1
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin0

       (To Build the doppler REST processing test cases)
       bazelisk build //modules/doppler_process/test:example_luna_test_rest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin5
       bazelisk build //modules/doppler_process/test:example_luna_test_rest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin6
       bazelisk build //modules/doppler_process/test:example_luna_test_selrest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin5
       bazelisk build //modules/doppler_process/test:example_luna_test_selrest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin6
       bazelisk build //modules/doppler_process/test:example_luna_test_restmap_app_emb --//modules/doppler_process/test/testdata:test_select=rbin17

       (To test range processing for lookA, B, C, D)
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookA
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookB
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookC
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookD

       For B0 RIDM SPBB in Range processing : Currently only for lookA configuration data set and will only work on FPGA since it is needed to use CSI emulator to inject that interfered data before MIPI , to generate gating signal.
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=False --//modules/range_process/test/testdata:range_test_select=LookA --bbe_asic_fpga=b0_fpga --verbose_failures

## Building and testing for B0 FPGA image
       (tested and works)
## baseline BBE build
       (To build .elf for running on the Target EVB, load it via trace32 for xtensa)
       bazelisk build //internal/bbe32:example_luna_test_app_emb --bbe_asic_fpga=b0_fpga

## Mipi related
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_test --bbe_asic_fpga=b0_fpga

      (yet, to be tested)
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_idm_test --bbe_asic_fpga=b0_fpga

       bazelisk build //modules/helpers/test/embedded/mipi:mipi_test --bbe_asic_fpga=b0_fpga --copt="-DENABLE_HELPERS_SFR_DEBUG" --copt="-DMIPI_HELPERS_DEBUG_ENABLE"

## SysMem related
       (yet, to be tested)

## RA and Range proc related
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --bbe_asic_fpga=b0_fpga --verbose_failures

       (To test range processing for lookA, B, C, D)
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookA --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookB
       --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookC
       --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/range_process/test:example_luna_test_app_emb --enable_adc_log=True --//modules/range_process/test/testdata:range_test_select=LookD
       --bbe_asic_fpga=b0_fpga --verbose_failures


## AA and First pass Related
       (tested and works)
       bazelisk build //modules/helpers/test/embedded/aux_acc:example_luna_test_app_emb --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/rdd_first_pass_process/test:example_luna_test_app_emb --//modules/rdd_first_pass_process/test/testdata:test_select=bbe_en_local_max --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/rdd_first_pass_process/test:example_luna_test_app_emb --//modules/rdd_first_pass_process/test/testdata:test_select=aa_en_local_max --bbe_asic_fpga=b0_fpga --verbose_failures
## Doppler proc related
       (To Build the doppler non-REST processing test cases)
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin3 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin1 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin0 --bbe_asic_fpga=b0_fpga --verbose_failures

       (To Build the doppler REST processing test cases)
       bazelisk build //modules/doppler_process/test:example_luna_test_rest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin5 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_rest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin6 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_selrest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin5 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_selrest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin6 --bbe_asic_fpga=b0_fpga --verbose_failures

## Second pass and Sw BW related
       (yet, to be tested)

## REST and Non-REST Doppler processing related

       (To Build the doppler non-REST processing test cases)
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin3 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin1 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_app_emb --//modules/doppler_process/test/testdata:test_select=rbin0 --bbe_asic_fpga=b0_fpga --verbose_failures

       (To Build the doppler REST processing test cases)
       bazelisk build //modules/doppler_process/test:example_luna_test_rest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin5 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_rest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin6 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_selrest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin5 --bbe_asic_fpga=b0_fpga --verbose_failures
       bazelisk build //modules/doppler_process/test:example_luna_test_selrest_app_emb --//modules/doppler_process/test/testdata:test_select=rbin6 --bbe_asic_fpga=b0_fpga --verbose_failures
## Building and testing using the Unity framework
      *TBD - NOT YET IMPLEMENTED*
   Use of the Unity test framework allows building a set of tests that can be loaded onto the DSP and executed. This setup requires a console output to get readable test results. The implementation is currently only confirmed to work on the Indie Chandra evaluation kit with the tarce32 programming . Eventually, this framework will allow running on the APTIV hardware as well, but this is TBD.

   ### Add Modules Here If Unity is implemented
   For the range_processing test use:
       bazelisk build //modules/range_process/test:example_luna_test_app_emb

## Building/Running Unit Tests using Google Test Framework
To run all the SPBB tests
   bazelisk test //:spbb_tests

### Adding unit tests\
      *TBD - NOT YET IMPLEMENTED*

## Running coverage test
To run all the SPBB tests and generate the coverage report
   bazelisk build //:coverage_report

## Committing changes to the Gerrit Repository
Our Gerrit repository is guarded against changes that may break the build.
You will not be able to submit your changes if any build is broken by them.

It is also guarded against changes that do not match the formatting standards identified by a team of your peers, guided by Aptiv's coding standards.
When commiting your changes, a set of scripts will run (installed by running repo_init.py above) which should automatically format your code, and flag some potential problems.
These checks will then be run again as part of the "Verified" Jenkins job to ensure that you have the pre-commit checks in place.
This is to remove the unnecessary burden on developers to format their code in a standard way, and to make sure all our code is formatted in the same method.

# Porting to application
When porting to the application, the versioning scheme is described below. An automated email should be delivered that contains the contents of the http_archive to include the BBs into the application.

The signal processing provides multiple building block implementations, but all build time configuration is provided by the application through a Bazel local repository. An example of this local repository is provided in the bb_cfg folder, and *MUST* be named spbb_cfg in the application WORKSPACE file. The spbb_cfg repository configuration can also be seen in the root WORKSPACE file of this repository.

The application must define this local_repository reference that is used by this building block. The local repository must be a standalone folder with an appropriate BUILD file and a WORKSPACE file. The WORKSPACE file can be empty, but must exist for Bazel to recognize the folder as a repository.

1. bb_cfg
   - Provides the build time configurations for the modules in spbb_cfg.h.

# PCRESIM

# Versioning
     *TBD - NOT YET IMPLEMENTED*
  Releases are automatically tagged and tracked by a Jenkins job that uploads the tagged code to JFrog. To make a new release version two files must be updated as detailed here.

  1. changelog.md
    i. This file contains the version and revision of the code releases. This file should be updated to include notes about the release – what is changed in the released version of the code. This might require checking the git log to appropriately capture any changes made since the last version of the repository was tagged.
  2. version.yaml
    i. This file contains the name of the component (SPBB) which does not change, a version number and a revision number.

  The version number is the same format we were using before, just specified as X.Y with revision Z in the version.yaml file.

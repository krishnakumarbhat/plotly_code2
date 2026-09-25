# Advanced Engineering - Gen 7 Project
This repoistory is the central place for Gen 7 source code. It is responsible
for creating an environment for building all the source code on the Gen 7 Radars
and providing the tools needed to run and debug Gen 7 radars.

[TOC]

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

## Building the Code
The following steps can be used to build the Gen 7 code.
All commands should be run from the software/build folder.

    SRR7P: bazelisk build //:gen7 --variant=srr7p
    SRR7HD: bazelisk build //:gen7 --variant=srr7hd
    FLR7: bazelisk build //:gen7 --variant=flr7
***Note:** --variant can now be used in place of --@appl_inclusion_dep//:variant*


For ease of use, build.bat and build.sh files have been provided to make the command easier. If you wish, you can run the following instead:

    SRR7P: build.bat srr7p
    SRR7HD: build.bat srr7hd
    FLR7: build.bat flr7


Bazel is a very good incremental build tool, therefore cleaning should not be necessary.
However, if you wish to clean out the build cache, you can do so by running:

    bazelisk clean

## Conditional Builds
Our builds have optional conditional builds to enable/disable certain parts of the code.
While production intent configurations should be done with the variant flags, these other conditional builds are used for debugging/testing.
The following conditional build flags are supported. They can be added to the command line to build with them.

Each Conditional build should contain a long form (starts with `--//:`) and a short form.
Short forms are defiend in .bazelrc as flag_alias configurations.
Only one form of the flag needs to be defined on the command line when building.

### Board Type
    --board=[B3|A2|A1|EDU|B3_Gbps|A2_Gbps|A1_Gbps|EDU_Gbps] -- Default=A1
    --//software/app/mss/mcal:boardVersion=[B3|A2|A1|EDU|B3_Gbps|A2_Gbps|A1_Gbps|EDU_Gbps]
Used to specify which hardware board version the software is being built to support.

### Microcontroller Revision
    --micro_revision=[ES1|ES2] -- Default=ES1
    --//:micro_revision=[ES1|ES2]
Used to specify which AWR2944 revision the software is being built for
If not specified, it defaults to ES1

### AWR Target Variant
    --awr_target_variant=[awr2944|awr2944eco] -- Default=awr2944
    --@appl_inclusion_dep//:awr_target_variant=[awr2944|awr2944eco]
Used to specify which AWR294x chip variant the software is being built for. Different variants have different hardware register definitions and driver implementations. The AWR2944 and AWR2944ECO variants require variant-specific source files for certain drivers and hardware register headers.

__awr2944__    - Build for AWR2944 chip variant (default)
__awr2944eco__ - Build for AWR2944ECO chip variant

If not specified, it defaults to awr2944

### Jenkins
    --jenkins
    --//:jenkins
Used on Jenkins to control a few steps of compilation.
Not to be used outside of the Jenkins environment.

### Generate Assembly Output
    --generate_assembly_output
    --//software/app/dss/src:generate_assembly_output
Gives additional information about DSS files as they are built

### Enable DSS Startup Tests
    --enable_dss_startup_test
    --//software/app/dss/src/helpers/tests:enable_dss_startup_test
Enables a set of startup tests that the DSS can run

### Disable Lauterbach Test ADC Output
    --disable_lauterbach_test_adc_output
    --//software/app/dss/src/range_proc:disable_lauterbach_test_adc_output
Disables ADC tracing via Lauterbach MDO lines

### RFFT Streaming
    --enable_dss_rfft_streaming
    --//software/app/dss/src/range_proc:enable_dss_rfft_streaming
    --disable_dss_rfft_streaming
    --no//software/app/dss/src/range_proc:enable_dss_rfft_streaming
Enables/Disables RFFT Streaming mode for sending RFFT compressed data onto communication bus.

### Compressed Data Cube (CDC)
    --enable_cdc
    --//software/app/dss/src:enable_cdc
    --disable_cdc
    --no//software/app/dss/src:enable_cdc
Enables/Disables Compressed Data Cube (CDC)

### Tailored Data Cube (TDC)
    --enable_tdc
    --//software/app/dss/src:enable_tdc
    --disable_tdc
    --no//software/app/dss/src:enable_tdc
Enables/Disables Tailored Data Cube (TDC)

### DSS L2 Cache Size
   dss_l2_cache_size=[0kB|32kB|64kB|128kB|256kB]
   --//software/app/dss/sys_config:dss_l2_cache_size=[0kB|32kB|64kB|128kB|256kB]
Sets the amount of DSS L2 memory to reserve for cache usage.

### Target For App Build
    --appVariantFlag=[mss|dss|boot] - Default=mss
    --//:appVariantFlag
Used to specify which target is being built. Set in transition elements to include appropriate libraries in the common libraries.

### Run Time Measurement (RTM)
    --enable_rtm
    --//software/app/mss/autosar/Aptiv_SWC/PLT_SWC/SWC_PLT_Appl_RTM:enable_rtm_measurement
Enables Run Time Measurement (RTM) for CPU data measurement. By default RTM is disabled.

 ### Run Time Measurement (RTM) for Task
    --enable_rtm_task
Enables Run Time Measurement (RTM) for Task measurement. By default RTM is disabled. Use the flag along with --enable_rtm

### Stack Monitering (Stack Monitor for DSS)
    --enable_stack_monitor_dss
    --//software/app/dss/src:enable_stack_monitor_dss
Disable Run Time stack monitering. By default Stack monitering is DISABLED.
### Stack Monitering (Stack Monitor for MSS)
    --disable_stack_monitor_mss
    --//software/app/mss/src:disable_stack_monitor_mss
Disable Run Time stack monitering. By default Stack monitering is ENABLED.

### Platform_Functional_Safety_Mechanisms
    --DISABLE_PLT_SAFETY_MECHANISMS
Disable Functional safety features. By default Functional safety is ENABLED.

### Enable Radar Capability Fault Injection Module
    --rc_fi
    --//libs/RC_fault_injection:enable_rc_fault_injection
Enables the Radar Capability Fault Injection module. By default the Radar Capability Fault Injection module is not built.

### Tracker Support
    --rot=<standalone|master|slave|none|sdv>
__master__ - Configures the sensor to subscribe to slave's detections service and transfer them to tracker module. Tracker will fuse slave(s) and master detections for objects.

__slave__  - Configures the sensor to offer and provide detections service to master and disable tracker objects service offering.

__standalone__  - Standalone operation: the radar will send out objects and tracker will operate in standalone mode.

__none__   - No tracker running in radar. No objects will be transmitted.

__sdv__   - No tracker running in radar. No objects will be transmitted. Angle finding downselection is enabled.

### Transmit Logging Streams Over Multicast Group
    --multicast
Allows to transmit all logging streams to a multicast group, so the same streams can be received simultaneously by all the members of the group of hosts that have joined the multicast group (useful in vehicle's instrumentation set-up).

### Vehicle Communication Protocol Selection
    --veh_com=<can|someip>
Allows to select either CAN or SOMEIP as the primary communication protocol of the sensor with the vehicle. Ethernet logging is always on in both cases.

__can__     - Enables CAN transmission logic from Application SWC.
__someip__  - Enables SOMEIP transmission logic from Application SWC.

default value: someip

### Enables the vehdata_backdoor (UDP)
    --enable_vehdata_backdoor
    --//software/app/mss/autosar/config/Appl/Source:enable_vehdata_backdoor

Enables UDP Veh_data. By default vehdata_backdoor is disabled.

### Enable ID Testing

    --enable_id_testing=True         : Enable the ID testing
    --enable_id_testing=False        : Disable the ID testing

By default enable_id_testing is disabled.

### Enable RC Testing

    --rc_bench_test=True         : Enable the RC testing
    --rc_bench_test=False        : Disable the RC testing

By default rc_bench_test is disabled.

### Enable Alignment Testing

    --enable_bench_testing=True         : Enable the Alignment testing
    --enable_bench_testing=False        : Disable the Alignment testing

By default vehdata_backdoor is disabled.


### Versioning
Several of the conditional builds are captured as part of software versioning in versions.c file. Refer the __[link](https://confluence.asux.aptiv.com/display/AASSA/GPO+Radar+Software+Versioning)__ here for more information

## Debugging the Code
Trace32 along with a Lauterbach debugger is used to debug the Gen 7 code on the target hardware.
An auto-install script is provided in the instrumentation/Lauterbach folder to easily install the tool.
You can either double click install_trace32.bat from the Windows Explorer or run install_trace32.bat from a command prompt.
This only needs to be done once per machine, as this becomes a system installation.

### Testing with ADC Offline Data Injection
Make Offline_Mode_Flag = TRUE

## Committing changes to the Gerrit Repository
Our Gerrit repository is guarded against changes that may break the build.
You will not be able to submit your changes if any build is broken by them.

It is also guarded against changes that do not match the formatting standards identified by a team of your peers, guided by Aptiv's coding standards.
When commiting your changes, a set of scripts will run (installed by running repo_init.py above) which should automatically format your code, and flag some potential problems.
These checks will then be run again as part of the "Verified" Jenkins job to ensure that you have the pre-commit checks in place.
This is to remove the unnecessary burden on developers to format their code in a standard way, and to make sure all our code is fomratted in the same method.

If your change fails the "Verified" check, there are 3 potential issues:
1. Your code did not build for all configured variants
2. Your code did not pass all the pre-commit checks
3. An unknown error occured and the CICD team needs to check what happened

If it is one of the first 2 options, it is your job to fix the issues.
If you have code that you think should *not* be required to follow the autoformatting standards, please reach out to the CICD team.
They can determine if it is a valid request and assist in excluding the files.

## Building/Running Unit Tests using Google Test Framework

   The following steps can be used to build the code for unit tests.

   From the root directory of the repository, run:

    bazelisk test //:all_unit_tests_mss --test_output=all (for unit tests in software/app/mss)
    bazelisk test //:all_unit_tests_dss --config=dss_ut --test_output=all (for unit tests in software/app/dss, including Signal Processing Building Block)

   This will build and run all unit tests listed in each core in the corresponding test_suites for each core.

   It is also possible to target single units for testing:

    bazelisk test //software/app/common/test:version_unit_test --test_output=all

   Bazel is a very good incremental build tool, therefore cleaning should not be necessary.
   However, if you wish to clean out the build cache, you can do so by running:

    bazelisk clean

   The following steps can be used to generate the coverage reports.

   From the root directory of the repository, run:

    bazelisk run //:coverage_mss
    bazelisk run //:coverage_dss --config=dss_ut

   This will create a report for all unit tests listed in the corresponding coverage gcovr_report for each core.

   Only through the jenkins UT job will a combined (mss+dss) UT report be generated.

## External Dependencies
Bazel uses the WORKSPACE file to define its external dependencies (both build tools and c code)

The preferred storage location for these external dependencies is in a static archive that can be obtained via the [http_archive](https://bazel.build/rules/lib/repo/http#http_archive) rule. Since Gerrit does not provide a link to download a static archive, JFrog Artifactory is used as our primary storage location for archive storage.

[git_repository](https://bazel.build/rules/lib/repo/git#git_repository) can also be used, however, **prefer http_archive to git_repository.** The reasons are:

 - Git repository rules depend on system git(1) whereas the HTTP downloader is built into Bazel and has no system dependencies.
 - http_archive supports a list of urls as mirrors, and git_repository supports only a single remote.
 - http_archive works with the repository cache, but not git_repository. See #5116 for more information.

### Point Bazel to an Updated JFrog Archive
When a new/updated archive needs to be used by Bazel, we update the http_archive in the WORKSPACE file.
The two fields which need to be updated are urls and sha256.
These values can be found in JFrog by selecting the package new package, and utilzing the "URL to file" and "SHA-256" fields in the General pane.

Example:
![!http_archive Fields to Update](/tools/readmeImages/WorkspaceFieldsToUpdate.PNG "Update the fields circled in red")
![!JFrog Fields to use in http_archive update](/tools/readmeImages/JFrogArchiveFields.PNG "Use the Fields highlighted in yellow")


# FBL:
## Introduction
FBL is a software used to download application over DOIP.
UDS is implemented as per the ISO-14229 Standard.

## Folder description
1. networking
   This folder contains LWIP stack
2. drivers
   This folder contains the qspi, flash drivers
3. os
   This folder contains freertos
4.tasks
  - DOIP : This folder contains implementation of UDS services, DoIP_handler , DoIP_server
5. boot
   boot_main : This function has the implementation of boot-manager

## Output Files
1. aptivBootloader
2. FBLImage
3. FBLMssApp

# Tool For Testing
DownloadTool:
Download tool is python based tool used to flash software over DOIP.

## Steps to Download the software
1. Flash aptivBootloaderSigned.hex and FBLImage.hex from T32 and run the software
2. Application will be downloaded via UDS from DownloadTool
3. Run python script DownloadApp.py present in tools\DownloadTool or by using following command
   `python Download.py
4. Configure and execute the tool by referring to README document present in tools to download the application software

## Macro usage in .bazelrc
You can define preprocessor macros for C and C++ compilation by adding flags to your `.bazelrc`.

### 1) Define a macro (no explicit value)
```text
build --conlyopt=-D<MACRO>      (ex: build --conlyopt=-DTEST_VALUES)
build --cxxopt=-D<MACRO>        (ex: build --cxxopt=-DTEST_VALUES)
build --define=<MACRO>=         (ex: build --define=TEST_VALUES=)
```

### 2) Define a macro with a value
```text
build --conlyopt=-D<MACRO>=value        (ex: build --conlyopt=-DTEST_VALUES=5)
build --cxxopt=-D<MACRO>=value          (ex: build --cxxopt=-DTEST_VALUES=5)
build --define=<MACRO>=value            (ex: build --define=TEST_VALUES=5)
```

### 3) Define a macro with an expression
```text
build --conlyopt=-D<MACRO>=expression   (ex: build --conlyopt=-DTEST_VALUES=((TEST_1)+(TEST_2)))
build --cxxopt=-D<MACRO>=expression     (ex: build --cxxopt=-DTEST_VALUES=((TEST_1)+(TEST_2)))
build --define=<MACRO>=expression       (ex: build --define=TEST_VALUES=((TEST_1)+(TEST_2)))
```

Notes:
1. `--conlyopt` and `--cxxopt` support both forms: `-D<NAME>` and `-D<NAME>=value`.
2. Bazel `--define` requires the `name=value` format. If you only need a macro to be present, use an empty value (for example: `--define=NAME=` or `--define=NAME=value`).
3. To avoid parsing issues, do not include spaces in macro assignments. Use parentheses when needed.

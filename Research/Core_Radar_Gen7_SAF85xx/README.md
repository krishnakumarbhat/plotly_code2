# Core Radar - Gen7_V2 Project

This repoistory is the central place for Gen7_V2 source code. It is responsible
for creating an environment for building all the source code and providing the tools needed to run and debug Gen7_V2 radars.

[toc]

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

## Git Submodules

This repo contains several Git submodules.  Functionally, these are nested git repositories where this (parent) repo keeps track of which commit to check out in the sub (child) repositories.  Git does not check out or update these child repositories automatically, it must be triggered by the user.

You can check the `.gitmodules` file to see the full list of currently configured repositories.

### More information

Please look at the official documentation for a deeper explanation of Git Submodules:
https://git-scm.com/book/en/v2/Git-Tools-Submodules

Or you can see command-line options for interacting with submodules with ```git submodule --help```

## Bazel

Bazel is used to build the code. Bazelisk is a wrapper around Bazel that ensures we all use the same Bazel version.
Bazel is a very powerful build tool and has many functionalities that may benefit the software development process (including dependency maps, build trees, etc.). More information can be found at https://bazel.build.

A detailed live demonstration was recorded on Bazel. You can find the link to the video at https://web.microsoftstream.com/embed/channel/04ec5a53-afff-4221-ba76-0a7d0dd50ed6?app=microsoftteams&sort=undefined&l=en-us#

If you do not have access to this link, request access to the Adv Active Safety SW/SYS Team in Microsoft Teams.

## Building the Code

The following steps can be used to build the Gen7_V2 code.
All commands should be run from the folder: "Core_Radar_Gen7_SAF85xx" (root folder).

    SRR7e: bazelisk build //:gen7 --variant=srr7e --board=A1 --micro_revision=ES2
    FLR7: bazelisk build //:gen7 --variant=flr7 --board=A1 --micro_revision=ES2
    SRR7p: bazelisk build //:gen7 --variant=srr7p --board=A1 --micro_revision=ES2
    FLR7V3: bazelisk build //:gen7 --variant=flr7v3 --board=A1 --micro_revision=ES2

Bazel is a very good incremental build tool, therefore cleaning should not be necessary.
However, if you wish to clean out the build cache, you can do so by running:

    bazelisk clean

>#### NOTE:
>Windows runs into long path issues when building and/or generating coverage reports.  Please create a `user.bazelrc` file in the root of the repo and add this line to it:
>```
>startup --output_base=C:/bzl
>```
>The --output_base entry _must_ be unique for each workspace on your PC.
>The paths do not need to be exact.  The goal is to make them as short as possible.
>You must use the same drive as your workspace.  (If you cloned into the D: drive, use `D:/bzl`)
>
>##### Advanced Windows Usage
>You can also create a .bazelrc in your C:\users\<netid>\ folder with this entry instead:
>```
>startup --output_user_root=C:/bzl
>```
>In this case, Bazel will create a unique sub-folder for each workspace on your PC.
>_HOWEVER_, this path might not be short enough in some circumstances, so bear in mind you may need to fall back to the typical `--output_base` method inside each workspace.

## Conditional Builds

Our builds have optional conditional builds to enable/disable certain parts of the code.
While production intent configurations should be done with the variant flags, these other conditional builds are used for debugging/testing.
The following conditional build flags are supported. They can be added to the command line to build with them.

Short and long form of conditional builds are defiend in .bazelrc as flag_alias configurations.
Only one form of the flag needs to be defined on the command line when building.

## Conditional Builds to Profile RDD

    SRR7e: bazelisk build //:gen7 --variant=srr7e --board=A1 --micro_revision=ES2 --//software/bbe32:enable_doppler_module_timing=True
    FLR7: bazelisk build //:gen7 --variant=flr7 --board=A1 --micro_revision=ES2 --//software/bbe32:enable_doppler_module_timing=True
    SRR7p: bazelisk build //:gen7 --variant=srr7p --board=A1 --micro_revision=ES2 --//software/bbe32:enable_doppler_module_timing=True
    FLR7V3: bazelisk build //:gen7 --variant=flr7v3 --board=A1 --micro_revision=ES2 --//software/bbe32:enable_doppler_module_timing=True
To get the profiling results the script tools/lauterbach/modulewise_time.cmm should be run in lauterbach.

### Build variant

    --variant=[srr7e|flr7|srr7p|flr7v3] -- Default=srr7p
Used to specify which SAF85xx variant the software is being built for

### Microcontroller Revision

    --micro_revision=[ES1|ES2] -- Default=ES2
Used to specify which SAF85xx revision the software is being built for

### Jenkins

    --jenkins
Used on Jenkins to control a few steps of compilation.
Not to be used outside of the Jenkins environment.

### Run Time Measurement (RTM)

    --disable_rtm
    --//:DISABLE_RTM_MEASUREMENT
Disables Run Time Measurement (RTM) for CPU data measurement and Task Measurement. By default RTM is enabled.

### NvM CRC

    --nvm_crc_disable=True   # disable CRC handling
    --nvm_crc_disable=False  # keep CRC enabled (default)
    --//:NVM_CRC_DISABLE=True
NvM CRC is enabled by default. Set the flag to `True` to bypass CRC calculation/validation; leave it `False` (or omit) to keep CRC active.

### CDC

    --enable_cdc       : Enables CDC
    --enable_cdc=False : Disables CDC

By defualt CDC is enabled.

    CDC_2K_BINS : Configs 2016 CDC bins
    CDC_5K_BINS : Configs 5094 CDC bins

    Add CDC_2K_BINS to defines[] in software/bbe32/bb_cfg/build to config CDC_2K_BINS

By default CDC_5K_BINS is configured.

### Enable CDC_CRC

    --enable_cdc_crc       : Enables CDC CRC
    --enable_cdc_crc=True  : Enables CDC CRC
    --enable_cdc_crc=False : Disables CDC CRC

By default CDC_CRC is enabled.

#### Tracker Variant Selection (single flag)
Use `--tracker_variant=<value>` to control tracker variant selection and disabling.

Supported values:
- `disabled` : disable tracker code from building
- `platform_flr7_standalone`       : PLATFORM_FLR8_STANDALONE (F360 variant G -> 1 MRR sensor, 128 detections, 64/50 objects)
- `platform_srr7p_standalone`      : PLATFORM_SRR8P_STANDALONE (Variant J -> 1 SRR sensor, 128 detections, 64/64 objects)
- `platform_srr7p_2_sensor_fusion` : PLATFORM_SRR8P_2_SENSOR_FUSION (Variant R -> 2 SRR sensors, 64 detections each, 64/64 objects)

####  Enable Tracker stub
Use  `--stub_tracker=true` to enable stub code for creating objects with syntatic det data, should be enabled with Tracker(`--tracker_variant=<value>`) option.

Note: By default tracker stub is disabled.

### Disable NM ( Network Management - CAN )

    --enable_nm       : Enables  NM(CAN)
    --enable_nm=False : Disables NM(CAN)

    By default NM is enabled. Enable the Network Management

### Aptiv Official Release

    --Aptiv_Official_Release       : All Det Errors will be filtered

### Enable Stream Re-generation with RESIM supported files

    --enable_stream_generation          : enable stream generation
    --enable_stream_generation=True     : enable stream generation
    --enable_stream_generation=False    : disable stream generation

    By default stream re-generation is disabled.

### Disabling the internal watchdog through Build

    --disable_int_wdg          : disable internal watchdog
    --disable_int_wdg=True     : disable internal watchdog
    --disable_int_wdg=False    : enable  internal watchdog

    By default internal watchdog is enabled.

### Enable SecOC

    --enable_secoc=True         : Enable the SecOC Tx/Rx Functionality
    --enable_secoc=False        : Disable the SecOC Tx/Rx Functionality

    By defatult SecOC is disabled.

### Enable VLAN

    --enable_vlan               : Enable VLAN configuration for EthIf
    --enable_vlan=True          : Enable VLAN configuration for EthIf
    --enable_vlan=False         : Disable VLAN configuration for EthIf

    By default VLAN is disabled. When enabled, uses VLAN-specific EthIf configuration files
    for Virtual LAN networking support.

### Enable XCP fault injection

	Build_XCP_FAULT_INJECTION_Test_Cases   : Enable XCP fault injection

### Enable ID Testing

    --enable_id_testing=True         : Enable the ID testing
    --enable_id_testing=False        : Disable the ID testing

### Enable Alignment Testing

    --enable_bench_testing=True         : Enable the Alignment testing
    --enable_bench_testing=False        : Disable the Alignment testing

### Enable Alignment stub detection only Testing

    --enable_bench_testing=True --align_det_test=True  : Only detection stubbed for the alignment testing
    --enable_bench_testing=True --align_det_test=False : All parameter stubbed for the Alignment testing

### Enable Static Alignment old Algo Testing

    --enable_sra_doppler=True  : Static alignment doppler algo testing
    --enable_sra_doppler=False  : Static alignment old algo testing

## Debugging the Code

Trace32 along with a Lauterbach debugger is used to debug the Gen7_V2 code on the target hardware.
An auto-install script is provided in the tools/lauterbach folder to easily install the tool.
You can either double click install_trace32.bat from the Windows Explorer or run install_trace32.bat from a command prompt.
This only needs to be done once per machine, as this becomes a system installation.

## Committing changes to the Gerrit Repository

Our Gerrit repository is guarded against changes that may break the build.
You will not be able to submit your changes if any build is broken by them.

It is also guarded against changes that do not match the formatting standards identified by a team of your peers, guided by Aptiv's coding standards.
When commiting your changes, a set of scripts will run (installed by running repo_init.py above) which should automatically format your code, and flag some potential problems.
These checks will then be run again as part of the "Verified" Jenkins job to ensure that you have the pre-commit checks in place.
This is to remove the unnecessary burden on developers to format their code in a standard way, and to make sure all our code is formatted in the same method.

## Building/Running Unit Tests using Google Test Framework

### Running Unit Tests Only
From the root directory of the repository, run:

  bazelisk test //:all_unit_tests

This will build and run all unit tests listed in each core in the corresponding test_suites for each core.

  bazelisk test //:tests_bbe --variant=srr7e
  bazelisk test //:tests_bbe --variant=srr7p
  bazelisk test //:tests_bbe --variant=flr7
  bazelisk test //:tests_bbe --variant=flr7v3
These will build and run all unit tests listed in each core in the corresponding test_suites for bbe.

It is also possible to target single units for testing:

    bazelisk test //software/common/versions/test:unit_tests

### Generating Coverage Reports
The following steps can be used to generate the coverage reports.  If the unit tests have not been run yet, these commands will automatically run them first.

>#### NOTE:
>Windows runs into long path issues when building and/or generating coverage reports.
>This should already be handled by the root .bazelrc file, but if you're building on a drive other than the C: drive please create a `user.bazelrc` file in the root of the repo and add this line to it:
>```
>test:windows --@bazel_platform//quality/ut:tmp_dir=D:/tmp
>```
>Replace `D:` with the drive where this workspace is stored.
>The path does not need to be exact.  The goal is to make it as short as possible and also not conflict with any other workspace.

From the root directory of the repository, run:

    bazelisk test //coverage:report --variant=srr7p

To run a given submodule's ut, you can give it a different path. BBE example:

    bazelisk test //coverage:bbe_report --variant=srr7p

Output files will be listed as the last comment in the console.  Variant can be replaced with whichever variant you wish to test.

## Building/Running Coverity

### Coverity Build with Bazel
bazel_platform provides a few rules to use Coverity within Bazel, specifically coverity_analysis.

The following command will run Coverity analysis for M7:

    bazelisk build //software/m7:windriver_v7_cov --variant srr7p

The following command will run Coverity analysis for A53:

    bazelisk build //software/a53:windriver_v7_cov --variant srr7p

The following command will run Coverity analysis for BBE32:

    bazelisk build //software/bbe32:xtensa_bbe32_cov --variant srr7p

### Coverity Analysis with Bazel
Once the build is complete, a local analysis can be run and compared against the latest snapshot on the server to determine if there are any new local Coverity defects.

    bazelisk run //software/m7:commit_defects --variant srr7p -- --auth-key-file="C:\Users\(your username)\coverity.auth"

An auth_key can be generated from your user account in [Coverity Connect](https://coverity.asux.aptiv.com/)


## External Dependencies

Bazel uses the MODULE.bazel files to define its external dependencies (both build tools and c code)

The preferred storage location for these external dependencies is in a static archive that can be obtained via the bazel registry.  Entries to the ADVRADAR_Bazel_Registry repo are automatically deployed to JFrog, where the available releases can be referenced by name, no need to know where it is stored.

Most of the BBs have not yet been migrated to Bzlmod, so we're still using the legacy [http_archive](https://bazel.build/rules/lib/repo/http#http_archive) rule. Since Gerrit does not provide a link to download a static archive, JFrog Artifactory is used as our primary storage location for archive storage.

[git_repository](https://bazel.build/rules/lib/repo/git#git_repository) can also be used, however, **prefer http_archive to git_repository.** The reasons are:

- Git repository rules depend on system git(1) whereas the HTTP downloader is built into Bazel and has no system dependencies.
- http_archive supports a list of urls as mirrors, and git_repository supports only a single remote.
- http_archive works with the repository cache, but not git_repository. See #5116 for more information.

### Point Bazel to an Updated JFrog Archive

When a new/updated archive needs to be used by Bazel, we update the http_archive in the bb.MODULE.bazel file.
The two fields which need to be updated are urls and sha256.
These values can be found in JFrog by selecting the package new package, and utilzing the "URL to file" and "SHA-256" fields in the General pane.

Example:
![!http_archive Fields to Update](/tools/readmeImages/WorkspaceFieldsToUpdate.PNG "Update the fields circled in red")
![!JFrog Fields to use in http_archive update](/tools/readmeImages/JFrogArchiveFields.PNG "Use the Fields highlighted in yellow")

## Building compile_commands.json for TiCS analysis

   The following step can be used to generate the compile_commands.json to the main workspace. TiCS expectes this file to be there during the analysis.

   From the root directory of the repository, run:

    bazel run //:compiledb

## Dynamic analysis

### Setup
Dynamic analysis is only supported on Linux.  To run Linux commands on a Windows machine, see the [WSL instructions](https://confluence.asux.aptiv.com/x/WUKqIg) on Confluence.

Install the [luci-cli](https://gitgerrit.asux.aptiv.com/plugins/gitiles/00000000_luci/cli) Python tools suite which will give you access to the aptiv_warnings_analyzer:
```
pip config set global.trusted-host jfrog.asux.aptiv.com
pip config set global.extra-index-url https://jfrog.asux.aptiv.com/artifactory/api/pypi/luci-aptiv-00000000-pypi-local/simple
pip install -U luci-cli==0.3.0+250117.b3acc92
```

We use a pre-release version at this time, so please use exactly the version listed above.

You'll also need to install a credential manager to store your Jira password:
```
pip install keyrings.cryptfile
```

### Running Dynamic Analysis
Run the pre-configured script to get the analysis for the respective Dynamic Analysis checkers.  It takes the checker and variant as parameters.  You can also run a given checker over all variants.  Examples:
```
./run_dynamic.sh asan srr7p
./run_dynamic.sh ubsan srr7p
./run_dynamic.sh valgrind srr7p
```
```
./run_dynamic.sh asan all
./run_dynamic.sh ubsan all
./run_dynamic.sh valgrind all
```

These will generate output files in the root of the repo with all the details of the report you generated.

### Bypassing Defects
A Confluence page on bypassing defects can be found here:
https://confluence.asux.aptiv.com/x/QomiKw

#### Temporary Bypass
In brief, JIRA tickets are used to bypass defects which need to be fixed.  The above link has details on how to create a JIRA ticket to allow a defect through merge gate.  We use the `warnings_justification` and sanitizer name (`asan`, `ubsan`, or `valgrind`) as JIRA labels in this project.  See the awa.sh script for the full list of JIRA projects which are searched.  This ticket can then be planned and resolved as usual.

*WARNING!* Do not close these JIRA tickets until any fixes have been completely merged to /dev, or all merges will be blocked!

#### Permanent Bypass
If a Dynamic Analysis defect is found which is either a false positive, or not worth fixing (perhaps coming from a unit test itself) then it can be bypassed without a JIRA ticket by appending the .toml entries from the JIRA ticket into the appropriate .yaml file in the root of the repo.  (`asan.toml`, `ubsan.toml`, `valgrind.toml`)  These files are protected by the Code-Owners plugin, so any changes will need to be verified by the CI team.

### Advanced Usage
Full details on how to use the aptiv_warnings_analzer can be found [online](https://gitgerrit.asux.aptiv.com/plugins/gitiles/00000000_luci/cli/+/HEAD/src/aptiv_warnings_analyzer#local-developer-workflow).  The most up-to-date information on the command line options can be found in the built in help text:
```
aptiv_warnings_analyzer --help
aptiv_warnings_analyzer code_checkers --help
```

## Fuzz Testing

Fuzz testing is powered by [Mayhem](https://mayhem.asux.aptiv.com/).
See the confluence page for setup and access instructions: [Fuzz Testing Quick Start Guide][fuzz0]

[fuzz0]: <https://confluence.asux.aptiv.com/display/ADVAS/Fuzz+Testing+Quick-Start+Guide> "Confluence: ADVAS: Fuzz Testing Quick-Start Guide"

### Running Fuzz tests
Fuzz tests are set up to be run through Bazel.  You can run all tests with this command:
```
bazelisk run :fuzz --config mayhem
```

Individual fuzz tests can be specified instead of the aggregate target.  Example:
```
bazelisk run //software/common/app_chksum/test:dd_app_chksum_fuzz --config mayhem
```

> **Note**
> These commands will compile a test package and then upload it to the Mayhem server to run to completion there.  The default time is 30 minutes.
> Follow the Run URL(s) in your console output to see the status of your mayhem tests.

### Writing Fuzz Tests
Please look at existing tests for inspiration.  There is also an example test with slightly more documentation at [tools/mayhem/example](tools/mayhem/example/).
For even more instructions and for presentation given by Mayhem representatives, please look at the [GSS page for Mayhem][fuzz1].  There are also extensive help articles on [the Mayhem website][fuzz2].

All of our tests are [base-executable c/c++ tests][fuzz3], running as [non-Docker Targets][fuzz4] using the publicly available [rules_mayhem][fuzz5] Bazel rules.

[fuzz1]: <https://spo.aptiv.com/sites/0109-ProductSecurity/SitePages/SW-Fuzz-Testing.aspx?web=1#trainings-and-useful-materials> "SW Fuzz Testing Trainings"
[fuzz2]: <https://mayhem.asux.aptiv.com/docs/code-testing/tutorials/start-here-code/> "Mayhem Tutorials"
[fuzz3]: <https://mayhem.asux.aptiv.com/docs/code-testing/by-language/c-cpp/base-executable/> "Mayhem C/C++ Tests"
[fuzz4]: <https://mayhem.asux.aptiv.com/docs/code-testing/tutorials/beginner/non-docker-testing/> "Non-Docker Targets"
[fuzz5]: <https://github.com/ForAllSecure/rules_mayhem> "GitHub: ForAllSecure: Bazel Rules for Mayhem"

> **Note**
> If you add a fuzz test, please ensure it is added to both the `//:fuzz`  and `//:fuzz_harnesses` filegroups.

### Regression Fuzz Tests
Regressions fuzz tests are invoked in much the same way as standard fuzz tests.  The difference is that a standard run must have been performed at some time in the past to establish the base suite of tests, so it is not available for brand-new tests.  Otherwise, we've abstracted the details for you, and you can just add the
```--regression``` flag to your build command to run the regression tests.  They should complete within a couple minutes of triggering.
```
bazelisk run :fuzz --config mayhem --regression
```

### Fuzz Testing Coverage
A list of un-fuzzed files can be checked with this command:
```
bazelisk build //coverage:fuzz_report --config mayhem
```


## Stream Bandwidth Calculation

### Running the Python Script
tools\python\streamGenerator\run_bandwidth.py -- the following script takes the first line from streamdef files (no_of_bytes) as the input and calculates the Bandwidth in Mbps
To execute the python script Run the following command:
'''
bazelisk build //tools/python/streamGenerator:generate_bandwidth
'''

### Output of the Script
The python script generates a .csv file in the following path bazel-bin\tools\python\streamGenerator\Stream_Bandwidth_Details.csv

Stream_Bandwidth_Details.csv has the information like
Path,Variant,Bytes,Bandwidth

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


## Manual Changes in Generated Code

>#### NOTE (Manual change in generated code)
>A manual modification has been applied to the generated file `software/m7/autosar/config/Appl/GenData/PduR_Lcfg.c` to address a SecOC payload length mismatch. The change pads the IF-Tx PDU to 52 bytes (appends one `0x00` byte) before routing.
>
>This is the workaround that was added in the generated source:
>```c
>/* Workaround: IdsM QSEv runtime length is 51 bytes, while SecOC FullAuthentic is configured as 52 bytes.
> * Passing 51 bytes to SecOC causes an internal offset of 1 (52-51).
> * Pad the IF-Tx PDU to 52 bytes here (append one 0x00 byte) before routing.
> *
> * Note: This is a manual change to generated code and will be overwritten on regeneration.
> */
>```
>If AUTOSAR/SIP configuration is regenerated, this file will be overwritten and the workaround must be re-applied.

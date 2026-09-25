# ALL ABOUT RDD SIL EXECUTABLE BUILD PROCEDURE
-------------------------
# Table of contents
- [ALL ABOUT RDD SIL EXECUTABLE BUILD PROCEDURE](#all-about-rdd-sil-executable-build-procedure)
- [Table of contents](#table-of-contents)
  - [1. Introduction](#1-introduction)
  - [2. Tools Required](#2-tools-required)
  - [3. External Repo Required](#3-external-repo-required)
  - [4. RDD SIL Framework](#4-rdd-sil-framework)
      - [4.1. RDD SIL API](#41-rdd-sil-api)
      - [4.2. RDD SIL IO Structure](#42-rdd-sil-io-structure)
          - [4.2.1 RDD SIL Input Structure](#421-rdd-sil-input-structure)
          - [4.2.2 RDD SIL Output Structure](#422-rdd-sil-output-structure)
      - [4.3 ID SIL API IO Structure](#43-id-sil-api-io-structure)
        - [4.3.1 ID SIL API Input Structure](#431-id-sil-api-input-structure)
        - [4.3.2 ID SIL API Output Structure](#432-id-sil-api-output-structure)
      - [4.4 RC SIL API IO Structure](#44-rc-sil-api-io-structure)
        - [4.4.1 RC SIL API Input Structure](#441-rc-sil-api-input-structure)
        - [4.4.2 RC SIL API Output Structure](#442-rc-sil-api-output-structure)
      - [4.5 DA SIL API IO Structure](#45-da-sil-api-io-structure)
        - [4.5.1 DA SIL API Input Structure](#451-da-sil-api-input-structure)
        - [4.5.2 DA SIL API Output Structure](#452-da-sil-api-output-structure)
      - [4.6 SA SIL API IO Structure](#46-sa-sil-api-io-structure)
        - [4.6.1 SA SIL API Input Structure](#461-sa-sil-api-input-structure)
        - [4.6.2 ID SIL API Output Structure](#462-id-sil-api-output-structure)
      - [4.7 RDD SIL Library / Header file  dependencies](#47-rdd-sil-library--header-file--dependencies)
        - [4.7.1 RDD SIL header file path:](#471-rdd-sil-header-file-path)
        - [4.7.2 CDC SIL Library to be include](#472-cdc-sil-library-to-be-include)
        - [4.7.3 External spbb libs](#473-external-spbb-libs)
        - [4.7.4 External afbb libs](#474-external-afbb-libs)
        - [4.7.5 External idbb libs](#475-external-idbb-libs)
        - [4.7.8 External rcbb libs](#478-external-rcbb-libs)
        - [4.7.9 External dabb libs](#479-external-dabb-libs)
        - [4.7.10 External sabb libs](#4710-external-sabb-libs)
        - [4.7.11 RDD SIL Library folder path:](#4711-rdd-sil-library-folder-path)
        - [4.7.12 ID SIL Library folder path:](#4712-id-sil-library-folder-path)
        - [4.7.13 RC SIL Library folder path:](#4713-rc-sil-library-folder-path)
        - [4.7.14 DA SIL Library folder path:](#4714-da-sil-library-folder-path)
        - [4.7.15 SA SIL Library folder path:](#4715-sa-sil-library-folder-path)
      - [4.8  RDD SIL BUILD Command:](#48--rdd-sil-build-command)
        - [Build flag for disabling BBE C-stubs on AFBB:](#build-flag-for-disabling-bbe-c-stubs-on-afbb)
        - [Build flag for Logging RDD Detection Data:](#build-flag-for-logging-rdd-detection-data)
        - [Build flag for Overriding SMC data:](#build-flag-for-overriding-smc-data)
        - [Build flag for Overriding USC data:](#build-flag-for-overriding-usc-data)
        - [Build flag for Enabling Fault Injection in Radar Capability](#build-flag-for-enabling-fault-injection-in-radar-capability)
      - [4.9  SPBB tag used:](#49--spbb-tag-used)
      - [4.10  AFBB tag used:](#410--afbb-tag-used)
      - [4.11 IDBB tag used:](#411-idbb-tag-used)
      - [4.12 RCBB tag used:](#412-rcbb-tag-used)
      - [4.13 DABB tag used:](#413-dabb-tag-used)
      - [4.14 SABB tag used:](#414-sabb-tag-used)
      - [4.15 SMC and USC Version:](#415-smc-and-usc-version)
- [5. Debugging SIL Executable](#5-debugging-sil-executable)
  - [5.1. configuring VS code for debugging](#51-configuring-vs-code-for-debugging)
- [6. SIL in ITF](#6-sil-in-itf)
  - [6.1. Introduction](#61-introduction)
  - [6.2. SIL in ITF Framework](#62-sil-in-itf-framework)
  - [6.3. Input Modes Supported in SIL in ITF](#63-input-modes-supported-in-sil-in-itf)
      - [6.3.1 MEX based input](#631-mex-based-input)
      - [6.3.2 LOG based input](#632-log-based-input)
  - [6.4. RDD SIL DLL](#64-rdd-sil-dll)
  - [6.5. Build Command](#65-build-command)
  - [6.6. Usage Summary](#66-usage-summary)

-------------------------
## 1. Introduction
Software-in-the-loop (SIL) is a method of testing and validating code in a simulation environment in order to quickly and cost-effectively catch bugs and improve the quality of the code. Typically, SIL testing is conducted in the early stages of the software development process, while the more complex, costlier hardware-in-the-loop (HIL) testing is done in later stages.

For more information please refer to https://www.aptiv.com/en/insights/article/what-is-software-in-the-loop-testing

## 2. Tools Required
The below tools are required  to build and debug SIL executable.
* Visual Studio code
* Bazel

## 3. External Repo Required

SRR7P SPBB : https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen7_NXP_Signal_Processing

AFBB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen7_SAF85xx_Angle_Finding

IDBB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Interference_Detection

DABB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Dynamic_Alignment

SABB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Static_Alignment

RCBB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Capability

## 4. RDD SIL Framework
RDD (Radar Data Detection)

#### 4.1. RDD SIL API
The RDD SIL API **Rdd_To_Detection_Configuration** is defined in **rdd_sil_interface.cpp**  in path shared  below :
**Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\rdd_sil_interface.cpp**

* The file **rdd_sil_api.hpp** includes the declaration of RDD SIL API.
* The design flow Digram for RDD SIL API has shown in the below picture.

!["RDD_SIL_Block_Diag_Design"](RDD_SIL_Block_Diag_Design.png)

#### 4.2. RDD SIL IO Structure
For current implementation the RDD data input (CFAR Threshold and MPRB) are provided in .bin format .
    RDD Data file path:**Core_Radar_Gen7_SAF85xx\sil\rsp_sil\data_bin\srr7e\cdc_data\rdd_input.bin**

###### 4.2.1 RDD SIL Input Structure

* Rdd_Stream_T
* Radar_Look_T

###### 4.2.2 RDD SIL Output Structure

* Rdd_Stream_T
* Detection_Stream_T
* Down_selection_Stream_T
* Detection_Debug_Stream_T

#### 4.3 ID SIL API IO Structure
##### 4.3.1 ID SIL API Input Structure
* Radar_Look_T
* Vse_Stream_T
* Rdd_Stream_T
* PSP_Input_Sil_T
* Rdd_Data_T
##### 4.3.2 ID SIL API Output Structure
* ID_Stream_T

#### 4.4 RC SIL API IO Structure
##### 4.4.1 RC SIL API Input Structure
* Radar_Look_T
* Detection_Stream_T
* RDD_Data_T
* Vse_Stream_T
* ID_Stream_T
* Dynamic_Alignment_Log_Stream_T
* PSP_Input_Sil_T
##### 4.4.2 RC SIL API Output Structure
* Radar_Capability_Stream_T

#### 4.5 DA SIL API IO Structure
##### 4.5.1 DA SIL API Input Structure
* Radar_Look_T
* PSP_Input_Sil_T
* Detection_Stream_T
* Vse_Stream_T
* Radar_Capability_Stream_T
##### 4.5.2 DA SIL API Output Structure
* Dynamic_Alignment_Log_Stream_T

#### 4.6 SA SIL API IO Structure
##### 4.6.1 SA SIL API Input Structure
* PSP_Input_Sil_T
* ID_Stream_T
* Detection_Stream_T
* Vse_Stream_T
##### 4.6.2 ID SIL API Output Structure
* Alignment_Stream_T

#### 4.7 RDD SIL Library / Header file  dependencies

##### 4.7.1 RDD SIL header file path:
* Core_Radar_Gen7_SAF85xx\software\common\rdd_stream.h
* Core_Radar_Gen7_SAF85xx\software\common\detection_stream.h
* Core_Radar_Gen7_SAF85xx\software\common\detection_debug_stream.h
* Core_Radar_Gen7_SAF85xx\software\common\down_selection_stream.h
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\rdd_sil_api.hpp
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\rdd_proc.h
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\af_sil_interface\af_sil_api.hpp
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\da_sil_interface\da_sil_api.hpp
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\id_sil_interface\sil_id_wrapper.hpp
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\sa_sil_interface\sa_sil_wrapper.hpp
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\rc_sil_interface\rc_sil_wrapper.hpp


##### 4.7.2 CDC SIL Library to be include
* //sil_wrapper:sil_wrapper_lib
* //sil_wrapper/utility:sil_wrapper_utils_lib
* @Gen7_SAF85xx//software/common/calibrations/usc:usc_module
* @Gen7_SAF85xx//software/common/calibrations/smc:smc_module
* @Gen7_SAF85xx//sil/rsp_sil/main/rsp_wrapper_interface/af_sil_interface:af_sil_lib
* @Gen7_SAF85xx//sil/rsp_sil/main/rsp_wrapper_interface/psp_sil_interface/da_sil_interface:da_sil_lib
* @Gen7_SAF85xx//sil/rsp_sil/main/rsp_wrapper_interface/psp_sil_interface/id_sil_interface:ID_sil_lib
* @Gen7_SAF85xx//sil/rsp_sil/main/rsp_wrapper_interface/psp_sil_interface/rc_sil_interface:rc_sil_lib
* @Gen7_SAF85xx//sil/rsp_sil/main/rsp_wrapper_interface/psp_sil_interface/sa_sil_interface:SA_sil_lib

##### 4.7.3 External spbb libs
* @spbb//common:bbe_native_sim_lib
* @spbb_include//:spbb_include_h

##### 4.7.4 External afbb libs
* @afbb//module/_imp/_src/flr7:angle_finding_imp_ut_src
* @afbb//module/_imp/_src/srr7p:angle_finding_imp_ut_src
* @afbb//module/_imp/_src/srr7e:angle_finding_imp_ut_src

##### 4.7.5 External idbb libs
* @idbb//module/id_imp/_src:interference_detection_src_v2_sil

##### 4.7.8 External rcbb libs
* @rcbb//module/common:rc_common_headers
* @rcbb//module/capability:radar_capability_feature_lib
* @rcbb//module/capability:radar_capability_feature_lib_h

##### 4.7.9 External dabb libs
* @dabb//module/dyn_alignment/dyn_alignment_imp/_src:dyn_align_module_sources_sil

##### 4.7.10 External sabb libs
* @sabb//module/static_alignment/static_alignment_api:static_alignment_algo_inc
* @sabb//module/static_alignment/static_alignment_imp/_src:static_alignment_algo_src

##### 4.7.11 RDD SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\sil_wrapper_interface
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rsp_common
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\test

##### 4.7.12 ID SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\id_sil_interface

##### 4.7.13 RC SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\rc_sil_interface

##### 4.7.14 DA SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\da_sil_interface

##### 4.7.15 SA SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\sa_sil_interface

#### 4.8  RDD SIL BUILD Command:
SRR7P :
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True  --@afbb//module/_common:sil_config_enable=True --psp_sil_config=True
```

FLR7:
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=flr7 --@spbb//common:use_bbe_cstub_simulator=True  --@afbb//module/_common:sil_config_enable=True --psp_sil_config=True
```

SRR7E:
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr7e --@spbb//common:use_bbe_cstub_simulator=True  --@afbb//module/_common:sil_config_enable=True --psp_sil_config=True
```
##### Build flag for disabling BBE C-stubs on AFBB:

The AFBB pcresim library can be built with either a native (x86) implementation of the functions or with the BBE32 version of the functions.

The 'use_bbe_cstub_simulator' flag can be used to specify that it is desired to use the BBE32 C-Stub version of the functions and execute them via the simulator.

This flag defaults to True,but if the SIL wrapper runs too slowly, setting this flag to False can improve performance which will build the library on pure c implementation.

* External to the AFBB, add this to your bazel command line:
  `--@afbb//module/_common/radar_math:use_bbe_cstub_simulator=False`

##### Build flag for Logging RDD Detection Data:
--//sil/rsp_sil/main/sil_wrapper_interface:enable_logging=True

##### Build flag for Overriding SMC data:
SRR7P :
--override_repository=smc_srr7p="./sil/rsp_sil/cal_bin/srr7p/smc_srr7p"

FLR7:
--override_repository=smc_flr7="./sil/rsp_sil/cal_bin/flr7/smc_flr7"

SRR7E:
--override_repository=smc_srr7e="./sil/rsp_sil/cal_bin/srr7e/smc_srr7e"

##### Build flag for Overriding USC data:
SRR7P :
--override_repository=usc_srr7p="./sil/rsp_sil/cal_bin/srr7p/usc_srr7p"

FLR7:
--override_repository=usc_flr7="./sil/rsp_sil/cal_bin/flr7/usc_flr7"

SRR7E:
--override_repository=usc_srr7e="./sil/rsp_sil/cal_bin/srr7e/usc_srr7e"

##### Build flag for Enabling Fault Injection in Radar Capability
**Optional** flag to enable fault injection in Radar Capability Module
--rc_fi=true

##### Build flag for run multiple scan index:
--//sil/rsp_sil/main/rsp_wrapper_interface/psp_sil_interface:enable_psp_sil_mutilple_scans=True

#### 4.9  SPBB tag used:
For Release verification done with Spbb Tag **https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-building_blocks-local/spbb/saf85xx/releases/3.12/spbb_saf85xx_3.12.00.zip**

#### 4.10  AFBB tag used:
For Release verification done with afbb Tag **https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/afbb/saf85xx/releases/1.6/gen7_saf85xx_afbb_1.6.01.zip**

#### 4.11 IDBB tag used:
For Release verification done with idbb Tag **https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/idbb/awr294x/releases/2.0.0/core_radar_interference_detection_2.0.0_08.zip**

#### 4.12 RCBB tag used:
For Release verification done with rcbb Tag **https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/rcbb/awr294x/releases/1.0.12/Core_Radar_Capability_1.0.12_12_01.zip**

#### 4.13 DABB tag used:
For Release verification done with dabb Tag **https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/dabb/awr294x/releases/1.0.9/Core_Radar_Dynamic_Alignment_1.0.9_03.zip**

#### 4.14 SABB tag used:
For Release verification done with sabb Tag **https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/sabb/awr294x/releases/1.0.12/Core_Radar_Static_Alignment_1.0.12_03.zip**

#### 4.15 SMC and USC Version:
`To be updated after Vehicle Log verification`
For Release verification done with SMC Version "smc_cal_Satellite_46_79_0_5" and USC Version "usc_cal_default_6_79_0_0"

# 5. Debugging SIL Executable
## 5.1. configuring VS code for debugging
1. While building RDD SIL Executable, add debug flag in command line as shown in below command.
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True --//sil/rsp_sil/main/sil_wrapper_interface:enable_logging=True --psp_sil_config=True --copt="-g" --copt="-O0" -c dbg
```
* **'-g'**: enables generating debug symbols along with executable.
* **'-O0'**: disable all compiler optimization.
* **'-c dbg'**: enables -g and disables stripping

2. In VS code in left panel open run and debug (press ctrl+shift+D) click on create launch.json file

3. Add following lines of code in json file
```
{
  "version": "0.2.0",
  "configurations": [

    {
      "name": "RDD_SIL_Debug",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/rdd_sil_Test.exe",
      "sourceFileMap": { ".": "${workspaceFolder}" },
      "stopAtEntry": true,
      "externalConsole": false,
      "MIMode": "gdb",
      "cwd": "${workspaceFolder}",
      "miDebuggerPath": "${workspaceFolder}/bazel-core_radar_gen7_saf85xx/external/_main~_repo_rules~mingw64_10_0_0_rev0/bin/gdb.exe",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        },
        {
          "description": "Set Disassembly Flavor to Intel",
          "text": "-gdb-set disassembly-flavor intel",
          "ignoreFailures": true
        }
      ]
    }
  ]
}
```

* ```"program": "${workspaceFolder}/PATH/TO/EXECUTABLE"``` is relative path from project repository to executable generated after building RDD SIL Executable **bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/rdd_sil_Test.exe**.
  On linux remove the .exe and pay attention to capitalization.
* ```"sourceFileMap": { ".": "${workspaceFolder}" }``` provides a replacement for source file location in the debug symbols.
  On linux, replace the "." with "/proc/self/cwd".
* ```"miDebuggerPath": "${workspaceFolder}/bazel-bazel-core_radar_gen7_saf85xx/external/mingw64_10_0_0_rev0/bin/gdb.exe"``` is the path to gdb.exe from mingw. (this folder becomes available only after building CDC SIL executable via bazel)
  On linux set this to /usr/bin/gdb.

1. To debug the RDD SIL, first build and make sure the path to executable is correct.
2. Add breakpoints to lines in vs code
3. In run and debug panel click on play button to start debugging (press F5). this will start debugging session

## 6. SIL in ITF
### 6.1. Introduction

SIL in ITF extends the existing RDD SIL flow by exposing the RDD SIL functionality as a DLL (Dynamic Link Library) instead of using only the static test executable flow.
The main purpose of this DLL based integration is to enable invocation of RDD SIL from MATLAB through the existing ITF framework.

In this implementation, the same RDD SIL logic used by the existing **rdd_sil_Test** flow is reused, but it is built and exported as a dynamic library.
The ITF invokes this RDD SIL DLL and returns the corresponding RDD SIL output.

### 6.2. SIL in ITF Framework

The SIL in ITF flow is based on the already existing RDD SIL processing pipeline and configuration mechanism documented in the RDD SIL framework. The main difference is that, instead of directly running the standalone SIL test executable, the RDD SIL functionality is packaged as a DLL and invoked through ITF.

The overall flow is as follows:
- Input is prepared in ITF
- ITF invokes the RDD SIL DLL
- The DLL internally executes the RDD SIL processing flow
- The corresponding RDD SIL output is returned back to ITF
- The output can then be consumed in MATLAB through the ITF integration path

Since this DLL uses the same underlying RDD SIL functionality, the output behavior is aligned with the existing RDD SIL output definition. The current RDD SIL output structures documented in the design are:
- Rdd_Stream_T
- Detection_Stream_T
- Down_selection_Stream_T
- Detection_Debug_Stream_T

### 6.3. Input Modes Supported in SIL in ITF

SIL in ITF supports two modes of providing input data to the RDD SIL DLL.

###### 6.3.1 MEX based input
- In MEX based flow, the input is taken from MEX Executable
- The MEX data is bypassed as input to the RDD SIL flow before invoking the SIL DLL

###### 6.3.2 LOG based input
- In LOG based flow, the input is taken from log data
- The required log data is used as input to the RDD SIL DLL through the ITF flow
- This mode is used mainly when logs are available and those logs need to be replayed

### 6.4. RDD SIL DLL

The RDD SIL DLL is functionally equivalent to the existing **rdd_sil_Test** library flow, but it is built as a dynamic library so that it can be integrated with MATLAB through ITF.

The DLL based approach provides the following advantages:
- Reuse of existing RDD SIL implementation
- Easier integration with ITF
- MATLAB invocation support through dynamic linking
- Consistent SIL behavior with the existing RDD SIL framework

The DLL uses the same RDD SIL processing concept already described in the existing framework, where the RDD SIL API and related SIL wrapper flow are used as the base implementation.

### 6.5. Build Command

The SIL in ITF DLL can be built using the following bazel commands.

FLR7:
bazelisk build //:rdd_sil_lib --variant=flr7 --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True --psp_sil_config=True

SRR7P:
bazelisk build //:rdd_sil_lib --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True --psp_sil_config=True

The build command pattern is consistent with the existing RDD SIL build configuration, where variant selection, SPBB simulator configuration, AFBB SIL enablement, and PSP SIL configuration are passed through Bazel flags.

### 6.6. Usage Summary

SIL in ITF is intended for cases where the existing ITF framework is used as the execution path for RDD SIL.
Instead of invoking the standalone executable directly, ITF invokes the DLL version of the RDD SIL library and returns the corresponding outputs.

This provides a reusable integration path for MATLAB while preserving the same core RDD SIL behavior.

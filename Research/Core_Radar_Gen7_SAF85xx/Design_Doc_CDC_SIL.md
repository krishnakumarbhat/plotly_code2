# ALL ABOUT CDC SIL EXECUTABLE BUILD PROCEDURE
-------------------------
# Table of contents
- [ALL ABOUT CDC SIL EXECUTABLE BUILD PROCEDURE](#all-about-CDC-sil-executable-build-procedure)
- [Table of contents](#table-of-contents)
    - [1. Introduction](#1-introduction)
    - [2. Tools Required](#2-tools-required)
    - [3. External Repo Required](#3-external-repo-required)
    - [4. CDC SIL Framework](#4-CDC-sil-framework)
            - [4.1. CDC SIL API](#41-CDC-sil-api)
            - [4.2. CDC SIL IO Structure](#42-CDC-sil-io-structure)
                    - [4.2.1. CDC SIL Input Structure](#421-CDC-sil-input-structure)
                    - [4.2.2. CDC SIL Output Structure](#422-CDC-sil-output-structure)
            - [4.3. ID SIL API IO Structure](#43-id-sil-api-io-structure)
                - [4.3.1. ID SIL API Input Structure](#431-id-sil-api-input-structure)
                - [4.3.2. ID SIL API Output Structure](#432-id-sil-api-output-structure)
            - [4.4. RC SIL API IO Structure](#44-rc-sil-api-io-structure)
                - [4.4.1. RC SIL API Input Structure](#441-rc-sil-api-input-structure)
                - [4.4.2. RC SIL API Output Structure](#442-rc-sil-api-output-structure)
            - [4.5. DA SIL API IO Structure](#45-da-sil-api-io-structure)
                - [4.5.1. DA SIL API Input Structure](#451-da-sil-api-input-structure)
                - [4.5.2. DA SIL API Output Structure](#452-da-sil-api-output-structure)
            - [4.6. SA SIL API IO Structure](#46-sa-sil-api-io-structure)
                - [4.6.1. SA SIL API Input Structure](#461-sa-sil-api-input-structure)
                - [4.6.2. ID SIL API Output Structure](#462-id-sil-api-output-structure)
            - [4.7. CDC SIL Library / Header file  dependencies](#47-CDC-sil-library--header-file--dependencies)
                - [4.7.1. CDC SIL header file path:](#471-CDC-sil-header-file-path)
                - [4.7.2. CDC SIL Library to be include](#472-cdc-sil-library-to-be-include)
                - [4.7.3. External spbb libs](#473-external-spbb-libs)
                - [4.7.4. External afbb libs](#474-external-afbb-libs)
                - [4.7.5. External idbb libs](#475-external-idbb-libs)
                - [4.7.6. External rcbb libs](#476-external-rcbb-libs)
                - [4.7.7. External dabb libs](#477-external-dabb-libs)
                - [4.7.8. External sabb libs](#478-external-sabb-libs)
                - [4.7.9. CDC SIL Library folder path:](#479-CDC-sil-library-folder-path)
                - [4.7.10. ID SIL Library folder path:](#4710-id-sil-library-folder-path)
                - [4.7.11. RC SIL Library folder path:](#4711-rc-sil-library-folder-path)
                - [4.7.12. DA SIL Library folder path:](#4712-da-sil-library-folder-path)
                - [4.7.13. SA SIL Library folder path:](#4713-sa-sil-library-folder-path)
            - [4.8. CDC SIL BUILD Command:](#48-CDC-sil-build-command)
                - [4.8.1. Build flag for Logging CDC Detection Data:](#481-build-flag-for-logging-CDC-detection-data)
                - [4.8.2. Build flag for Overriding SMC data:](#482-build-flag-for-overriding-smc-data)
                - [4.8.3. Build flag for Overriding USC data:](#483-build-flag-for-overriding-usc-data)
                - [4.8.4. Build flag for Enabling Fault Injection in Radar Capability](#484-build-flag-for-enabling-fault-injection-in-radar-capability)
            - [4.9. SPBB tag used:](#49-spbb-tag-used)
            - [4.10. AFBB tag used:](#410-afbb-tag-used)
            - [4.11. IDBB tag used:](#411-idbb-tag-used)
            - [4.12. RCBB tag used:](#412-rcbb-tag-used)
            - [4.13. DABB tag used:](#413-dabb-tag-used)
            - [4.14. SABB tag used:](#414-sabb-tag-used)
            - [4.15. SMC and USC Version:](#415-smc-and-usc-version)
- [Debugging SIL Executable](#debugging-sil-executable)
    - [1. configuring VS code for debugging](#1-configuring-vs-code-for-debugging)

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

## 4. CDC SIL Framework
CDC to AF Detections

#### 4.1. CDC SIL API
The CDC SIL API **CDC_To_Detection_Configuration** is defined in **CDC_sil_interface.cpp**  in path shared  below :
**Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\CDC_sil_interface\CDC_sil_interface.cpp**

* The file **CDC_sil_api.hpp** includes the declaration of CDC SIL API.
* The design flow Digram for CDC SIL API has shown in the below picture.

!["CDC_SIL_Block_Diag_Design"](CDC_SIL_Block_Diag_Design.png)

#### 4.2.  CDC SIL IO Structure
For current implementation the CDC data input (CFAR Threshold and MPRB) are provided in .bin format .
    CDC Data file path:**Core_Radar_Gen7_SAF85xx\sil\rsp_sil\data_bin\srr7p\rdd_data\CDC_stream.bin**

###### 4.2.1 CDC SIL Input Structure
The CDC Input Output structure has been kept same as that used by RESIM Embedded Library

Parsed Entire CDC Frame ->

* SIL_CDC_Stream_T

  * uint32_t num_cdc_streams

  * CDC_Stream_T cdc_stream_array[SPBB_MAX_CDC_DBIN_COUNT_PER_FRAME/TOTAL_CDC_RECORDS_PER_FRAME]


* CDC_SIL_Input_Tag

  * Radar_Look_T look_type

  * SIL_CDC_Stream_T sil_cdc_stream

  * Rdd_Stream_T rdd_sil_in

  * Vse_Stream_T vse_sil_in

  * PSP_Input_Sil_T psp_sil_in

  * Radar_Capability_Stream_T rc_sil_in

###### 4.2.2 CDC SIL Output Structure

* CDC_SIL_Output_T

  * Rdd_Stream_T rdd_sil_out

  * Detection_Stream_T af_det_sil_out

  * Down_selection_Stream_T af_ds_det_output

  * Detection_Debug_Stream_T p_af_det_debug_output

  * ID_Stream_T id_sil_out

  * Dynamic_Alignment_Log_Stream_T da_sil_out

  * Alignment_Stream_T sa_sil_out

  * Radar_Capability_Stream_T rc_sil_out


#### 4.3 ID SIL API IO Structure
##### 4.3.1 ID SIL API Input Structure
* Radar_Look_T
* Vse_Stream_T
* CDC_Stream_T
* PSP_Input_Sil_T
* CDC_Data_T
##### 4.3.2 ID SIL API Output Structure
* ID_Stream_T

#### 4.4 RC SIL API IO Structure
##### 4.4.1 RC SIL API Input Structure
* Radar_Look_T
* Detection_Stream_T
* CDC_Data_T
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

#### 4.7 CDC SIL Library / Header file  dependencies

##### 4.7.1 CDC SIL header file path:
* Core_Radar_Gen7_SAF85xx\software\common\CDC_stream.h
* Core_Radar_Gen7_SAF85xx\software\common\detection_stream.h
* Core_Radar_Gen7_SAF85xx\software\common\down_selection_stream.h
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\CDC_sil_interface\CDC_sil_api.hpp
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\CDC_sil_interface\CDC_proc.h
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
* @afbb//module/_common/radar_math:bbe_native_sim_lib
* @afbb//module:srr7e_angle_finding_module_ut_lib
* @afbb//module:flr7_angle_finding_module_ut_lib
* @afbb//module:srr7p_angle_finding_module_ut_lib
* @afbb//module/_imp/_src/common_src:angle_finding_downselect_ut_lib

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

##### 4.7.11 CDC SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\sil_wrapper_interface
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\rsp_common
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\CDC_sil_interface
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\CDC_sil_interface\test

##### 4.7.12 ID SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\id_sil_interface

##### 4.7.13 RC SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\rc_sil_interface

##### 4.7.14 DA SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\da_sil_interface

##### 4.7.15 SA SIL Library folder path:
* Core_Radar_Gen7_SAF85xx\sil\rsp_sil\main\rsp_wrapper_interface\psp_sil_interface\sa_sil_interface

#### 4.8  CDC SIL BUILD Command:
SRR7P :
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/cdc_sil_interface/test:cdc_sil_test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True
```

FLR7:
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/cdc_sil_interface/test:cdc_sil_test --variant=flr7 --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True
```

##### Build flag for Logging CDC Detection Data:
--//sil/rsp_sil/main/sil_wrapper_interface:enable_logging=True

##### Build flag for Overriding SMC data:
SRR7P :
--override_repository=smc_srr7p="./sil/rsp_sil/cal_bin/srr7p/smc_srr7p"

FLR7:
--override_repository=smc_flr7="./sil/rsp_sil/cal_bin/flr7/smc_flr7"

##### Build flag for Overriding USC data:
SRR7P :
--override_repository=usc_srr7p="./sil/rsp_sil/cal_bin/srr7p/usc_srr7p"

FLR7:
--override_repository=usc_flr7="./sil/rsp_sil/cal_bin/flr7/usc_flr7"

##### Build flag for Enabling Fault Injection in Radar Capability
**Optional** flag to enable fault injection in Radar Capability Module
--rc_fi=true

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
1. While building CDC SIL Executable, add debug flag in command line as shown in below command.
```
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/CDC_sil_interface/test:CDC_sil_Test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:pcresim_enable=True --//sil/rsp_sil/main/sil_wrapper_interface:enable_logging=True --copt="-g" --copt="-O0" -c "dbg"
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
      "name": "CDC_SIL_Debug",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/cdc_sil_interface/test/cdc_sil_test.exe",
      "sourceFileMap": {
        ".": "${workspaceFolder}"
      },
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

* ```"program": "${workspaceFolder}/PATH/TO/EXECUTABLE"``` is relative path from project repository to executable generated after building CDC SIL Executable **bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/CDC_sil_interface/test/CDC_sil_Test.exe**.
  On linux remove the .exe and pay attention to capitalization.
* ```"sourceFileMap": { ".": "${workspaceFolder}" }``` provides a replacement for source file location in the debug symbols.
  On linux, replace the "." with "/proc/self/cwd".
* ```"miDebuggerPath": "${workspaceFolder}/bazel-bazel-core_radar_gen7_saf85xx/external/mingw64_10_0_0_rev0/bin/gdb.exe"``` is the path to gdb.exe from mingw. (this folder becomes available only after building CDC SIL executable via bazel)
  On linux set this to /usr/bin/gdb.

1. To debug the CDC SIL, first build and make sure the path to executable is correct.
2. Add breakpoints to lines in vs code
3. In run and debug panel click on play button to start debugging (press F5). this will start debugging session

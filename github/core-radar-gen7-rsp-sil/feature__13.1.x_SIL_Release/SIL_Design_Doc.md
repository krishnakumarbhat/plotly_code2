# ALL ABOUT SIL EXECUTABLE BUILD PROCEDURE
-------------------------
# Table of contents
- [ALL ABOUT SIL EXECUTABLE BUILD PROCEDURE](#all-about-sil-executable-build-procedure)
- [Table of contents](#table-of-contents)
  - [1. Introduction](#1-introduction)
  - [2. Tools Required](#2-tools-required)
  - [3. External Repo Required](#3-external-repo-required)
  - [4. CDC SIL Framework](#4-cdc-sil-framework)
      - [4.1. CDC SIL API](#41-cdc-sil-api)
      - [4.2. CDC SIL IO Structure ](#42-cdc-sil-io-structure)
      - [4.3. CDC SIL Library and Header file  dependencies](#43-cdc-sil-library-and-header-file-dependencies)
      - [4.4. CDC SIL Build Command](#44-cdc-sil-build-command)
      - [4.5. SPBB tag used](#45-spbb-tag-used)
      - [4.6. SMC and USC Version](#46-smc-usc-version)
- [5. Debugging SIL Executable](#5-debugging-sil-executable)
  - [5.1. configuring VS code for debugging](#51-configuring-vs-code-for-debugging)

-------------------------
## 1. Introduction
Software-in-the-loop (SIL) is a method of testing and validating code in a simulation environment in order to quickly and cost-effectively catch bugs and improve the quality of the code. Typically, SIL testing is conducted in the early stages of the software development process, while the more complex, costlier hardware-in-the-loop (HIL) testing is done in later stages.

• For more information please refer to https://www.aptiv.com/en/insights/article/what-is-software-in-the-loop-testing

-------------------------
## 2. Tools Required
The below tools are required  to build and debug SIL executable.

• Visual Studio code
• Bazel

-------------------------
## 3. External Repo Required

SRR7P SPBB : https://gitgerrit.asux.aptiv.com/admin/repos/ADVRADAR_Gen7_Signal_Processing
AFBB: https://gitgerrit.asux.aptiv.com/q/project:Core_Radar_Gen7_AWR294x_Angle_Finding
IDBB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Interference_Detection
DABB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Dynamic_Alignment
RCBB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Capability
SABB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Static_Alignment

-------------------------
## 4. CDC SIL Framework
CDC (Compressed Data Cube) : Selected Beam Vector Arrays from 'range doppler array' based on certain threshold and  Selection logic.
All bins within rdop_avg that are above the CDC_thold, along with any that are neighbors in range or Doppler to bins above the CDC_thold, shall be identified for logging.The data that is logged for such bins is the complex beamvector, range bin, and Doppler bin. 

-------------------------
#### 4.1. CDC SIL API
The CDC SIL API **Cdc_To_Detection_Configuration** is defined in **cdc_sil_interface.cpp**  in path shared  below :
**ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\cdc_interface**

• The file **cdc_sil_api.hpp** includes the declaration of CDC SIL API.

• The design flow Digram for CDC SIL API has shown in the below picture.

!["CDC_SIL_Block_Diag_Disgn"](CDC_SIL_Block_Diag_Disgn.png)

-------------------------
#### 4.2.  CDC SIL IO Structure
For current implementation the CDC data (CDC records and Record count) and RDD data input (CFAR Thrshold and MPRB) are provided in .bin format .
    CDC data file path : **gen7_sil_wrapper\data_bin\srr7p\cdc_data\cdc_input.bin**
    RDD Data file path:**gen7_sil_wrapper\data_bin\srr7p\cdc_data\rdd_input.bin**

For DRA Internals input for SIL, sample data taken from logs is present in .bin format in the path : **gen7_sil_wrapper\data_bin\srr7p\cdc_data\dra_internals.bin**

###### 4.2.1 CDC SIL Input Structure
• Cdc_Output_Sil_T

• Rdd_Log_Input_T

• Radar_Look_T

• int16_t avg_temp

• SIL_RDD_Data_T

• SIL_Vse_Stream_T

• PSP_Input_Sil_T

###### 4.2.2 CDC SIL Output Structure
• SIL_RDD_Data_T

• Cdc_Output_Sil_T

• AF_Output_Sil_T

• ID_Output_Sil_T

• DA_Output_Sil_T

• RC_Output_Sil_T

• SA_Output_Sil_T

#### 4.3 CDC SIL Library / Header file  dependencies

##### 4.3.1 CDC SIL header file path:
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\cdc_interface\cdc_sil_api.hpp
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_af_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_cdc_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_rdd_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_id_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_da_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_rc_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_sa_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_vse_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_psp_in_stream.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\stream_header.h
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface\sil_dra_internal_stream.h

##### 4.3.2 CDC SIL Library to be include
• //gen7_sil_wrapper/main/sil_wrapper_interface:sil_wrapper_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface:cdc_sil_interface_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/rsp_common:sil_rsp_lib
• @appl_inclusion_dep//calibration/usc:usc_module
• @appl_inclusion_dep//calibration/smc:smc_module
• @appl_inclusion_dep//calibration/smc:radar_sw_config_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/af_sil_interface:af_sil_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/id_sil_interface:ID_sil_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/da_sil_interface:DA_sil_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/rc_sil_interface:rc_sil_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/sa_sil_interface:SA_sil_lib
• //gen7_sil_wrapper/main/rsp_wrapper_interface/psp_sil_interface:psp_sil_interface_lib
• //gen7_sil_wrapper/main/sil_wrapper_interface:sil_dra_internal_stream_h

##### 4.3.3 External spbb libs
• @spbb//common/test/ti/c6xsim:c6x_lib
• @spbb//cfar_process_module/cfar_process/cfar_process_imp/_src:cfar_process_lib
• @spbb//sweep_bw_process_module/sweep_bw_process/sweep_bw_process_imp/_src:sweep_bw_process_lib
• @spbb//rdd_first_pass_module/rdd_first_pass/rdd_first_pass_imp/_src:rdd_first_pass_lib
• @spbb//rdd_second_pass_module/rdd_second_pass/rdd_second_pass_imp/_src:rdd_second_pass_lib
• @spbb//cdc_tdc_process_module/cdc_tdc_process/cdc_tdc_process_imp/_src:cdc_tdc_pcresim_lib
• @spbb//common:ti_mathutils_lib
• @spbb//common:spbb_math_pcresim_lib
• @spbb//common:hwa_helpers_pcresim_lib
• @spbb//common/test/ti/hwa-c-model/hwam:hwa_sim_lib

##### 4.3.4 External afbb libs
• @afbb//module:bb_angle_finding_module_lib
• @afbb//module/_common/radar_math/pcresim:radar_math_pcresim_lib

##### 4.3.5 External idbb libs
• @idbb//module:interference_detection_feature_sil_lib
• @idbb//module:interference_detection_feature_sil_h
• @idbb//module:interference_detection_inc
• @idbb//module/id_imp/_inc:id_stream_header


##### 4.3.6 External dabb libs
• @dabb//module/dyn_alignment/dyn_alignment_api:dra_interface_sil_h
• @dabb//module/dyn_alignment/dyn_alignment_imp/_src:dyn_align_module_sources_sil
• @dabb//module/dyn_alignment/dyn_alignment_api:dyn_align_module_h

##### 4.3.6 External sabb libs
• @sabb//module/static_alignment/static_alignment_api:sra_interface_sil_h
• @sabb//module/static_alignment/static_alignment_imp/_src:static_align_module_sources_sil
• @sabb//module/static_alignment/static_alignment_api:static_alignment_algo_inc
• @sabb//module/static_alignment/static_alignment_api:static_alignment_stream_header

##### 4.3.7 External rcbb libs
• @rcbb//module/capability:radar_capability_feature_lib_h
• @rcbb//module/capability:radar_capability_feature_lib
• @rcbb//module/capability/diagnostics:rc_diag_module
• @rcbb//module/common:rc_common_headers
• @rcbb//module/capability/RC_fault_injection:rc_fault_injection_lib
• @rcbb//module/common:rc_stream_headers

##### 4.3.8 CDC SIL Library folder path:
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\rsp_common
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\cdc_interface
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\anglefinding\anglefinding_imp\_inc\SRR7P
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\anglefinding\anglefinding_imp\_src\SRR7P
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\anglefinding\anglefinding_interface
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\anglefinding\common\radar_math
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\common
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\common\calibration\usc
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\common\calibration\smc
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\sil_wrapper_interface
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\id_sil_interface
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\da_sil_interface
• ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\rc_sil_interface

#### 4.4  CDC SIL BUILD Command:
## SRR7P :
``` bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface/test:cdc_sil_Test  --variant=srr7p --@spbb//common:build_pcresim=True --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True  --copt="-g" --copt="-O0" ```

## FLR7:

``` bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface/test:cdc_sil_Test  --variant=flr7 --@spbb//common:build_pcresim=True --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True  --copt="-g" --copt="-O0" ```

## SRR7HD:
``` bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface/test:cdc_sil_Test  --variant=srr7hd --@spbb//common:build_pcresim=True --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True  --copt="-g" --copt="-O0" ```


### Build flag for Logging CDC Detection Data:
--//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True

### Build flag for enabling RDD FP CI Pruning Mode:
--//gen7_sil_wrapper/main/sil_wrapper_interface:enable_ci_pruning=True

### Build flag for enabling Fault Injection module of RCBB:
--rc_fi

Adding this flag will enable and activate the Fault Injection module present in RCBB

#### Added Macro Flag for Rest Limit Functionality:
ENABLE_REST_LIMIT: This macro is disabled by default, enable this define to integrate Rest Limit Functionality
The Rest Limit functionality and flag update is implemented as part of SIL only, this is not present for Embedded.

#### 4.5  SPBB tag used:

For R4.0 Release verification done with  Spbb Tag **https://gitgerrit.asux.aptiv.com/a/plugins/gitiles/ADVRADAR_Gen7_Signal_Processing/+archive/refs/tags/1.0.06.01.tar** .
For R5.2 Release verification done with  Spbb Tag **https://gitgerrit.asux.aptiv.com/a/plugins/gitiles/ADVRADAR_Gen7_Signal_Processing/+archive/refs/tags/1.0.07.01.tar** .

#### 4.6  SMC and USC Version:
For R4.0 Release verification done with SMC Version **smc_cal_27_72_0_0** and USC Version **usc_cal_2_72_0_2**
For R5.2 Release verification done with SMC Version **smc_cal_27_72_0_0** and USC Version **usc_cal_2_72_0_3**

# 5. Debugging SIL Executable
## 5.1. configuring VS code for debugging
1. While building CDC SIL Executable, add debug flag in command line as shown in below command.
```bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface/test:cdc_sil_Test  --//:variant=srr7p --@spbb//common:build_pcresim=True --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True  --copt="-g" --copt="-O0"```
**'-g'**: enables generating debug symbols along with executable.
**'-O0'**: disable all compiler optimization.
2. In VS code in left panel open run and debug (press ctrl+shift+D) click on create launch.json file

3. Add following lines of code in json file
```
{
    "version": "0.0.1",
    "configurations": [
        {
            "name": "CDC_SIL_Debug",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/bazel-bin/gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface/test/cdc_sil_Test.exe",
            "stopAtEntry": false,
            "MIMode": "gdb",
            "cwd" : "${workspaceFolder}",
            "miDebuggerPath": "${workspaceFolder}/bazel-advradar_gen7_rsp_sil/external/mingw64_10_0_0_rev0/bin/gdb.exe"
        },
    ],
}
```
```"program": "${workspaceFolder}/PATH/TO/EXECUTABLE"``` is relative path from project repository to executable generated after building CDC SIL Executable **bazel-bin/gen7_sil_wrapper/main/rsp_wrapper_interface/cdc_interface/test/cdc_sil_Test.exe**.

```"miDebuggerPath": "${workspaceFolder}/bazel-advradar_gen7_rsp_sil/external/mingw64_10_0_0_rev0/bin/gdb.exe"``` is the path to gdb.exe from mingw. (this folder becomes available only after building CDC SIL executable via bazel)

4. To debug the CDC SIL, first build and make sure the path to executable is correct.
5. Add breakpoints to lines in vs code
6. In run and debug panel click on play button to start debugging (press F5). this will start debugging session

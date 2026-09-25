# ALL ABOUT AF SIL EXECUTABLE BUILD PROCEDURE
--------------------------------------------------------
### Introduction
This is an example executable used for supporting standalone testing of Angle Finding module by calling wrapper function for executing the Angle Finding (AF) SIL (Software-in-the-Loop) API.

### AF SIL API
The AF SIL API **AF_Configure_And_Execute** is defined in **af_sil_api.cpp**  in path shared  below :
**Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\af_sil_interface\api\af_sil_api.cpp**

• The file **af_sil_api.hpp** includes the declaration of AF SIL API.

### AF SIL IO Structure
#### AF SIL Input data
• Rdd_Stream_T *p_rdd_stream_data
• Vse_Stream_T *p_vse_stream_data
• uint8_t *p_radar_position_data

TBD: Need to implement utility functions to read the input data from .bin files

#### AF SIL Output data
• Detection_Stream_T *p_af_det_stream_data

### AF SIL LIB
The  **af_sil_lib** provides a shared interface AF SIL API **AF_Configure_And_Execute** that can be used by other modules in the project. This library abstracts the AF SIL API which configures and execute anglefinding algorithm.Lib path : **//sil/rsp_sil/main/rsp_wrapper_interface/af_sil_interface/api:af_sil_lib**

#### External Dependencies
* @Gen8_iND13400//software/bbe32/inc:mem_pool_h
* @Gen8_iND13400//software/bbe32/src:angle_finding_project_interface_lib
* @Gen8_iND13400//software/common:UDPLoggingStreamHeaders
* @Gen8_iND13400//software/common/calibrations/smc:radar_sw_config_h
* @Gen8_iND13400//software/common/calibrations/smc:smc_module
* @Gen8_iND13400//software/common/calibrations/usc:usc_module
* @Gen8_iND13400//software/common/ipc:ipc_data_h

### AF SIL BUILD Command:
#### SRR8P :
``` bazelisk build  //sil/rsp_sil/main/rsp_wrapper_interface/af_sil_interface/test:AF_sil_Test  --variant=srr8p --@afbb//module/_common:sil_config_enable=True -c dbg --copt="-g" ```

#### FLR8:

``` bazelisk build  //sil/rsp_sil/main/rsp_wrapper_interface/af_sil_interface/test:AF_sil_Test  --variant=flr8 --@afbb//module/_common:sil_config_enable=True -c dbg --copt="-g"  ```

##### Build flag for disabling BBE C-stubs on AFBB:

The AFBB library can be built with either a native (x86) implementation of the functions or with the BBE32 version of the functions.

The 'use_bbe_cstub_simulator' flag can be used to specify that it is desired to use the BBE32 C-Stub version of the functions and execute them via the simulator.

This flag defaults to True,but if the SIL wrapper runs too slowly, setting this flag to False can improve performance which will build the library on pure c implementation.

* External to the AFBB, add this to your bazel command line:
  `--@afbb//module/_common:use_bbe_cstub_simulator=False`

### Test procedure
* TBD

### AFBB Repo details:
**https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen8_iND13400_Angle_Finding**

#### AFBB tag used:

**https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-building_blocks-local/afbb/iND13400/releases/2.0/Gen8_AF_BB_2.0.07.zip**

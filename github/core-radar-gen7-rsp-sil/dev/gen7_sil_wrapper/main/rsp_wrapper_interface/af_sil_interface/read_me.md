# ALL ABOUT AF SIL STANDALONE EXECUTABLE BUILD PROCEDURE
--------------------------------------------------------
### Introduction
This mode is used for supporting standalone testing of Angle Finding module.


### AFBB Repo details:
**https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen7_AWR294x_Angle_Finding**


### AF SIL API
The AF SIL API **AF_Process_Execute** is defined in **af_sil_interface.cpp**  in path shared  below :
**ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\main\rsp_wrapper_interface\af_sil_interface**

• The file **af_sil_api.hpp** includes the declaration of AF SIL API.

### AF SIL IO Structure
For current implementation the AF input data are provided in .bin format .
    RDD data file path : **gen7_sil_wrapper\data_bin\srr7p\af_data\af_input.bin**

### AF SIL Input Structure
• SIL_RDD_Data_T

### AF SIL Output Structure
• AF_Output_Sil_T

### AF SIL BUILD Command:
#### SRR7P :
``` bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/af_sil_interface/test:af_sil_Test  --variant=srr7p  --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True --build_pcresim=True --copt="-g"  --copt="-O0" ```

#### FLR7:

``` bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/af_sil_interface/test:af_sil_Test  --variant=flr7  --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True --build_pcresim=True --copt="-g"  --copt="-O0"  ```

#### SRR7HD:
``` bazelisk build //gen7_sil_wrapper/main/rsp_wrapper_interface/af_sil_interface/test:af_sil_Test  --variant=srr7hd  --//gen7_sil_wrapper/main/sil_wrapper_interface:enable_logging=True --build_pcresim=True --copt="-g"  --copt="-O0"  ```

### AFBB tag used:

**https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/afbb/awr294x/releases/1.0/gen7_awr294x_afbb_1.0.01.zip**

# ALL ABOUT RDD SIL EXECUTABLE BUILD PROCEDURE
------------------------------
# Table of Contents

------------------------------

## 1. Introduction
Software-in-the-loop (SIL) is a method of testing and validating code in a simulation environment in order to quickly and cost-effectively catch bugs and improve the quality of the code. Typically, SIL testing is conducted in the early stages of the software development process, while the more complex, costlier hardware-in-the-loop (HIL) testing is done in later stages.

For more information please refer to https://www.aptiv.com/en/insights/article/what-is-software-in-the-loop-testing

## 2. Tools Required
The below tools are required  to build and debug SIL executable.
* Visual Studio code
* Bazel

## 3. External Repo Required

FLR8 SPBB : https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen8_iND13400_Signal_Processing

AFBB: https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen8_iND13400_Angle_Finding

## 4. RDD SIL Framework
RDD (Radar Data Detection)

#### 4.1. RDD SIL API
The RDD SIL API **Rdd_To_Detection_Configuration** is defined in **rdd_sil_interface.cpp**  in path shared  below :
**Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\rdd_sil_interface.cpp**

* The file **rdd_sil_api.hpp** includes the declaration of RDD SIL API.
* The design flow Digram for RDD SIL API has shown in the below picture.

["RDD_SIL_Block_Diag_Design"](RDD_SIL_Block_Diag_Design.png)

#### 4.2. RDD SIL IO Structure
For current implementation the RDD data input (CFAR Threshold and MPRB) are provided in .bin format .

###### 4.2.1 RDD SIL Input Structure

* RDD_Data_T
* Radar_Look_T

###### 4.2.2 RDD SIL Output Structure

* RDD_Data_T
* Detection_Stream_T

###### 4.2.3 RDD SIL Wrapper Interface prototype
Rdd_To_Detection_Configuration(Radar_Look_T look_id,
Rdd_Stream_T *p_rdd_data_in,
Rdd_Stream_T *p_rdd_data_out,
Detection_Stream_T *p_af_det_output)


#### 4.3 RDD SIL Library / Header file  dependencies

#### 4.3.1 RDD SIL header file path:
* Core_Radar_Gen8_iND13400\software\common\rdd_stream.h
* Core_Radar_Gen8_iND13400\software\common\detection_stream.h
*
* Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\rdd_sil_interface.hpp
* Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\af_sil_interface\af_sil_api.hpp

#### 4.3.2 External spbb libs
* @spbb//common:bbe_native_sim_lib
* @spbb_include//:spbb_include_h

#### 4.3.3 External afbb libs
* ToDo: add lib here

#### 4.3.4 RDD SIL Library folder path:
* Core_Radar_Gen8_iND13400\sil\rsp_sil\main\sil_wrapper_interface
* Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\common
* Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface
* Core_Radar_Gen8_iND13400\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\test

#### 4.4 RDD SIL BUILD Command:
FLR8:
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=flr8   --@afbb//module/_common:sil_config_enable=True --psp_sil_config = true  --@afbb//module/_common:use_bbe_cstub_simulator=False
SRR8P:
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr8p   --@afbb//module/_common:sil_config_enable=True --psp_sil_config = true  --@afbb//module/_common:use_bbe_cstub_simulator=False

Debug command:
FLR8:
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=flr8 --copt="-O0" --copt="-g" --copt="-fno-exceptions" "--strip=never"
SRR8P:
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr8p --copt="-O0" --copt="-g" --copt="-fno-exceptions" "--strip=never"


#### Build flag for Logging RDD Detection Data:
TBD

#### 4.5 SPBB tag used:
TBD

#### 4.6 AFBB tag used:
TBD

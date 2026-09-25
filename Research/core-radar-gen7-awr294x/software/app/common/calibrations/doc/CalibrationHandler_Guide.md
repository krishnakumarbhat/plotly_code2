# All About Calibration Handler

[TOC]

-------------------------
## 1. Introduction
The radar uses several calibration data for proper functioning such as USC (Unit Specific Calibration), SMC (System Modifiable Calibration), PSC (Platform Specific Calibration), TRK (Tracker Calibration) etc. These radar calibrations are generated and provided by the radar / algorithm / system engineer using certain tools such as Matlab, Python etc. The calibrations may contain sub-calibrations. Each calibration data includes **source files** (*.c/h) and **binary file** (*.ptp). The source files make it to build process while binary file makes it to flash on the ECU.

The binary file, when loaded from flash into the RAM, it needs to be validated against reference data for integrity and consistency. On successful validation the radar is turned on otherwise it's turned off. The reference data is derived from the calibration source files. The preparation of reference data previously involved manual effort by the software engineer wherein, the necessary data is extracted from source files into another file and is fed in to build process.

This calibration handler module, eliminates this manual effort. Refer the section [Working of calibration handler](#5-working-of-calibration-handler) for more details.


-------------------------
## 2. Flash memory layout
Each calibration has a dedicated memory section. Each section contains, various headers as shown in the image here.

**H1 Header**: It is for the whole calibration binary file e.g. usc_cal.ptp. It comprises of length and checksum of the whole file.

| Datatype | Field | Remark |
| --- | --- | --- |
| uint32_t  | length | Total length in bytes |
| uint32_t  | checksum | Simple checksum of the file **|


** same as *H2.section_size*

**H2 Header**: It contains version information of the calibration. Usually the versioning is in the format as here **Major.Minor.Platform.Patch**

| Datatype | Field | Remark |
| --- | --- | --- |
| uint8_t  | customer | Minor Version |
| uint8_t  | ptp_cal_type | Natural number **|
| uint8_t  | platform | Platform Version |
| uint8_t  | no_of_sections | Number of sub calibrations |
| uint8_t  | section_compatibility | Major Version |
| uint8_t  | unused1 | Future scope  |
| uint16_t | version | Patch Version |
| uint32_t | section_size | Total size of the section in bytes |
| uint32_t | unused2 | Future scope |
| uint32_t | unused3 | Future scope |

  ** *ptp_cal_type* identifies the type of calibration such as USC (1), SMC (2) etc.

**H3 Header**: It contains information about the sub-calibration of the calibration, e.g. Look Processing, RDD, AF etc., are sub-calibrations of the SMC. Versioning format is **Major.Minor**

| Datatype | Field | Remark |
| --- | --- | --- |
| uint8_t  | cal_type | Natural Number ** |
| uint8_t  | unused1 | Future scope |
| uint16_t | unused2 | Future scope |
| uint16_t | section_compatibility  | Major Version of sub-calibration |
| uint16_t | version | Minor Version of sub-calibration |
| uint32_t | section_size | Total size of the sub-calibration in bytes |

** *cal_type* identifies the type of sub-calibration.

All the header information needs to be validated against the reference data. Any mismatch shall set the error flags accordingly.

![Flash Memory Layout](img/MemoryLayout.jpg)


-------------------------
## 3. Prepare the reference data

Preparing the reference data involves two steps.

Firstly, the calibration header file shall contain some information in a defined format, this is the pre-requisite.
**Wavemaker** is a matlab based tool. It already generates the calibration header files in the defined format. However its not mandatory to use wavemaker.
Other way around one can refer the existing sample smc_cal.h or usc_cal.h files, on the similar lines create the new one. For example, psc_cal.h (Platform Specific Calibration) was created by refering smc_cal.h but not by Wavemaker.

Secondly, fetch the reference data from the calibration header files such as smc_cal.h, usc_cal.h etc.

### 3.1 Format of the calibration header file
The calbiration header file shall contain
* H2 header information as macros
* H3 header informatin for each sub-calibration as macros
* Single structure for the whole calibration e.g. **SMC_Cal_T**, **USC_Cal_T**, **PSC_Cal_T** etc.
* Tokenized self referencing macros, for more information refer smc_cal.h or usc_cal.h

### 3.2 Read the calibration header files
The calibration handler module shall refer and gather the information from the tokenized calibration header files such as smc_cal.h, usc_cal.h, psc_cal.h etc. ([refer the section above on how to format the header file](#31-format-of-the-calibration-header-file))


-------------------------
## 4. How to use calibration handler
Follow the steps below,

### 4.1 Prepare calibration header file
Refer the section [Format of the calibration file](#31-format-of-the-calibration-header-file)

### 4.2 Add new calibration to the list
In the file **calibration.h**, add the new calibration to **CAL_TABLE** similar to the existing USC and SMC calibrations

### 4.3 Include new cal files in the build
The new calibration files (*.c/h) shall be included in the build as needed by the build. Ensure the calibration source files are latest.

### 4.4 Build the software
That's it. Calibration handler shall take care of the rest


-------------------------
## 5. Working of calibration handler

The below steps are followed for every calibration listed by the **calibration.h** file

### 5.1 Read the calibration header file
Prepare the reference data for validating the flash content of the calibration by reading the tokenized calbiration header file. Refer the section [Read the calibration header files](#32-read-the-calibration-header-files)

### 5.2 Validate the header information
Use the reference data thus available and compare it with the flash content of the calibration data. Usually the header infromation viz, **H2 Header** and **H3 Header** are validated. Error flags shall be set accordingly if the fields do not match. The variable *PTP_Calib_Error* captures several types of errors for each calibration as below,

| Field | Remark |
| --- | --- |
| ptp_size_err | Set if H1 'size' mismatch |
| ptp_load_err | Set if H2/H3 headers mismatch |
| ptp_h2_err | Set if H2 header mismatch |
| ptp_h3_err | Set if H3 header mismatch |
| byte_diff_err | Set if data mismatch **|
| load_err | Set if ptp_size_err or ptp_load_err |


** disabled for now, can be enabled if necessary

For more information on header refer [Flash memory layout](#2-flash-memory-layout)


# 6. File Revision History
|Rev|Date|NetId|Name|SCR|
|---|---|---|---|---|
|1.0|19-Oct-2023| bz571t| Umesh| DDR-2713|

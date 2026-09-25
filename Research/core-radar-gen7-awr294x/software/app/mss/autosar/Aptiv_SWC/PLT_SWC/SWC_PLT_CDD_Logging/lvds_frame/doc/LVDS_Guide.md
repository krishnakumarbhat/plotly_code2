# ALL ABOUT Low Voltage Differential Signaling(LVDS)
-------------------------
# Table of contents
1. [Introduction](#1-introduction)
2. [Advantages of LVDS](#2-Advantages-of-LVDS)
2.1. [High Data Rate and Long Distance](#21-High-Data-Rate-and-Long-Distance)
2.2. [Lower Power Consumption](#22-Lower-Power-Consumption)
2.3. [Noise Immunity](#23-Noise-Immunity)
3. [LVDS Interface Signals](#3-LVDS-Interface-Signals)
3.1. [LVDS bit clock](#31-LVDS-bit-clock)
3.2. [LVDS data lanes](#32-LVDS-data-lanes)
3.3. [LVDS frame clock](#33-LVDS-frame-clock)
4. [LVDS Frame](#4-LVDS-Frame)
5. [LVDS Configuration](#5-LVDS-Configuration)
6. [LVDS Streaming](#6-LVDS-Streaming)
7. [Process for data transfer](#7-Process-for-data-transfer)
8. [Functions](#8-Functions)
9. [Reference](#9-Reference)
10. [File Revision History](#10-file-revision-history)
-------------------------
## 1. Introduction

LVDS technology uses differential data transmission. The differential scheme has a tremendous advantage over single-ended schemes as it is less susceptible to common mode noise. Noise coupled onto the interconnect is seen as common mode modulations by the receivers and is rejected. The receivers respond only to differential voltages.

-------------------------
## 2. Advantages of LVDS

### 2.1. High Data Rate and Long Distance:
* LVDS can Achieve speed of 900Mbps for long distance.
* LVDS having high data rate because of low voltage swing.
* It’s having 350mV voltage swing which takes 0.8 nanoSecond for LDVS to switch from High to low or Low to high

### 2.2. Lower Power Consumption:
* Because of Low voltage swing it consume low power

### 2.2. Noise Immunity:
* Since the receiver only cares about the voltage potential between the two wires, the external noise Will be canceled out

-------------------------
## 3. LVDS Interface Signals:

### 3.1. LVDS bit clock:
* it is typically center-aligned and both clock edges can be used to latch serial ADC data

### 3.2. LVDS data lanes :
* Transmitting data

### 3.3. LVDS frame clock:
* frame clock rising and falling edges are aligned with the transitions of data. It helps the receiver to correctly load parallel data after de-serialization

LVDS Interface timings is shown in below image,

![LVDS' Signal Interface](LVDS_Signal_Interface.PNG)

-------------------------
## 4. LVDS Frame:

LVDS Frame divided into 3 parts

### 4.1. LVDS Header:
LVDS header shall be defined in **stream_handler.h**. Some of the fields available at the moment for LVDS are as below,

* **LVDS Protocol Version:** Version of the LVDS protocol
* **LVDS Header Length:** Size of the LVDS Header
* **LVDS Stream Type:** for type of stream information
* **Aptiv Header Length:** Size of Aptiv Header
* **APTIV Payload Length:** Size of Aptiv Payload data
* **LVDS frame counter:**  Counts LVDS frame

**note:** More fields shall be added as per need.

### 4.2. APTIV Stream Header:

Refer the document below for details, **~\software\app\mss\autosar\Aptiv_SWC\PLT_SWC\SWC_PLT_CDD_Logging\stream_handler\doc\StreamHandler_Guide.md**

### 4.3. APTIV Stream Payload:

Refer the document below for details, **~\software\app\mss\autosar\Aptiv_SWC\PLT_SWC\SWC_PLT_CDD_Logging\stream_handler\doc\StreamHandler_Guide.md**

LVDS frame structure shown in below image
![LVDS' Frame Structure](LVDS_Frame.PNG)

-------------------------
## 5. LVDS Configuration:

The current LVDS configurations are below
* Number of lanes enabled - 2
* Clocking Mode - Double Data Rate(DDR)
* Most Significant Bit (MSB) first
* CRC Disabled
* LVDS data rates - 400 Mbps (200-MHz DDR Clock)
* Number of Buffers configured - 1
* LVDS Bus - Point to Point

**Note:** Configuration can be changed based on need.

-------------------------
## 6. LVDS Streaming:

* The LVDS streaming feature enables the streaming of HW data (ADC data) and/or user specific SW data through LVDS interface
* The streaming is done  by the CBUFF and EDMA peripherals with minimal CPU intervention
* The total amount of data to be transmitted in a HW or SW packet must be greater than the minimum required by CBUFF, which is 64 bytes or 32 CBUFF Units (this is the definition CBUFF_MIN_TRANSFER_SIZE_CBUFF_UNITS in the CBUFF driver implementation)
* If above threshold condition is violated, the CBUFF driver will return an error during configuration and the setup will generate a fatal exception as a result
* Data shall pass through EDMA(Enhanced Direct Memory Access) Channel

-------------------------
## 7. Process for data transfer:

* Configure the data of streams(Detection,Header,Status,RDD,VSE,CDC,Debug,MMIC and Calibration)
* Create Cbuff session
* Allocate a Cbuff session
* Validate Cbuff configuration to check size of user data it should be less than CBUFF_MAX_TRANSFER_SIZE_CBUFF_UNITS(i.e. 16383 bytes) and update configuration with EDMA address
* Validate Hardware/Software session configuration to make sure user buffers can be streamed out via the Cbuff
* Transfer the data
* Frame Done interrupt execute for multiple session transfer. For single session it’s not required

Timing Diagram shown in below image
![Timing Diagram](Timing_Diagram.PNG)

-------------------------
## 8. Functions:

* **LVDS_Frame_Config_Init:**  This function is initialize constant data of LVDS frame
* **LVDS_Frame_Configuration:** This function fill the buffer,non constant data of LVDS frame and send LVDS frame through TransferLVDSData function
* **RE_Logging_Init:** This function call the LVDS_Frame_Configuration function if RDI board(AURIX board) is connected.

-------------------------
## 9. Reference:

* https://spo.aptiv.com/:b:/r/sites/0304-AdvEngSystems/Shared%20Documents/Micro_MMIC_DataSheets/TI/TI_AWR2944/TI%20Collaterals/awr2944.pdf?csf=1&web=1&e=mRweaz
* https://training.ti.com/lvds-overview?context=1135817-1139372-1135818
* https://en.wikipedia.org/wiki/Low-voltage_differential_signaling
* https://confluence.asux.aptiv.com/display/AASSA/SRR7p+LVDS+to+Ethernet+Gateway

-------------------------
# 10. File Revision History
|Rev|Date|NetId|Name|SCR|
|-|-|-|-|-|
|0.1|12-09-2022| tjy9rb| Deepak Kumar Pandey| DDR-1911|

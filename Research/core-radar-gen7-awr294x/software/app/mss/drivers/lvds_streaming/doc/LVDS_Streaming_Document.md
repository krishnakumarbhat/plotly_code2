# 1. LVDS Streaming
    Details about LVDS Streaming used in ADVRADAR_AWR294X repository.

 ## 1.1. Table of Contents
-------------------------

[TOC]

-------------------------

## 1.2. Introduction

### 1.2.1. Scope
    This document describes the necesary information for use of LVDS streaming in the Generation 7 radars for Aptiv.

### 1.2.2. Definition of Terms
    | Abbreviation / Acronym | Description                         |
    | ---------------------- | ----------------------------------- |
    | LVDS                   | Low Voltage Differential Signalling |
    | CBUFF                  | Common Buffer                       |
    | EDMA                   | Enhanced Direct Memory Access       |

### 1.2.3. Supporting Documents

#### 1.2.3.1. TI Reference Manual
File Name: AWR294x_TRM.pdf
This document further describes RTI functionality of AWR294x microcontroller

## 1.3. Background

LVDS interface is used for streaming logging data when RDI board is connected.
CBUFF & EDMA are other two dependency modules which need to be configured for LVDS to function.

## 1.4. Specifications

### 1.4.1. Configurations

Some of the important configuration info are below:

    | Parameter              | Value                                            |
    | ---------------------- | ------------------------------------------------ |
    | No of Lanes used       | 2                                                |
    | Clock Mode             | 1 (DDR)                                          |
    | Data Sent out          | MSB First                                        |
    | EDMA channels used     | Channels 18, 19 & 20 from DSS EDMA C Instance    |
    | EDMA Shadow Channels   | Channels 70, 71 & 72                             |
    | Interrupt Mode         | Polling                                          |

There are reserved EDMA channels given below, only those need to be used for LVDS Streaming purpose.

    | DMA Number    | EDMA Physical Channel(s) |
    | ------------- | ------------------------ |
    |   18          | EDMA_DSS_TPCC_A/B/C      |
    |   19          | EDMA_DSS_TPCC_A/B/C      |
    |   20          | EDMA_DSS_TPCC_A/B/C      |
    |   21          | EDMA_DSS_TPCC_A/B/C      |
    |   22          | EDMA_DSS_TPCC_A/B/C      |
    |   23          | EDMA_DSS_TPCC_A/B/C      |
    |   24          | EDMA_DSS_TPCC_A/B/C      |

### 1.4.2. Error Codes

In the Main Control Block structure "LVDSStreamMCB", "errStatus" holds the error code if any has occurred.
The description of the error codes are below:

    | Error Code             | Description                                      |
    | ---------------------- | ------------------------------------------------ |
    | 0x10                   | CBUFF Init Failure                               |
    | 0x11                   | CBUFF Create Session Failure                     |
    | 0x12                   | CBUFF Deactivate Session Failure                 |
    | 0x13                   | CBUFF Delete Session Failure                     |
    | 0x20                   | EDMA Init Failure                                |
    | 0x21                   | EDMA Channel Not Available                       |
    | 0x30                   | LVDS Lane is Busy                                |
    | 0x31                   | USer Buffer Not Configured                       |

## 1.5. External Functions

### 1.5.1. LVDSStream_Init
It initializes the necessary modules that implement the streaming - LVDS, CBUFF, EDMA.

### 1.5.2. TransferLVDSData
Starts the SW session to transfer the user provided data over LVDS interface.

## 1.6. File Revision History
|Rev|Date       |NetId  |Name           |SCR      |
|---|-----------|-------|---------------|---------|
|0.1|08-Jan-2024|xjswg6 |Ashish Hegde   |DND-2960 |

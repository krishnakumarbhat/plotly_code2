# 1. General Purpose Timer (GPT)
    Details about GPT and driver used in Core_Radar_Gen7_SAF85xx repository.

 ## 1.1. Table of Contents
-------------------------

[TOC]

-------------------------

## 1.2. Introduction
### 1.2.1. Scope
    This detailed design describes the use of GPTs in the Generation 7 version 2 radars for Aptiv.
### 1.2.2. Definition of Terms
    | Abbreviation / Acronym | Description                       |
    | ---------------------- | --------------------------------- |
    | MCAL                   | MicroController Abstraction Layer |
    | STM                    | System Timer Module               |
    | PIT                    | Periodic Interrupt Timer          |
    | FTM                    | FlexTimer                         |
    | ISR                    | Interrupt Service Routine         |

### 1.2.3. Supporting Documents
#### 1.2.3.1. NXP MCAL GPT User Manual
Location: Core_Radar_Gen7_SAF85xx_MCAL\tresos\plugins\Gpt_TS_T40D47M30I0R0\doc\RTD_GPT_UM.pdf
This document is used to refer for configuration of GPT module to enable Timer functionality.

#### 1.2.3.2. NXP MCAL GPT Integration Manual
Location: Core_Radar_Gen7_SAF85xx_MCAL\tresos\plugins\Gpt_TS_T40D47M30I0R0\doc\RTD_GPT_IM.pdf
This document is used to refer for integration of GPT module into the Radar project.

#### 1.2.3.3. NXP Reference Manual
File Name: RM708925-SAF85xx Digital Reference Manual(2.5).pdf
Location: https://spo.aptiv.com/:b:/r/sites/0304-AdvEngSystems/Shared%20Documents/Micro_MMIC_DataSheets/NXP/SmartTRX/Collateral/RM708925-SAF85xx%20Digital%20Reference%20Manual(2.5).pdf?csf=1&web=1&e=Sm5saX
This document is further describes GPT functionality of SAF85xx microcontroller

## 1.3. Background
With the introduction of the Generation 5 radars, Aptiv ADP is migrating the traditional, handcoded drivers to use the AUTOSAR MCAL layers. As part of this process, the controlling the STM, PIT, and eTimer drivers were migrated into the NXP MCAL module GPT (General Purpose Timer). However, Aptiv’s uses of timers goes above what is supported by the MCAL. Therefore, a new driver (dd_gpt) was created in Generation 6 to support the additional functionality needed. All external calls to timers should pass through this driver.

In previous generations, the System Timer Module (STM) and the Periodic Interrupt Timer (PIT) had separate drivers to control them. The eTimer module had code scattered throughout various files, without a true driver. The goal of the dd_gpt driver was to combine all the code for these 3 modules into as unified of a call structure as possible, utilizing the MCAL underneath to do most of the legwork. Upon integrating the eTimer module, it became evident that the MCAL did not support the needed functionality. Therefore, the eTimer module is predominantly not utilizing the MCAL, however its configuration was incorporated into the dd_gpt module.

In Generation 7 version 1 and 2 radars, MCAL GPT has the wrapper functionality available in the Device driver module (dd_gpt). Details on how the MCAL GPT functions work are not in the scope of this document. For further details on the MCAL GPT functions and features, please see supporting User and integration manual documents.

Following are the timer features for SAF85xx.
PIT timer module features :
• Four 32-bit counters per module
• Independent timeout periods for each timer
• Independent interrupt source.

STM timer module features :
• One 32-bit up counter with 8-bit prescaler (1 to 256) per module.
• Four 32-bit compare channels
• Independent interrupt source for each channel.
Note: STM 0 Channel 0 is reserved for OS timer in this project.

FTM timer module features :
• One 16-bit up counter with 16-bit
• Prescaler divide-by 1, 2, 4, 8, 16, 32, 64, or 128
• Independent interrupt source for each channel.

## 1.4. External Functions
### 1.4.1. Timer Functions

#### 1.4.1.1. Description
The following section details the functions which will work for all of the different timer modules. Any limitations are noted inside of the description. For details on the input/output arguments, see the source and header file directly.

#### 1.4.1.2. GPT_Set_Timer_Cnt_Value_ms
This is the main function to set a timer for a certain number of milliseconds. This is the preferred function to use, as the inputs are in engineering units and the user does not need to know the timer clock frequency.

#### 1.4.1.3. STM_Set_Timer_Cnt_Value_us
Provides a means of setting a STM Device count to a certain value in microseconds. This can be used to reset a timer, or, in our case, assist with Radar Time Sync, where a timer needs to be adjusted to a “master clock”.

#### 1.4.1.4. STM_Get_Timer_Cnt_Value_us
Provides the STM timer count value in integer microseconds back to the user.

#### 1.4.1.5. Get_Timestamp_ms
Provides a timestamp in milliseconds calculated as the sum of current timestamp and elapsed time value. Elapsed time value is calculated based on elapsed time tick value.

#### 1.4.1.6. Get_Elapsed_Time_ms
Provides an elapsed time in milliseconds when provided a starting timestamp, calculated as difference between starting and ending timestamp. Ending timestamp is the timestamp when the elapsed time needs to be calculated.

#### 1.4.1.7. Get_Timestamp_us
Provides an timestamp in microseconds.
Limitations: The max value for this timestamp is one hour. If you need a timer for longer than this, use the ms timestamp.

#### 1.4.1.8. Get_Elapsed_Time_us
Provides an elapsed time when provided a starting timestamp, which should be obtained via Get_Timestamp_us()
Limitations: The max value for this timestamp is one hour. If you need a timer for longer than this, use the ms timestamp.

#### 1.4.1.9. GPT call-back Notifcation
The GPT Driver provides a notification per channel that is called whenever the defined time period is over.
Gpt_StartTimer shall invoke the timer and when this time period is over this user notification shall be invoked.
An extern declaration of this function is available in Gpt_PBcfg.c file and this function has to be implemented in wrapper module for all the timers.
Respectively wrapper has implementation of notifications: Gpt_PltTime_STM2Ch0_Notification and GPT_STM_1_Notification

#### 1.4.1.10 Init_GPT_Wrapper
Init wrapper functions for free running and RTM timer configured. This function shall be handling Start and notification enabling of the timers mentioned.

#### 1.4.1.11 EcuSync_Init
Init wrapper functions for RADAR trigger, time synchronization and Platform timer configured. This function shall be handling Start and notification enabling of the timers mentioned.

## 1.5. File Revision History
|Rev|Date       |NetId  |Name           |SCR      |
|---|-----------|-------|---------------|---------|
|0.1|28-Feb-2024| lv612v| Anuja D       | GNZ-861 |
|0.2|18-Sep-2024| lv612v| Anuja D       | GNZ-1298|

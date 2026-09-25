# 1. General Purpose Timer (GPT)
    Details about GPT and driver used in ADVRADAR_AWR294X repository.

 ## 1.1. Table of Contents
-------------------------

[TOC]

-------------------------

## 1.2. Introduction
### 1.2.1. Scope
    This detailed design describes the use of GPTs in the Generation 7 radars for Aptiv.
### 1.2.2. Definition of Terms
    | Abbreviation / Acronym | Description                       |
    | ---------------------- | --------------------------------- |
    | MCAL                   | MicroController Abstraction Layer |
    | RTI                    | Real-Time interrupt               |
    | WDG                    | Watchdog                          |
    | FTM                    | FlexTimer                         |
    | ISR                    | Interrupt Service Routine         |

### 1.2.3. Supporting Documents
#### 1.2.3.1. TI MCAL GPT User Manual
Location: ADVRADAR_AWR294X\tools\tresos\mcal_docs\User_guide\MCAL_GPT_UserGuide.pdf
This document is used to refer for configuration of GPT module to enable Timer functionality.

#### 1.2.3.2. TI Reference Manual
File Name: AWR294x_TRM.pdf
This document further describes RTI functionality of AWR294x microcontroller

## 1.3. Background
With the introduction of the Generation 5 radars, Aptiv ADP is migrating the traditional, handcoded drivers to use the AUTOSAR MCAL layers. As part of this process, the controlling the STM, PIT, and eTimer drivers will be migrated into the NXP MCAL module GPT (General Purpose Timer). However, Aptiv’s uses of timers goes above what is supported by the MCAL. Therefore, a new driver (dd_gpt) was created to support the additional functionality needed. All external calls to timers should pass through this driver.

In previous generations, the System Timer Module (STM) and the Periodic Interrupt Timer (PIT) had separate drivers to control them. The eTimer module had code scattered throughout various files, without a true driver. The goal of the dd_gpt driver was to combine all the code for these 3 modules into as unified of a call structure as possible, utilizing the MCAL underneath to do most of the legwork. Upon integrating the eTimer module, it became evident that the MCAL did not support the needed functionality. Therefore, the eTimer module is predominantly not utilizing the MCAL, however its configuration was incorporated into the dd_gpt module.

In Generation 7 version 1 and 2 radars, MCAL GPT has the wrapper functionality available in the Device driver module (dd_gpt). Details on how the MCAL GPT functions work are not in the scope of this document. For further details on the MCAL GPT functions and features, please see supporting User manual document.

Following are the timer features for AWR294x.
• Two independent 64 bit counter blocks
• Windowed Watchdog Timer (WWDT) Feature
• Four configurable compares for generating operating system ticks or DMA requests. Each event can be driven by either counter block 0 or counter block 1.
• Fast enabling/disabling of events
• Two time stamp (capture) functions for system or peripheral interrupts, one for each counter block
• Digital windowed watchdog

The RTI module has two independent counter blocks for generating different timebases: counter block 0 and counter block 1. The two counter blocks provide the same basic functionality.

A compare unit compares the counters with programmable values and generates four independent interrupt or DMA requests on compare matches. Each of the compare registers can be programmed to be compared to either counter block 0 or counter block 1.

## 1.4. External Functions
### 1.4.1. Timer Functions

#### 1.4.1.1. Description
The following section details the functions which will work for all of the different timer modules. Any limitations are noted inside of the description. For details on the input/output arguments, see the source and header file directly.

#### 1.4.1.2. Init_GPT_Wrapper
This function initializes the GPT Wrapper. It must be called before any fucnctions to read or write to the timers in physical units.
Provides wrapper functions for free running, RTM timer and Platform timer configured. This function shall be handling Start and notification enabling of the timers mentioned.

#### 1.4.1.3. Start_EcuSync_Timer
Provides wrapper functions for RADAR trigger and time synchronization timer configured. This function shall be handling Start and notification enabling of the timers mentioned.

#### 1.4.1.4. Start_Wdg_Timer
Provides wrapper functions for Watchdog timer configured. This function shall be handling Start and notification enabling of the WDG timer.

#### 1.4.1.5. Stop_Wdg_Timer
Provides wrapper functions for Watchdog timer configured. This function shall handle stopping by disabling the notification of the WDG timer.

#### 1.4.1.6. Get_Timestamp_ms
Provides a timestamp in milliseconds calculated as the sum of current timestamp and elapsed time value. Elapsed time value is calculated based on elapsed time tick value.

#### 1.4.1.7. Get_Elapsed_Time_ms
Provides an elapsed time in milliseconds when provided a starting timestamp, calculated as difference between starting and ending timestamp. Ending timestamp is the timestamp when the elapsed time needs to be calculated.

#### 1.4.1.8. Get_Timestamp_us
Provides an timestamp in microseconds.
Limitations: The max value for this timestamp is one hour. If you need a timer for longer than this, use the ms timestamp.

#### 1.4.1.9. Get_Elapsed_Time_us
Provides an elapsed time when provided a starting timestamp, which should be obtained via Get_Timestamp_us()
Limitations: The max value for this timestamp is one hour. If you need a timer for longer than this, use the ms timestamp.

#### 1.4.1.10. GPT call-back Notifcation
The GPT Driver provides a notification per channel that is called whenever the defined time period is over. Gpt_StartTimer shall invoke the timer and when this time period is over this user notification shall be invoked. An extern declaration of this function is available in Gpt_PBcfg.c file and this function has to be implemented in wrapper module for all the timers. Respectively wrapper has implementation of notifications: Time_Sync_Timer_ISR and WdgTrigger_ISR

#### 1.4.1.11. Get_RTI_dev
This function returns the RTI device for given channel. Provides the RTI type RTIA, RTIB, RTIC and Invalid channel/device value.

#### 1.4.1.12. RTI_Set_Timer_Cnt_Value_us
This function sets a given timer counts to the number of microseconds provided as an input argument.

#### 1.4.1.13. RTI_Get_TimeSyncTimer_Cnt_Value_us
This function returns timer count value considering the reference as Time_Sync timer, it's counter value and elapsed time ticks from MCAL GPT module.

#### 1.4.1.14. RTI_Set_Timer_Cnt_Value_ms
This function sets a given timer counts to the number of milliseconds provided as an input argument

#### 1.4.1.15. RTI_Get_Device_Base_Address
This function provides the base address of channedId passed as an argument. The Base address of RTI channels are RTI1 is 0x02F7A000, RTI2 is 0x02F7A100 and RTI3 is 0x02F7A200.

#### 1.4.1.16. RTI_Get_Timer_Cnt_Value_Ticks
This function provides the timer value of channelId passed as argument and current Free running counter register value.

## 1.5. File Revision History
|Rev|Date       |NetId  |Name           |SCR      |
|---|-----------|-------|---------------|---------|
|0.1|25-Sep-2024| lv612v| Anuja D      | GNZ-1077|

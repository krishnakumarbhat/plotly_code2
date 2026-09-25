# All About Radar Control

[TOC]

-------------------------
## 1. Introduction
This module is mainly coordinates the initialization and triggering of radar frames or looks.

-------------------------

## 2. Module Overview

![Module Overview](img/RadarfrontendManagement.png)

Radar Control module process input from MMIC (MMIC Diagnostics - Data Discard and Configuration errors) and Mode Manager (Radar Data - Radiate Command shall be from Core 0-IPC or XCP Request) to evaluate Radar Control State.
In a running system, the state of the radar is configured or Radar transmitting state. Look_trigger and Look_Complete are the major event will control the radar in running state.
There is another two state will be there ,instead of transmitting chirps and collecting the actual data we can stub the ADC data with A2D online injection phase and the other mode is Pure CW for End-Of-Line Process.

A summary of the states is as follows :
| State | Regular operation/Debug | Feature |
| --- | --- | --- |
| OFF  | Regular operation | Radar in OFF State |
| FAULTED | Regular Operation | MMIC Faults Detected|
| INIT  | Regular Operation | Initalization of MMIC and peripherals|
| READY  | Regular Operation |State to evaluate A2D injection/CW mode requests after init/re-init|
| CONFIGURED  | Regular Operation | Data Acqusition of a RadarLook (after driver configuration)|
| TRANSMITTING  | Regular Operation | Driver diagnostics of a Radar Look and next look chnage|
| DEGRADED  | Regular Operation | Degraded-Idle mode based on over-temperature fault |
| IDLE  | Regular Operation | Degraded-Idle mode based on over-temperature fault |
| A2D_INJECTION  | Debug/EOL|A2D Injection - debug and development state only  |
| PURE_CW  | Debug/EOL | Continuous Wave Mode with no data acquisition - EOL State only |

# 3. File Revision History
|Rev|Date|NetId|Name|SCR|
|---|---|---|---|---|
|1.0|2-Feb-2024| wgxdq1|Sachu| EPB-2408|

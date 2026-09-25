# All About MMIC

[TOC]

-------------------------
## 1. Introduction
The operation of the Monolithic Microwave IC(MMIC) and the supporting radar drivers is responsible for generation and transmission of chirp, and receiving the same at receiving chain and sharing the raw ADC detection data to signal processing module.

-------------------------

## 2. Module Overview

The Radar Front End (RFE) has the following functional modules :
1.	Data Acquisition - MMIC + Radar Drivers (MIPI, SPI, CTE and SPT)
2.	Radar Control
3.	Look processing

The interaction with other RFE modules, Calibration and Instrumentation is represented by the following diagram :

![Module Overview](img/RadarfrontendManagement.png)

MMIC module communicates using radar drivers for configuration and status read from registers. Transmission and reception of chirps is controlled by the Radar Control Module.
The received information is passed to the Look Processing module for further processing. Error reporting and logging is the another major role of MMIC module.


# 3. File Revision History
|Rev|Date|NetId|Name|SCR|
|---|---|---|---|---|
|1.0|2-Feb-2024| wgxdq1|Sachu| EPB-2408|

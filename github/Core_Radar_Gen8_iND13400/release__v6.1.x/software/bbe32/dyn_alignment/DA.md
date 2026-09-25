# All About Dynamic Alignment

[TOC]

-------------------------
## 1. Introduction
Dynamic Alignment module consist of actual calculations and Algorithms to calculate dynamic misalignment. Dynamic Alignment as the name suggests continuously calculates misalignment and won't have a fixed value. The calculated misalignment values are further applied on detections to estimate the objects.

-------------------------

## 2. Module Overview

Dynamic alignment consists of 3 states:
1.Short track
2.Service Alignment
3.Auto Alignment

Service Alignment:
Service Alignment is intended to be used by workshops. After an accident the sensor may need to be replaced, thus a new misalignment value needs to be calculated. As the End of Line equipment is expensive it is not suitable to have it located in every workshop. Instead the workshop mechanic may execute the Service Alignment. The Service alignment calibrates by driving the car for a few minutes. The Service alignment is supposed to work on normal urban roads and provide a result in less then 20 minutes.

Short Track Alignment:
Short Track Alignment is intended to by used by the manufacturing plant. As the car needs to drive from the EOL station the train or truck, in order to be transported to the customer, anyway some OEMs prefer to use this short distance in order to calculate the misalignment values. This is an improved version of the Service Alignment. In order to provide a reasonable misalignment values after a short distance, the area must be suitable for Short Track alignment, e.g. by dedicated radar reflectors on its way

Auto Alignment
Auto Alignment is running constantly during typical driving operation of the customer, This is needed, in order to detect whether the sensor has moved, e.g. accident or bumpy road.

Short track and Service alignments are initial alignments which are executed on user's request. Auto alignment continuously runs on the system calculating the misalignments.

# 3. File Revision History
|Rev|Date|NetId|Name|SCR|
|---|---|---|---|---|
|1.0|31-May-2024| xj3cnw|Hema| GOC-1813|

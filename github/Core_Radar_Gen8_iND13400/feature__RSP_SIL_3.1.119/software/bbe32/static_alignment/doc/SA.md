# All About Static Alignment

[TOC]

-------------------------
## 1. Introduction
Static Alignment algorithms main purpose is to pass or fail the vehicle depending on its front center radar mounting after factory assembly.

-------------------------

## 2. Module Overview

Static Alignment is used to verify if the detection range of the radar is sufficient over the range of planned vehicle pitch variation. Along with validating the detection range of the radar, an approximate values of horizontal and vertical angles are also computed as a byproduct of this algorithm. To test and validate the radars’ detection range in its specific mounting position on a vehicle, a particular factory setup is required. The vehicle is expectedto be mounted on a hoist to guarantee a straight horizontal plane. In front of this mounted vehicle a flat plate is placed approximately at a 2m distance from the front center of the radar

Once the plate detection signals like signal strength, azimuth, elevation, range, etc., are gathered from the radar at plate positions, a series of checks are performed to validate the detection signals along with calculating the approximate value of horizontal and vertical angle of the radar.

Functional diagram of Static Alignment is given below:

![Module Overview](img/StaticAlignmentFunctionalDiag.png)
![Module Overview](img/StaticFlatePlate.png)

# 3. File Revision History
|Rev|Date|NetId|Name|SCR|
|---|---|---|---|---|
|1.0|11-Sep-2024| xj3cnw|Hema| GOC-2282|

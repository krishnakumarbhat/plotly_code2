# ALL ABOUT SENSOR POSITION
-------------------------
[TOC]

-------------------------
## 1. Introduction
Radar sensors can be mounted at various locations of the vehicle, apart from conventional positions such as RL, RR, FL and FR etc. It would be difficult to identify the mounting position by position name, and hence each position shall be identified by the ***Position_ID***. This document explains the details of associating position ID to mounting positions. The vehicle/radar systems’ could further associate the ***Position_ID*** to particular names depending on the customer or variant specific configurations.

### 1.1. Sensor Mount Locations
Table below shows examples of conventional and unconventional mount locations

|  | Conventional | Unconventional |
| --- | --- | --- |
| **Examples** | ![Conventional](img/Conventional_Positions_Ex.jpg) | ![Unconventional1](img/Unconventional_Positions_Ex1.jpg) ![Unconventional2](img/Unconventional_Positions_Ex2.jpg) |
| **Courtesy**|Gen3/5|FLR4p-Motional |

-------------------------
## 2. Vehicle Communication Interfaces

The radar sensors are configured to output detections or objects or both, along with other status information. Both detection and object shall be compliant with ISO23150. As of today, two types of interfaces are used for vehicle communication namely, CAN and Ethernet.

### 2.1. CAN Interface
Note: CAN here means CAN-FD.
When CAN is used for vehicle communication, the following assumptions or design constraints are taken in to account while preparing the dbc.

#### 2.1.1. Length of CAN identifier
* CAN identifiers length shall be 11 bits.
* Thus the range of CAN Ids is [**0x0 - 0x7FF**].

#### 2.1.2. Number of detections and objects
* CAN dbc shall support 256 detections, 64 objects along with other necessary information.
* Any other options shall be treated as customer specific requirement.

#### 2.1.3. Detections in CAN message
* Each detection shall be characterized by parameters as per ISO23150, e.g., range, range rate, azimuth, elevation etc.
* A single CAN message accommodates 4 detections, thus 64 CAN messages are required to account for 256 detections.
* As characterization parameters increase, the number of detections per message may reduce.

#### 2.1.4. Objects in CAN mesage
* Each object shall be characterized by parameters as per ISO23150.
* A single CAN message accommodates only one object and thus 64 CAN messages required to account for 64 objects.
* As characterization parameters increase, multiple CAN messages may be required per object.

#### 2.1.5. Dedicated CAN messages

* A dedicated/distinct range of CAN messages shall be reserved for each sensor mount location. viz, for detections, objects, header, status, XCP req/response/daq, Diag req/response, Functional Req, and custom messages.
* This helps to easily identify the source of information.
* This also helps independently program / flash the particular sensor of interest in the CAN network.

#### 2.1.6. Maximum Positions

* The maximum distinct sensors positions referred heretofore as ***MAX_POS***
* Taking into consideration [length](#21-can-interface), [number of detections/objects](#212-number-of-detections-and-objects), and [unique CAN messages](#215-dedicated-can-messages), ***MAX_POS*** shall be 14.
* The image below, shall serve as quick reference. For more information refer the sheet [Gen5_CAN_IDs_Pin](https://spo.aptiv.com/:x:/r/sites/0304-AdvEngSystems/AE_Software/Radar/NextGenRadar_WP_CI/SWE.2%20Software%20Architectural%20Design/CAN%20database%20interface/Gen5_CAN_ID_Range_v2_2.xlsx?d=w2f9bc277d9fb44bd89e10a3e84eda40e&csf=1&web=1&e=kq19Na).

   ![CAN_Messages](img/RangeOfCANMessages.jpg)

  ***Note:*** CAN messages IDs are in hex

### 2.2.  Ethernet Interface
* Ethernet could be used for logging purpose or vehicle communication.
* Each sensor shall have unique IP address derived by ***Position_ID***.


## 3. Sensor *Position_ID*
* Each sensor shall be identified by a unique number, the same shall be termed as ***Position_ID***.
* ***Position_ID*** shall be a natural number (1,2,3,...N)
* The sensor shall learn its mount location on its own.
* The learning of mount location could be hardware based, software based or a combination of both (Remapped).
* Software shall determine ***Position_ID*** as below,
***Position_ID = Pin_Position_Map[pin_value];***
 where,

***pin_value***: pin value derived from position pins. For more information refer [Hardware Based Sensor Position](#31-hardware-based-sensor-position)

***Pin_Position_Map***: An array indicating default mapping as below,

| Index / pin_value | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Pin_Position_Map | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |

### 3.1 Hardware Based Sensor Position
* Depending on the number of pins available in the harness, the position shall be determined. Refer the sheet [Pin_Position_Map](https://spo.aptiv.com/:x:/r/sites/0304-AdvEngSystems/AE_Software/Radar/NextGenRadar_WP_CI/SWE.2%20Software%20Architectural%20Design/CAN%20database%20interface/Gen5_CAN_ID_Range_v2_2.xlsx?d=w2f9bc277d9fb44bd89e10a3e84eda40e&csf=1&web=1&e=kq19Na)
* Each pin comes with a cost, thus some customers may or may not prefer to have pins added to the harness.
* Software shall enable the sensor position hardware circuitry (POS_GND_EN) before reading the position pins.
* Software shall read the pins, through respective ADC channels.
* Software shall read the pins, for a defined number of times.
* Software shall take into account the average value, in order to keep the  voltage transients or fluctuations at bay.
* Software shall disable the sensor position hardware circuitry (POS_GND_EN) after reading the position pins.
* The table below shows the relation between number of pins and distinct positions.

| Number of Pins Available | pin_value | Distinct Positions Possible [Position_IDs] | Comments |
| --- | --- | --- | --- |
| 0 | - | - | No pins available. SPC could be used, more information at [Software Based Sensor Position](#32-software-based-sensor-position) |
| 1: [POS_GND_A] | [0-1] | 2 [1-2] | e.g. RL & RR |
| 2: [POS_GND_B,A] | [0-3] |4 [1-4] | e.g. FL, FR, RL and RR |
| 3: [POS_GND_C,B,A] | [0-7] | 8 [1-8] | more positions |
| 4: [POS_GND_D,C,B,A] | [0-15] | 16 [1-16] | helpful for unconventional positions |

* POS_GND_x stands for POSITION GROUND for pin x.
* The ***pin_value*** shall act as an index in the array of ***Pin_Position_Map*** to determine the ***Position_ ID***

### 3.2 Software Based Sensor Position
* As the pins and related hardware circuitry comes with cost associated, some customers may prefer NOT to have the same e.g., Gen3 - HKMC.
* In such cases, USC has been used in the past.
* However, USC is generated only once after successful calibration of the radar. For system engineer to experiment with mount locations, its difficult to update the USC.
* Thus the position information is separated from USC and termed as SPC - Sensor Position Calibration.
* A dedicated flash memory section shall be allocated to SPC.
* The ***spc.ptp*** file shall contain the ***k_radar_position*** and the ***spc.k_position_id_map*** information.
* Excel based utility used to generate the same as of today.
* Table below shows the typical content of the SPC

| SPC Content | Size (bytes) | Comments |
| --- | --- | --- |
| H2_Header | 20 | ** |
| H3_Header | 12 | ** |
| ***k_radar_position*** | 1 |  uint8 |
| ***k_position_id_map***[16] | 16 | array of uint8 - for more information refer [Remapped Sensor Position](#33-remapped-sensor-position) |
| k_unsed[7] | 7 | reserved for future use (aligned) |

where,
***spc.k_radar_position*** for **CAN** based design

| *spc.k_radar_position* | Comments |
| --- | --- |
| 0 | ***spc.k_position_id_map*** shall be used instead of default ***Pin_Position_Map*** along with ***pin_value*** derived from the position pins |
| [1-14] | Valid range, software shall use the ***spc.k_radar_position***, spc.k_position_id_map fields are insignificant |
| >14 | Beyond range, set a fault, default position shall be used |

** Refer calibration handler for more information on H2 and H3 headers.

* For CAN based vehicle communication, the valid range of ***Position_ID*** shall be [1 to 14]
* For Etherenet based vehicle communication, there is no limitation as such.

### 3.3 Remapped Sensor Position
* As described in [Gen5_CAN_IDs_Pin](https://spo.aptiv.com/:x:/r/sites/0304-AdvEngSystems/AE_Software/Radar/NextGenRadar_WP_CI/SWE.2%20Software%20Architectural%20Design/CAN%20database%20interface/Gen5_CAN_ID_Range_v2_2.xlsx?d=w2f9bc277d9fb44bd89e10a3e84eda40e&csf=1&web=1&e=kq19Na), each position is associated with a range of CAN messages.
* Consider a case where there are limited number of pins available but required to use different range of CAN messages.
* In such cases, a combination of both software and hardware based logic is used.

For example, let's say only two pins are available but need to use different range of CAN messages as in here below,

| POS_GND_B | POS_GND_A | pin_value | Position_ID | Associated CAN messages | ***Planned CAN messages*** | ***Corresponding Position_ID*** |
| --- | ---  | --- | --- | --- | --- | --- |
| OPEN |OPEN | 0 | 1 | 0x100 | **0x400** | **7**  |
| OPEN |GND  | 1 | 2 | 0x180 | **0x500** | **9**  |
| GND  |OPEN | 2 | 3 | 0x200 | **0x600** | **11** |
| GND  |GND  | 3 | 4 | 0x280 | **0x700** | **13** |

where **OPEN: Logic 0** and **GND: Logic 1**

In such cases, prepare the SPC (*spc.ptp*) with the following information,

| SPC.Field | Value |
| --- | --- |
| *spc.k_radar_position*        | **0**  |
| *spc.k_position_id_map*[0]    | **7**  |
| *spc.k_position_id_map*[1]    | **9**  |
| *spc.k_position_id_map*[2]    | **11** |
| *spc.k_position_id_map*[3]    | **13** |
| *spc.k_position_id_map*[4-15] | 0      |

## 4. Determining *Position_ID*

1. Software shall check for the validity of the SPC.
2. If the valid SPC is available, software shall read the ***spc.k_radar_position*** field
   1. If ***spc.k_radar_position* = 0**, then software shall
      1. read the  hardware pins and determine ***pin_value***
      2. ***Pin_Position_Map*** shall be overwritten by ***spc.k_position_id_map*** For more information refer[Remapped Sensor Position](#33-remapped-sensor-position)

   2. If **1 ≤  *spc.k_radar_position* ≤ *MAX_POS***, indicates no hardware pins available. The software shall directly use the ***spc.k_radar_position*** as is - i.e, ***Position_ID* = *spc.k_radar_position***

   3. If ***spc.k_radar_position* > *MAX_POS***, then software shall determine the ***Position_ID*** only through hardware pins. For more information refer [Hardware Based Sensor Position](#31-hardware-based-sensor-position)
3. If SPC is invalid, software shall read hardware pins to determine the ***Position_ID***
4. The flowchart for the same is below
   ![Sensor_Position_ID](img/SensorPositionID.png)

## 5. File Revision History
| Rev | Date | NetId | Name | SCR |
| ---  | --- | --- | --- | --- |
| 0.1 | 27-Oct-2022 | bz571t | Umesh  | DDR-1837 |
| 0.2 | 02-Nov-2022 | tjy9rb | Deepak | DDR-1837 |
| 0.3 | 30-Oct-2023 | tjy9rb | Deepak | DDR-2640 |
| 0.4 | 10-Nov-2023 | bz571t | Umesh  | DDR-2739 |

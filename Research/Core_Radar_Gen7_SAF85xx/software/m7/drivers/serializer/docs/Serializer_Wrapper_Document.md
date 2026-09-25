# ALL ABOUT SERIALIZER WRAPPER FUNCTIONS
-------------------------
[TOC]

-------------------------
## 1. Introduction
The DS90UB953-Q1 serializer is part of TI's FPDLink III device family designed to support high-speed raw data sensors including 2.3MP imagers at 60-fps and as well as 4MP, 30-fps cameras, satellite RADAR, LIDAR, and Time-of-Flight (ToF) sensors.The chip delivers a 4.16-Gbps forward channel and an ultra-low latency, 50-Mbps bidirectional control channel and supports power over a single coax (PoC) or STP cable. The DS90UB953-Q1 features advanced data protection and diagnostic features to support ADAS and autonomous driving. Together with a companion deserializer, the DS90UB953-Q1 delivers precise multi-camera sensor clock and sensor synchronization.

**Note**: Scope of this document will only explain how to communicate with external device serializer over I2c communication.

## 2.I2C Configuration

        HW Channel : 1
        I2c Prescaled Shift : 0
        I2c Prescaler Divide : 4
        I2c Shift Tap Point : 5
        I2c SCL Divider : 320
        I2c SDA Hold Delay : 49
        I2c Hold Start Delay : 158
        I2c Hold Stop Delay : 161
        I2c Baud Rate : 250Khz
        I2c Asynchronous Method : Interrupt Method

### 2.1. I2C Read/Write Operation
The serial control bus consists of two signals: SCL and SDA. SCL is a Serial Bus Clock Input / Output signal and the SDA is the Serial Bus Data Input / Output signal. Both SCL and SDA signals require an external pullup resistor to VI2C, chosen to be either 1.8 V or 3.3 V.

To communicate with an I2C target, the host controller (controller) sends data to the target address and waits for a response. This response is referred to as an acknowledge bit (ACK). If a target on the bus is addressed correctly, the target Acknowledges (ACKs) the controller by driving the SDA bus low. If the address does not match a target address of the device, the target Not-acknowledges (NACKs) the controller by pulling the SDA High. ACKs also occur on the bus when data is being transmitted. When the controller is writing data, the
target ACKs after every data byte is successfully received. When the controller is reading data, the controller ACKs after every data byte is received to let the target know that the controller wants to receive another data byte. When the controller wants to stop reading, the controller NACKs after the last data byte and creates a stop condition on the bus. All communication on the bus begins with either a start condition or a repeated start condition. All communication on the bus ends with a stop condition. A READ is shown in Figure 1-1 and a
WRITE is shown in Figure 1-2.

        7bit Serializer device address : 0x19

![alt text](I2c_bus_read.png)
![alt text](I2c_bus_write.png)

### 2.2. I2C Wrapper Functions
#### 2.2.1. I2C_Write_To_Serializer()
        This Wrapper function is used to write data to the Serializer.
#### 2.2.2. I2C_Read_From_Serializer()
        This Wrapper function is used to read data from the Serializer

###### Note: Refer datasheet of DS90UB953-Q1 (Serializer) for more details.


###### DS90UB953-Q1 Datasheet link : /https://www.ti.com/lit/ds/symlink/ds90ub953-q1.pdf?ts=1709624108026&ref_url=https%253A%252F%252Fwww.google.co.in%252F

## 3. File Revision History
|Rev|Date       |NetId  |Name           |SCR      |
|---|-----------|-------|---------------|---------|
|0.1|05-MAR-2024| ulb4jq| Jagadish B    | GNZ-761 |

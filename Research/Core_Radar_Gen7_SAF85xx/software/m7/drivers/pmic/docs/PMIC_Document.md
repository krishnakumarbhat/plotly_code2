# ALL ABOUT POWER MANAGMENT IC AND ITS WRAPPER FUNCTIONS
-------------------------
[TOC]

-------------------------
## 1. Introduction
The LT3390 is a 5 channel PMIC purpose-built for automotive radar. It contains two buck regulators, a boost regulator, an LDO controller and a load-switch controller.The input supply is required to be 3.3V±2%.

Safety features include a voltage monitor on the input supply as well as monitors on 4 of the 5 output channels. Each of the switching regulators also has an over-temperature sensor. Also included is a question-answer window watchdog that is serviced via the I²C port.

The RST output will assert if any of the monitored output channels exceed its programmed monitor level, the input voltage exceeds ±5%, the temperature exceeds 150°C or the watchdog is not serviced on time or with the correct response.

### 1.1 LT3390 Register map
Table 1 shows the LT3390 PMIC register map
![alt text](Pmic_Register.png)

## 2. I2C Port Specifications
### 2.1. I2C Protocol
The LT3390 serial port is compatible with the I²C electrical specifications and uses a 400kHz clock frequency, however, the communications with the device is compatible with the SMBus read byte and write byte protocol.
The LT3390 device address is a 7-bit address(0x69).
### 2.2. I2C Message Format
![alt text](I2C_Msg_Format.png)
### 2.3. I2C Timing Diagram & Specification
**I2c Timing Diagram:**
![alt text](I2c_Timing.png)
**I2c Timing Specification:**
![alt text](I2c_Timing_Spec.png)
### 2.4. Packet Error Code (PEC)
The LT3390 serial port requires the use of an SMBus packet error check on every transaction.  The CRC-8 packet error code polynomial is C(x) = x8+ x2+ x1+ 1.

For an SMBus read, the LT3390-1 will generate a PEC byte but the host has no obligation to read or verify it.  A STOP can be issued at any time.  Conversely, the LT3390-1 will not respond to any SMBus write byte that does not also include a valid PEC byte.
### 2.5 I2C Configurations Used
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
### 2.6. Wrapper Functions
#### 2.6.1. I2c_Pmic_Read()
        Wrapper function is used to read PMIC register over i2c communication
#### 2.6.2. I2c_Pmic_Write()
        Wrapper function is used to write to PMIC register over i2c communication

## 3. Question-Answer Watchdog
The LT3390 features a windowed question-answer watchdog.

To prevent the LT3390 from resetting the system, the host micro must read the question value from LT3390 register WD_QUESTION (0x4) and respond by writing the correct answer value into LT3390 register WD_ANSWER (0x5).

The answer value is obtained by applying the CRC-8 value X⁸+X²+X+1 to the question value.

If an incorrect value is written to the WD_ANSWER register, the LT3390 will assert the reset for 128µs.

The watchdog has a minimum response time, wd_min_oper_period, of 16ms.  If a watchdog response is received less than 16ms after reset release or less than 16ms after the successful response of the previous watchdog cycle, reset will be asserted for 128µs and the fault counter will be incremented.

The watchdog has a maximum response time, wd_max_oper_period, of 144ms.  If a correct watchdog response is not provided within 144ms of the release of reset or the successful response of the previous watchdog cycle, reset will be asserted for 128µs and the fault counter will be incremented.

To facilitate sufficient system boot up time, the first watchdog maximum timeout following a reset pin release is extended to 2.24 seconds and has no minimum response period.

During the extended boot window (every watchdog cycle following reset release) it is possible to overwrite the default minimum operating watchdog period wd_min_oper_period (Register WD_MIN_PERIOD, 0x2) and the maximum default operating watchdog period wd_max_oper_period (Register WD_MAX_OPER_PERIOD, 0x3).  Once the extended boot window expires, these settings cannot be changed.

The wd_min_oper_period register value is 5 bits wide with a resolution of 16ms giving an upper limit of the minimum response time of 496ms.

The wd_max_oper_period register value is 5 bits wide with a resolution of 16ms and an offset of 16ms giving an upper limit of the maximum response time of 512ms.

The 2.24 second boot window timeout period can be factory altered from 320mseconds to 10.24 seconds upon request but is not adjustable by a register write.  This value is currently set to the factory default of 2.24 seconds
### 3.1. Watchdog Settings
The following watchdog settings shall be used:

	wd_min_oper_period:	64ms

	wd_max_oper_period:	144ms

	Boot Window:  2.24 seconds (Factory default setting, cannot be changed by register setting)

### 3.2 Wrapper Functions
#### 3.2.1. Watchdog_Init()
        Wrapper function is used to initialise watchdog's maximum & minimum service intervals.
#### 3.2.2. Watchdog_Service()
        Wrapper function is used to service watchdog.

## 4. External Clock Input Sync
To switch from the LT3390 internal clock source to an external clock source connected to the SYNC pin, the microprocessor must request the switch by setting the req_sync bit in LT3390 register 0x0.  The req_sync bit is the least significant bit (bit0) in register 0x0.

Prior to requesting the switch to the external clock source, an 8MHz clock must be applied to the SYNC pin of the LT3390.  This signal originates on the FLR4+ micro’s port PA_08 (Pin AD21).

Once the switch from the internal clock to the external clock is requested, the synchronizer block waits until a rising edge of the internal LT3390 clock and the external clock on the SYNC pin to occur within approximately 15ns of each other.  Once this alignment occurs, the switch to the external clock is made.

To determine if the switch to the external clock has been made, the ack_sync bit in register 0x0 (bit 1) should be queried.  The ack_sync bit will be asserted once the switch has been made.

### 4.1. Wrapper Functions
#### 4.1.1. Pmic_Clk_Sync_Enable()
        Wrapper function enables the pmic to switch to external generated clock.

###### Note: Refer pmic source and headers file for input/output information of wrapper functions.

###### PMIC(LT3390) Datasheet link : https://spo.aptiv.com/sites/0304-AdvEngSystems/Shared%20Documents/Forms/AllItems.aspx?newTargetListUrl=%2Fsites%2F0304%2DAdvEngSystems%2FShared%20Documents&viewpath=%2Fsites%2F0304%2DAdvEngSystems%2FShared%20Documents%2FForms%2FAllItems%2Easpx&id=%2Fsites%2F0304%2DAdvEngSystems%2FShared%20Documents%2FPMIC%5FEthPHY%5FDatasheets%2FPMIC%2FADI&viewid=92bab1dc%2D9f10%2D4521%2Daa5b%2D87445067ff42

## 5. File Revision History
|Rev|Date       |NetId  |Name           |SCR     |
|---|-----------|-------|---------------|--------|
|0.1|29-Feb-2024| ulb4jq| Jagadish B    | GNZ-21 |

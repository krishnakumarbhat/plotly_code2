# ALL ABOUT ON BOARD TEMPERATURE AND ITS WRAPPER FUNCTIONS
-------------------------
[TOC]

-------------------------
## 1. Introduction
The TMP112-Q1 device is a digital temperature sensor that is optimal for thermal-management and thermalprotection applications. The TMP112-Q1 device is two-wire, SMBus and I2C interface-compatible. The device is specified over an operating temperature range of –40°C to 125°C.

The digital output from each temperature measurement conversion is stored in the read-only temperature register.

To convert a positive digital data format to temperature:
1. Convert the 12-bit, left-justified binary temperature result, with the MSB = 0 to denote a positive sign, to a decimal number.
2. Multiply the decimal number by the resolution to obtain the positive temperature.
Example: 0011 0010 0000 = 320h = 800 × (0.0625°C / LSB) = 50°C

To convert a negative digital data format to temperature:
1. Generate the twos compliment of the 12-bit, left-justified binary number of the temperature result (with MSB = 1, denoting negative temperature result) by complementing the binary number and adding one. This represents the binary number of the absolute value of the temperature.
2. Convert to decimal number and multiply by the resolution to get the absolute temperature, then multiply by–1 for the negative sign.
Example: 1110 0111 0000 has twos compliment of 0001 1001 0000 = 0001 1000 1111 + 1 Convert to temperature: 0001 1001 0000 = 190h = 400; 400 × (0.0625°C / LSB) = 25°C = (|–25°C|); (|– 25°C|) × (–1) = –25°C
### 1.1. Pin Configuration and Functions

![alt text](Pin_cfg.png)
![alt text](Schematic.png)

### 1.2. TMP112 Register Address
        Pointer Register : 0x48
        Temperature Register : 0x00
        Configuration Register : 0x01

        Note : Above register address are 7bit address for more details on these register refer tmp112-q1 temperature sensor data sheet.
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
Accessing a particular register on the TMP112-Q1 device is accomplished by writing the appropriate value to thepointer register. The value for the pointer register is the first byte transferred after the target address byte withthe R/ W bit low.

#### 2.1.1. Target Transmitter Mode
The first byte transmitted by the controller is the target address with the R/ W bit high. The target acknowledges reception of a valid target address. The next byte is transmitted by the target and is the most significant byte of the register indicated by the pointer register. The controller acknowledges reception of the data byte. The next byte transmitted by the target is the least significant byte. The controller acknowledges reception of the data byte. The controller can terminate data transfer by generating a not-acknowledge on reception of any data byte or by generating a START or STOP condition.

![alt text](Write_operation.png)

#### 2.1.2. Target Receiver Mode
The first byte transmitted by the controller is the target address with the R/ W bit low. The TMP112-Q1 device then acknowledges reception of a valid address. The next byte transmitted by the controller is the pointer register. The TMP112-Q1 device then acknowledges reception of the pointer register byte. The next byte or bytes are written to the register addressed by the pointer register. The TMP112-Q1 device acknowledges reception of each data byte. The controller can terminate data transfer by generating a START or STOP condition.

![alt text](Read_operation.png)

### 2.2. I2C Wrapper Functions
#### 2.2.1. Read_Board_Temperature()
        This function will read Onboard temperature from TMP112 sensor by writing temperature address (0x00) to the pointer register of external device(Temperature sensor) over I2c Communication and converts the raw temeprature value to degree celcius.
#### 2.2.2. Temperature_Init()
        This function will initilize and configure the external temperature sensor(TMP112) by writing the Byte1 data (0x60) and Byte2 data(0xA0) to the configuration register(0x01) which will initilize the external device.
#### 2.2.3. I2C_Write_To_Temperature_Sensor()
        This function is used to write data to the onboard temperature sensor over I2c Communication
#### 2.2.4. I2C_Read_From_Temperature_Sensor()
        This function is used to read data from onboard temperature sensor over I2c Communication

###### Note: Refer datasheet For more details about above tmp112-q1 External device.


###### tmp112-q1 Datasheet link : https://www.ti.com/lit/ds/symlink/tmp112-q1.pdf?ts=1709582846201&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTMP112-Q1

## 3. File Revision History
|Rev|Date       |NetId  |Name           |SCR      |
|---|-----------|-------|---------------|---------|
|0.1|05-MAR-2024| ulb4jq| Jagadish B    | GNZ-761 |

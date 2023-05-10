# Communication Repository

- Communication within embedded circuits and devices is an efficient method to exchange data. The communication protocols like I2C and SPI, UART use a physical circuit... Other digital signals considered include Pulse Width Modulation (PWM)...

## Code Utilities
- **BusI2C.h** under development...
- **PWM.h** presently being used as PWM-meter. 

## Devices ##
- MCU
    - Master-MCU communication ***I2C Address 0x14***
        - Arduino Uno as Master
            - **20230425** Code working...
    - Slave-MCU communication ***I2C Address 0x16***
        - Arduino Uno as Slave
            - **20230425** Code working...
- PWM
    - **PCA9685** communication ***I2C Address 0x40***
        - NXP Semiconductors
            - Device: Adafruit PCA9685 16-Channel Servo Driver
            - Code: Adafruit Industries
                - **20230404** Code **PASSED** using the following steps...
                    - (1) Arduino Uno self-test - TEST_STEP_1().
                    - (2) Adafruit driver to Arduino Input - TEST_STEP_2().
                    - (3) Adafruit Calibration - TEST_STEP_3().
                    - (4) Mapping Servo Motor - TEST_STEP_4().
                - The last three tests used Adafruit's code...

## Sensors ##

- Magnetometers
    - **HMC5883L** communication ***I2C Address 0x1E***
        - Honeywell
            - Device: Parallax HMC5883L ()
            - Code: Individual (see citations)
                - **20230411** Code **PASSED** the Magnetometer Accuracy Test when using the *offsets* and *magnetic declination*. Tested by Carpenter Software.
    - **LIS2MDL**  communication ***I2C Address 0x1E***
        - STMicroelectronics
            - Device: Adafruit LIS2MDL Triple Axis Magnetometer
            - Code: Adafruit Industries
                - **20230509** Code **PASSED** the Magnetometer Accuracy Test. Carpenter Software **rewrote** the Calibration and Compass code in the src folder. Did a data review using comma separated values (CSV) and plotted the data. The data will be posted here... Tested by Carpenter Software. 
    - **LSM303D**  communication ***I2C Address 0x1E***
        - STMicroelectronics
            - Device: Pololu LSM303D (2127) 3D Compass and Accelerometer
            - Code: Pololu.com Arduino
                - **20230505** Code **PASSED** the Magnetometer Accuracy Test when using the *offsets* and *magnetic declination*. Magnetic North was off by 30 degrees but this could be corrected... Tested by Carpenter Software.
    - **LSM303AGR**  communication ***I2C Address 0x1E***
        - STMicroelectronics
            - Device: Adafruit LSM303AGR Accelerometer Magnetometer
            - Code: Adafruit Industries
                - Testing **Pending**
    - **Magnetometer Accuracy Test**
        - The *heading* (reading) is performed on the xy-plane while rotating the magnetometer. 
            - (1) Aim the electronic compass to align with the Earth's Magnetic North giving a *heading* of zero degrees... This is considered the first reading at zero degrees...
            - (2) Orient the electronic compass 180 degrees (or pi radians) around from the first reading. The reading (result) at this point should match the physical orientation of the compass which should be 180 degrees. Both the reading and orientation should be equal.
        - To clarify, the heading is defined by taking the arc-tangent(y, x) of the xy-plane of the magnetometer... The code testing **FAILS** if the orientations do not align exactly on their opposite sides after each reading. In other words, the relative readings should match the physical orientations at the two points. The **difference** of the **physical** orientations between *angle-1* and *angle-2* should match exactly to the **difference** readings of *heading-1* and *heading-2*. 
            - Thus the **difference** = (*angle-2* **-** *angle-1*) = (*heading-2* **-** *heading-1*). The importance of understanding how to apply the magnetometer's readings to the the physical orientation of the compass is key.
    - **Test** two magnetometers having the same I2C address while using two MCUs. Interesting thought... See *MCU* under the folder *Devices* at this repository.

## Testing Platform

- This MCU chip **Atmega328P** is well known and the price is affordable as well as most of the robotic hardware that is described here. The Arduino Uno board comes with a removable MCU.

- **PlatformIO** is a cross-platform, cross-architecture, multiple framework, professional tool for embedded systems engineers and for software developers who write applications for embedded products. 

- **Visual Studio Code** is a streamlined code editor and it runs on macOS, Linux, and Windows. The best thing about VSCode, it is free.

## Issues

- None

## In Development

- 20230509

## NOTICE

## Disclaimer and Terms

- The Devices and Sensors folders might **not** contain MIT LICENSE. Please read through all third-party material at their Github website for thier LICENSE. The citations are listed within this Communication repository.

- Please follow the ***Disclaimer*** and ***Terms*** in the ***Communication*** Repository.
   

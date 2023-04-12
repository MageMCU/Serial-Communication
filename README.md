# Communication Repository

- Communication within embedded circuits and devices is an efficient method to exchange data. The communication protocols like I2C and SPI, UART use a physical circuit... Other digital signals considered include Pulse Width Modulation (PWM)...

## Devices ##

- PWM
    - **PCA9685** communication ***I2C Address 0x40***
        - NXP Semiconductors
            - Device: Adafruit PCA9685 16-Channel Servo Driver
            - Code: Adafruit Industries

## Sensors ##

- Magnetometers
    - **HMC5883L** communication ***I2C Address 0x1E***
        - Honeywell
            - Device: Parallax HMC5883L ()
            - Code: Individual
            - **20230411** testing with offsets and magnetic declination - *surprisingly good results* - more testing required... Carpenter Software.
    - **LIS2MDL**  communication ***I2C Address 0x1E***
        - STMicroelectronics
            - Device: Adafruit LIS2MDL Triple Axis Magnetometer
            - Code: Adafruit Industries
    - **Test**
        - A **quick-test** method for the magnetometer. The *heading* (reading) is performed on the xy-plane on a electronic magnetometer. 
            - (1) **Test** whether the electronic compass aligns with the Earth's Magnetic North giving a *heading* of zero or 360 degrees where the results may rock back and forth between these values....
            - (2) Rotate the electronic compass around 180 degrees (or pi radians) from its initial orientation. The result between the two points 180 degrees apart should align exactly on their oposite sides.  **Test** whether the *heading* is reading about the same of 180 degrees (or pi radians)...
        - To clarify, the heading is defined by taking the arc-tangent of the xy-plane of the magnetometer... The testing **FAILS** if the orientations do not align exactly on their opposite sides while rotating the magnetometer to the assigned readings of zero and 180 degrees. In other words, the relative readings should match the two points. The **difference** of the physical orientation between angle-1 and angle-2 should match exactly to the **difference** of heading-1 and heading-2. 
            - Thus the **difference** = (angle-2 - angle-1) = (heading-2 - heading-1).

## Testing Platform

- This MCU chip **Atmega328P** is well known and the price is affordable as well as most of the robotic hardware that is described here. The Arduino Uno board comes with a removable MCU.

- **PlatformIO** is a cross-platform, cross-architecture, multiple framework, professional tool for embedded systems engineers and for software developers who write applications for embedded products. 

- **Visual Studio Code** is a streamlined code editor and it runs on macOS, Linux, and Windows. The best thing about VSCode, it is free.

## Issues

- None

## In Development

- 20230411

## NOTICE

- Please read notes... The **Communication** repository has consolidated other repositories which has included the those deleted below:
    - **HMC5883L** repository deleted...
    - **LSM303** repository deleted...
    - **Servomotor** repository deleted...

## Disclaimer and Terms

- The Devices and Sensors folders might **not** contain MIT LICENSE. Please read through all third-party material at their Github website for thier LICENSE. The citations are listed within this Communication repository.

- Please follow the ***Disclaimer*** and ***Terms*** in the ***Communication*** Repository.
   

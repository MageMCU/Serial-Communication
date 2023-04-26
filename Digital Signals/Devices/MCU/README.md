## MCU 

- namespace ***dsg***

## ***include folder***
- **BusI2C.h** 
    - file can be located in this repository in the **Code Utilities** folder...
- **Button.h**
    - file can be located in *Simplified-Joystick-Uno_L298N* repository...
- **Common.h**
- **StateMachine.h**
- **Timer.h**
    - file can be located in *Numerics* repository

## ***src folder***

- MasterUno  
    - main.cpp
- SlaveUno  
    - main.cpp

## ***root folder***

- platformio.ini        
    - (Please review this document for platform configuration while testing the code.)
        - CAUTION: platformio.ini
        - MASTER MCU: Change to **MasterUno** folder
            - Upload to this folder
        - SLAVE MCU: Change to **SlaveUno** folder
            - Upload to this folder

## ***Notes***

- 20230425 - This can be considered complete as a startup (barebones)...
- 20230423 - Initial development - work in progress...
## MCU 

- namespace ***dsg***

## ***include folder***
- **Bitwise.h** 
- **BusI2C.h** 
- **Button.h**
- **MCU_Common.h**
- **MCU_Master.h**
- **MCU_Slave.h**
- **Timer.h**

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

- 20230423 - Initial development - work in progress...
## Serial Communication
- Serial communication uses *protocols* to transfer data between devices or components. A *protocol* is a method for transmitting data in a sequential manner, one bit at a time. It is a set of formal rules describing how to transmit or exchange data. A list of generic protocols are listed below:
  - **UART** - Universal Asynchronous Receiver Transmitter, a commonly used protocol that allows asynchronous data exchange between devices. It uses two wires and operates at speeds ranging from 300 bps to 25 Mbps.
  - **I2C** - An Inter-Integrated Circuit protocol that uses two wires to transmit data between devices. It's ideal for sensors and modules.
  - **SPI** - A Serial Peripheral Interface protocol that uses a master-slave architecture to operate in full duplex mode. It's used for short distance communication in embedded systems.
  - **USB** - A Universal Serial Bus protocol that ...
### Testing Platform
- **Arduino Uno** Rev3 with the MCU chip Atmega328P. Arduino Uno is well known and the price is affordable as well as most of the robotic hardware that is described here. The Arduino Uno board comes with a removable MCU.
- **PlatformIO** is a cross-platform, cross-architecture, multiple framework, professional tool for embedded systems engineers and for software developers who write applications for embedded products. 
- **Raspberry Pi** 3 Model v1.2 (2015) with 1GB RAM (minimum hardware setup)
### In Development
- Please note that this repository was previously named *Communication*.
- Consolidating all serial communications under this repository.
- 20241111 - This repository is undergoing REVIEW
    - The old projects will undergo testing and their folders have been appropriately re-named. Under the folder *Compass*, the folders are named with both connected devices, for example, Uno-HMC5883L.
- During the review, all depended files will be incorporated into the project folder even though there might be redundancy in the others, and in turn, the scatted files will be removed eliminating any confusion.
### Disclaimer and Terms
- The Devices and Sensors folders might **not** contain MIT LICENSE. Please read through all third-party material at their Github website for thier LICENSE. The citations are listed within this Communication repository.
- Please follow the ***Disclaimer*** and ***Terms*** in the ***Serial Communication*** Repository.
   

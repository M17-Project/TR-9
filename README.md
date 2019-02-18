# TR-9
Open-source digital mobile radio, compatible with M17 standard. Uses Codec2 vocoder and internal Advanced Encryption Standard (AES) hardware for optional OTA encryption. Designed for STM32F777 microcontroller. RFM26W module is used as a complete transceiver. There is a possibility of using other block ciphers like ChaCha.

This is a complete SW4STM32 project for the TR-9 board.

# TODO:
- Replace the RFM26W module with a genuine Si4463-C2 chip and adjust the antenna PCB trace to match 50ohm impedance  
- Re-design audio amplifier stage  
- Swap SPI1 and SPI2  
- Replace SD slot with a different type  
- Redraw the schematic and PCB using KiCAD to keep the project 100% open and editable  
- Design a PCB for the keyboard and encoder  
  
- Fix NVIC interrupts priorities (causing freezes)  
- Implement hardware AES  
- Add a GPS module  
- Add an I2C accelerometer module  

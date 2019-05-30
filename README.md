# TR-9
Open-source digital mobile radio, compatible with M17 standard. Uses Codec2 vocoder and internal Advanced Encryption Standard (AES) hardware for optional OTA encryption. Designed for STM32F777 microcontroller. Si4463-C2 chip is used as a complete transceiver. There is a possibility of using other block ciphers like ChaCha.

This is a complete SW4STM32 project for the TR-9 board.

# Done: 
- Changed the RF switch to HMC546MS8GETR 
- Completely redesigned mainboard, everything done by hand 
- Display and keyboard boards 
- Added 3..5W RF output amplifier 

# TODO: 
- Merge keyboard and display boards into one 
- Bypass R20 and R23 with 33p caps 
- Add PWR_ON signal for the GPS module 
- Add 4.7k pull-up resistor for the PTT microswitch 
- Remove 5V voltage regulator? 
- Redraw the schematic and PCB using KiCAD to keep the project 100% open and editable   
  
- Implement hardware AES  
- Add a GPS module  

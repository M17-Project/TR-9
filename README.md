# TR-9
Open-source digital mobile radio, compatible with M17 standard. Uses Codec2 vocoder and internal Advanced Encryption Standard (AES) hardware for optional OTA encryption. Designed for STM32F777 microcontroller. Si4463-C2 chip is used as a complete transceiver. There is a possibility of using other block ciphers like ChaCha.

This is a complete SW4STM32 project for the TR-9 board.

# Done: 
- Changed the RF switch to HMC546MS8GETR 
- Completely redesigned mainboard, verything done by hand 
- Display and keyboard boards 
- Added 3..5W RF output amplifier 

# TODO: 
- Adjust the antenna PCB trace
- Test and tweak tge 3..5W RF output power amplifier 
- Redraw the schematic and PCB using KiCAD to keep the project 100% open and editable   
  
- Implement hardware AES  
- Add a GPS module  

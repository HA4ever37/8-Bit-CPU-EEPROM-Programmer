# 8-Bit-CPU-EEPROM-Programmer
This is an Arduino code to program EEPROMs (microcode instructions programmer) for a breadboard 8-Bit CPU inspired by Ben Eater's CPU design https://eater.net/8bit

CPU Features include:
* Reduced components in compare with Ben Eater's design
* 512 bytes of SRAM (256 addresses producing 2 bytes of data per address) instead of 16 bytes (16 addresses and 1 byte of data)
* 4 CPU cycles per instruction instead of 6
* Separated 16bit ram bus for instructions and data  (8 bits for instructions + 8 bits for data)


To program the EEPROM, you will need the following:
* An Arduino compatible board
* 2 shift registers (e.g. 74hc595)
* Of course, the EEPROM chips

| Clock Module| Program Counter| Input Switcher | Ram Module | Logic Control | Registers Module (A&B) | ALU Module | Display Module | Input Module |
|------------|------------|-----|---------------|------------|----|-----------|------------|-----|
| 2x 555 Timer |2x Binary Counter (SN74HC161N) | 4x  2to1 data selector (SN74HC157N) | 4x 4K-RAM (CY7C168A) | Binary Counter (SN74HC161N) | Quad AND (SN74HC08N) |  2x Binary Adder (CD74HC283E) | 555 Timer | Octal D-Type Flip-Flop (CD74HC273E) |
| Tactile Button | 8x Led | 3-State Transceiver (SN74HC245N) | 3-State Transceiver (SN74HC245N) | 2x EEPROM (AT28C64B-15PU) | 2x  3-State Transceiver (SN74HC245N) | 2x Quad XOR (SN74HC86N) | Binary Counter (SN74HC161N) | Hex Inverter (SN74HC04) |
| Switch (double throw) | 3-State Transceiver (SN74HC245N) | Octal D-Type Flip-Flop (CD74HC273E) | Switch (double throw) | 16x Led | 2x Octal D-Type Flip-Flop (CD74HC273E) | 3-State Transceiver (SN74HC245N) | Dual 2-to-4 Line Decoder (CD74HC139E) | Tactile Button |
| Quad NAND (SN74HCT00N) | | | | | | | EEPROM (AT28C64B-15PU) | 3x 8-DIP Switch|
| Hex Inverter (SN74HC04) | | | | | | |4x 7-seg display | |

![alt text](https://github.com/HA4ever37/8-Bit-CPU-EERPOM-Prorammer/blob/master/IMG_20191224_224308.jpg)
![alt text](https://github.com/HA4ever37/8-Bit-CPU-EERPOM-Prorammer/blob/master/IMG_20200111_134529.jpg)

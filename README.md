# 8-Bit-CPU-EEPROM-Programmer
This is an Arduino code to program EEPROMs (microcode instructions programmer) for a breadboard 8-Bit CPU inspired by Ben Eater's CPU desgin https://eater.net/8bit

CPU Features include:
* Reduced components in compare with Ben Eater's design
* 512 bytes of SRAM (256 addresses producing 2 byes of data per address) instead of 16 bytes (16 addresses and 1 byte of data)
* 4 CPU cycles per micro-instruction instead of 6
* Speated ram buses for instructions and data  (8 bits for instructions + 8 bits for data)


To program the EEPROM, you will need the following:
* An Arduino compatible board
* 2 shift registers (e.g. 74hc595)
* Of course, the EEPROM chips


![alt text](https://github.com/HA4ever37/8-Bit-CPU-EERPOM-Prorammer/blob/master/IMG_20191224_224308.jpg)
![alt text](https://github.com/HA4ever37/8-Bit-CPU-EERPOM-Prorammer/blob/master/IMG_20200111_134529.jpg)

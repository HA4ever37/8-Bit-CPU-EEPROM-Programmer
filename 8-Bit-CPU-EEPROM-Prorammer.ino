#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D5 10
#define EEPROM_D6 16
#define EEPROM_D7 14
#define WRITE_EN 15

#define CE  0b1000000000000000         // Program counter enable
#define CI  0b0100000000000000         // Program counter in (jump)
#define RO  0b0010000000000000         // RAM data out
#define RI  0b0001000000000000         // RAM data in
#define AO  0b0000100000000000         // A register out
#define AI  0b0000010000000000         // A register in
#define BO  0b0000001000000000         // B register out
#define BI  0b0000000100000000         // B register in

#define SU  0b0000000010000000         // ALU subtract
#define EO  0b0000000001000000         // ALU out
#define DP  0b0000000000100000         // Display in
#define CO  0b0000000000010000         // Program counter our
#define MI  0b0000000000001000         // Memory address register in
#define F1  0b0000000000000100         // Not implemented yet
#define F2  0b0000000000000010         // Not implemented yet
#define HT  0b0000000000000001         // Clock halt

uint16_t data[] = {
  CO | MI,  CE, 0,  0,                            // 0000 - NOP
  CO | MI,  RO | AI | CE, 0,  0,                  // 0001 - LDI
  CO | MI,  RO | MI, RO | AI | CE,  0,            // 0010 - LDA
  CO | MI,  RO | BI, EO | AI | CE,  0,            // 0011 - ADI
  CO | MI,  RO | MI, RO | BI, EO | AI | CE,       // 0100 - ADD
  CO | MI,  RO | BI, SU | EO | AI | CE, 0,        // 0101 - SBI
  CO | MI,  RO | MI, RO | BI, SU | EO | AI | CE,  // 0110 - SUB
  CO | MI,  RO | MI, AO | RI | CE, 0,             // 0111 - STA
  CO | MI,  RO | MI, BO | RI | CE, 0,             // 1000 - STB
  CO | MI,  RO | CI, 0,  0,                       // 1001 - JMP
  CO | MI,  RO,  CE,  0,                          // 1010
  CO | MI,  RO,  CE,  0,                          // 1011
  CO | MI,  RO,  CE,  0,                          // 1100
  CO | MI,  RO,  CE,  0,                          // 1101
  CO | MI,  AO | DP | CE,  0,  0,                 // 1110 - OUT
  CO | MI,  HT, 0,  0,                            // 1111 - HLT
};

void pinInput() {
  for (int pin = EEPROM_D0; pin <= EEPROM_D5; pin += 1)
    pinMode(pin, INPUT);
  pinMode(EEPROM_D6, INPUT);
  pinMode(EEPROM_D7, INPUT);
}

void pinOutput() {
  for (int pin = EEPROM_D0; pin <= EEPROM_D5; pin += 1)
    pinMode(pin, OUTPUT);
  pinMode(EEPROM_D6, OUTPUT);
  pinMode(EEPROM_D7, OUTPUT);
}
/*
   Output the address bits and outputEnable signal using shift registers.
*/
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
   Read a byte from the EEPROM at the specified address.
*/
byte readEEPROM(int address) {
  pinInput();
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;

  data = (data << 1) + digitalRead(EEPROM_D7);
  data = (data << 1) + digitalRead(EEPROM_D6);
  for (int pin = EEPROM_D5; pin >= EEPROM_D0; pin -= 1)
    data = (data << 1) + digitalRead(pin);

  return data;
}


/*
   Write a byte to the EEPROM at the specified address.
*/
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  pinOutput();
  for (int pin = EEPROM_D0; pin <= EEPROM_D5; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(EEPROM_D6, data & 1);
  data = data >> 1;
  digitalWrite(EEPROM_D7, data & 1);
  data = data >> 1;

  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}

/*
   Read the contents of the EEPROM and print them to the serial monitor.
*/
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);

  // Program data bytes
  Serial.print("Programming EEPROM");

  // Program the 8 high-order bits of microcode into the first 128 bytes of EEPROM
  for (int address = 0; address < sizeof(data) / sizeof(data[0]); address += 1) {
    writeEEPROM(address, data[address] >> 8);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  // Program the 8 low-order bits of microcode into the second 128 bytes of EEPROM
  for (int address = 0; address < sizeof(data) / sizeof(data[0]); address += 1) {
    writeEEPROM(address + 128, data[address]);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  Serial.println(" done");

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}

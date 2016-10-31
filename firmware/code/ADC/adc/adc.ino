#include <SPI.h>

const int conv_pin = 10;

void setup() {
  Serial.begin(230400);
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  // start the SPI library:
  SPI.begin();

  // initalize the  data ready and chip select pins:
  pinMode(conv_pin, OUTPUT);
  digitalWrite (conv_pin, HIGH);
}

void loop() {
  digitalWrite (conv_pin, HIGH);
  delayMicroseconds(50);
  digitalWrite (conv_pin, LOW);
  unsigned int tempData = readRegister(0x21, 2);
  delayMicroseconds(50);
  Serial.println(tempData);
}

unsigned int readRegister(byte thisRegister, int bytesToRead) {
  byte inByte = 0;           // incoming byte from the SPI
  unsigned int result = 0;   // result to return

  result = SPI.transfer(0x00);

  // decrement the number of bytes left to read:
  bytesToRead--;
  // if you still have another byte to read:
  if (bytesToRead > 0) {
    // shift the first byte left, then get the second byte:
    result = result << 8;
    inByte = SPI.transfer(0x00);
    // combine the byte you just got with the previous one:
    result = result | inByte;
    // decrement the number of bytes left to read:
    bytesToRead--;
  }

  return (result);
}


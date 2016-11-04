#include <SPI.h>

const int conv_pin = 10;
unsigned long time_origin = 0;
unsigned long acq_time[3];
void setup() {
  Serial.begin(9600);
//  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  // start the SPI library:
  SPI.begin();

  // initalize the  data ready and chip select pins:
  pinMode(conv_pin, OUTPUT);
  digitalWrite (conv_pin, LOW);
  analogReference(INTERNAL);
  time_origin = millis();
}

void loop() {
  digitalWrite (conv_pin, HIGH);
  delay(10);
  digitalWrite (conv_pin, LOW);
//  unsigned int tempData = readRegister(0x21, 4);
  acq_time[0] = millis() - time_origin;
  unsigned int thermocoupleDataRaw = SPI.transfer16(0x00);
  delay(10);
  acq_time[1] = millis();
  int thermoelectricVoltageRaw = analogRead(A0);

  delay(10);
  acq_time[2] = millis();
  int coldJunctionRaw = analogRead(A2);
  delay(10);
  Serial.print (acq_time[0]);
  Serial.print (",");
  Serial.print (thermocoupleDataRaw);
  Serial.print (",");

  Serial.print (acq_time[1]);
  Serial.print (",");
  Serial.print (thermoelectricVoltageRaw);
  Serial.print (",");

  Serial.print (acq_time[2]);
  Serial.print (",");
  Serial.print (coldJunctionRaw);
  Serial.print ("\n");
}

unsigned int readRegister(byte thisRegister, int bytesToRead) {
  byte inByte = 0;           // incoming byte from the SPI
  unsigned int result = 0;   // result to return

  result = SPI.transfer(0x00);
  inByte = SPI.transfer(0x00);
  result = (result << 8) | inByte;
  return result;
//  // decrement the number of bytes left to read:
//  bytesToRead--;
//  // if you still have another byte to read:
//  if (bytesToRead > 0) {
//    // shift the first byte left, then get the second byte:
//    result = result << 8;
//    inByte = SPI.transfer(0x00);
//    // combine the byte you just got with the previous one:
//    result = result | inByte;
//    // decrement the number of bytes left to read:
//    bytesToRead--;
//  }

//  return (result);
}


#include <SPI.h>

const int conv_pin = 6;
const int chipSelectPin = 7;

//Sensor's memory register addresses:
const int PRESSURE = 0x1F;      //3 most significant bits of pressure
const int PRESSURE_LSB = 0x20;  //16 least significant bits of pressure
const int TEMPERATURE = 0x21;   //16 bit temperature reading
const byte READ = 0b11111100;     // SCP1000's read command
const byte WRITE = 0b00000010;   // SCP1000's write command

void setup() {
  Serial.begin(9600);

  // start the SPI library:
  SPI.begin();

  // initalize the  data ready and chip select pins:
  pinMode(conv_pin, OUTPUT);
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite (conv_pin, HIGH);
}

void loop() {
  digitalWrite (conv_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite (conv_pin, LOW);
  unsigned int tempData = readRegister(0x21, 2);
  delayMicroseconds(50);
  Serial.println(tempData);
  delay (1);
}

unsigned int readRegister(byte thisRegister, int bytesToRead) {
  byte inByte = 0;           // incoming byte from the SPI
  unsigned int result = 0;   // result to return

//  digitalWrite(chipSelectPin, LOW);
  // send the device the register you want to read:
//  SPI.transfer(dataToSend);
  // send a value of 0 to read the first byte returned:
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
  // take the chip select high to de-select:
//  digitalWrite(chipSelectPin, HIGH);
  // return the result:
  return (result);
}


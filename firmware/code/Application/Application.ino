#include <SPI.h>
#include "Comm.h"
#include <string.h>

#include <stdlib.h>

class Application;
String check (void);
//enum OPCODE
//{
//  NOP = 0,
//  IDN,
//  ROOM_TEMP
//};

String readString;

void setup() {
  while (!Serial);
  Serial.begin (9600);
  delay (100);
}

void loop() {

  char c = Serial.read();
  auto opcode = atoi (&c);
  
  if (opcode == OPCODE::IDN)
    Serial.println ("DAQ_PHY315");
  if (opcode == OPCODE::ROOM_TEMP)
    Serial.println ("27.0");
    Comm o;
    o.transmit();
}

class Application
{
  public:
  
};




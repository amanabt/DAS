#include <SPI.h>
#include "Comm.h"
#include <string.h>

#include <stdlib.h>

class Application;
void check (void);
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
  check();
}

void check (void)
{
  
}

class Application
{
  public:
  
};




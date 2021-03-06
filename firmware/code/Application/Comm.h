#ifndef __DAQ_COMM__
#define __DAQ_COMM__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum OPCODE
{
	NOP,
	IDN,
	ROOM_TEMP
};

class CommPacket
{
public:
	CommPacket (OPCODE opcode)
  {
    _sof[0] = 'I';
    _sof[1] = 'P';
	  _opcode = opcode;
  }
public:
  uint16_t opcode (void) const
  { return _opcode; }

private:
  char     _sof[2];
	uint16_t _opcode;
};

class CommRequest_Identity : CommPacket
{
private:
	CommRequest_Identity(void);
};

class CommResponse_Identity : CommPacket
{
public:
	CommResponse_Identity (const char* identity) :
		CommPacket(OPCODE::IDN)
	{
		memset (_identity, 0, sizeof (_identity));
		strcpy (_identity, identity);
	}

private:
	const char _identity[32];
};

class Comm
{
public:
	Comm (void)
	{}
	
public:
	void transmit (const char* packet, uint32_t size);
  void transmit (void);
  uint16_t interpret (const char* request);
  
public:
	void packetIdentity (const char* identity) const;
};

#endif

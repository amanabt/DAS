#include "../Comm.h"

void
Comm::transmit (const char* packet, uint32_t size)
{
	volatile int i = 0;
}

void
Comm::transmit (void)
{
	volatile int i = 0;
}

void
Comm::packetIdentity (const char* identity)
{
	auto o = CommResponse_Identity (identity);
	uint32_t size = sizeof (CommResponse_Identity)
	char* packet = new char[size] (reinterpret_cast<char*> (o));
	this->transmit (packet, size);
}

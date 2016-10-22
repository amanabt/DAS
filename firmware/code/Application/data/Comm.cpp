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
Comm::transmitIdentity (const char* identity) const
{
	auto o = CommResponse_Identity (identity);
	uint32_t size = sizeof (CommResponse_Identity)
	char* packet = new char[size];
	this->transmit (packet, size);
}

#ifndef __COMM__
#define __COMM__

#include "../avr/include/UART.h"

#include "../../sys/include/QP4.h"
#include "../../sys/include/Applet.h"

#include "inet"
#include <memory>
#include <vector>
#include <stdint.h>
#include <string.h>

/******************************************************************/

enum class COMM_OPCODE
{
	NOP,
	IDN,

	__END__
};

/******************************************************************/
/******************************************************************/

class CommPacket
{
protected:
	CommPacket (uint16_t opcode) :
		_opcode (std::hton (opcode)),
		_reserved (0)
	{}

public:
	uint16_t opcode (void) const {return std::ntoh (_opcode);}

private:
	uint16_t _opcode;
	uint16_t _reserved;
};

/******************************************************************/

class CommPacket_Identity : public CommPacket
{
protected:
	CommPacket_Identity (void) :
		CommPacket (static_cast<uint16_t> (COMM_OPCODE::IDN))
	{}
};

class CommRequest_Identity : public CommPacket_Identity
{
private:
	CommRequest_Identity (void);
};

class CommResponse_Identity : public CommPacket_Identity
{
public:
	CommResponse_Identity (const char* identity);

private:
	char     _identity[32];
};

/******************************************************************/
/******************************************************************/

enum class COMM_CBCODE
{
	NOP,
	IDN,
	
	__END__
};

/******************************************************************/

class CommCB
{
protected:
	CommCB (COMM_CBCODE code) :
		code_ (code)
	{}

public:
	COMM_CBCODE code (void) const {return code_;}

private:
	COMM_CBCODE code_;
};

class CommCB_Identity : public CommCB
{
public:
	CommCB_Identity (void) :
		CommCB (COMM_CBCODE::IDN)
	{}
};

/******************************************************************/
/******************************************************************/

union CommCB_Union
{
	char gen0 [sizeof (CommCB)];
	char gen1 [sizeof (CommCB_Identity)];
};

/******************************************************************/
/******************************************************************/

class Comm : public Applet
{
public:
	static Comm* _ (void);

public:
	virtual void check (void);

public:
	void transmitIdentity (const char* identity);

private:
	UART* _uart;
	std::unique_ptr<QP4> _qp4;
	CommCB_Union _callbackObject;

private:
	Comm (void);

private:
	void checkReceiveQueue (void);
	void check_async_write_complete (void);

	void processReceivedData   (const void* data, uint16_t size);
	void interpret             (const void* data, uint16_t size);

	void nopCB                 (const void* data, uint16_t size);
	void identityCB            (const void* data, uint16_t size);

private:
	const void* transmit (const void* data, uint16_t size);
};

/******************************************************************/

#define appComm    Comm::_()

#endif

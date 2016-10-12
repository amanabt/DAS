// #define DEBUG_COMM

#include "../include/Comm.h"

#include "inet"
#include "pgmspace"
#include <tuple>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <util/delay.h>

using namespace std;

#ifdef DEBUG_COMM

#define DEBUG_PORT PORTC
#define DEBUG_DDR  DDRC
static constexpr uint8_t DEBUG_BIT = static_cast <uint8_t> (1);
static constexpr uint8_t DEBUG_MSK = static_cast <uint8_t> (1 << DEBUG_BIT);
static constexpr uint8_t HIGH      = 1 << DEBUG_BIT;
static constexpr uint8_t LOW       = 0 << DEBUG_BIT;


void debug_comm (void)
{
	DEBUG_PORT = (DEBUG_PORT & (~DEBUG_MSK)) | (HIGH & DEBUG_MSK);
	_delay_us (10);
	DEBUG_PORT = (DEBUG_PORT & (~DEBUG_MSK)) | (LOW & DEBUG_MSK);
	_delay_us (10);
}
#endif

/*************************************************************************/
/*************************************************************************/

CommResponse_Identity::CommResponse_Identity (const char* identity)
{
	memset (_identity, 0, sizeof (_identity));
	strcpy (_identity, identity);
}

/*************************************************************************/
/*************************************************************************/

Comm* Comm::_ (void)
{
	static Comm* o = new Comm;
	return o;
}

Comm::Comm (void) :
	_uart (UART::_()),
	_qp4 (new QP4)
{
#ifdef DEBUG_COMM
	DEBUG_DDR = (DEBUG_DDR & (~DEBUG_MSK)) | (HIGH & DEBUG_MSK);
#endif
}

void Comm::check (void)
{
	check_async_write_complete();
	checkReceiveQueue();
}

const void* Comm::transmit (const void* data, uint16_t size)
{
	check_async_write_complete();
	return _uart->async_write (data, size);
}

void Comm::check_async_write_complete (void)
{
	while (void* packet = const_cast<void*> (_uart->async_write_complete()))
		operator delete (packet);
}

void Comm::checkReceiveQueue (void)
{
	const UART_ReceiveQueue* rx = _uart->read();
	processReceivedData (rx->data(), rx->size());
}

void Comm::processReceivedData (const void* data, uint16_t size)
{
	const uint8_t* src = reinterpret_cast<const uint8_t*> (data);

	if (size) do {

		_qp4->receiver->push_back (*src++);
// 		if (size == 4)
// 		debug_comm();
		if (_qp4->receiver->ready()) {

			uint16_t size;
			const uint8_t* data;
			std::tie (data, size) = _qp4->receiver->data();

			interpret (data, size);
			_qp4->receiver->clear();
		}

	} while (--size);
}

void Comm::interpret (const void* data, uint16_t size)
{
	typedef void (Comm::*cb_t)(const void*, uint16_t);

	static const cb_t cbs[] PROGMEM =
	{
		&Comm::nopCB,
		&Comm::identityCB,
		&Comm::keepAliveCB
	};

// 	debug_comm();
	if (size < sizeof (CommPacket))
		return;

	const CommPacket *packet =
	    reinterpret_cast<const CommPacket *> (data);

	if (packet->opcode() < sizeof (cbs) / sizeof (cbs[0]))
		(this->*pgm_read (cbs[packet->opcode()]))(data, size);
}

/******************************************************************/

void Comm::nopCB (const void*, uint16_t)
{}

void Comm::identityCB (const void* , uint16_t size)
{
	if (size < sizeof (CommRequest_Identity))
		return;
	
	do_callback (new (&_callbackObject) CommCB_Identity);
}

/*********************************************************************/

void Comm::keepAliveCB (const void* data, uint16_t size)
{
	if (size < sizeof (CommRequest_KeepAlive))
		return;

	do_callback (new (&_callbackObject)
		CommCB_KeepAlive());
	
// 	debug_comm ();
}

/******************************************************************/
/******************************************************************/

void Comm::transmitIdentity (const char* identity)
{
	typedef CommResponse_Identity response_type;
	typedef QP4_Packet<sizeof (response_type)> packet_type;
	packet_type* packet = new packet_type;

	if (packet) {

		new (packet->body()) response_type (identity);
		packet->seal();

		if (transmit (packet, packet->size()) == 0)
			delete packet;
	}
}

/******************************************************************/

void Comm::transmitKeepAlive (void)
{
	typedef CommResponse_KeepAlive response_type;
	typedef QP4_Packet<sizeof (response_type)> packet_type;
	packet_type* packet = new packet_type;

	if (packet) {

		new (packet->body()) response_type();
		packet->seal();

		if (transmit (packet, packet->size()) == 0)
			delete packet;
	}
}

/******************************************************************/

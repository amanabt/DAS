#include "../include/Comm.h"
#include <iostream>

using namespace DAS;

/***************************************************************************/


Comm::Comm (void) :
	_qp4	(new QP4),
	_ftdi	(new FTDI)
{}

/**************************************************************************/


Comm::~Comm (void)
{
	if (_ftdi->good())
		_ftdi->close();
}

/***************************************************************************/

std::vector<FTDI_DeviceInfo> Comm::scan (void)
{
	return FTDI::scan();
}

/***************************************************************************/
/***************************************************************************/

void Comm::open (const char* serialNo)
{
	_ftdi->open (serialNo, 38400);

	if (_ftdi->good() && open_)
		open_ (serialNo);
}

/***************************************************************************/

void Comm::close (void)
{
	_ftdi->close();

	if (close_)
		close_();
}

/***************************************************************************/

// class CommRequest_Debug
// {
// public:
// 	CommRequest_Debug (void) :
// 		_test (0b10101010)
// 	{}
// 
// private:
// 	uint8_t _test;
// };

void Comm::identify (void)
{
	QP4_Packet* req =
	    _qp4->transmitter().alloc_packet (
			sizeof (CommRequest_Identity));

	new (req->body()) CommRequest_Identity;

	req->seal();
	transmit (req);
	_qp4->transmitter().free_packet (req);
}

/***************************************************************************/

void Comm::keep_alive (float lease_time)
{
	QP4_Packet* req =
	    _qp4->transmitter().alloc_packet (
			sizeof (CommRequest_KeepAlive));

	new (req->body()) CommRequest_KeepAlive ();

	req->seal();
	transmit (req);
	_qp4->transmitter().free_packet (req);
}


/*********************************************************************/
/*********************************************************************/

int Comm::check (void) {
	int activity = 0;
	activity += read_interface();
	return activity;
}

/******************************************************************************/
/******************************************************************************/

void Comm::transmit (const QP4_Packet* packet)
{
	uint32_t written = 0;
	uint16_t size = packet->size();
	const char* src = reinterpret_cast<const char*> (packet);

	do {

		written += _ftdi->write (src + written, size - written);

	} while (_ftdi->good() && (written < size));
}

/******************************************************************************/
/******************************************************************************/

int Comm::read_interface (void)
{
	char rxbuf[4096];
	unsigned int rxsize = 0;

	if (_ftdi->good()) {

		rxsize = _ftdi->read (rxbuf, sizeof (rxbuf));
		if (rxsize) process_rxdata (rxbuf, rxsize);
	}

	return rxsize;
}

/******************************************************************/

void Comm::process_rxdata (const void* data, uint16_t size)
{
	const uint8_t* src = static_cast<const uint8_t*> (data);

	if (size) do {

		_qp4->receiver().push_back (*src++);

		if (_qp4->receiver().ready()) {

			void* data; uint16_t size;
			std::tie (data, size) = _qp4->receiver().data();

			interpret_rxdata (data, size);
			_qp4->receiver().clear();
		}

	} while (--size);
}

/***************************************************************************/
/***************************************************************************/

void Comm::interpret_rxdata (const void* data, uint16_t size)
{
	typedef void (Comm::*cb_t)(const void*, uint16_t);

	static const cb_t cbs[] =
	{
		&Comm::_nop,
		&Comm::_identify,
		&Comm::_keep_alive
	};

	if (size >= sizeof (CommPacket)) {

		const CommPacket *packet =
			static_cast<const CommPacket *> (data);

		unsigned int opcode = static_cast<unsigned int> (packet->opcode());

		if (opcode < sizeof (cbs) / sizeof (cbs[0]))
			(this->*cbs[opcode])(data, size);
	}
}

/***************************************************************************/

void Comm::_nop (const void*, uint16_t)
{}

/***************************************************************************/

void Comm::_identify (const void* data, uint16_t size)
{
	if (size >= sizeof (CommResponse_Identity)) {

		const CommResponse_Identity* response =
			static_cast<const CommResponse_Identity*> (data);

		if(identify_)
			identify_(response->identity());
	}
}

/***************************************************************************/

void Comm::_keep_alive (const void* data, uint16_t size)
{
	if (size >= sizeof (CommResponse_KeepAlive))
		if (keep_alive_)
			keep_alive_();
}

/***************************************************************************/
/***************************************************************************/

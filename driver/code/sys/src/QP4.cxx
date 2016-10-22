#include "../include/QP4.h"
#include <stdlib.h>

using namespace DAS;

QP4_Packet *QP4_Packet::alloc (uint16_t size)
{
	void* mem = malloc (size + sizeof (QP4_Packet));
	return new (mem) QP4_Packet (size);
}

void QP4_Packet::free (QP4_Packet *packet)
{
	::free (packet);
}

QP4_Packet::QP4_Packet (uint16_t size) :
    _startFrameMarker (hton (QP4_SOF_MARKER)),
    _size (hton (size)),
    _checksum (hton ((uint16_t)((~0) + 1)))
{}

void QP4_Packet::seal (void)
{
	uint16_t checksum = 0;
	const uint8_t* begin = _body;
	const uint8_t* end = begin + datasize();

	for (const uint8_t* it = begin; it != end; ++it)
		checksum += *it;

	checksum = ~checksum + 1;
	_checksum = hton (checksum);
}

/***********************************************************************/

QP4_Packet *QP4_Transmitter::alloc_packet (uint16_t size)
{
	return QP4_Packet::alloc (size);
}

void QP4_Transmitter::free_packet (QP4_Packet *packet)
{
	QP4_Packet::free (packet);
}

/***********************************************************/

QP4_Receiver::QP4_Receiver (void) :
	_state            (QP4_RX_STATE_IDLE),
	_size             (0),
	_expectedChecksum ((~0) + 1),
	_receivedChecksum (0),
	_data             (new uint8_t [_maxAllowedDataSize]),
	_dataWriter       (_data.get()),
	_ready            (false),
	_window           (0),
	_byteCounter      (0)
{}

void QP4_Receiver::clear (void)
{
	_state            = QP4_RX_STATE_IDLE;
	_size             = 0;
	_expectedChecksum = ((~0) + 1);
	_receivedChecksum = 0;
	_dataWriter       = _data.get();
	_ready            = false;
	_window           = 0;
	_byteCounter      = 0;
}

/***********************************************************/

void QP4_Receiver::push_back (uint8_t x)
{
	typedef void (QP4_Receiver::*Callback)(uint8_t x);

	Callback callbacks[] =
	{
		&QP4_Receiver::idle_cb,
		&QP4_Receiver::size_cb,
		&QP4_Receiver::checksum_cb,
		&QP4_Receiver::data_cb,
	};

	_window = (_window << 8) | (uint32_t)(x);

	if (_window == QP4_SOF_MARKER) {

		initiateReceptionSequence();
		return;
	}

	(this->*callbacks[_state])(x);
}

void QP4_Receiver::setState (QP4_RxState state)
{
	typedef void (QP4_Receiver::*cb_t)(void);

	static const cb_t cbs[] = {

		&QP4_Receiver::setStateIdle_cb,
		&QP4_Receiver::setStateSize_cb,
		&QP4_Receiver::setStateChecksum_cb,
		&QP4_Receiver::setStateData_cb
	};

	_state = state;
	(this->*cbs[_state])();
}

void QP4_Receiver::setStateIdle_cb (void)
{}

void QP4_Receiver::setStateSize_cb (void)
{
	_byteCounter = sizeof (_size);
}

void QP4_Receiver::setStateChecksum_cb (void)
{
	_byteCounter = sizeof (_receivedChecksum);
}

void QP4_Receiver::setStateData_cb (void)
{
	_byteCounter = _size;
}

/***********************************************************/

void QP4_Receiver::initiateReceptionSequence (void)
{
	clear();
	setState (QP4_RX_STATE_SIZE);
}

void QP4_Receiver::abortReceptionSequence (void)
{
	clear();
}

/***********************************************************/

void QP4_Receiver::idle_cb (uint8_t x)
{}

void QP4_Receiver::size_cb (uint8_t x)
{
	_size = (_size << 8) | x;

	if (--_byteCounter == 0) {

		if (_size > _maxAllowedDataSize)
			abortReceptionSequence();
		else
			setState (QP4_RX_STATE_CHECKSUM);
	}
}

void QP4_Receiver::checksum_cb (uint8_t x)
{
	_receivedChecksum = (_receivedChecksum << 8) | x;

	if (--_byteCounter == 0) {

		if (_size)
			setState (QP4_RX_STATE_DATA);

		else {

			if (_receivedChecksum == _expectedChecksum)
				_ready = true;

			setState (QP4_RX_STATE_IDLE);
		}
	}
}

void QP4_Receiver::data_cb (uint8_t x)
{
	*_dataWriter++ = x;
	_expectedChecksum += x;

	if (--_byteCounter == 0) {

		_expectedChecksum = ~_expectedChecksum + 1;

		if (_expectedChecksum == _receivedChecksum)
			_ready = true;

		setState (QP4_RX_STATE_IDLE);
	}
}

/***********************************************************/

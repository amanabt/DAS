#include "../include/QP4.h"
#include "pgmspace"

using namespace std;

QP4_PacketBase::QP4_PacketBase (uint16_t size) :
    _startFrameMarker (std::hton (QP4_SOF_MARKER)),
    _size (std::hton (size)),
    _checksum (std::hton ((uint16_t)((~0) + 1)))
{}

void QP4_PacketBase::seal (void)
{
	uint16_t checksum = 0;
	const uint8_t* begin = _body;
	const uint8_t* end = begin + datasize();

	for (const uint8_t* it = begin; it != end; ++it)
		checksum += *it;

	checksum = ~checksum + 1;
	_checksum = std::hton (checksum);
}

/***********************************************************************/

QP4_Receiver::QP4_Receiver (void) :
	_state            (QP4_RxState::IDLE),
	_size             (0),
	_expectedChecksum ((~0) + 1),
	_receivedChecksum (0),
	_data             (0),
	_dataWriter       (0),
	_ready            (false),
	_window           (0),
	_byteCounter      (0)
{}

void QP4_Receiver::clear (void)
{
	_state            = QP4_RxState::IDLE;
	_size             = 0;
	_expectedChecksum = ((~0) + 1);
	_receivedChecksum = 0;
	_data.reset         (_dataWriter = 0);
	_ready            = false;
	_window           = 0;
	_byteCounter      = 0;
}

/***********************************************************/

void QP4_Receiver::push_back (uint8_t x)
{
	_window = (_window << 8) | x;

	if (_window == QP4_SOF_MARKER)
		initiateReceptionSequence();

	else switch (_state)
	{
		case QP4_RxState::IDLE     : this->idle_cb     (x); break;
		case QP4_RxState::SIZE     : this->size_cb     (x); break;
		case QP4_RxState::CHECKSUM : this->checksum_cb (x); break;
		case QP4_RxState::DATA     : this->data_cb     (x); break;
	}
}

void QP4_Receiver::setState (QP4_RxState state)
{
	switch (_state = state)
	{
		case QP4_RxState::IDLE     : this->setStateIdle_cb     (); break;
		case QP4_RxState::SIZE     : this->setStateSize_cb     (); break;
		case QP4_RxState::CHECKSUM : this->setStateChecksum_cb (); break;
		case QP4_RxState::DATA     : this->setStateData_cb     (); break;
	}
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
	setState (QP4_RxState::SIZE);
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

		if (_size > QP4_Receiver::max_datasize)
			abortReceptionSequence();

		else
			setState (QP4_RxState::CHECKSUM);
	}
}

void QP4_Receiver::checksum_cb (uint8_t x)
{
	_receivedChecksum = (_receivedChecksum << 8) | x;

	if (--_byteCounter == 0) {

		if (_size) {

			_data.reset (_dataWriter = new uint8_t[_size]);
			setState (QP4_RxState::DATA);
		}

		else {

			if (_receivedChecksum == _expectedChecksum)
				_ready = true;

			setState (QP4_RxState::IDLE);
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

		setState (QP4_RxState::IDLE);
	}
}

/***********************************************************/

QP4::QP4 (void) :
	receiver (new QP4_Receiver)
{}

/***********************************************************/

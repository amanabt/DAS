	#ifndef __DAS_QP4__
#define __DAS_QP4__

#include "../../stl/inet"

#include <tuple>
#include <memory>
#include <stdint.h>

namespace DAS {

enum QP4_Enumerations
{
	QP4_SOF_MARKER  =
	    (((uint32_t)('Q') << 24) |
	     ((uint32_t)('P') << 16) |
	     ((uint32_t)('4') << 8) |
	     ((uint32_t)('1') << 0)),
};

class QP4_Packet
{
public:
	static void free (QP4_Packet *);
	static QP4_Packet *alloc (uint16_t size);

public:
	void* body (void) {return _body;}
	const void* body (void) const {return _body;}

public:
	uint16_t datasize (void) const {return DAS::ntoh (_size);}
	uint16_t size (void) const {
		return DAS::ntoh (_size) + sizeof (QP4_Packet);
	}

public:
	void seal (void);

private:
	uint32_t _startFrameMarker;
	uint16_t _size, _checksum;
	uint8_t _body[];

private:
	QP4_Packet (uint16_t size);
};

class QP4_Transmitter
{
public:
	void free_packet (QP4_Packet *packet);
	QP4_Packet *alloc_packet (uint16_t size);
};

enum QP4_RxState
{
	QP4_RX_STATE_IDLE,
	QP4_RX_STATE_SIZE,
	QP4_RX_STATE_CHECKSUM,
	QP4_RX_STATE_DATA
};

class QP4_Receiver
{
public:
	QP4_Receiver (void);

public:
	bool ready (void) const { return _ready; }

	std::tuple<void*, uint16_t> data (void) const
	{
		return std::tuple<void*, uint16_t> (_data.get(), _size);
	}

public:
	void clear (void);
	void push_back (uint8_t x);

private:
	QP4_RxState _state;

private:
	uint16_t _size;
	uint16_t _expectedChecksum, _receivedChecksum;

private:
	std::unique_ptr<uint8_t[]> _data;
	static const uint16_t _maxAllowedDataSize = 4096;
	uint8_t* _dataWriter;

private:
	void initiateReceptionSequence (void);
	void abortReceptionSequence (void);

private:
	void idle_cb (uint8_t x);
	void size_cb (uint8_t x);
	void checksum_cb (uint8_t x);
	void data_cb (uint8_t x);

private:
	void setState (QP4_RxState state);
	void setStateIdle_cb (void);
	void setStateSize_cb (void);
	void setStateChecksum_cb (void);
	void setStateData_cb (void);

private:
	bool _ready;
	uint32_t _window;
	uint16_t _byteCounter;
};

class QP4 : public QP4_Receiver, public QP4_Transmitter
{
public:
	QP4_Receiver &receiver (void)
	{
		return static_cast<QP4_Receiver &> (*this);
	}

	QP4_Transmitter &transmitter (void)
	{
		return static_cast<QP4_Transmitter &> (*this);
	}
};

}; // namespace DAS

#endif

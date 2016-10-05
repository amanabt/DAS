#ifndef __QP4__
#define __QP4__

#include "inet"
#include <vector>
#include <memory>
#include <utility>
#include <stdint.h>
#include <stddef.h>

enum QP4_Enumerations
{
	QP4_SOF_MARKER  =
	    (((uint32_t)('Q') << 24) |
	     ((uint32_t)('P') << 16) |
	     ((uint32_t)('4') << 8)  |
	     ((uint32_t)('1') << 0)),
};

/******************************************************************/
/******************************************************************/

class QP4_PacketBase
{
protected:
	QP4_PacketBase (uint16_t size);

public:
	void* body (void) {return _body;}
	const void* body (void) const {return _body;}

public:
	uint16_t datasize (void) const { return std::ntoh (_size); }

public:
	void seal (void);

private:
	uint32_t _startFrameMarker;
	uint16_t _size, _checksum;
	uint8_t* _body;
};

/******************************************************************/

template <uint16_t N>
class QP4_Packet : public QP4_PacketBase
{
public:
	QP4_Packet (void) : QP4_PacketBase (N) {}

public:
	uint16_t size (void) const { return sizeof (*this); }

public:
	static const QP4_Packet* body_to_packet (const void* body)
	{
		const char* __body = reinterpret_cast<const char*> (body);
		const char* __packet = __body - offsetof (QP4_Packet, _body);
		return reinterpret_cast<const QP4_Packet*> (__packet);
	}

public:
	static QP4_Packet* body_to_packet (void* body)
	{
		char* __body   = reinterpret_cast<char*> (body);
		char* __packet = __body - offsetof (QP4_Packet, _body);
		return reinterpret_cast<QP4_Packet*> (__packet);
	}

private:
	uint8_t _body[N];
};

/******************************************************************/
/******************************************************************/

enum class QP4_RxState
{
	IDLE,
	SIZE,
	CHECKSUM,
	DATA
};

class QP4_Receiver
{
public:
	QP4_Receiver (void);

public:
	bool ready (void) const { return _ready; }
	std::pair<uint8_t*, uint16_t> data (void)
	{
		return std::make_pair (_data.get(), _size);
	}

public:
	void clear (void);
	void push_back (uint8_t x);

private:
	QP4_RxState _state;

private:
	uint16_t _size;
	uint16_t _expectedChecksum, _receivedChecksum;

	std::unique_ptr<uint8_t[]> _data;
	uint8_t* _dataWriter;

public:
#if defined (linux)  || defined (__linux) || defined (__linux__) || \
    defined (_WIN32) || defined (_WIN64)
	static const uint16_t max_datasize = 1024;
#else
	static const uint16_t max_datasize = 64;
#endif

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

class QP4
{
public:
	QP4 (void);

public:
	std::unique_ptr<QP4_Receiver> receiver;
};

#endif

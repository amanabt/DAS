#ifndef __UART__
#define __UART__

#include <utility>
#include <stdint.h>
#include <deque>

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 9600
#endif

class UART_ReceiveQueue
{
public:
	UART_ReceiveQueue (void) :
	_writer (_space)
	{}

public:
	void clear (void) { _writer = _space; }
	void push_back (char data) { *_writer++ = data; }

public:
	bool empty (void) const { return (_writer == _space); }
	uint16_t size (void) const { return (uint16_t)(_writer - _space); }

public:
	const void* data (void) const {return _space;}
	const char& at (uint16_t i) const {return _space[i];}
	const char& operator[] (uint16_t i) const {return _space[i];}

private:
	char _space[128], *_writer;
};

/*******************************************************************/

class TxTransfer
{
public:
	TxTransfer (void) : _data (0), _size (0) {}

public:
	TxTransfer (const char* data, uint16_t size) :
		_data (data),
		_size (size)
	{}

public:
	void clear (void) { _data = 0; _size = 0; }

public:
	bool empty (void) const { return (_data == 0); }

public:
	const char* data (void) const { return _data; }
	uint16_t    size (void) const { return _size; }

private:
	const char* _data;
	uint16_t _size;
};

/*******************************************************************/
/*******************************************************************/

class UART
{
public:
	static UART* _ (void);

public:
	void baudrate (uint32_t baudrate);
	uint32_t baudrate (void) const {return _baudrate;}

public:
	const UART_ReceiveQueue* read (void);
	const void* async_write (const void* data, uint16_t size);
	const void* async_write_complete (void);

private:
	uint32_t _baudrate;

	/**** Receive functions and resources ****/

private:
	UART_ReceiveQueue _rxq[2];
	static UART_ReceiveQueue *_activeRxq, *_standbyRxq;

public:
	void receiveCompleteISR (void);
	void disableReceiveCompleteInterrupt (void) const;
	void enableReceiveCompleteInterrupt (void) const;

	/**** Transmit functions ****/

public:
	void dataRegisterEmptyISR (void);

private:
	void enableDataRegisterEmptyInterrupt (void) const;
	void disableDataRegisterEmptyInterrupt (void) const;

	/**** Misc ****/

public:
	void transmitCompleteISR (void);

private:
	void enableTransmitCompleteInterrupt (void) const;
	void disableTransmitCompleteInterrupt (void) const;

	/**** Transmit resources ****/

private:
	std::deque<TxTransfer, 2> _in_txq;
	std::deque<TxTransfer, 2> _out_txq;
	TxTransfer _activeTxTransfer;
	const char* _tx_pointer;
	uint16_t _tx_size;
	bool _tx_complete;

private:
	UART (void);
};

#endif

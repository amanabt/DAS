#include "../include/UART.h"
#include "../../sys/include/hardware.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include <algorithm>
#include <cstring>

static UART* __this = 0;

#if defined (__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined (__AVR_ATmega328__)

#define UCSRA             UCSR0A
#define UCSRB             UCSR0B
#define UCSRC             UCSR0C
#define UDR               UDR0
#define UBRRH             UBRR0H
#define UBRRL             UBRR0L

// #define USART_RXC_vect    USART0_RX_vect
// #define USART_TXC_vect    USART0_TX_vect
// #define USART_UDRE_vect   USART0_UDRE_vect
// 
// #define USART_RXC_vect_num    USART_RX_vect_num
// #define USART_TXC_vect_num    USART_TX_vect_num
// #define USART_UDRE_vect_num   USART_UDRE_vect_num

#define RXC               RXC0
#define TXC               TXC0
#define UDRE              UDRE0
#define U2X               U2X0
#define MPCM              MPCM0

#define RXCIE             RXCIE0
#define TXCIE             TXCIE0
#define UDRIE             UDRIE0
#define RXEN              RXEN0
#define TXEN              TXEN0

#endif

/***************************************************************/

// UCSRA
#define RECEIVE_COMPLETE_MASK                     (0x1 << RXC)
#define TRANSMIT_COMPLETE_MASK                    (0x1 << TXC)
#define DATA_REGISTER_EMPTY_MASK                  (0x1 << UDRE)

#define NO_DOUBLE_TRANSMISSION_SPEED              (0x0 << U2X)
#define NO_MULTI_PROCESSOR_COMMUNICATION_MODE     (0x0 << MPCM)

// UCSRB
#define RECEIVE_COMPLETE_INTERRUPT_MASK           (0x1 << RXCIE)
#define RECEIVE_COMPLETE_INTERRUPT_ENABLE         (0x1 << RXCIE)
#define RECEIVE_COMPLETE_INTERRUPT_DISABLE        (0x0 << RXCIE)

#define TRANSMIT_COMPLETE_INTERRUPT_MASK          (0x1 << TXCIE)
#define TRANSMIT_COMPLETE_INTERRUPT_ENABLE        (0x1 << TXCIE)
#define TRANSMIT_COMPLETE_INTERRUPT_DISABLE       (0x0 << TXCIE)

#define DATA_REGISTER_EMPTY_INTERRUPT_MASK        (0x1 << UDRIE)
#define DATA_REGISTER_EMPTY_INTERRUPT_ENABLE      (0x1 << UDRIE)
#define DATA_REGISTER_EMPTY_INTERRUPT_DISABLE     (0x0 << UDRIE)

#define RECEIVER_ENABLE                           (0x1 << RXEN)
#define TRANSMITTER_ENABLE                        (0x1 << TXEN)

// UCSRC
#if defined __AVR_ATmega644__
#define UCSRC_ACCESS                              (0x0 << 7)
#elif defined __AVR_ATmega32__
#define UCSRC_ACCESS                              (0x1 << 7)
#elif defined (__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined (__AVR_ATmega328__)
#define UCSRC_ACCESS                              (0X0 << 7)
#endif

#define ASYNC_OPERATION                           (0x0 << 6)
#define PARITY_DISABLE                            (0x0 << 4)
#define STOP_BIT_1                                (0x0 << 3)
#define CHAR_SIZE_8                               (0x3 << 1)

#define UBRRH_ACCESS                              (0x0 << 7)

/***************************************************************/

UART_ReceiveQueue* UART::_activeRxq  = 0;
UART_ReceiveQueue* UART::_standbyRxq = 0;

UART* UART::_ (void)
{
	static UART* o = new UART;
	return o;
}

UART::UART (void) :
	_tx_pointer (0),
	_tx_size (0),
	_tx_complete (true)
{
	__this      = this;
	_activeRxq  = _rxq + 0;
	_standbyRxq = _rxq + 1;

	UCSRA = NO_DOUBLE_TRANSMISSION_SPEED |
			NO_MULTI_PROCESSOR_COMMUNICATION_MODE;

	UCSRB = RECEIVE_COMPLETE_INTERRUPT_ENABLE  |
			TRANSMIT_COMPLETE_INTERRUPT_ENABLE |
			RECEIVER_ENABLE                    |
			TRANSMITTER_ENABLE;

	UCSRC = UCSRC_ACCESS    |
			ASYNC_OPERATION |
			PARITY_DISABLE  |
			STOP_BIT_1      |
			CHAR_SIZE_8;

	baudrate (UART_BAUDRATE);
}

void UART::baudrate (uint32_t baudrate)
{
	_baudrate = baudrate;

	const uint16_t baudReg12 =
		(uint16_t)(cpu_clock() / 16 / baudrate - 1) & 0x0FFF;
	
	const uint8_t baudRegMSB = (uint8_t)(baudReg12 >> 8);
	const uint8_t baudRegLSB = (uint8_t)(baudReg12);

	UBRRH = baudRegMSB | UBRRH_ACCESS;
	UBRRL = baudRegLSB;
}

/***************************************************************/

ISR (USART_RX_vect)
{
	__this->receiveCompleteISR();
}

void UART::receiveCompleteISR (void)
{
	while (UCSRA & RECEIVE_COMPLETE_MASK) {

		uint8_t rxdata = UDR;
// 		UDR = rxdata;
		_activeRxq->push_back (rxdata);
	}
}

const UART_ReceiveQueue* UART::read (void)
{
	_standbyRxq->clear();
	UART_ReceiveQueue* __activeRxq  = _activeRxq;
	UART_ReceiveQueue* __standbyRxq = _standbyRxq;

	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		_activeRxq  = __standbyRxq;
		_standbyRxq = __activeRxq;
	}

	return _standbyRxq;
}

void UART::disableReceiveCompleteInterrupt (void) const
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		UCSRB &= ~RECEIVE_COMPLETE_INTERRUPT_ENABLE;
	}
}

void UART::enableReceiveCompleteInterrupt (void) const
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		UCSRB |= RECEIVE_COMPLETE_INTERRUPT_ENABLE;
	}
}

/***************************************************************/
/***************************************************************/

const void* UART::async_write (const void* data, uint16_t size)
{
	const void* rv = 0;

	disableTransmitCompleteInterrupt();
	{
		if (!_in_txq.full()) {

			rv = data;

			_in_txq.push_back (
				TxTransfer (reinterpret_cast<const char*> (data), size));

			if (_activeTxTransfer.empty()) {

				_activeTxTransfer = _in_txq.front();
				_in_txq.pop_front();

				_tx_pointer  = _activeTxTransfer.data();
				_tx_size     = _activeTxTransfer.size();
				_tx_complete = false;

				enableDataRegisterEmptyInterrupt();
			}
		}
	}
	enableTransmitCompleteInterrupt();

	return rv;
}

const void* UART::async_write_complete (void)
{
	const void* rv = 0;

	disableTransmitCompleteInterrupt();
	{
		if (!_out_txq.empty()) {

			rv = _out_txq.front().data();
			_out_txq.pop_front();
		}

		if (_tx_complete && !_activeTxTransfer.empty()) {

			_out_txq.push_back (_activeTxTransfer);
			_activeTxTransfer.clear();

			if (!_in_txq.empty()) {

				_activeTxTransfer = _in_txq.front();
				_in_txq.pop_front();

				_tx_pointer  = _activeTxTransfer.data();
				_tx_size     = _activeTxTransfer.size();
				_tx_complete = false;

				enableDataRegisterEmptyInterrupt();
			}
		}
	}
	enableTransmitCompleteInterrupt();

	return rv;
}

/***************************************************************/

ISR (USART_TX_vect)
{
	__this->transmitCompleteISR();
}

void UART::transmitCompleteISR (void)
{
	_tx_complete = true;

	if (!_out_txq.full()) {

		_out_txq.push_back (_activeTxTransfer);
		_activeTxTransfer.clear();

		if (!_in_txq.empty()) {

			_activeTxTransfer = _in_txq.front();
			_in_txq.pop_front();

			_tx_pointer  = _activeTxTransfer.data();
			_tx_size     = _activeTxTransfer.size();
			_tx_complete = false;

			enableDataRegisterEmptyInterrupt();
		}
	}
}

void UART::disableTransmitCompleteInterrupt (void) const
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		UCSRB &= ~TRANSMIT_COMPLETE_INTERRUPT_ENABLE;
	}
}

void UART::enableTransmitCompleteInterrupt (void) const
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		UCSRB |= TRANSMIT_COMPLETE_INTERRUPT_ENABLE;
	}
}

/***************************************************************/

ISR (USART_UDRE_vect)
{
	__this->dataRegisterEmptyISR();
}

void UART::dataRegisterEmptyISR (void)
{
	UDR = *_tx_pointer++;

	if (--_tx_size == 0)
		disableDataRegisterEmptyInterrupt();
}

void UART::enableDataRegisterEmptyInterrupt (void) const
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		UCSRB |= DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;
	}
}

void UART::disableDataRegisterEmptyInterrupt (void) const
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		UCSRB &= ~DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;
	}
}

/***************************************************************/

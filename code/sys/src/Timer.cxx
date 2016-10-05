#include "../include/Timer.h"

#include <avr/io.h>
#include <util/atomic.h>

static Timer* __this = 0;

#if defined __AVR_ATmega32__
/*************************************************************************/

#define TCCR_MODE_CTC                 0x08
#define TCCR_NORMAL_PORT_OPERATION    0x00
#define TCCR_PRESCALER_1024           0x05

#define TIMSK_ENABLE_CTC_INTERRUPT   (1 << OCIE0)

Timer::Timer (void)
{
	for (auto it = _cbs; it != _cbs + max_callbacks(); ++it) {

		it->_cb = 0;
		it->_userdata = 0;
	}

	TCNT0 = 0;
	OCR0  = counterStartValue();

	TIMSK |= TIMSK_ENABLE_CTC_INTERRUPT;

	TCCR0 = TCCR_MODE_CTC |
			TCCR_NORMAL_PORT_OPERATION |
			TCCR_PRESCALER_1024;

	__this = this;
}

void Timer::end_atomic (void)
{
	TIMSK |= TIMSK_ENABLE_CTC_INTERRUPT;
}

void Timer::begin_atomic (void)
{
	TIMSK &= ~TIMSK_ENABLE_CTC_INTERRUPT;
}

ISR (TIMER0_COMP_vect, ISR_NOBLOCK)
{
	__this->ctcISR();
}

#elif defined (__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined (__AVR_ATmega328__)
/********************************************************************/

#define TCCR0A_NORMAL_PORT_OPERATION_OCR0A ((0 << COM0A1) | (0 << COM0A0))
#define TCCR0A_NORMAL_PORT_OPERATION_OCR0B ((0 << COM0B1) | (0 << COM0B0))

#define TCCR0A_MODE_CTC         ((1 << WGM01) | (0 << WGM00))
#define TCCR0B_MODE_CTC         (0 << WGM02)

#define TCCR0B_PRESCALER_64     ((0 << CS02) | (1 << CS01) | (1 << CS00))
#define TCCR0B_PRESCALER_1024   ((1 << CS02) | (0 << CS01) | (1 << CS00))

#define TIMSK0_ENABLE_CTC_INTERRUPT (1 << OCIE0A)

Timer::Timer (void)
{
	for (auto it = _cbs; it != _cbs + max_callbacks(); ++it) {

		it->_cb = 0;
		it->_userdata = 0;
	}

	TCNT0 = 0;
	OCR0A  = counterStartValue();

	TIMSK0 |= TIMSK0_ENABLE_CTC_INTERRUPT;

	TCCR0A = TCCR0A_NORMAL_PORT_OPERATION_OCR0A |
			 TCCR0A_NORMAL_PORT_OPERATION_OCR0B |
			 TCCR0A_MODE_CTC;

	TCCR0B = TCCR0B_MODE_CTC |
			 TCCR0B_PRESCALER_1024;

	__this = this;
}

void Timer::end_atomic (void)
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		TIMSK0 |= TIMSK0_ENABLE_CTC_INTERRUPT;
	}
}

void Timer::begin_atomic (void)
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		TIMSK0 &= ~TIMSK0_ENABLE_CTC_INTERRUPT;
	}
}

ISR (TIMER0_COMPA_vect)
{
	__this->ctcISR();
}

#endif

Timer* Timer::_ (void)
{
	static Timer* o = new Timer;
	return o;
}

void Timer::ctcISR (void)
{
	TIMSK0 &= ~TIMSK0_ENABLE_CTC_INTERRUPT;
	NONATOMIC_BLOCK (NONATOMIC_FORCEOFF)
	{
		do_tick();
		do_callback();
	}
	TIMSK0 |= TIMSK0_ENABLE_CTC_INTERRUPT;
}

void Timer::do_tick (void)
{
	++_ticks;
}

void Timer::do_callback (void) const
{
	for (auto it = _cbs; it != _cbs + max_callbacks(); ++it)
		if (it->_cb)
			(*it->_cb)(it->_userdata);
}

void Timer::callback (Callback cb, void* userdata)
{
	for (auto it = _cbs; it != _cbs + max_callbacks(); ++it)

		if (!it->_cb) {

			Timer::begin_atomic();
			{
				it->_cb = cb;
				it->_userdata = userdata;
			}
			Timer::end_atomic();

			break;
		}
}

void Timer::remove_callback (Callback cb, void* userdata)
{
	for (auto it = _cbs; it != _cbs + max_callbacks(); ++it)

		if (it->_cb == cb && it->_userdata == userdata) {

			Timer::begin_atomic();
			{
				it->_cb = 0;
				it->_userdata = 0;
			}
			Timer::end_atomic();

			break;
		}
}

Timer::Tick Timer::ticks (void) const
{
	Tick ticks;

	Timer::begin_atomic();
	{
		ticks = _ticks;
	}
	Timer::end_atomic();

	return ticks;
}

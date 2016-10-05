// #define DEBUG_APP

#include "../include/Application.h"
#include "../include/firmware_version.h"
// #include "../include/Calibration.h"
#include "../../sys/include/hardware.h"

#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <pgmspace>

#define DEFAULT_LEASE_TIME    10

#ifdef DEBUG_APP
#define DEBUG_PORT PORTC
#define DEBUG_DDR  DDRC
static constexpr uint8_t DEBUG_BIT = static_cast <uint8_t> (1);
static constexpr uint8_t DEBUG_MSK = static_cast <uint8_t> (1 << DEBUG_BIT);
static constexpr uint8_t HIGH      = 1 << DEBUG_BIT;
static constexpr uint8_t LOW       = 0 << DEBUG_BIT;

void debug (void)
{
	DEBUG_PORT = (DEBUG_PORT & (~DEBUG_MSK)) | (HIGH & DEBUG_MSK);
	_delay_us (1);
	DEBUG_PORT = (DEBUG_PORT & (~DEBUG_MSK)) | (LOW & DEBUG_MSK);
	_delay_us (1);
}
#endif

using namespace std;

/********************************************************************/

void* operator new (unsigned int size)
{
	return malloc (size);
}

void* operator new[] (unsigned int size)
{
	return malloc (size);
}

void operator delete (void *mem)
{
	free (mem);
}

void operator delete[] (void *mem)
{
	free (mem);
}

/********************************************************************/

Application::Application (void) :
	_comm    (Comm::_())
// 	_timer   (Timer::_()),
{
	_comm->callback (appCommCB, this);
	
// 	_end_session_after    = _timer->ticks ();

#ifdef DEBUG_APP
	DEBUG_DDR = (DEBUG_DDR & (~DEBUG_MSK)) | (HIGH & DEBUG_MSK);
#endif

	sei();
}

Application* Application::_ (void)
{
	static Application* o = new Application;
	return o;
}

void Application::run (void)
{
	Applet::run();
}

void Application::monitorSession (void)
{
// 	if (_timer->ticks () > _end_session_after)
// 		endSession ();
}

void Application::endSession (void)
{
	_active_session = false;
}
/********************************************************************/

void Application::appCommCB (void* user_data, const void* oCB)
{
	reinterpret_cast<Application*> (user_data)->
		appCommCB(reinterpret_cast<const CommCB*> (oCB));
}

void Application::appCommCB (const CommCB* oCB)
{
	typedef void (Application::*cb_t)(const CommCB*);

	static const cb_t cbs[] PROGMEM =
	{
		&Application::nopCB,
		&Application::identityCB,
	};

	uint16_t cbcode = static_cast<uint16_t> (oCB->code());

	if (cbcode < sizeof (cbs) / sizeof (cbs[0]))
		(this->*pgm_read (cbs[cbcode]))(oCB);
}

/********************************************************************/

void Application::nopCB (const CommCB*)
{}

void Application::identityCB (const CommCB*)
{
	_active_session = true;
	_comm->transmitIdentity ("PHY315-DAQ");
}

/********************************************************************/
/********************************************************************/


void Application::check (void)
{
	if (isSessionActive ())
	{
		monitorSession ();
	}
// 	debug ();
}

/********************************************************************/
/********************************************************************/

int main (void)
{
	_delay_ms (1e3);
	application->run();
	return 0;
}

/********************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#ifndef TIMER_INTERVAL
#define TIMER_INTERVAL    10e-3
#endif

// #include <cmath>
#include <stdint.h>
#include <stddef.h>
#include "hardware.h"

class Timer
{
public:
	typedef uint32_t Tick;
	typedef void (*Callback)(void*);

public:
	static Timer* _ (void);

public:
	float requested_interval (void) const { return TIMER_INTERVAL; }

	float interval (void) const {
		return (1 + counterStartValue()) / counterClockFrequency();
	}

public:
	void callback (Callback cb, void* userdata);
	void remove_callback (Callback cb, void* userdata);

public:
	Tick ticks (void) const;
	Tick ticks (float time) const {
		return static_cast<Tick> (uint32_t (time / interval()));
	}
	
	float ticks2sec (Tick tick) const {
		return static_cast<float> (tick * interval());
	}

public:
	size_t max_callbacks (void) const {
		return sizeof (_cbs) / sizeof (_cbs[0]);
	}

public:
	static void begin_atomic (void);
	static void end_atomic (void);

private:
	void do_tick (void);
	void do_callback (void) const;

private:
	Timer (void);

	float counterClockFrequency (void) const
	{
		return cpu_clock() / TIMER_CLOCK_PRESCALER;
	}

	uint8_t counterStartValue (void) const
	{
		return static_cast<uint8_t> (
			uint8_t (requested_interval() * counterClockFrequency())) - 1;
	}

private:
	enum { TIMER_CLOCK_PRESCALER = 1024 };
	struct { Callback _cb; void* _userdata; } _cbs[8];
	Tick _ticks;

public:
	void ctcISR (void);
};

#endif

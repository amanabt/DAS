#ifndef __SPI__
#define __SPI__

#include <stdint.h>

typedef uint8_t SPI_Configuration;

enum SPI_ClockFrequency
{
	SPI_CLOCK_MCLK_BY_4,
	SPI_CLOCK_MCLK_BY_16,
	SPI_CLOCK_MCLK_BY_64,
	SPI_CLOCK_MCLK_BY_128
};

enum SPI_ClockPhase
{
	SPI_LEADING_EDGE_SAMPLING,
	SPI_TRAILING_EDGE_SAMPLING,
};

enum SPI_ClockPolarity
{
	SPI_IDLE_CLOCK_LOW,
	SPI_IDLE_CLOCK_HIGH
};

enum SPI_Mode
{
	SPI_MODE_SLAVE,
	SPI_MODE_MASTER
};

enum SPI_DataOrder
{
	SPI_MSB_FIRST,
	SPI_LSB_FIRST
};

class SPI
{
public:
	typedef void (*Callback)(SPI*, void*);

public:
	static SPI* _ (void);

public:
	void write   (const void* data, uint16_t size);
	void write_P (const void* data, uint16_t size);

public:
	void read   (void* data, uint16_t size, char fill);
	void read   (void* data, uint16_t size, const void* fill);
	void read_P (void* data, uint16_t size, const void* fill);

public:
	void reset (void);
	void enable (SPI_Configuration configuration);

public:
	static SPI_Configuration makeConfiguration (SPI_ClockFrequency freq,
												SPI_ClockPhase phase,
												SPI_ClockPolarity polarity,
												SPI_Mode mode,
												SPI_DataOrder order);
public:
	void do_transaction (void* dst, const void* src, uint16_t size);

private:
	SPI (void);
	void write (char c);
	char read (char fill);

private:
	void   first_transaction (uint8_t wdata);
	uint8_t next_transaction (uint8_t wdata);
	uint8_t last_transaction (void);
};

#endif

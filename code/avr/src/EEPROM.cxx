#include "../include/EEPROM.h"
#include <avr/io.h>
#include <util/atomic.h>

#if defined (__AVR_ATmega644__) || defined (__AVR_ATmega88__)

#define EEWE                         EEPE
#define EEMWE                        EEMPE

#endif

/*****************************************************************/

#define EEPROM_WR_MASK              (1 << EEWE)

#define EEPROM_WR                   (1 << EEWE)
#define EEPROM_RD                   (1 << EERE)
#define EEPROM_MASTER_WR_ENABLE     (1 << EEMWE)

EEPROM& EEPROM::_ (void)
{
	static EEPROM o;
	return o;
}

EEPROM::EEPROM (void)
{
	EEAR = 0;
	EEDR = 0;
	EECR = 0;
}

/*****************************************************************/

uint8_t EEPROM::read (uint16_t address)
{
	EEAR = address;
	EECR |= EEPROM_RD;
	return EEDR;
}

void EEPROM::write (uint16_t address, uint8_t data)
{
	EEAR = address;
	EEDR = data;

	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		EECR |= EEPROM_MASTER_WR_ENABLE;
		EECR |= EEPROM_WR;
	}

	while (EECR & EEPROM_WR_MASK);
}

/*****************************************************************/

uint16_t EEPROM::read (uint16_t address, void* data, uint16_t size)
{
	uint16_t remain_size = size;
	uint8_t* rd = reinterpret_cast<uint8_t*> (data);

	if (remain_size) do {

		*rd++ = read (address++);

	} while (--remain_size);

	return size;
}

uint16_t EEPROM::write (uint16_t address, const void* data, uint16_t size)
{
	uint16_t remain_size = size;
	const uint8_t* wd = reinterpret_cast<const uint8_t*> (data);

	if (remain_size) do {

		write (address++, *wd++);

	} while (--remain_size);

	return size;
}

/*****************************************************************/

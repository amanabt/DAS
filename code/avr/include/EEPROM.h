#ifndef __EEPROM__
#define __EEPROM__

#include <stdint.h>

class EEPROM
{
public:
	static EEPROM& _ (void);

public:
	uint16_t read  (uint16_t address, void* data,       uint16_t size);
	uint16_t write (uint16_t address, const void* data, uint16_t size);

private:
	uint8_t read  (uint16_t address);
	   void write (uint16_t address, uint8_t data);

private:
	EEPROM (void);
};

#define eeprom    EEPROM::_()

#endif

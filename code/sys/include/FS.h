#ifndef __FS__
#define __FS__

#include <stdint.h>
#include <memory>
#include "../../avr/include/EEPROM.h"

struct FS_FATEntry
{
	uint16_t size;
	uint16_t address;
};

struct FS_FileHeader
{
	uint16_t size;
	uint16_t checksum;
};

class FS
{
public:
	FS (uint16_t size, const uint16_t* file_sizes);

public:
	const uint16_t& size (void) const { return size_; }

public:
	uint16_t read  (uint16_t file_no, void* data);
	uint16_t write (uint16_t file_no, const void* data);

private:
	uint16_t _size;
	std::unique_ptr<FS_FATEntry[]> _fat;

private:
	uint16_t calculateChecksum (const void* data, uint16_t size);
};

#endif

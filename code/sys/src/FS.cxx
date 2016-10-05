#include "../include/FS.h"
#include "avr/EEPROM.h"
#include "../../app/include/Relay.h"

#include <cstdlib>
#include "pgmspace"

using namespace std;

/*********************************************************/
/*********************************************************/

FS::FS (uint16_t size, const uint16_t* file_sizes) :
	_size (chn_cfg::TOTAL_CHN)
{
	_fat.reset (new FS_FATEntry [_size]);

	uint16_t next = 0;
	for (uint16_t i = 0; i < _size; ++i) {

		_fat[i].address = next;
		next += sizeof (FS_FileHeader) + (
			fat_[i].size = pgm_read (file_sizes[i]));
	}
}

/*********************************************************/
/*********************************************************/

uint16_t FS::calculateChecksum (const void* data, uint16_t size)
{
	const uint8_t* byte = reinterpret_cast<const uint8_t*> (data);

	uint16_t checksum = 0;
	if (size) do {

		checksum += *byte++;

	} while (--size);

	return ~checksum + 1;
}

/*********************************************************/
/*********************************************************/

uint16_t FS::read (uint16_t file_no, void* data)
{
	FS_FileHeader header;
	const FS_FATEntry& fatEntry = _fat [file_no];

	uint16_t address = fatEntry.address;
	address += eeprom.read (address,
							&header,
							sizeof (header));

	if (header.size != fatEntry.size)
		return 0;

	address += eeprom.read (address, data, header.size);

	return (calculateChecksum (data, header.size) == 
		header.checksum) ? header.size : 0;
}

/*********************************************************/
/*********************************************************/

uint16_t FS::write (uint16_t file_no, const void* data)
{
	FS_FileHeader header;
	const FS_FATEntry& fatEntry = fat_[file_no];

	header.size = fatEntry.size;
	header.checksum = calculateChecksum (data, header.size);

	uint16_t address = fatEntry.address;
	address += eeprom.write (address, &header,
							 sizeof (header));
	address += eeprom.write (address, data, header.size);

	return header.size;
}

/*********************************************************/
/*********************************************************/

#include <avr/pgmspace.h>

namespace std
{

template <typename T>
inline T pgm_read (const T& p)
{
	union U
	{
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		T o;
	};

	U u;

	switch (sizeof (p))
	{
		case sizeof (uint8_t):
			u.u8 = pgm_read_byte (&p);
			break;

		case sizeof (uint16_t):
			u.u16 = pgm_read_word (&p);
			break;

		case sizeof (uint32_t):
			u.u32 = pgm_read_dword (&p);
			break;
	}

	return u.o;
}

};

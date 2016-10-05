#ifndef __FIRMWARE_VERSION__
#define __FIRMWARE_VERSION__

#include <stdint.h>

#include <stdint.h>

#define MAKE_VERSION(x,y,z) \
	(((uint32_t)x << 16) | ((uint32_t)y << 8) | (uint32_t)z)

#define MAJOR_VERSION_NO(x)   ((x >> 16) & 0xFF)
#define MINOR_VERSION_NO(x)   ((x >>  8) & 0xFF)
#define BUGFIX_VERSION_NO(x)  ((x >>  0) & 0xFF)

#define FIRMWARE_VERSION    MAKE_VERSION (1, 0, 0)

// Uncomment the following line in the tagged version
//#define FIRMWARE_RELEASED

#endif

#ifndef __HARDWARE__
#define __HARDWARE__

#include <stdint.h>

#ifndef F_CPU
#define F_CPU    8000000UL
#endif

inline double cpu_clock (void) { return F_CPU; }

#endif

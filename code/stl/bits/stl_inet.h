#ifndef __DAS_STL_INET__
#define __DAS_STL_INET__

#include <algorithm>

namespace DAS
{

template<typename T>
inline T hton (const T& x)
{
	union U
	{
		T x;
		char a[sizeof(T)];
	};

	U u;
	u.x = x;

	for (unsigned char i = 0; i < sizeof(T) / 2; ++i)
		std::swap (u.a[i], u.a[sizeof(T) - 1 - i]);

	return u.x;
}

template<typename T>
inline T ntoh (const T& x)
{
	union U
	{
		T x;
		char a[sizeof(T)];
	};

	U u;
	u.x = x;

	for (unsigned char i = 0; i < sizeof(T) / 2; ++i)
		std::swap (u.a[i], u.a[sizeof(T) - 1 - i]);

	return u.x;
}

}; // namespace std

#endif

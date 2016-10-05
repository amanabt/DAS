#ifndef __STD_SWAP__
#define __STD_SWAP__

namespace std
{

template<typename T>
inline void swap (T& a, T& b)
{
	T tmp (a);
	a = b;
	b = tmp;
}

};

#endif

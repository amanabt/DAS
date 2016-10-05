#ifndef __STD_PAIR__
#define __STD_PAIR__

namespace std
{

template<typename T, typename U>
class pair
{
	public:
	pair (void) : first(), second()  {}
	pair (const T& o1, const U& o2) : first (o1), second (o2) {}

	public:
	T first;
	U second;
};

template<typename T, typename U>
inline pair<T, U> make_pair (const T& o1, const U& o2)
{
	return pair<T, U> (o1, o2);
};

}

#endif

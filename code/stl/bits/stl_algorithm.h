#ifndef __STL_ALGORITHM__
#define __STL_ALGORITHM__

namespace std
{

template <typename _Iter, typename _Tp>
_Iter find (_Iter first, _Iter last, const _Tp& val)
{
	for (; first != last; ++first)
		if (*first == val)
			return first;

	return last;
}

template <typename _Iter, typename _Tp>
void fill (_Iter first, _Iter last, const _Tp& val)
{
	while (first != last) *first++ = val;
}

};

#endif

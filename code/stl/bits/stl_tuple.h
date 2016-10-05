#ifndef __S_Tp1L__Tp1_Tp2PLE__
#define __S_Tp1L__Tp1_Tp2PLE__

namespace std
{

template <typename _Tp1, typename _Tp2>
class _pair
{
public:
	_pair (_Tp1& first, _Tp2& second) : first (first), second (second) {}

	template <typename _Pair>
	void operator= (const _Pair& o) { first = o.first; second = o.second; }

public:
	_Tp1& first;
	_Tp2& second;
};

template <typename _Tp1, typename _Tp2>
inline _pair<_Tp1, _Tp2> tie (_Tp1& first, _Tp2& second)
{
	return _pair<_Tp1, _Tp2> (first, second);
}

};

#endif

#ifndef __STL_DEQUE__
#define __STL_DEQUE__

#include <new>
#include <stdint.h>
#include <stddef.h>

namespace std
{

template <class _Tp, uint16_t _Nm>
class deque
{
public:
	deque (void) :
		_begin (_values), _end (_values),
		_empty (true), _full (false)
	{}

public:
	void push_back (const _Tp& o)
	{
		new (_end) _Tp (o);
		if (++_end == _values + _Nm)
			_end = _values;

		_empty = false;
		if (_end == _begin)
			_full = true;
	}

public:
	void pop_front (void)
	{
		_begin->~_Tp();

		if (++_begin == _values + _Nm)
			_begin = _values;

		_full = false;
		if (_begin == _end)
			_empty = true;
	}

public:
	bool full  (void) const { return _full;  }
	bool empty (void) const { return _empty; }

public:
	_Tp& front (void) { return *_begin; }
	const _Tp& front (void) const { return *_begin; }

public:
	void clear (void) { while (!empty()) pop_front(); }
	uint16_t size (void) const { return (_Nm + (_end - _begin)) % _Nm; }

private:
	_Tp _values[_Nm];
	_Tp *_begin, *_end;
	bool _empty, _full;
};

};

#endif

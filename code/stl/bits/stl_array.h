#ifndef __STL_ARRAY__
#define __STL_ARRAY__

#include <stddef.h>
#include <algorithm>

namespace std
{

template <typename _Tp, size_t _Nm>
class array
{
public:
	typedef _Tp                value_type;
	typedef value_type*        iterator;
	typedef const value_type*  const_iterator;
	typedef size_t             size_type;

public:
	      _Tp& at (size_type i)       { return _array[i]; }
	const _Tp& at (size_type i) const { return _array[i]; }

public:
	      _Tp& operator[] (size_type i)       { return _array[i]; }
	const _Tp& operator[] (size_type i) const { return _array[i]; }

public:
	size_type size (void) const { return _Nm; }
	bool empty (void) const { return size() == 0; }

public:
	iterator begin (void) { return _array; }
	iterator end   (void) { return _array + _Nm; }

public:
	const_iterator begin (void) const { return _array; }
	const_iterator end   (void) const { return _array + _Nm; }

public:
	void fill (const _Tp& val) { std::fill (begin(), end(), val); }

private:
	_Tp _array[_Nm];
};

};

#endif

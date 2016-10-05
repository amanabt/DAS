#ifndef __STL_VECTOR__
#define __STL_VECTOR__

#include <new>
#include <stdint.h>

namespace std
{

template <typename T, unsigned int N>
class vector
{
public:
	vector (void) :
		_end (_begin)
	{}

public:
	typedef T* iterator;
	typedef const T* const_iterator;

public:
	void push_back (const T& o) { new (_end++) T (o); }
// 	void pop_back (void)        { delete (--_end);    }
	void pop_back (void)        { (--_end)->~T(); }

public:
	T& front (void) { return _begin[0];   }
	T& back  (void) { return *(_end - 1); }
	const T& front (void) const { return _begin[0];   }
	const T& back  (void) const { return *(_end - 1); }

public:
	T& operator[] (uint16_t idx) { return _begin[idx]; }
	const T& operator[] (uint16_t idx) const { return _begin[idx]; }

public:
	iterator begin (void)      { return _begin;           }
	iterator end   (void)      { return _end;             }
	const_iterator begin (void) const {return _begin;}
	const_iterator end   (void) const {return _end;  }

public:
	uint16_t size (void) const { return _end - _begin;    }
	bool empty (void) const    { return (_end == _begin); }
	bool full  (void) const    { return (_end == _begin + N); }

public:
	void clear (void) { while (!empty()) pop_back(); }

private:
	T _begin[N];
	T* _end;
};

};

#endif

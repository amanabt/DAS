#ifndef __UNIQUE_PTR__
#define __UNIQUE_PTR__

#include <algorithm>

namespace std
{

template<typename _Tp>
struct default_delete
{
	void operator()(_Tp* __ptr) const
	{
		delete __ptr;
	}
};

template<typename _Tp>
struct default_delete<_Tp[]>
{
	void operator()(_Tp* __ptr) const
	{
		delete [] __ptr;
	}
};

template <typename _Tp, typename _Dp = default_delete<_Tp> >
class unique_ptr
{
private:
	typedef _Tp* pointer;
	typedef _Dp deleter_type;

public:
	unique_ptr (void)      : _up (0), _deleter (deleter_type()) {}
	unique_ptr (pointer p) : _up (p), _deleter (deleter_type()) {}

public:
	~unique_ptr (void)
	{
		_deleter (_up);
		_up = pointer();
	}

public:
	void reset (void) { reset (pointer()); }

	void reset (pointer p)
	{
		if (p != _up) {
			std::swap (p, _up);
			_deleter (p);
		}
	}

	pointer get (void) const { return _up; }

	pointer release (void)
	{
		pointer __p = get();
		_up = pointer();
		return __p;
	}

	_Tp&    operator*()  const { return *get(); }
	pointer operator->() const { return  get(); }

	operator bool() const { return _up == pointer() ? false : true; }

private:
	pointer _up;
	deleter_type _deleter;

private:
	unique_ptr (const unique_ptr&);
};


template<typename _Tp, typename _Dp>
class unique_ptr<_Tp[], _Dp>
{
private:
	typedef _Tp* pointer;
	typedef _Dp deleter_type;

public:
	unique_ptr (void)      : _up (0), _deleter (deleter_type()) {}
	unique_ptr (pointer p) : _up (p), _deleter (deleter_type()) {}

public:
	~unique_ptr (void)
	{
		_deleter (_up);
		_up = pointer();
	}

public:
	void reset (void) { reset (pointer()); }

	void reset (pointer p)
	{
		if (p != _up) {
			std::swap (p, _up);
			_deleter (p);
		}
	}

	pointer get (void) const { return _up; }

	pointer release (void)
	{
		pointer __p = get();
		_up = pointer();
		return __p;
	}

	_Tp&    operator*()  const { return *get(); }
	pointer operator->() const { return  get(); }

	operator bool() const { return _up == pointer() ? false : true; }

private:
	pointer _up;
	deleter_type _deleter;

private:
	unique_ptr (const unique_ptr&);
};


}; // namespace std

#endif

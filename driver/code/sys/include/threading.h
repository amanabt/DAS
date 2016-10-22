#ifndef __DAS_THREADING__
#define __DAS_THREADING__

/************************************************************************/

#include <mutex>
#include <vector>

namespace DAS
{

namespace threading
{

template <class _value_type,
		  class _container_type = std::vector<_value_type> >

class vector : protected _container_type
{
public:
	typedef _value_type value_type;
	typedef typename _container_type::size_type size_type;

public:
	void push_back (const _value_type& v)
	{
		_mutex.lock();
		_container_type::push_back (v);
		_mutex.unlock();
	}

public:
	void swap (_container_type& v)
	{
		_mutex.lock();
		_container_type::swap (v);
		_mutex.unlock();
	}

public:
	size_type size (void)
	{
		_mutex.lock();
		auto size = _container_type::size();
		_mutex.unlock();
		return size;
	}

public:
	void clear (void)
	{
		_mutex.lock();
		_container_type::clear();
		_mutex.unlock();
	}

private:
	std::mutex _mutex;
};

}; // namespace threading

}; // namespace DAS

/************************************************************************/

#include <mutex>
#include <deque>

namespace DAS
{

namespace threading
{

template <class _value_type,
		  class _container_type = std::deque<_value_type> >

class deque : protected _container_type
{
public:
	typedef _value_type value_type;
	typedef typename _container_type::size_type size_type;

public:
	void push_back (const _value_type& v)
	{
		_mutex.lock();
		_container_type::push_back (v);
		_mutex.unlock();
	}

public:
	void pop_front (void)
	{
		_mutex.lock();
		_container_type::pop_front();
		_mutex.unlock();
	}

public:
	_value_type front (void)
	{
		_mutex.lock();
		auto front = _container_type::front();
		_mutex.unlock();
		return front;
	}

public:
	bool empty (void)
	{
		_mutex.lock();
		auto empty = _container_type::empty();
		_mutex.unlock();
		return empty;
	}

private:
	std::mutex _mutex;
};

}; // namespace threading

}; // namespace DAS

/************************************************************************/

#include <future>
#include <memory>
#include <functional>
#include <exception>

// namespace threading
// {
//
// template <class _return_type  = void*,
// 		  class _task_type    = std::packaged_task<_return_type()>,
// 		  class _future_type  = std::future<_return_type>,
// 		  class _q_type       = threading::deque<_task_type*> >
//
// class taskq : public _q_type
// {
// public:
// 	typedef _future_type future_type;
//
// public:
// 	~taskq (void)
// 	{
// 		for (; !_q_type::empty(); _q_type::pop_front())
// 			std::unique_ptr<_task_type> discarded_task (_q_type::front());
// 	}
//
// public:
// 	template<class Task, class... Args>
// 	_future_type push_back (Task&& task, Args&&... args)
// 	{
// 		auto func = new _task_type (std::bind (
// 			std::forward<Task>(task), std::forward<Args>(args)...));
//
// 		auto future = func->get_future();
//
// 		_q_type::push_back (func);
// 		return future;
// 	}
//
// public:
// 	int process (void)
// 	{
// 		int activity = 0;
//
// 		for (; !_q_type::empty(); _q_type::pop_front(), ++activity) {
//
// 			std::unique_ptr<_task_type> func (_q_type::front());
// 			func->operator()();
// 		}
//
// 		return activity;
// 	}
// };

namespace DAS
{

namespace threading
{

template <class _return_type  = void*,
		  class _func_type    = std::function<_return_type()>,
		  class _promise_type = std::promise<_return_type>,
		  class _future_type  = std::future<_return_type>,
		  class _task_type    = std::tuple<_func_type*, _promise_type*>,
		  class _q_type       = threading::deque<_task_type> >

class taskq : public _q_type
{
public:
	typedef _future_type future_type;

public:
	~taskq (void)
	{
		while (!_q_type::empty()) {

			auto task = _q_type::front(); _q_type::pop_front();
			std::unique_ptr<_func_type> func (std::get<0> (task));
			std::unique_ptr<_promise_type> promise (std::get<1> (task));
		}
	}

public:
	template<class Task, class... Args>
	_future_type push_back (Task&& task, Args&&... args)
	{
		auto func = new _func_type (std::bind (
			std::forward<Task>(task), std::forward<Args>(args)...));

		auto promise = new _promise_type;
		auto future = promise->get_future();

		_q_type::push_back (std::make_tuple (func, promise));
		return future;
	}

public:
	int process (void)
	{
		int activity = 0;

		for (; !_q_type::empty(); ++activity) {

			auto task = _q_type::front(); _q_type::pop_front();
			std::unique_ptr<_func_type> func (std::get<0> (task));
			std::unique_ptr<_promise_type> promise (std::get<1> (task));

			try {

				auto result = func->operator()();
				promise->set_value (result);
			}

			catch (std::exception&) {

				auto p = std::current_exception();
				promise->set_exception (p);
				std::rethrow_exception (p);
			}
		}

		return activity;
	}
};

}; // namespace threading

}; // namespace DAS

/************************************************************************/

#endif

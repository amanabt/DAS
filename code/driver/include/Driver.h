#ifndef __DAS_DRIVER__
#define __DAS_DRIVER__

#include "Comm.h"
#include "Data.h"
#include "RuntimeError.h"
#include "../../sys/include/threading.h"

#include <vector>
#include <chrono>
#include <stdint.h>

namespace DAS {

//! \brief The Driver class
/*!
 * It consists of functions required by the software to connect to and
 * communicate with the firmware.
 */
class Driver
{
/*!
 * The typedef can be changed to change the way in which the DAC and ADC
 * transfers are stored by the Driver class.
 */
typedef std::vector<int16_t> container_type;
public:
	 Driver (void);
	~Driver (void);

public:
	static std::vector<FTDI_DeviceInfo> scan (void);

public:
	template <class T>
	std::unique_ptr<T> make_unique (T* po)
	{ return std::unique_ptr<T> (po); }

public:
	void connect	(const char* serialNo, float leaseTime = 10.0);
	void disconnect (void);

	/*!
	 * \brief Returns whether or not the connections is still active.
	 */
	bool alive		(void) const { return _alive; }

public:
	data::Identity*				  identify			 (void);
	data::KeepAlive*        	  keep_alive         (float lease_time);

private:
	data::Open* 				  open 				(const char*);
	data::Close* 				  close 			(void);

private:
	template <class Func, class... Args> Data*
	communicate (Data::Type check_bit, float timeout,
				 const char* errmsg, Func&& func, Args&&... args);

private:
	Data* _open										(const char* serialNo);
	Data* _close									(void);
	Data* _identify									(void);
	Data* _keep_alive								(float lease_time);

private:
	void open_										(const char* serialNo);
	void close_										(void);
	void identify_									(const char* identify);
	void keep_alive_								(void);

private:
	int thread (void);

private:
	Comm* make_comm_obj (void);

private:
	static constexpr double default_timeout = 0.5;
	static constexpr double default_extended_timeout = 5.0;

private:
	template<class T, class S>
	std::unique_ptr<T> unique_cast (S* po);

private:
	double _lease_time;
	std::unique_ptr<Comm> _comm;
	std::chrono::steady_clock::time_point _ping_at;
	std::array< std::unique_ptr<Data>, 64 > _datapool;

private:
	std::future<int> 						_exit_status;
	std::atomic<bool>						_alive;
	threading::taskq<Data*>					_taskq;

private:
	std::string _serialNo;
	static std::vector<std::string> _connected_devices;
};
}; //namespace DAS
#endif

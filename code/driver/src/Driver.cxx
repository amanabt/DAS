#include "../include/Driver.h"
#include "../../stl/bits/stl_inet.h"

#include <cstring>
#include <algorithm>

#include <iostream>

using namespace DAS;
/*!
 * \brief Stores a vector of connected devices.
 * It is static member variable of the Driver class.
 */
std::vector<std::string> Driver::_connected_devices;

/*!
 * \brief Scans for devices with matching description.
 * \param description The string against whitch description of attached devices
 * is tested.
 */
std::vector<FTDI_DeviceInfo> Driver::scan (void)
{
	constexpr char description[] = "DAS-PHY315";
	std::vector<FTDI_DeviceInfo> qdaDevices;
	std::vector<FTDI_DeviceInfo>::const_iterator it;
	std::vector<FTDI_DeviceInfo> allDevices = Comm::scan();

	for (it = allDevices.begin(); it != allDevices.end(); ++it)
	{
		if (strcmp (it->description(), description) == 0) {

			bool connected = false;
			for (const auto& dev : _connected_devices)
				if (strcmp (dev.c_str(), it->serialNo()) == 0) {
					connected = true;
					break;
				}

			if (!connected)
				qdaDevices.push_back (*it);
		}
	}

	return qdaDevices;
}

/********************************************************************/
/********************************************************************/
/*!
 * \brief The class constructor.
 * \par It sets the value of alive to false.
 */
Driver::Driver (void) :
	_lease_time (9.0),
	_alive (false)
{}

/******************************************************************************/
/*!
 * \brief The class destructor.
 * \par
 * It deletes the \ref Comm object and removes the disconnected device from
 * connected devices.
 */
Driver::~Driver (void)
{
	try {

		_alive = false;

		if (_exit_status.valid())
			_exit_status.get();
	}
	catch (RuntimeError&) {}

	/**** Reset keep alive timeout to zero ****/
	if (_comm) try {
		make_unique (_keep_alive (0));
	}
	catch (RuntimeError&) {}

	/**** Close the interface ****/
	if (_comm) try {
		make_unique (_close());
	}
	catch (RuntimeError&) {}

	auto it = std::find (_connected_devices.begin(),
						 _connected_devices.end(),
						 _serialNo);

	if (it != _connected_devices.end())
		_connected_devices.erase (it);

	_serialNo.clear();
}

/******************************************************************************/
/*!
 * \brief * Connects to the required device and launches a thread.
 * \param serialNo The serial no. of the device to connect to.
 * \param lease_time The lease time in seconds for the keep alive operation.
 * \par
 * Creates the necessary channel objects, sets keep alive lease time and
 * lauches the thread.
 */
void Driver::connect (const char* serialNo, float lease_time)
{

	/********************************************/

	class Watch {

	public:
		Watch (std::unique_ptr<Comm>& comm) :
			_over (false),
			_comm (comm)
		{}


/********************************************************************/
/********************************************************************/

	public:
		~Watch (void)
		{
			if (!_over)
				_comm.reset();
		}

	public:
		void over (void) { _over = true; }

	private:
		bool _over;
		std::unique_ptr<Comm>& _comm;
	};

	/********************************************/


	Watch watch (_comm);
	_comm.reset (make_comm_obj());

	/**** Open interface ****/
	make_unique (_open (serialNo));

	/**** Identify hardware ****/
	if (auto rxdata = unique_cast<data::Identity> (_identify()))
		if (!rxdata->goodID())
			throw RuntimeError ("goodID");

	/**** Set keep alive timeout ****/
	make_unique (_keep_alive (lease_time));


	_serialNo = serialNo;
	_connected_devices.push_back (serialNo);
	watch.over();

	_alive = true;
	_exit_status  = std::async (std::launch::async, &Driver::thread, this);
}

/******************************************************************************/
/******************************************************************************/
/*!
 * \brief Disconnects from the device.
 * \par
 *  Removes disconnected device from connected devices and resets \ref Comm
 *  object and serial no.
 */
void Driver::disconnect (void)
{
	try {
		_alive = false;
		if (_exit_status.valid())
			_exit_status.get();
	}
	catch (RuntimeError&) {

	}

	try {
		make_unique (_keep_alive (0));
	}
	catch (RuntimeError&) {}

	try {
		make_unique(_close());
	}
	catch (RuntimeError&) {}

	auto it = std::find (_connected_devices.begin(),
						 _connected_devices.end(),
						 _serialNo);

	if (it != _connected_devices.end())
		_connected_devices.erase (it);

	_serialNo.clear();
	_comm.reset();
}

/******************************************************************************/
/******************************************************************************/
/*!
 * \brief A blocking call which returns back the identity of connected device.
 * \return Returns back a pointer to a \ref data::Identity object.
 */
data::Identity* Driver::identify (void)
{
	if (!_alive) throw RuntimeError ("Resource dead");
	else return static_cast<data::Identity*> (_taskq.push_back (
											&Driver::_identify, this).get());
}

/******************************************************************************/
/*!
 * \brief A blocking call which performs a keep alive operation
 * \return Returns back a pointer to a \ref data::KeepAlive object.
 */
data::KeepAlive* Driver::keep_alive (float lease_time)
{
	if (!_alive) throw RuntimeError ("Resource dead");
	else return static_cast<data::KeepAlive*> (_taskq.push_back (
								&Driver::_keep_alive, this, lease_time).get());
}

/**********************************************************************/
/**********************************************************************/

data::Open* Driver::open (const char* serialNo) {

	if (!_alive) throw RuntimeError ("Resource dead");
	else return static_cast<data::Open*> (_taskq.push_back (
			&Driver::_open, this, serialNo).get());
}

/********************************************************************/

data::Close* Driver::close (void) {

	if (!_alive) throw RuntimeError ("Resource dead");
	else return static_cast<data::Close*> (_taskq.push_back (
			&Driver::_close, this).get());
}

/********************************************************************/
/********************************************************************/

template <class Func, class... Args> Data*
Driver::communicate (Data::Type check_bit, float timeout,
						 const char* errmsg, Func&& func, Args&&... args) {

	const int __check_bit = static_cast<int> (check_bit);

	std::function<void()> transmit = std::bind (
		std::forward<Func> (func), std::forward<Args> (args)...);

	_datapool[__check_bit].reset();

	auto timeout_at =
		std::chrono::steady_clock::now() +
			std::chrono::duration_cast<
				std::chrono::steady_clock::duration> (
					std::chrono::duration<float> (timeout));

	transmit();

	do {

		int activity = 0;
		activity += _comm->check();

		if (!activity)
			std::this_thread::sleep_for (
				std::chrono::milliseconds (1));

		if (std::chrono::steady_clock::now() > timeout_at)
			throw RuntimeError ((
				std::string ("RuntimeError: ") + errmsg).c_str());

	} while (!_datapool[__check_bit]);

	return _datapool[__check_bit].release();
}

/********************************************************************/
/********************************************************************/

Data* Driver::_open (const char* serialNo)
{
	return communicate (Data::Type::OPEN, default_timeout,
						"open", &Comm::open, _comm.get(), serialNo);
}

/******************************************************************************/

Data* Driver::_close (void)
{
	return communicate (Data::Type::CLOSE, default_timeout,
						"close", &Comm::close, _comm.get());
}

/******************************************************************************/

Data* Driver::_identify (void)
{
	return communicate (Data::Type::IDN, default_timeout,
						"identify", &Comm::identify, _comm.get());
}

/********************************************************************/

Data* Driver::_keep_alive (float lease_time)
{
	return communicate (Data::Type::KEEP_ALIVE, default_timeout,
						"keep_alive",
						&Comm::keep_alive, _comm.get(), lease_time);
}

/********************************************************************/
/********************************************************************/

void Driver::open_ (const char* serialNo) {

	auto rxdata = new data::Open (serialNo);
	_datapool[static_cast<int> (rxdata->type())].reset (rxdata);
}

/********************************************************************/

void Driver::close_ (void) {

	auto rxdata = new data::Close;
	_datapool[static_cast<int> (rxdata->type())].reset (rxdata);
}


/********************************************************************/

void Driver::identify_ (const char* identity)
{
	bool goodID = (std::string (identity) == "DAS-PHY315");
	auto rxdata = new data::Identity (identity, goodID);
	_datapool[static_cast<int> (rxdata->type())].reset (rxdata);
}

/********************************************************************/

void Driver::keep_alive_ (void)
{
	using namespace std::chrono;

	/**
	* Keep alive is sent at every 45% of lease time.
	*
	* This is so because if the command fails,
	* the second try might succeed at 90% of the leased time.
	*/

// 	_lease_time = lease_time;
	_ping_at = steady_clock::now() +
		milliseconds (static_cast<int> (1000 * 0.45 * _lease_time));

	auto rxdata = new data::KeepAlive (_lease_time);
	_datapool[static_cast<int> (rxdata->type())].reset (rxdata);
}

/********************************************************************/
/********************************************************************/

int Driver::thread (void)
{
	/********************************************/

	class Watch
	{
	public:
		Watch (std::atomic<bool>& alive) :
			_over  (false),
			_alive (alive)
		{}

	public:
		~Watch (void)
		{
			if (!_over)
				_alive = false;
		}

	public:
		void over (void) { _over = true; }

	private:
		bool _over;
		std::atomic<bool>& _alive;
	};

	/********************************************/

	Watch watch (_alive);
	using namespace std::chrono;

	while (_alive) {

		int activity = 0;
		activity += _taskq.process();
		activity += _comm->check();

		if (steady_clock::now() > _ping_at)
			std::unique_ptr<Data> rxdata
			(_keep_alive (_lease_time));

		if (!activity)
			std::this_thread::sleep_for (milliseconds (1));
	}

	return 0;
}

/********************************************************************/

Comm* Driver::make_comm_obj (void)
{
	auto comm = make_unique (new Comm);
	using namespace std::placeholders;

	/*******************************************************/

	comm->open_							 	= std::bind (
		&Driver::open_,					this, _1);

	comm->close_    				     	= std::bind (
		&Driver::close_,				this);

	comm->identify_             		 	= std::bind (
		&Driver::identify_,				this, _1);

	comm->keep_alive_               	 	= std::bind (
		&Driver::keep_alive_,			this);
	
	/*******************************************************/

	return comm.release();
}

/********************************************************************/
/********************************************************************/

template<class T, class S>
std::unique_ptr<T> Driver::unique_cast (S* po)
{
	return std::unique_ptr<T> (static_cast<T*> (po));
}

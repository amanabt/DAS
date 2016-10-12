#ifndef __DAS_DATA__
#define __DAS_DATA__

#include <deque>
#include <string>
#include <algorithm>
#include <stdint.h>

namespace DAS {

/******************************************************************************/

class Data {

public:
	enum class Type {

		OPEN,
		CLOSE,

		IDN,
		KEEP_ALIVE,
		SET,
		RESET,
		GAIN1,
		GAIN2,
		SET_INPUT,
		SET_OUTPUT
	};

public:
	virtual ~Data (void) {}

	Data (Type type) :
		_type (type)
	{}

public:
	Type type (void) const { return _type; }

private:
	Type _type;
};

/******************************************************************************/
/******************************************************************************/

namespace data {

class Open : public Data {

public:
	Open (const char *serialNo) :
		Data (Type::OPEN)
		{}
public:
	const char* serialNo (void) const { return _serialNo.c_str(); }

private:
	std::string _serialNo;
};

/******************************************************************************/

class Close : public Data {

public:
	Close (void) :
		Data (Type::CLOSE)
	{}
};

/******************************************************************************/

class Identity : public Data {

public:
	Identity (const char* identity, bool goodID) :
		Data		(Type::IDN),
		_identity   (identity),
		_goodID     (goodID)
	{}

public:
	bool 		goodID	 (void) const { return _goodID; }
	const char* identity (void) const { return _identity.c_str(); }

private:
	std::string _identity;
	bool 		_goodID;
};

/******************************************************************************/

class KeepAlive : public Data
{
public:
	KeepAlive (float leaseTime) :
		Data 	   (Type::KEEP_ALIVE),
		_leaseTime (leaseTime)
	{}

public:
	float leaseTime (void) const { return _leaseTime; }

private:
	float _leaseTime;
};

/******************************************************************************/

class Set: public Data
{
public:
	Set (uint8_t address, uint8_t status) :
		Data 	   (Type::SET),
		_address (address),
		_status (status)
	{}

public:
	uint8_t address (void) const { return _address; }
	uint8_t status  (void) const { return _status; }

private:
	uint8_t _address;
	uint8_t _status;
};

/******************************************************************************/

class Reset: public Data
{
public:
	Reset () :
	Data (Type::RESET)
	{}
};

/******************************************************************************/

class Gain1: public Data
{
public:
	Gain1 (uint8_t channel, uint8_t gain) :
		Data 	   (Type::GAIN1),
		_channel (channel),
		_gain (gain)
	{}

public:
	uint8_t channel (void) const { return _channel; }
	uint8_t gain (void) const    { return _gain; }

private:
	uint8_t _channel;
	uint8_t _gain;
};

/******************************************************************************/

class Gain2: public Data
{
public:
	Gain2 (uint8_t channel, uint8_t gain) :
		Data 	   (Type::GAIN2),
		_channel (channel),
		_gain (gain)
	{}

public:
	uint8_t channel (void) const { return _channel; }
	uint8_t gain (void) const    { return _gain; }

private:
	uint8_t _channel;
	uint8_t _gain;
};

/******************************************************************************/

class Set_input: public Data
{
public:
	Set_input (uint8_t channel) :
		Data 	   (Type::SET_INPUT),
		_channel (channel)
	{}

public:
	uint8_t channel (void) const { return _channel; }

private:
	uint8_t _channel;
};

/******************************************************************************/

class Set_output: public Data
{
public:
	Set_output (uint8_t channel) :
		Data 	   (Type::SET_OUTPUT),
		_channel (channel)
	{}

public:
	uint8_t channel (void) const { return _channel; }

private:
	uint8_t _channel;
};

/******************************************************************************/
/******************************************************************************/
};}; // namespace data, DAS

#endif


#ifndef __DAS_COMM__
#define __DAS_COMM__

#include "../../sys/include/QP4.h"
#include "../../sys/include/FTDI.h"
#include "../../stl/inet"
#include "../../stl/enum"

#include <vector>
#include <cstring>
#include <string>
#include <functional>

namespace DAS
{
/*****************************************************************************/

enum class Comm_Opcode
{
	NOP, IDN, KEEP_ALIVE,

	__END__
};

/******************************************************************************/
/******************************************************************************/

class CommPacket
{
protected:
	CommPacket (Comm_Opcode opcode) :
		opcode_ (DAS::hton (static_cast<uint16_t> (opcode))),
		_reserve_ (0)
	{}

public:
	Comm_Opcode opcode (void) const {
		return DAS::valid_enum<Comm_Opcode> (DAS::ntoh (opcode_));}

private:
	uint16_t opcode_;
	uint16_t _reserve_;
};

/*************************************************************************/
/*************************************************************************/

class CommPacket_Identity : public CommPacket
{
protected:
	CommPacket_Identity (void) :
		CommPacket (Comm_Opcode::IDN)
	{}
};

class CommRequest_Identity : public CommPacket_Identity
{
public:
	CommRequest_Identity (void)
	{}
};

class CommResponse_Identity : public CommPacket_Identity
{
public:
	const char* identity (void) const { return identity_; }

private:
	char identity_[32];

private:
	CommResponse_Identity (void);
};

/*************************************************************************/
/*************************************************************************/

class CommPacket_KeepAlive : public CommPacket
{
protected:
	CommPacket_KeepAlive (void) :
		CommPacket (Comm_Opcode::KEEP_ALIVE)
	{}
};

class CommRequest_KeepAlive : public CommPacket_KeepAlive
{
public:
	CommRequest_KeepAlive (void)
	{}
};

class CommResponse_KeepAlive : public CommPacket_KeepAlive
{
private:
	CommResponse_KeepAlive (void);
};

/**********************************************************************/
/**********************************************************************/

class Comm
{
typedef std::vector<int16_t> container_type;
public:
	Comm (void);
	virtual ~Comm (void);
	static std::vector<FTDI_DeviceInfo> scan (void);
	bool good (void) const { return _ftdi->good(); }

public:
	void open						(const char* serialNo);
	void close						(void);
	void identify					(void);
	void keep_alive					(float lease_time);

public:
	int check (void);

public:
	std::function<void(const char* identity)>			 identify_;
	std::function<void(const char* serialNo)> 			 open_;
	std::function<void()> 								 close_;
	std::function<void(void)> 				             keep_alive_;

private:
	int  read_interface       		(void);
	void process_rxdata       		(const void* data, uint16_t size);
	void interpret_rxdata     		(const void* data, uint16_t size);
	void _nop						(const void* data, uint16_t size);
	void _open						(const void* data, uint16_t size);
	void _close						(const void* data, uint16_t size);
	void _identify					(const void* data, uint16_t size);
	void _keep_alive				(const void* data, uint16_t size);

private:
	void transmit (const QP4_Packet* packet);

private:
	QP4*  _qp4;
	FTDI* _ftdi;
};

/******************************************************************************/
}; // namespace DAS


#endif

#ifndef __DAS_FTDI__
#define __DAS_FTDI__

#include <vector>
#include <string>
#include <stdint.h>

namespace DAS {

class FTDI_DeviceInfo
{
	public:
	FTDI_DeviceInfo (const char* serialNo, const char* description);

	public:
	const char* serialNo (void) const {return serialNo_.c_str();}
	const char* description (void) const {return description_.c_str();}

	private:
	std::string serialNo_;
	std::string description_;
};

}; // namespace DAS

#if defined(linux) || defined(__linux) || defined(__linux__)

#include <ftdi.h>

namespace DAS {

class FTDI
{
	public:
	FTDI (void);

	public:
	static std::vector<FTDI_DeviceInfo> scan (void);

	public:
	void open (const char* serialNo);
	void open (const char* serialNo, int baudrate);
	uint32_t read (void* data, uint32_t size);
	uint32_t write (const void* data, uint32_t size);
	void close (void);

	public:
	bool good (void) const;

	private:
	ftdi_context* handle_;
	static std::vector<FTDI_DeviceInfo> scan (int vid, int pid);
};

}; // namespace DAS

#endif

#if defined (_WIN32) || defined (_WIN64)

#include <Windows.h>
#include <ftd2xx.h>

namespace DAS {

class FTDI
{
	public:
	FTDI (void);

	public:
	std::vector<FTDI_DeviceInfo> scan (void);

	public:
	void open (const char* serialNo);
	void open (const char* serialNo, int baudrate);
	uint32_t read (void* data, uint32_t size);
	uint32_t write (const void* data, uint32_t size);
	void close (void);

	public:
	bool good (void) const;

	private:
	FT_HANDLE handle_;
	void configureTimeouts (void);
};

}; // namespace DAS

#endif // WIN32

#endif

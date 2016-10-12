#define DEBUG

#include "../include/FTDI.h"

#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

using namespace DAS;

#ifdef min
#undef min
#endif



/*************************************************************************/

FTDI_DeviceInfo::FTDI_DeviceInfo (const char* serialNo,
				  const char* description) :
    serialNo_ (serialNo),
    description_ (description)
{}

/*************************************************************************/

#if defined(linux) || defined(__linux) || defined(__linux__)

#define FTDI_OK          0
#define FTDI_VID         0x0403
#define FTDI_FT232_PID   0x6001
#define FTDI_FT2232_PID  0x6010
#define FTDI_FT4232_PID  0x6011

FTDI::FTDI (void) : handle_ (0)
{}

std::vector<FTDI_DeviceInfo> FTDI::scan (int vid, int pid)
{
	ftdi_context* handle = 0;
	ftdi_device_list* list = 0;
	std::vector<FTDI_DeviceInfo> devices;

	if ((handle = ftdi_new()) == 0)
		goto abort;

	if (ftdi_usb_find_all (handle, &list, vid, pid) < 0)
		goto abort;

	for (ftdi_device_list* it = list; it != 0; it = it->next) {

		char manufacturer[256], description[256], serialNo[256];

		if (ftdi_usb_get_strings (handle, it->dev,
			manufacturer, sizeof (manufacturer),
			description, sizeof (description),
			serialNo, sizeof (serialNo)) != FTDI_OK)
				continue;

		std::string _serialNo (serialNo);

		if (pid == FTDI_FT232_PID) {

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "A").c_str(), description));
		}

		else if (pid == FTDI_FT2232_PID) {

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "A").c_str(), description));

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "B").c_str(), description));
		}

		else if (pid == FTDI_FT4232_PID) {

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "A").c_str(), description));

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "B").c_str(), description));

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "C").c_str(), description));

			devices.push_back (FTDI_DeviceInfo (
				(_serialNo + "D").c_str(), description));
		}
	}

abort:
	if (list) ftdi_list_free (&list);
	if (handle) ftdi_free (handle);
	return devices;
}

std::vector<FTDI_DeviceInfo> FTDI::scan (void)
{
	std::vector<FTDI_DeviceInfo> devs, devices;

	devs = FTDI::scan (FTDI_VID, FTDI_FT232_PID);
	devices.insert (devices.end(), devs.begin(), devs.end());

	devs = FTDI::scan (FTDI_VID, FTDI_FT2232_PID);
	devices.insert (devices.end(), devs.begin(), devs.end());

	devs = FTDI::scan (FTDI_VID, FTDI_FT4232_PID);
	devices.insert (devices.end(), devs.begin(), devs.end());

	return devices;
}

void FTDI::open (const char* serialNumber)
{
	/**** Extracts serial number and interface ****/
	char serialNo[256];
	strcpy (serialNo, serialNumber);
	size_t len = strlen (serialNo);

	if (len == 0)
		return;

	char interface_name = serialNo[len - 1];
	serialNo[len - 1] = '\0';

	if ((interface_name < 'A') || (interface_name > 'D'))
		return;

	/**** Extracts interface ****/
	static const ftdi_interface interfaces[] =
		{ INTERFACE_A, INTERFACE_B, INTERFACE_C, INTERFACE_D };

	const ftdi_interface& interface = interfaces[interface_name - 'A'];

	if (good() || ((handle_ = ftdi_new()) == 0))
		return;

	/**** Selects an interface ****/
	if (ftdi_set_interface (handle_, interface) != FTDI_OK) {

		close();
		return;
	}

	/**** Opens the device ****/
	if (ftdi_usb_open_desc (handle_,
	FTDI_VID, FTDI_FT232_PID, 0, serialNo) != FTDI_OK)

		if (ftdi_usb_open_desc (handle_,
		FTDI_VID, FTDI_FT2232_PID, 0, serialNo) != FTDI_OK)

			if (ftdi_usb_open_desc (handle_,
			FTDI_VID, FTDI_FT4232_PID, 0, serialNo) != FTDI_OK) {

				close();
				return;
			}

	/**** Purge everything in the Rx and Tx buffers ****/
	if (ftdi_usb_purge_buffers (handle_) != FTDI_OK) {

		close();
		return;
	}
}

void FTDI::open (const char* serialNo, int baudrate)
{
	if (good()) return;

	open (serialNo);
	if (!good()) return;

	if (ftdi_set_baudrate (handle_, baudrate) != FTDI_OK) {

		close();
		return;
	}

	if (ftdi_set_line_property2 (handle_, BITS_8,
		STOP_BIT_1, NONE, BREAK_OFF) != FTDI_OK) {

		close();
		return;
	}
}

uint32_t FTDI::read (void* data, uint32_t size)
{
	int rxsize;
	if (!good()) return 0;

	if ((rxsize = ftdi_read_data (handle_,
		reinterpret_cast<unsigned char*> (data),
		size)) < FTDI_OK) {

			close();
			return 0;
	}

#ifdef DEBUG
	const unsigned char* cdata = static_cast<const unsigned char*> (data);
	for (int idx = 0; idx < rxsize; ++idx)
		std::cout << ">>>> "
				  << " : " << std::hex << (int) cdata[idx]
				  << " : " << cdata[idx] << std::endl;
#endif
	return rxsize;
}

uint32_t FTDI::write (const void* data, uint32_t size)
{
	int txsize;
	if (!good()) return 0;

	if ((txsize = ftdi_write_data (handle_, const_cast<unsigned char*> (
		reinterpret_cast<const unsigned char*> (data)), size)) < FTDI_OK) {

			close();
			return 0;
	}

#ifdef DEBUG
	const unsigned char* cdata = static_cast<const unsigned char*> (data);
	for (int idx = 0; idx < txsize; ++idx)
		std::cout << "<<<< "
				  << " : " << std::hex << (int) cdata[idx]
				  << " : " << cdata[idx] << std::endl;
#endif
	return txsize;
}

void FTDI::close (void)
{
	if (good()) {

		ftdi_usb_close (handle_);
		ftdi_free (handle_);
		handle_ = 0;
	}
}

bool FTDI::good (void) const
{
	return handle_;
}

#endif // linux

/*************************************************************************/

#if defined(_WIN32) || defined(_WIN64)

FTDI::FTDI (void) : handle_ ((FT_HANDLE)INVALID_HANDLE_VALUE)
{}

std::vector<FTDI_DeviceInfo> FTDI::scan (void)
{
	DWORD totalDevices = 0;
	std::vector<FTDI_DeviceInfo> deviceList;
	FT_DEVICE_LIST_INFO_NODE* nodes = 0;

	if (FT_CreateDeviceInfoList (&totalDevices) != FT_OK)
		goto abort;

	nodes = reinterpret_cast<FT_DEVICE_LIST_INFO_NODE* > (
	    malloc (totalDevices * sizeof (FT_DEVICE_LIST_INFO_NODE)));

	if (!nodes)
		goto abort;

	if (FT_GetDeviceInfoList (nodes, &totalDevices) != FT_OK)
		goto abort;

	for (DWORD i = 0; i < totalDevices; ++i) {

		const FT_DEVICE_LIST_INFO_NODE &node = nodes[i];

		if (node.Flags & FT_FLAGS_OPENED)
			continue;

		deviceList.push_back (FTDI_DeviceInfo (node.SerialNumber,
						       node.Description));
	}

abort:
	free (nodes);
	return deviceList;
}

void FTDI::open (const char* serialNo)
{
	if (good()) return;

	handle_ = FT_W32_CreateFile (serialNo,
	    GENERIC_READ | GENERIC_WRITE,
	    0,             // must be opened with exclusive-access
	    0,             // no security attributes
	    OPEN_EXISTING, // must use OPEN_EXISTING
	    FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_SERIAL_NUMBER,
	    0);            // hTemplate must be NULL for comm devices

	if (!good()) return;

	/**** Purge buffers and setup USB latency timer ****/
	if (FT_Purge (handle_, FT_PURGE_RX | FT_PURGE_TX) != FT_OK)
		goto abort;

	if (FT_SetUSBParameters (handle_, 64 * 1024, 64 * 1024) != FT_OK)
		goto abort;

	configureTimeouts();
	return;

abort:
	close();
}

void FTDI::open (const char* serialNo, int baudrate)
{
	if (good()) return;

	open (serialNo);
	if (!good()) return;

	FTDCB dcb;

	if (!FT_W32_GetCommState (handle_, &dcb))
		goto abort;

	dcb.BaudRate = baudrate;
	dcb.ByteSize = 8;
	dcb.Parity   = 0; // NOPARITY
	dcb.StopBits = 0; // ONESTOPBIT;

	if (!FT_W32_SetCommState (handle_, &dcb))
		goto abort;

	return;

abort:
	close();
}

uint32_t FTDI::read (void* data, uint32_t size)
{
	DWORD actualReadSize = 0;
	DWORD requestedReadSize = 0;
	DWORD availableDataSize = 0;

	if (!good()) return 0;

	if (FT_GetQueueStatus (handle_, &availableDataSize) != FT_OK)
		goto abort;

	if (availableDataSize == 0)
		return 0;

	requestedReadSize = std::min ((uint32_t)availableDataSize, size);

	if (!FT_W32_ReadFile (handle_,
		reinterpret_cast<char*> (data),
		requestedReadSize, &actualReadSize, 0))
			goto abort;

	return actualReadSize;

abort:
	close();
	return 0;
}

uint32_t FTDI::write (const void* data, uint32_t size)
{
	if (!good()) return 0;

	DWORD actualWriteSize = 0;

	if (!FT_W32_WriteFile (handle_,
		const_cast<char*> (reinterpret_cast<const char*> (data)),
		size, &actualWriteSize, 0))
			goto abort;

	return actualWriteSize;

abort:
	close();
	return 0;
}

void FTDI::close (void)
{
	if (good()) {
		FT_W32_CloseHandle (handle_);
		handle_ = (FT_HANDLE)INVALID_HANDLE_VALUE;
	}
}

bool FTDI::good (void) const
{
	return (handle_ != (FT_HANDLE)INVALID_HANDLE_VALUE);
}

void FTDI::configureTimeouts (void)
{
	FTTIMEOUTS ftTS;
	ftTS.ReadIntervalTimeout = 0;
	ftTS.ReadTotalTimeoutMultiplier = 0;
	ftTS.ReadTotalTimeoutConstant = 0;
	ftTS.WriteTotalTimeoutMultiplier = 0;
	ftTS.WriteTotalTimeoutConstant = 0;

	if (!FT_W32_SetCommTimeouts (handle_, &ftTS))
		close();
}

#endif // WIN32

/*************************************************************************/

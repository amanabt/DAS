#include "../code/driver/include/Driver.h"

#include <iostream>
#include <bitset>
#include <unistd.h>
#include <cstdint>

int main (void)
{
	try
	{
		std::unique_ptr<DAS::Driver> driver (new DAS::Driver);
		auto devices = driver->scan();
		if (devices.size () == 0)
			std::cout << "No device found!" << std::endl;

		else
		{
			const char* serialNo = devices[0].serialNo();

			std::cout << "Serial no " << serialNo << std::endl;
			driver->connect (serialNo);
			
			driver->disconnect ();
		}

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}

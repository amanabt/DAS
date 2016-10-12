#include "../code/driver/include/Driver.h"

#include <iostream>
#include <bitset>
#include <unistd.h>
#include <cstdint>

int main (void)
{
	try
	{
		std::unique_ptr<QAFE::Driver> driver (new QAFE::Driver);
		auto devices = driver->scan();
		if (devices.size () == 0)
			std::cout << "No device found!" << std::endl;

		else
		{
			std::cerr << "Hello" << std::endl;
			const char* serialNo = devices[0].serialNo();

			std::cout << "Serial no " << serialNo << std::endl;
			driver->connect (serialNo);
			
// 			std::unique_ptr <QAFE::Data> reset;
// 			reset.reset (driver->reset());
// 			std::cerr << "Set_Input" << std::endl;
			uint32_t g;
			std::cin >> g;
			std::cerr << g << std::endl;
			std::unique_ptr <QAFE::Data> gain;
			gain.reset (driver->gain2(0, static_cast<uint8_t>(g)));
			std::cerr << "*********Voltage : " << driver->toInputVoltage (0, 1.0);
// 			std::unique_ptr <QAFE::Data> set_input;
// 			set_input.reset (driver->set_output (1));

// 			std::uint8_t status = 1;
// 			std::uint32_t addr = 16;
// 			while (true) {
// // 				long unsigned int i = 0;
// 				std::cerr << "Enter Address : " << addr << std::endl;
// 				std::cin >> addr;
// 				std::cerr << "Enter Status : " << addr << std::endl;
// 				std::cin >> status;
// 				for (uint8_t addr = 0; addr < 32; ++addr) {
// 					addr = static_cast <std::uint8_t> (addr);
// 					std::unique_ptr <QAFE::Data> set;
// 					for (uint8_t addrs = 0; addrs < 32; ++addrs)
// 					set.reset (driver->set (static_cast <std::uint8_t> (addr), status));
// 				}
// 				++i;
// 				sleep (1);
// 			}
			driver->disconnect ();
		}

// 		else
// 			std::cout << "Multiple devices found! "
// 					<< "Please connect only one." << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}

#include <iostream>
#include <string>
#include <egis0570.hpp>

int main(int argc, char **argv) {
	egis::Egis egis;
	egis.initUsb();

	std::cout << std::to_string(egis.readRegister(0x20)) << std::endl;
	std::cout << std::to_string(egis.readRegister(0x58)) << std::endl;
	std::cout << std::to_string(egis.readRegister(0x21)) << std::endl;
	std::cout << std::to_string(egis.readRegister(0x57)) << std::endl;
	std::cout << std::to_string(egis.readRegister(0x22)) << std::endl;
	std::cout << std::to_string(egis.readRegister(0x56)) << std::endl;
	std::cout << std::to_string(egis.readRegister(0x23)) << std::endl;
}


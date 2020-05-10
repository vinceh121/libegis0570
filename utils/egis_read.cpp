#include <iostream>
#include <cstring>
#include <fstream>
#include <egis0570.hpp>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "egis_read <output>" << std::endl;
		return 0;
	}

	std::basic_ostream<char> *out = nullptr;
	if (std::strcmp(argv[1], "-") == 0) {
		out = &std::cout;
	} else {
		std::ofstream of(argv[1], std::ios::binary);
		out = &of;
	}

	egis::Egis egis;
	egis.initUsb();
	egis.readFingerprint();
	unsigned char *data = egis.repeatFingerprint();
	out->write((char*) data, EGIS_IMGSIZE);
	out->flush();

	egis.terminate();
}

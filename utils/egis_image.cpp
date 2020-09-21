#include <libpng/png.h>
#include <iostream>
#include <fstream>
#include <egis0570.hpp>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "egis_read <output>" << std::endl;
		return 0;
	}

	std::ofstream out(argv[1], std::ios::binary);

	egis::Egis egis;
	egis.initUsb();

	unsigned char *data[EGIS_IMGSIZE];
	egis.requestFlyEstimation(*data);

	egis.terminate();
}

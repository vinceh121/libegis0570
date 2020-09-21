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
	//egis.timeout = 0;
	egis.initUsb();
	egis.setDefaultsForReading();

	unsigned char *data[EGIS_IMGSIZE];
	egis.requestFlyEstimation(*data);
	out.write((char*) data, EGIS_IMGSIZE);
	// out.flush();

	out.close();

	egis.terminate();

	return 0;
}

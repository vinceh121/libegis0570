#include <egis0570.hpp>

int main(int argc, char **argv) {
	egis::Egis egis;
	egis.initUsb();
	egis.terminate();
}

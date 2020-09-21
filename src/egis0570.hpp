#ifndef EGIS0570_HPP_
#define EGIS0570_HPP_
#include <libusb-1.0/libusb.h> // TODO proper linking

#define EGIS_VENDOR_ID 0x1c7a
#define EGIS_PRODUCT_ID 0x0570
#define EGIS_EPOUT 0x04
#define EGIS_EPIN 0x83
#define EGIS_INTF 0x0
#define EGIS_CONF 0x1
#define EGIS_IMGSIZE 32512
#define EGIS_IMG_WIDTH 114
#define EGIS_IMG_HEIGHT 285

namespace egis {

	class Egis {
	protected:
		int timeout = 5000;
		libusb_device_handle *handle;
		// libusb_context *ctx;
	public:
		Egis();
		~Egis();
		void initUsb();
		int sendPacket(unsigned char *buf, unsigned char *cmd, int length,
				int returnLength);
		int sendMessage(unsigned char *buf, unsigned char cmd,
				unsigned char arg0, unsigned char arg1, int returnLength);
//		void sendSequence(unsigned char *data, unsigned char *seq,
//				int seqLength);
		unsigned char readRegister(unsigned char reg);
		unsigned char writeRegister(unsigned char reg, unsigned char val);
		int requestFlyEstimation(unsigned char *data);
		void setDefaultsForReading();

		unsigned char setSmallGain(int gain);
		unsigned char getSmallGain();
		unsigned char setNormalGain(int gain);
		unsigned char getNormalGain();
		unsigned char setLargeGain(int gain);
		unsigned char getLargeGain();

		void terminate();
	};
}

#endif /* EGIS0570_HPP_ */

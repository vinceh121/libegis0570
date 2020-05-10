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
	enum PacketType {
		INDEV29, VINCEH121
	};

	class Egis {
	protected:
		unsigned char *pktInit;
		unsigned char *pktRepeat;
		int pktInitLen;
		int pktRepeatLen;
		int timeout = 5000;
		libusb_device_handle *handle;
		libusb_context *ctx;
	public:
		Egis();
		~Egis();
		void initUsb();
		void setPackets(PacketType type);
		unsigned char* sendSequence(unsigned char *seq, int seqLength);
		unsigned char* readFingerprint();
		unsigned char* repeatFingerprint();
		void terminate();
	};
}

#endif /* EGIS0570_HPP_ */

#include <libusb-1.0/libusb.h> // TODO proper linking
#include <stdexcept>
#include "egis0570.hpp"
#include "packets.hpp"

namespace egis {
	Egis::Egis() {
		setPackets(PacketType::VINCEH121);
	}

	Egis::~Egis() {

	}

	void Egis::initUsb() {
		int status = libusb_init(NULL);
		if (status != 0) {
			throw std::runtime_error("failed to init libusb");
		}

		this->handle = libusb_open_device_with_vid_pid(nullptr, EGIS_VENDOR_ID,
		EGIS_PRODUCT_ID);

		if (libusb_kernel_driver_active(handle, EGIS_INTF)) {
			libusb_detach_kernel_driver(handle, EGIS_INTF);
		}

		status = libusb_set_configuration(handle, EGIS_CONF);
		if (status != 0) {
			throw std::runtime_error("failed to set config");
		}

		status = libusb_claim_interface(handle, EGIS_INTF);
		if (status != 0) {
			throw std::runtime_error("failed to claim interface");
		}

		status = libusb_reset_device(handle);
		if (status != 0) {
			throw std::runtime_error("failed to reset device");
		}
	}

	void Egis::setPackets(PacketType type) {
		switch (type) {
		case PacketType::VINCEH121:
			this->pktInit = *init_pkts_vinceh;
			this->pktRepeat = *repeat_pkts_vinceh;
			this->pktInitLen = 24; // TODO make dynamic, maybe packet set struct
			this->pktRepeatLen = 4;
			break;
		case PacketType::INDEV29:
			this->pktInit = *init_pkts_indev;
			this->pktRepeat = *repeat_pkts_indev;
			this->pktInitLen = sizeof(init_pkts_indev);
			this->pktRepeatLen = sizeof(repeat_pkts_indev);
			break;
		default:
			throw std::runtime_error("invalid packet type");
		}
	}

	unsigned char* Egis::sendSequence(unsigned char *seq, int seqLength) {
		static unsigned char data[EGIS_IMGSIZE];
		int transfered;
		for (int i = 0; i < seqLength; i++) {
			int addr = i * 7;
			int status = libusb_bulk_transfer(handle, EGIS_EPOUT, &seq[addr],
			EGIS_PKTSIZE, &transfered, timeout);

			if (status != 0) {
				throw std::runtime_error("failed sending");
			}
			bool lastPkt = i == seqLength - 1;
			status = libusb_bulk_transfer(handle, EGIS_EPIN, data,
					lastPkt ? EGIS_IMGSIZE : EGIS_PKTSIZE, &transfered,
					timeout);
			if (status != 0) {
				throw std::runtime_error("failed receive");
			}
			if (lastPkt && transfered < EGIS_IMGSIZE) {
				throw std::runtime_error("device sent less data");
			}
		}
		return data;
	}

	unsigned char* Egis::readFingerprint() {
		return sendSequence(pktInit, pktInitLen);
	}

	unsigned char* Egis::repeatFingerprint() {
		return sendSequence(pktRepeat, pktRepeatLen);
	}

	void Egis::terminate() {
		libusb_release_interface(handle, EGIS_INTF);
//		libusb_attach_kernel_driver(handle, EGIS_INTF);
		libusb_close(handle);
		libusb_exit(NULL);
	}
}

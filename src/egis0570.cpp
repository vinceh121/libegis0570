#include <libusb-1.0/libusb.h> // TODO proper linking
#include <stdexcept>
#include "egis0570.hpp"
#include "packets.hpp"

namespace egis {
	Egis::Egis() {
	}

	Egis::~Egis() {
	}

	void Egis::initUsb() {
		int status = libusb_init(NULL);
		if (status != 0) {
			throw std::runtime_error("failed to init libusb");
		}

		this->handle = libusb_open_device_with_vid_pid(NULL, EGIS_VENDOR_ID,
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

//		status = libusb_reset_device(handle);
		if (status != 0) {
			throw std::runtime_error("failed to reset device");
		}
	}

	int Egis::sendPacket(unsigned char *buf, unsigned char *msg, int msgLength,
			int returnLength) {
		int transfered;
		int status = libusb_bulk_transfer(handle, EGIS_EPOUT, msg, msgLength,
				&transfered, timeout);
		if (status != 0) {
			return status;
		}

		status = libusb_bulk_transfer(handle, EGIS_EPIN, buf, returnLength,
				&transfered, timeout);
		if (status != 0) {
			return status;
		}

		return 0;
	}

	int Egis::sendMessage(unsigned char *buf, unsigned char cmd,
			unsigned char arg1, unsigned char arg2, int returnLength) {
		unsigned char *msg[7];
		buildHeader(*msg);
		buildPacket(*msg, cmd, arg1, arg2);
		return sendPacket(buf, *msg, 7, returnLength);
	}

	/*void Egis::sendSequence(unsigned char *data, unsigned char *seq,
	 int seqLength) {
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
	 }*/

	unsigned char Egis::readRegister(unsigned char reg) {
		unsigned char *buf[7];
		sendMessage(*buf, CMD_READ_REG, reg, 0, 7);
		return *buf[5];
	}

	unsigned char Egis::writeRegister(unsigned char reg, unsigned char val) {
		unsigned char *buf[7];
		sendMessage(*buf, CMD_WRITE_REG, reg, val, 7);
		return *buf[5];
	}

	int Egis::requestFlyEstimation(unsigned char *data) {
		writeRegister(0x02, 0x0f);
		writeRegister(0x02, 0x2F);
		return sendMessage(data, CMD_FLY_ESTIMATION, 0x0, 0xfe, EGIS_IMGSIZE);
	}

	void Egis::setDefaultsForReading() {
		writeRegister(0x11, 0x38);
		writeRegister(0x12, 0x0); // width
		writeRegister(0x13, 0x71); // width
		writeRegister(0x20, 0x44);
		writeRegister(0x58, 0x44);
		writeRegister(0x21, 0x9);
		writeRegister(0x57, 0x9);
		writeRegister(0x22, 0x2);
		writeRegister(0x56, 0x2);
		writeRegister(0x23, 0x1);
		writeRegister(0x55, 0x1);
		writeRegister(0x24, 0x1);
		writeRegister(0x54, 0x1);
		writeRegister(0x25, 0x0);
		writeRegister(0x53, 0x0);
		writeRegister(0x15, 0x0);
		writeRegister(0x16, 0x51);
		writeRegister(0x9, 0xa);
		writeRegister(0x14, 0);

		// writeRegister(0x2, 0xf); ////// maybe finger detection
		writeRegister(0x3, 0x80);
		// readRegister(0x2);
		// writeRegister(0x2, 0x2f);
		setSmallGain(0x51);
		setNormalGain(0xa);
		setLargeGain(0);
	}

	unsigned char Egis::setSmallGain(int gain) {
		return writeRegister(0x16, gain);
	}

	unsigned char Egis::getSmallGain() {
		return readRegister(0x16);
	}

	unsigned char Egis::setNormalGain(int gain) {
		return writeRegister(0x9, gain);
	}

	unsigned char Egis::getNormalGain() {
		return readRegister(0x9);
	}

	unsigned char Egis::setLargeGain(int gain) {
		return writeRegister(0x14, gain);
	}

	unsigned char Egis::getLargeGain() {
		return readRegister(0x14);
	}

	void Egis::terminate() {
		libusb_release_interface(handle, EGIS_INTF);
		libusb_attach_kernel_driver(handle, EGIS_INTF);
		libusb_close(handle);
		libusb_exit(NULL);
	}
}

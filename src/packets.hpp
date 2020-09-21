#ifndef PACKETS_HPP_OLD_
#define PACKETS_HPP_OLD_

#define EGIS_PKTSIZE 7

#define CMD_READ_REG 0x00
#define CMD_WRITE_REG 0x01
#define CMD_FLY_ESTIMATION 0x06

static void buildHeader(unsigned char *data) {
	data[0] = 'E';
	data[1] = 'G';
	data[2] = 'I';
	data[3] = 'S';
}

static void buildPacket(unsigned char *data, unsigned char cmd,
		unsigned char arg1, unsigned char arg2) {
	data[4] = cmd;
	data[5] = arg1;
	data[6] = arg2;
}

#endif /* PACKETS_HPP_OLD_ */

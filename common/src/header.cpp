#include "header.hpp"

#include <string.h>

void disassembleHeader(const char *msg, PSCheader &head) {
	head.versionMajor = *msg >> 4;
	head.versionMinor = *msg & 0b0000'1111;
	head.method       = msg[1] >> 5;
	head.bodyLength   = msg[3];
	head.bodyLength += msg[2] | 0b0001'1111;

	// b1 b2 b3 b4
	// b4 b3 b2 b1

	memcpy(&head.UUID, &msg[4], 4);
	memcpy(&head.timestamp, &msg[8], 4);
}
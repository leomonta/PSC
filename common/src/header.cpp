#include "header.hpp"

#include "constants.hpp"

#include <arpa/inet.h>
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

void assembleHeader(char *msg, const PSCheader &head) {
	// 24 bits long
	uint32_t method_variant = htonl(head.method);
	uint32_t length         = htonl(head.bodyLength);

	uint32_t met_var_len = method_variant << 21 | head.bodyLength & 0b0000'0000'0001'1111'1111'1111'1111'1111;

	// uint32_t first_line = htonl(met_var_len);
	memcpy(&msg[0], &met_var_len, 4);

	// 8 bits long
	uint8_t version = head.versionMajor << 4 | head.versionMinor & 0b00001111;

	msg[0] = version;
}
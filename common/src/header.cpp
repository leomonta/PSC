#include "header.hpp"

#include "constants.hpp"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

void disassembleHeader(const uint8_t *msg, PSCheader &head) {

	// split version into 2
	head.versionMajor = msg[0] >> 4;          // move the first 4 bits to the end
	head.versionMinor = msg[0] & 0b0000'1111; // remove the first four bits

	head.method = msg[1] >> 5; // we are interested only in the first 3 bits

	// we can just copy the last byte in the 32bit line in the bodyLenght, that is always correct
	head.bodyLength = msg[3];
	// then i can add the ramaining bits
	head.bodyLength += (msg[2] & 0b0001'1111) << 8;

	// copy the 32 bits and reverse endianness
	memcpy(&head.UUID, &msg[4], 4);
	head.UUID = htonl(head.UUID);

	memcpy(&head.timestamp, &msg[8], 4);
	head.timestamp = htonl(head.timestamp);
}

void assembleHeader(uint8_t *msg, const PSCheader &head) {
	// 24 bits long

	// technically htonl is not needed since tha values are store in registers (on my machine) that are already big endian
	// but i keep them cus i think its the correct thing to do
	uint32_t method_variant = head.method;
	uint32_t length         = head.bodyLength;

	uint32_t met_var_len = method_variant << 21 | head.bodyLength & 0b0000'0000'0001'1111'1111'1111'1111'1111;

	uint32_t first_line = htonl(met_var_len);
	memcpy(&msg[0], &first_line, 4);

	// 8 bits long
	uint8_t version = head.versionMajor << 4 | head.versionMinor & 0b00001111;

	msg[0] = version;

	auto UUID = htonl(head.UUID);
	memcpy(&msg[4], &UUID, 4);

	auto timestamp = htonl(head.timestamp);
	memcpy(&msg[8], &timestamp, 4);
}

/*
memory printer

    for (int i = 0; i < TOT_HEADER_LEN; ++i) {
        for (int j = 7; j >= 0; --j) {
            unsigned char bit = (msg[i] >> j) & 1;
            printf("%u", bit);
        }
        printf(" ");
    }

    printf("\n");
*/
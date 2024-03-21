#include "header.h"

#include "constants.h"

#include <arpa/inet.h>
#include <string.h>

void disassembleHeader(const uint8_t *msg, PSCheader *head) {

	// split version into 2
	head->versionMajor = msg[0] >> 4;          // move the first 4 bits to the end
	head->versionMinor = msg[0] & 0b0000'1111; // remove the first four bits

	head->method = msg[1] >> 5; // we are interested only in the first 3 bits

	// the bodyLenght is a 21 bit number, this means that we need the fourth, the third, and part of the second byte of the first 4 bytes

	// we can just copy the last byte in the bodyLenght, that is always correct
	// this i also the least significant byte since the message is in Big Endian
	head->bodyLength = msg[3];
	// same for the second-to-last byte but shifted by 8 bits
	head->bodyLength += msg[2] << 8;
	// then i can add the ramaining 5 bits, shifted by 2 bytes
	head->bodyLength += (msg[1] & 0b0001'1111) << 16;

	// for timestamp and UUID the process is easier, just copy and reverse endianness
	memcpy(&head->UUID, &msg[4], 4);
	head->UUID = htonl(head->UUID);

	memcpy(&head->timestamp, &msg[8], 4);
	head->timestamp = htonl(head->timestamp);
}

void assembleHeader(uint8_t *msg, const PSCheader *head) {
	// I use 32bit to not worry about integer promotion
	uint32_t method_variant = head->method;

	// shift the version 21 bits to the left, outside the lenght space, and limit lenght to 21 bits
	uint32_t met_var_len = method_variant << 21 | (head->bodyLength & 0b0000'0000'0001'1111'1111'1111'1111'1111);

	// copy the first 32 bits into the string
	uint32_t first_line = htonl(met_var_len);
	memcpy(&msg[0], &first_line, 4);

	// compact the two versions in one byte
	uint8_t version = head->versionMajor << 4 | (head->versionMinor & 0b00001111);

	// overwrite the first byte with the version
	msg[0] = version;

	// reorder the endianness of the other two values and copy them in the msg string
	auto UUID = htonl(head->UUID);
	memcpy(&msg[4], &UUID, 4);

	auto timestamp = htonl(head->timestamp);
	memcpy(&msg[8], &timestamp, 4);
}

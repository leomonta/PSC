#include "constants.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "tcpConn.hpp"
#include "utils.hpp"

#include <iostream>

int main() {

	auto csck = tcpConn::initializeClient(DEFAULT_PORT, "127.0.0.1");

	srand(time(0));

	PSCheader head;
	head.versionMajor = rand() % 16;
	head.versionMinor = rand() % 16;
	head.bodyLength   = rand() % MAX_MESSAGE_LEN;
	head.timestamp    = time(0);
	head.UUID         = rand();
	head.method       = rand() % 3 << 1 | rand() % 2;

	log(LOG_DEBUG, "Sent this header\n");

	printHeaderStruct(&head);

	uint8_t *msg = (uint8_t *)(malloc(TOT_HEADER_LEN + 1));

	assembleHeader(msg, head);
 
	printHeaderStr((char *)(msg));

	msg[TOT_HEADER_LEN] = '\0';

	tcpConn::sendSegmentC(csck, (char *)(msg));
}
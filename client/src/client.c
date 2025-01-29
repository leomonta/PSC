#include "constants.h"
#include "header.h"
#include "logger.h"
#include "tcpConn.h"
#include "utils.h"

#include <stdlib.h>
#include <time.h>

int main() {

	auto csck = TCPinitializeClient(DEFAULT_PORT, "127.0.0.1", 4);

	srand(time(0));

	PSCheader head;
	head.versionMajor = (uint8_t)rand() % 16;
	head.versionMinor = (uint8_t)rand() % 16;
	head.bodyLength   = (uint32_t)rand() % MAX_MESSAGE_LEN;
	head.timestamp    = (uint32_t)time(0);
	head.UUID         = (uint32_t)rand();
	head.method       = (uint8_t)(rand() % 3 << 1 | rand() % 2);

	llog(LOG_DEBUG, "Sent this header\n");

	printHeaderStruct(&head);

	uint8_t *msg = (uint8_t *)(malloc(TOT_HEADER_LEN + 1));

	assembleHeader(msg, &head);

	printHeaderStr((char *)(msg));

	msg[TOT_HEADER_LEN] = '\0';

	TCPsendSegment(csck, (char *)(msg));

	free(msg);

	TCPterminate(csck);

}

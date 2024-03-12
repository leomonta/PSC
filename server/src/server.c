#include "server.h"

#include "constants.h"
#include "header.h"
#include "logger.h"
#include "tcpConn.h"
#include "user.h"
#include "utils.h"

#include <string.h>

int main() {

	userFull tmp;

	genUUID(&tmp.UUID);
	memset(tmp.uname, 0, MAX_UNAME_LEN);

	auto uname = "Try changing something\0";
	memcpy(tmp.uname, uname, strlen(uname));

	saveUser(&tmp);

	auto ssck = TCPinitializeServer(DEFAULT_PORT, 4);

	auto stopAllThreads = false;

	acceptClient(ssck, &stopAllThreads);
	return 0;
}

void acceptClient(Socket serverSocket, bool *threadStop) {

	while (!*threadStop) {

		auto client = TCPacceptClientSock(serverSocket);

		if (client == -1) {
			continue;
		}

		//std::thread(resolveClient, client, threadStop).detach();
		resolveClient(client, threadStop);
		log(LOG_INFO, "Launched thread for resolving the client %d request.\n", client);
	}
}

void resolveClient(Socket clientSocket, bool *threadStop) {

	while (!*threadStop) {
		char *msg;

		auto bytes = TCPreceiveSegment(clientSocket, &msg);

		if (bytes > 0) {
			PSCheader header;
			printHeaderStr(msg);
			disassembleHeader((uint8_t *)(msg), &header);
			printHeaderStruct(&header);
		}
		if (bytes <= 0) {
			return;
		}
	}
}

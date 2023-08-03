#include "server.hpp"

#include "constants.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "sslConn.hpp"
#include "tcpConn.hpp"
#include "utils.hpp"
#include "user.hpp"

#include <thread>

int main() {

	userFull tmp;

	tmp.UUID = rand();
	auto uname = "A really complex name with utf-8 shit in it";
	memcpy(tmp.uname, uname, 44);

	saveUser(&tmp);
 
	return 0;

	auto ssck = tcpConn::initializeServer(DEFAULT_PORT, 4);

	auto stopAllThreads = false;

	acceptClient(ssck, &stopAllThreads);
}

void acceptClient(Socket serverSocket, bool *threadStop) {

	while (!*threadStop) {

		auto client = tcpConn::acceptClientSock(serverSocket);

		if (client == -1) {
			continue;
		}

		std::thread(resolveClient, client, threadStop).detach();
		log(LOG_INFO, "Launched thread for resolving the client %d request.\n", client);
	}
}

void resolveClient(Socket clientSocket, bool *threadStop) {

	while (!*threadStop) {
		char *msg;

		auto bytes = tcpConn::receiveSegmentC(clientSocket, msg);

		if (bytes > 0) {
			PSCheader header;
			printHeaderStr(msg);
			disassembleHeader((uint8_t *)(msg), header);
			printHeaderStruct(&header);
		}
		if (bytes <= 0) {
			return;
		}
	}
}

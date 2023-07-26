#include "server.hpp"

#include "constants.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "sslConn.hpp"
#include "tcpConn.hpp"

#include <thread>

int main() {

	PSCheader psch;
	psch.versionMajor = 13;
	psch.versionMinor = 10;

	psch.method     = METHOD_PATCH << 1 | VARIANT_BIN;
	psch.bodyLength = 12;

	psch.UUID = rand() * 53;

	psch.timestamp = time(0);

	char msg[TOT_HEADER_LEN];

	assembleHeader(msg, psch);

	auto ssck = tcpConn::initializeServer(DEFAULT_PORT, 4);

	auto stopAllThreads = false;

	acceptClient(ssck, &stopAllThreads);
	// Wait for
}

void acceptClient(Socket serverSocket, bool *threadStop) {

	while (!*threadStop) {

		auto client = tcpConn::acceptClientSock(serverSocket);

		if (client == -1) {
			continue;
		}

		std::thread(resolveClient, client, threadStop).detach();
	}
}

void resolveClient(Socket clientSocket, bool *threadStop) {

	while (!*threadStop) {
		char *msg;

		auto bytes = tcpConn::receiveSegmentC(clientSocket, msg);

		if (bytes > 0) {
		}
	}
}
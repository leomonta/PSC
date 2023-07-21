#include "server.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "sslConn.hpp"
#include "tcpConn.hpp"

#include <thread>

int main() {

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
	
	char *msg;

	tcpConn::receiveSegmentC(clientSocket, msg);

	printf("%s\n", msg);
}
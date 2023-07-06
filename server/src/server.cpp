#include "tcpConn.hpp"

int main() {

	auto ssck = tcpConn::initializeServer(800, 4);

	while (true) {
		auto client = tcpConn::acceptClientSock(ssck);

		if (client != -1) {
			const char* fy = "No, FuckYou Too";
			tcpConn::sendSegmentC(client, fy);
			break;
		}
	}
}
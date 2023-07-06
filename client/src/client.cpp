#include "tcpConn.hpp"

#include <iostream>

int main() {

	auto csck = tcpConn::initializeClient(800, "127.0.0.1");

	std::string msg = "Fuck You";

	tcpConn::sendSegment(csck, msg);

	tcpConn::receiveSegment(csck, msg);

	std::cout << msg << std::endl;
}
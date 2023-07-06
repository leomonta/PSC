#pragma once

/*
Thanks to -> https://www.linuxhowtos.org/C_C++/socket.htm
*/

#include <string>

#define INVALID_SOCKET -1

#define DEFAULT_BUFLEN 8192
typedef int Socket;

namespace tcpConn {

	/**
	 * setup a server listening on port
	 * @return the server socket if successfull, INVALID_SOCKET if not
	 * @param port the port to listen from
	 * @param protocol Which IPv protocol to use, defaults to 4
	 */
	Socket initializeServer(const short port, const char protocol = 4);

	/**
	 * retup a client connected to server_name
	 * @return the client socket
	 * @param port the port to connect with
	 * @param server_name the name of the server to connect to
	 * @param protocol Which IPv protocol to use, defaults to 4
	 */
	Socket initializeClient(const short port, const char *server_name, const char protocol = 4);

	/**
	 * shorthand to close and shutdown a socket
	 */
	void terminate(const Socket sck);

	/**
	 * close the given socket, close the related fd
	 */
	void closeSocket(const Socket sck);

	/**
	 * send the tcp shutdown message through the socket
	 */
	void shutdownSocket(const Socket sck);

	/**
	 * receive a segment from the specified socket
	 * @return the amount of bytes sent
	 */
	int receiveSegment(const Socket sck, std::string &result);

	/**
	 * send a segment through specified socket
	 * @return the amount of bytes received
	 */
	long sendSegment(const Socket sck, std::string &buff);

	/**
	 * @return a client that wants to connect to this server
	 */
	Socket acceptClientSock(const Socket ssck);
} // namespace tcpConn
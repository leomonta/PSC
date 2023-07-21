#pragma once

#include "tcpConn.hpp"

void acceptClient(Socket serverSocket, bool *threadStop);

void resolveClient(Socket clientSocket, bool *threadStop);
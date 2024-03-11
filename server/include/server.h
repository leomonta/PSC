#pragma once

#include "tcpConn.h"

void acceptClient(Socket serverSocket, bool *threadStop);

void resolveClient(Socket clientSocket, bool *threadStop);

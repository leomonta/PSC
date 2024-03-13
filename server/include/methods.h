#pragma once

#include "tcpConn.h"
#include "threadpool.h"

typedef struct {
	Socket serverSocket;
	tPool *threadPool;
} runtimeInfo;

void* proxy_accClient(void* data);

void acceptClient(const runtimeInfo *rti);

void* proxy_resReq(void* data);

void resolveRequest(const threadData clientSocket);

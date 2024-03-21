#pragma once

#include "header.h"
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

/**
 * store the message body and some metadata in the 
int storeMessage(const PSCheader* msg);

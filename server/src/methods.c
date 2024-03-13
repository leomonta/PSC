#include "methods.h"

#include "header.h"
#include "logger.h"
#include "threadpool.h"
#include "utils.h"

#include <poll.h>

void *proxy_accClient(void *data) {
	acceptClient((runtimeInfo *)(data));
	return nullptr;
}

void acceptClient(const runtimeInfo *rti) {
	Socket client = -1;

	struct pollfd ss = {
	    .fd     = rti->serverSocket,
	    .events = POLLIN,
	};

	// continue until the entire threadPool stops
	while (!rti->threadPool->stop) {

		// infinetly wait for the socket to become usable
		poll(&ss, 1, -1);

		client = TCPacceptClientSock(rti->serverSocket);

		if (client == -1) {
			// error, ignore
			continue;
		}

		log(LOG_DEBUG, "[SERVER] Launched request resolver for socket %d\n", client);

		threadData tinfo = {client};

		TPOOLenque(rti->threadPool, &tinfo);
	}
}

void *proxy_resReq(void *data) {
	tPool *pool = (tPool *)data;

	while (!pool->stop) {
		threadData tdata = TPOOLdequeue(pool);

		// dequeue might return a null value for how it is implemented, i deal with that
		if (tdata.clientSocket == -1) {
			log(LOG_DEBUG, "DEQUEUE return null\n");
			continue;
		}

		resolveRequest(tdata);
	}
	return nullptr;
}

void resolveRequest(const threadData tdata) {

	while (true) {
		char *msg;

		ssize_t bytes = TCPreceiveSegment(tdata.clientSocket, &msg);

		if (bytes > 0) {
			PSCheader header;
			printHeaderStr(msg);
			disassembleHeader((uint8_t *)(msg), &header);
			printHeaderStruct(&header);
		}
		if (bytes <= 0) {
			return;
		}
	}
}

#include "server.h"

#include "constants.h"
#include "methods.h"
#include "threadpool.h"

int main() {

	Socket ssck = TCPinitializeServer(DEFAULT_PORT, 4);

	tPool *threadPool = TPOOLcreate(CONCURRENT_THREADS);

	//pthread_t clientAcceptor;

	runtimeInfo rti = {
		ssck,
		threadPool
	};

	//pthread_create(&clientAcceptor, NULL, proxy_accClient, ssck);
	acceptClient(&rti);
	return 0;
}

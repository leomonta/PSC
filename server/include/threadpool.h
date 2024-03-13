#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <sslConn.h>
#include <stddef.h>
#include <tcpConn.h>

/**
 * A thread pool created exclusively for managing the resolveRequest function
 */
typedef struct {
	Socket clientSocket;
} threadData;

// linked list node
typedef struct tjob {
	threadData   data; // data of the node. I use a copy since the data is small (12B) and trivial int + ptr
	struct tjob *next; // next job
} TPOOLjob;

// linked list + other thread related data
typedef struct {
	TPOOLjob       *head;      // head of linked list
	TPOOLjob       *tail;      // tail of linked list
	sem_t           sempahore; // semaphore to decide how many can get in the critical section
	pthread_mutex_t mutex;     // mutex to enter the critical section
	size_t          tCount;    // num of threads allocated in *threads
	pthread_t      *threads;   // array of the pthreads started, it will not change once the tpoll is created
	bool            stop;      // should the threads stop
} tPool;

/**
 * Create a thread pool on the heap and returns it.
 * It also start tCount threads with the proxy_resReq function
 *
 * @param tCount the number of concurrent threads to start
 * @return an allocated thread pool
 */
tPool *TPOOLcreate(const size_t tCount);

/**
 * Free all the resource allocated in the given thrad pool and the thread pool itself
 *
 * @parma tp the thread pool the destroy
 */
void TPOOLdestroy(tPool *tp);

/**
 * Take a job form the given threadpool queue
 *
 * @parama tPool the thread pool where to take the job from
 * @return a pointer to a resolver_data structure
 */
threadData TPOOLdequeue(tPool *tpool);

/**
 * Add the given data to the queue as a job for ay waiting thread
 *
 * @param tpool the thread pool where to put the job in
 * @param data the data that will be copied into the thread job
 */
void TPOOLenque(tPool *tpool, const threadData *data);

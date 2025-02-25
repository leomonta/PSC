#include "threadpool.h"

#include "methods.h"

#include <errno.h>
#include <fcntl.h>
#include <logger.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

tPool *TPOOLcreate(const size_t tCount) {
	// TODO
	// the tpool is kinda big to be created on the stack (112 B)
	// but maybe it's fine due to copy elision
	tPool *res = (tPool *)(malloc(sizeof(tPool)));

	// init linked list
	res->head   = nullptr;
	res->tail   = nullptr;
	res->tCount = 0;

	// the semaphore is the one responsible for preventing race conditions
	int errCode = sem_init(&res->sempahore, 0, 0);
	if (errCode != 0) {
		llog(LOG_ERROR, "[THREAD POOL] Could not initilize semphore -> %s\n", strerror(errno));
	}

	errCode = pthread_mutex_init(&res->mutex, NULL);

	switch (errCode) {
	case EAGAIN:
		llog(LOG_ERROR, "[THREAD POOL] Could not initializer mutex, not enough resources\n");
		break;
	case ENOMEM:
		llog(LOG_ERROR, "[THREAD POOL] Could not initializer mutex, not enough memory\n");
		break;
	case EPERM:
		llog(LOG_ERROR, "[THREAD POOL] Could not initializer mutex, not enough priviledges\n");
		break;
	case EINVAL:
		llog(LOG_ERROR, "[THREAD POOL] Could not initializer mutex, invalid settings in attr\n");
		break;
	case 0:
	default:
		break;
	}

	res->threads = (pthread_t *)(malloc(tCount * sizeof(pthread_t)));

	for (size_t i = 0; i < tCount; ++i) {
		errCode = pthread_create(&res->threads[res->tCount], NULL, proxy_resReq, (void *)(res));
		switch (errCode) {
		case EINVAL:
			// since I don't use attr for the pthread this should never happen
			llog(LOG_ERROR, "[THREAD POOL] Could not initialize thread, invalid settings in attr\n");
			break;
		case EPERM:
			// since I don't use attr for the pthread this should never happen
			llog(LOG_ERROR, "[THREAD POOL] Could not initialize thread, no permission to set schedule policies for attr\n");
			break;
		case EAGAIN:
			llog(LOG_ERROR, "[THREAD POOL] Could not initialize thread, not enough resources\n");
			break;

		default:
		case 0:
			res->tCount++;
			break;
		}
	}

	return res;
}

void TPOOLdestroy(tPool *tp) {

	// stops all threads
	tp->stop = true;

	// wait for the threads to finish
	for (size_t i = 0; i < tp->tCount; ++i) {
		// the thread may be wating for a job at the sempahore, by faking data even if there actually is data
		// I force the thread to check the stop value. Thus ensuring the thread exits gracefully
		// I might also start a timer of 0.5s to then call pthread_kill(SIG_KILL) to forcefully terminate the thread
		sem_post(&tp->sempahore);
		pthread_join(tp->threads[i], NULL);
	}

	// free the arrat used to store the thread ids
	free(tp->threads);

	// freeing the leftover data (if any)
	TPOOLjob *curr = tp->head;
	while (curr != nullptr) {
		TPOOLjob *t = curr->next;
		free(curr);
		curr = t;
	}

	pthread_mutex_destroy(&tp->mutex);
	sem_destroy(&tp->sempahore);

	// TODO remove this if stack allocation is used in ThreadPool::create
	// lastly
	free(tp);
}

threadData TPOOLdequeue(tPool *tpool) {

	// this takes care of lletting pass n threads for n datas
	sem_wait(&tpool->sempahore);
	pthread_mutex_lock(&tpool->mutex);
	// ---------------------------------------------------------------------------------------------- CRITICAL SECTION START

	// I'm sure there is a job for a thread

	threadData res;

	if (tpool->head == nullptr) {
		res.clientSocket = -1;
	} else {
		res = tpool->head->data;
		if (tpool->head->next == nullptr) {
			tpool->tail = nullptr;
		}
		TPOOLjob *t = tpool->head;
		tpool->head = tpool->head->next;
		free(t);
	}

	// let the next one in
	// ---------------------------------------------------------------------------------------------- CRITICAL SECTION END
	pthread_mutex_unlock(&tpool->mutex);

	return res;
}

void TPOOLenque(tPool *tpool, const threadData *data) {

	pthread_mutex_lock(&tpool->mutex);
	// ---------------------------------------------------------------------------------------------- CRITICAL SECTION START
	TPOOLjob *ntail = (TPOOLjob *)(malloc(sizeof(TPOOLjob)));

	ntail->next = nullptr;
	ntail->data = *data;

	// headless / empty
	if (tpool->head == nullptr) {
		tpool->head = ntail;
		tpool->tail = ntail;
	} else {
		tpool->tail->next = ntail;
		tpool->tail       = ntail;
	}

	// notify anywating thread that there is data
	sem_post(&tpool->sempahore);
	// ---------------------------------------------------------------------------------------------- CRITICAL SECTION END
	pthread_mutex_unlock(&tpool->mutex);
}

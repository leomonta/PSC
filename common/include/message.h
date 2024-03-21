#pragma once

#include <stdint.h>
typedef struct {
	uint32_t UUID; // of who wrote the message
	uint32_t MSGID; // static id of the message
	uint32_t sentTS; // timestamp at the time of sending for the user
	uint32_t recvTS; // timestamp at the time of receiving for the server
	uint32_t size; // size of the message (4 Mln chars are enough)
	char *data;
} Message;

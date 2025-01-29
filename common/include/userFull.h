#pragma once

#include "constants.h"

#include <stdint.h>

typedef struct userFull {
	uint32_t UUID;
	char     uname[MAX_UNAME_LEN];
	void    *publicKey; // for when i put encryption
} userFull;

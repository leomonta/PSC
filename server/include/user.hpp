#pragma once

#include "stdint.h"

typedef struct userFull {
	uint32_t UUID;
	char    *uname[256];
	void    *publicKey; // for when i put encryption
} userFull;

/**
 * Given a user struct saves it to the user.dat file
 * If the user is already present in the file, updates the data on the file
 */
void saveUser(const userFull user);

/**
 * Generate a random 32bit UUID and ensures that it's already used
 */
uint32_t genUUID();

/**
 * Reads the users from the users.dat and puts it the given pointer
 */
void getAllUsers(userFull **users);

/**
 * Given a UUID get all of that user info and puts it in the given userFull struct
 */
void getUser(const uint32_t UUID, userFull *user);
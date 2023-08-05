#pragma once

#include "stdint.h"

typedef struct userFull {
	uint32_t UUID;
	char    uname[256];
	void    *publicKey; // for when i put encryption
} userFull;

// separator used in the users.dat file to separate the different data
// This symbol could be used in the user name, but that is not a problem,
// since we can just check The first and / or the last one, since the UUID does not include this symbol
#define SEPARATOR '|'

/**
 * Given a user struct saves it to the user.dat file
 * If the user is already present in the file, updates the data on the file
 * 
 * @param user the user to write to disk
 * @return 0 if success, 1 if failure
 */
int saveUser(const userFull *user);

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
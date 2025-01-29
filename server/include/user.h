#pragma once

#include <stdint.h>
#include "userFull.h"

#include "miniVector_userFull.h"

typedef struct {
	uint8_t versionMajor;
	uint8_t versionMinor;
	uint32_t numUsers;
} userFileMeta;

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
 *
 * @param num the uint32_t where to put the selected UUID
 *
 * @return 0 if successful 1 otherwise
 */
int genUUID(uint32_t *num);

/**
 * Reads the users from the users.dat and puts it the given pointer
 * If a line contains something that cannot be parsed to a user the line is ignored
 *
 * @param users the poitner where the put the allocated pointer to all of the users
 *
 * @return 0 if successful, 1 if file is not found, 2 if io error
 */
int getAllUsers(miniVector_userFull *users);

/**
 * Given a UUID get all of that user info and puts it in the given userFull struct
 * 
 * @param UUID the UUID of the user to get
 * @param user where to put the information found
 * 
 * @return 0 if successfull, 1 otherwise
 */
int getUser(const uint32_t UUID, userFull *user);

/**
 * Search the default user file for the metadata in it (the first line)
 *
 * @param meta where to put the fetched metadata
 * @return 0 if successfull, 1 otherwise
 */
int getUserFileMetadata(userFileMeta *meta);

/**
 * set the default file metadata to what is passed
 *
 * @param neta the metadata to write into the file
 * @return 0 if successfull, 1 otherwise
 */
int setUserFileMetadata(const userFileMeta *meta);

/**
 * updates the metedata inside the file by the amount of the specified struct, if the negative flag is true, the ampunt os subtracted instead of added
 *
 * @param meta the metadata values to update by
 * @param negative whether to subtract the amount in 'meta' instead of adding it
 * @return 0 if successfull, 1 otherwise
 */
int updateUserFileMetadata(const userFileMeta *meta, const bool negative);

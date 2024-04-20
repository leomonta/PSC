#include "user.h"

#include "constants.h"
#include "logger.h"
#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFLEN 1024

// since i check many times for io errors, i'm gonna use a single function to print the message, so i can replace it easily if needed
void fileErrLog() {
	log(LOG_ERROR, "An error occurred while operating if the user file on disk, -> %s\n", strerror(errno));
}

/**
 * Given a user, write it in the file "./users.dat" at the specified offset (from ftell) and whence (SEEK_SET, SEEK_CURR, SEEK_END)
 *
 * @param user the user to write to file
 * @param offset the offset to write it into the file starting from whence
 * @param the origin of the write operation
 * @return 0 if successfull 1 otherwise
 */
int writeUserToFile(const userFull *user, const long offset, const int whence) {
	// make the string to write in the file

	// space for the hex repr of the UUID (8b) the '|' (1b) separator and the entire name (namelen)
	size_t lineLen = 8 + 1 + MAX_UNAME_LEN; // + 1 + KEY_LENGTH;
	char   lineToWrite[lineLen];            // stack allocated

	stringifyHex(user->UUID, (unsigned char *)lineToWrite, true);

	lineToWrite[8] = SEPARATOR;

	// put the uname in the line to write on the file
	strncpy(&lineToWrite[9], user->uname, MAX_UNAME_LEN);

	FILE *fd = fopen("./users.dat", "a+");

	// file does not exists
	if (fd == NULL) {
		fileErrLog();
		return 1;
	}

	// go to the end
	if (fseek(fd, offset, whence)) {
		fclose(fd);
		fileErrLog();
		return 1;
	}

	// make a new line
	if (fwrite("\n", 1, 1, fd) < 1) {
		fclose(fd);
		fileErrLog();
		return 1;
	}

	// write the line
	if (fwrite(lineToWrite, 1, lineLen, fd) < lineLen) {
		fclose(fd);
		fileErrLog();
		return 1;
	}

	fclose(fd);

	return 0;
}

int saveUser(const userFull *user) {

	miniVector_userFull allUsrs = makeMiniVector_userFull(sizeof(userFull));

	int err = getAllUsers(&allUsrs);

	if (err == 2) {
		// io err, just propagate it up
		destroyMiniVector_userFull(&allUsrs);
		return 1;
	} else if (err == 1) {

		// no old file, make a new one and write to it

		int res = writeUserToFile(user, 0, SEEK_END);

		if (res != 0) {
			destroyMiniVector_userFull(&allUsrs);
			return res;
		}
	}

	// need to add or modify the given user

	// find the user in all of the users in the file, if it's not there add it at the end, if found modify it

	bool found = false;

	// for every user,
	//   Is it the one that needs to be modified, write the updated one
	// else
	//   write the current user
	for (size_t i = 0; i < allUsrs.count; ++i) {
		userFull *elem = getElement_userFull(&allUsrs, i);

		// wrong user? check next
		if (elem->UUID != user->UUID) {
			continue;
		}

		found = true;

		// FIXME: reading the file twice
		FILE *fd = fopen("./users.dat", "r");

		if (fd == NULL) {
			fileErrLog();
			destroyMiniVector_userFull(&allUsrs);
			return 1;
		}

		char buffer[BUFLEN];

		char *read = nullptr;

		// search file line by line
		while (true) {
			read = fgets(buffer, BUFLEN, fd);

			if (read == nullptr) {
				// break in any case
				break;
			}

			uint32_t hex;
			unStringifyHex((uint8_t *)buffer, &hex);
			if (hex == user->UUID) {
				writeUserToFile(user, ftell(fd), SEEK_SET);
			}
		}

		fclose(fd);
	}

	if (!found) {
		writeUserToFile(user, 0, SEEK_END);
	}

	destroyMiniVector_userFull(&allUsrs);
	return 0;
}

/*
 * parse a str to a userFull struct and puts it in dest
 */
int toUserFull(const char *str, userFull *dest) {

	// the line should be something like
	// a1b2c3d4|username.....

	// UUID len (8) + Separator (1) + minimum name size (1);
	if (strlen(str) < 8 + 1 + 1) {
		return 1; // not a valid string
	}

	// extract the UUID
	// I don't need to modify the string in any way since unstringify check only the first 8 bytes
	if (unStringifyHex((const unsigned char *)str, &dest->UUID)) {
		// this means that the first 8 bytes are not a hex representation
		return 1;
	}

	// skip the UUID and the separator
	strncpy(dest->uname, str + 9, MAX_UNAME_LEN - 1);
	dest->uname[MAX_UNAME_LEN - 1] = '\0'; // ensures a null terminator in all cases

	return 0;
}

int getAllUsers(miniVector_userFull *users) {

	// I use errno to check for errors
	errno      = NO_ERR;
	FILE *file = fopen("./users.dat", "r");

	// file does not exist, err 1
	if (file == NULL) {
		fileErrLog();
		return 1;
	}
	// opening failed for other reasons, err 2
	if (errno != NO_ERR) {
		fileErrLog();
		fclose(file);
		return 2;
	}

	userFull curr;

	// read BUFLEN chars at the time
	char buffer[BUFLEN];

	char *read = nullptr;
	// search file line by line
	while (true) {
		read = fgets(buffer, BUFLEN, file);

		if (read == nullptr) {
			// this means ferror or feof

			if (ferror(file)) {
				fileErrLog();
			}
			// break in any case
			break;
		}

		// tuUserFull should not SIGSEGV or such, so i can just pass watherver fgets gives me
		if (!toUserFull(buffer, &curr)) {
			append_userFull(users, &curr);
		}
	}

	fclose(file);
	return 0;
}

int genUUID(uint32_t *num) {
	// not really need a secure random
	srand((unsigned int)(time(0)));
	uint32_t candidate = (uint32_t)rand();

	// TODO
	// Yes this allocates 5 * sizeof(userFull) -> 5 * (256 + 4) -> 1300 bytes
	// maybe in the future i will use something smaller
	miniVector_userFull allUsrs = makeMiniVector_userFull(sizeof(userFull));

	// file does not exist, cannot collide with other UUIDs
	switch (getAllUsers(&allUsrs)) {

	case 1:
		*num = candidate;
		return 0;
	case 2: // io err
		return 1;
	};

	for (size_t i = 0; i < allUsrs.count; ++i) {
		userFull *elem = getElement_userFull(&allUsrs, i);

		// Another user has this UUID
		if (candidate == elem->UUID) {
			candidate = (uint8_t)rand();

			// restart
			i = 0;
			// prevent the ++i
			continue;
		}
	}

	*num = candidate;

	destroyMiniVector_userFull(&allUsrs);

	return 0;
}

int getUserFileMetadata(userFileMeta *meta) {
	// I use errno to check for errors
	errno      = NO_ERR;
	FILE *file = fopen("./users.dat", "r");

	*meta = (userFileMeta){0, 0, 0};

	// file does not exist, err 1
	if (file == NULL) {
		fileErrLog();
		return 1;
	}
	// opening failed for other reasons, err 2
	if (errno != NO_ERR) {
		fileErrLog();
		fclose(file);
		return 2;
	}

	uint32_t buffer;

	fread(&buffer, 1, 4, file);

	meta->versionMajor = buffer & 0b1111'0000'0000'0000'0000'0000'0000'0000 >> 28;
	meta->versionMinor = buffer & 0b0000'1111'0000'0000'0000'0000'0000'0000 >> 24;

	meta->numUsers = buffer & 0b1111'1111'1111'1111'1111'1111;

	return 0;
}

int setUserFileMetadata(const userFileMeta *meta);

/**
 * updates the metedata inside the file by the amount of the specified struct, if the negative flag is true, the ampunt os subtracted instead of added
 *
 * @param meta the metadata values to update by
 * @param negative whether to subtract the amount in 'meta' instead of adding it
 * @return 0 if successfull, 1 otherwise
 */
int updateUserFileMetadata(const userFileMeta *meta, const bool negative);

#include "user.h"

#include "constants.h"
#include "logger.h"
#include "miniVector.h"
#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFLEN 1024

// since i check many times for io errors, i'm gonna use a single function to print the message, so i can replace it easily if needed
void fileErrLog() {
	log(LOG_ERROR, "A file error occurred while saving the user to disk, -> %s\n", strerror(errno));
}

int logNfree(char *buff) {
	fileErrLog();
	free(buff);
	return 1;
}

int writeUserToFile(const userFull *user, const long offset, const int whence) {
	// make the string to write in the file

	// space for the hex repr of the UUID (8b) the '|' (1b) separator and the entire name (namelen)
	size_t lineLen = 8 + 1 + MAX_UNAME_LEN; // + 1 + KEY_LENGTH;
	char lineToWrite[lineLen];            // stack allocated

	stringifyHex(user->UUID, (unsigned char *)lineToWrite, true);

	lineToWrite[8] = SEPARATOR;

	// put the uname in the line to write on the file
	strncpy(&lineToWrite[9], user->uname, MAX_UNAME_LEN);

	FILE *fd = fopen("./users.dat", "w");

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

	auto allUsrs = makeMiniVector(sizeof(userFull), 5);

	auto err = getAllUsers(&allUsrs);

	if (err == 2) {
		// io err, just propagate it up
		destroyMiniVector(&allUsrs);
		return 1;
	} else if (err == 1) {

		// no old file, make a new one and write to it

		auto res = writeUserToFile(user, 0, SEEK_END);

		if (res != 0) {
			destroyMiniVector(&allUsrs);
			return res;
		}

	}

	// need to add or modify the given user

	// find the user in all of the users in the file, if it's not there add it at the end, if found modify it

	// for every user,
	//   Is it the one that needs to be modified, write the updated one
	// else
	//   write the current user
	for (size_t i = 0; i < allUsrs.count; ++i) {
		auto elem = (const userFull *)(getElement(&allUsrs, i));

		// wrong user? check next
		if (elem->UUID != user->UUID) {
			continue;
		}

		// FIXME: reading the file twice
		auto fd = fopen("./users.dat", "r");
		
		if (fd == NULL) {
			destroyMiniVector(&allUsrs);
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
			unStringifyHex((uint8_t *) buffer, &hex);
			if (hex == user->UUID) {
				writeUserToFile(user, ftell(fd), SEEK_SET);
			}
		}

		fclose(fd);
	}

	writeUserToFile(user, 0, SEEK_END);

	destroyMiniVector(&allUsrs);
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

int getAllUsers(miniVector *users) {

	// I use errno to check for errors
	errno     = NO_ERR;
	auto file = fopen("./users.dat", "r");

	// file does not exist, err 1
	if (file == NULL) {
		log(LOG_ERROR, "File opening failed -> %s\n", strerror(errno));
		return 1;
	}
	// opening failed for other reasons, err 2
	if (errno != NO_ERR) {
		log(LOG_ERROR, "File opening failed -> %s\n", strerror(errno));
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
				log(LOG_ERROR, "Could not retrive all of the users from the file, -> %s\n", strerror(errno));
			}
			// break in any case
			break;
		}

		// tuUserFull should not SIGSEGV or such, so i can just pass watherver fgets gives me
		if (!toUserFull(buffer, &curr)) {
			append(users, &curr);
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
	miniVector allUsrs = makeMiniVector(sizeof(userFull), 5);

	// file does not exist, cannot collide with other UUIDs
	switch (getAllUsers(&allUsrs)) {

	case 1:
		*num = candidate;
		return 0;
	case 2: // io err
		return 1;
	};

	for (size_t i = 0; i < allUsrs.count; ++i) {
		auto elem = (userFull *)(getElement(&allUsrs, i));

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

	destroyMiniVector(&allUsrs);

	return 0;
}

#include "user.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "utils.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// since i check many times for io errors, i'm gonna use a single function to print the message, so i can replace it easily if needed
void fileErrLog() {
	log(LOG_ERROR, "A file error occurred while saving the user to disk\n");
}

int logNfree(char *buff) {
	fileErrLog();
	free(buff);
	return 1;
}

int saveUser(const userFull *user) {

	auto allUsrs = makeMiniVector(sizeof(userFull), 5);

	auto err = getAllUsers(&allUsrs);

	FILE *fd;

	if (err == 2) {
		// io err, just propagate it up
		return 1;
	} else if (err == 1) {

		// make the string to write in the file

		auto namelen = strlen(user->uname);
		namelen      = namelen > MAX_UNAME_LEN ? MAX_UNAME_LEN : namelen;

		// space for the hex repr of the UUID (8b) the '|' (1b) separator and the entire name (namelen)
		auto lineLen = 8 + 1 + namelen;
		char lineToWrite[lineLen]; // stack allocated

		stringifyHex(user->UUID, lineToWrite, true);

		lineToWrite[8] = SEPARATOR;

		// put the uname in the line to write on the file
		strncpy(&lineToWrite[9], user->uname, namelen);
		// file does not exists

		fd = fopen("./users.dat", "w");

		if (fseek(fd, 0, SEEK_END)) {
			fileErrLog();
			return 1;
		}

		if (fwrite("\n", 1, 1, fd) < 1) {
			fileErrLog();
			return 1;
		}
		if (fwrite(lineToWrite, 1, lineLen, fd) < lineLen) {
			fileErrLog();
			return 1;
		}

		fclose(fd);
	}

	// need to add or modify the given user

	// find the user in all of the users in the file, if it's not there add it at the end, if found modify it

	// for every user,
	//   Is it the one that needs to be modified, write the updated one
	// else
	//   write the current user
	for (size_t i = 0; i < allUsrs.count; ++i) {
		auto elem = (const userFull *)(getElement(&allUsrs, i));

		// write the new user instead of the old one
		if (elem->UUID == user->UUID) {
			elem = user;
		}

		auto namelen = strlen(elem->uname);
		namelen      = namelen > MAX_UNAME_LEN ? MAX_UNAME_LEN : namelen;

		// space for the hex repr of the UUID (8b) the '|' (1b) separator and the entire name (namelen)
		auto lineLen = 8 + 1 + namelen;
		char lineToWrite[lineLen]; // stack allocated

		stringifyHex(elem->UUID, lineToWrite, true);

		// temporarily use the null terminator here for searching this in the file
		lineToWrite[8] = '\0';

		// put the uname in the line to write on the file
		strncpy(&lineToWrite[9], elem->uname, namelen);
	}

	return 0;

	/*
	// I use errno to check for errors
	errno     = NO_ERR;
	auto file = fopen("./users.dat", "r+b");

	// file does not exist, create it
	if (errno == ENOENT) { // no such file or directory
	    errno = NO_ERR;
	    log(LOG_DEBUG, "users file not found, creating a new one\n");
	    file = fopen("./users.dat", "wb");
	}

	// opening failed for other reasons
	if (errno != NO_ERR) {
	    log(LOG_ERROR, "File opening failed -> %s\n", strerror(errno));
	    fclose(file);
	    return 1;
	}

	// now we have an open file rw in binary positioned at the start

	auto namelen = strlen(user->uname);

	// space for the hex repr of the UUID (8) the '|' (1) separator and the entire name (namelen)
	auto lineLen = 8 + 1 + namelen;
	char UUID_name[lineLen]; // stack allocated

	stringifyHex(user->UUID, UUID_name, true);

	// temporarily use the null terminator here for searching this in the file
	UUID_name[8] = '\0';

	// complete the string with the username
	strncpy(&UUID_name[9], user->uname, namelen);

	size_t lineNum = 0;
	size_t colNum  = 0;

	// FIXME: by calling findInFile and then reading thee file again in a buff i'm reading the entire file twice
	if (findInFile(UUID_name, file, &lineNum, &colNum)) {

	    // get the file len
	    if (fseek(file, 0, SEEK_END)) {
	        fileErrLog();
	        return 1;
	    }
	    auto fileLen = ftell(file);

	    // return at start
	    if (fseek(file, 0, SEEK_SET)) {
	        fileErrLog();
	        return 1;
	    }

	    // entire file + the entire line
	    auto  bufLen = fileLen + 1;
	    char *buff   = (char *)(malloc(bufLen));

	    if (buff == nullptr) {
	        log(LOG_ERROR, "Could not allocate %d bytes of memory\n", bufLen);
	        free(buff);
	        return 1;
	    }

	    // read the file into the buffer
	    fread(buff, 1, fileLen, file);
	    if (!ferror(file)) {
	        return logNfree(buff);
	    }
	    // needed to prevent strchr from going rogue
	    buff[bufLen - 1] = '\0';

	    if (fseek(file, 0, SEEK_SET)) {
	        return logNfree(buff);
	    }

	    // I write line per line the old content of the file,
	    // when i get to the line i have to replace i write the new line and skip the old

	    size_t nl  = 0;
	    char  *pos = buff;

	    // lines pre modification
	    while (nl < lineNum) {
	        auto   pos_n = strchr(pos, '\n') + 1; // strchr points to the \n, skip it
	        size_t sz    = pos_n - pos;
	        if (fwrite(pos, 1, sz, file) < sz) {
	            return logNfree(buff);
	        }
	        pos = pos_n;
	        ++nl;
	    }

	    // modified line
	    UUID_name[8] = SEPARATOR;
	    if (fwrite(UUID_name, 1, lineLen, file) < lineLen) {
	        return logNfree(buff);
	    }
	    if (fwrite("\n", 1, 1, file) < 1) {
	        return logNfree(buff);
	    };

	    // skip the old replace line
	    pos = strchr(pos, '\n') + 1;

	    // lines post modification
	    while (true) {
	        auto pos_n = strchr(pos, '\n') + 1; // strchr points to the \n, skip it
	        if (pos_n == (char *)(1)) {         // nullptr + 1
	            break;
	        }
	        size_t sz = pos_n - pos;
	        if (fwrite(pos, 1, sz, file) < sz) {
	            return logNfree(buff);
	        }
	        pos = pos_n;
	    }

	    size_t sz = buff + fileLen - pos;
	    if (fwrite(pos, 1, sz, file) < sz) {
	        return logNfree(buff);
	    }

	    free(buff);

	} else {
	    if (fseek(file, 0, SEEK_END)) {
	        fileErrLog();
	        return 1;
	    }
	    UUID_name[8] = SEPARATOR;

	    if (fwrite("\n", 1, 1, file) < 1) {
	        fileErrLog();
	        return 1;
	    }
	    if (fwrite(UUID_name, 1, lineLen, file) < lineLen) {
	        fileErrLog();
	        return 1;
	    }
	}

	fclose(file);
	return 0;*/
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
	// I don't need to modify the string in any way since unstringify check only the first 8 positions
	if (unStringifyHex(str, &dest->UUID)) {
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
	if (errno == ENOENT) {
		return 1;
	}
	// opening failed for other reasons, err 2
	if (errno != NO_ERR) {
		log(LOG_ERROR, "File opening failed -> %s\n", strerror(errno));
		fclose(file);
		return 2;
	}

	// the linestr is automatically allocated by getline, but we need to free it at the end
	char  *linestr = nullptr;
	size_t len     = 0;

	userFull curr;

	ssize_t read = 0;
	// search file line by line
	while (true) {
		read = getline(&linestr, &len, file);

		if (read == -1) {
			// this means ferror or feof

			if (ferror(file)) {
				log(LOG_ERROR, "Could not retrive all of the users from the file\n");
			}
			// break in any case
			break;
		}

		// tuUserFull should not SIGSEGV or such, so i can just pass watherver getline gives me
		if (!toUserFull(linestr, &curr)) {
			append(users, &curr);
		}
	}

	if (linestr) {
		free(linestr);
	}

	fclose(file);
	return 0;
}

int genUUID(uint32_t *num) {
	// not really need a secure random
	srand((unsigned int)(time(0)));
	uint32_t candidate = rand();

	// TODO
	// Yes this allocates 5 * sizeof(userFull) -> 5 * (256 + 4) -> 1300 bytes
	// maybe in the future i will use somthing smaller
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
			candidate = rand();

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
#include "user.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "utils.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// since i check many times for io errors, i'm gonna use a single function to print the message, so i can replace it easily if needed
void fileErrLog() {
	log(LOG_ERROR, "A file error occurred while saving the user to disk\n");
}

int logNfree(char *buff) {
	fileErrLog();
	free(buff);
	return 1;
}

// I don't like this, but i don't know how to do better
int saveUser(const userFull *user) {
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
			auto pos_n = strchr(pos, '\n') + 1; // strchr points to the \n, skip it
			auto sz    = pos_n - pos;
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
			auto sz = pos_n - pos;
			if (fwrite(pos, 1, sz, file) < sz) {
				return logNfree(buff);
			}
			pos = pos_n;
		}

		auto sz = buff + fileLen - pos;
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
	return 0;
}
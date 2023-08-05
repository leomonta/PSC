#include "user.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int saveUser(const userFull *user) {
	// I use errno to check for errors
	errno     = 0;
	auto file = fopen("./users.dat", "r+b");

	// file does not exist, create it
	if (errno == ENOENT) { // no such file or directory
		errno = 0;
		file  = fopen("./users.dat", "wb");
	}

	// opening failed for other reasons
	if (errno != 0) {
		log(LOG_ERROR, "File opening failed -> %s\n", strerror(errno));
		fclose(file);
		return 1;
	}

	// now we have an open file rw bìin binary positioned at the start

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

	// FIXME: by calling findInFile and then reading it again in a buff i'm reading the entire file twice
	if (findInFile(UUID_name, file, &lineNum, &colNum)) {

		// get the file len
		if (fseek(file, 0, SEEK_END) != 0) {
			return 1;
		}
		auto fileLen = ftell(file);

		// return at start
		if (fseek(file, 0, SEEK_SET) != 0) {
			return 1;
		}

		// entire file + the entire line
		auto  bufLen = fileLen + 1;
		char *buff   = (char *)(malloc(bufLen));

		if (buff == nullptr) {
			log(LOG_ERROR, "Could not allocate %d bytes of memory\n", bufLen);
			return 1;
		}

		// read the file into the buffer
		fread(buff, 1, fileLen, file);

		if (ferror(file) != 0) {
			return 1;
		}

		// needed to prevent strchr from going rogue
		buff[bufLen - 1] = '\0';
		if (fseek(file, 0, SEEK_SET) != 0) {
			return 1;
		}

		// I write line per line the old conent of the file, 
		// when i get to the line i have to replace i write the new line and skip the old


		size_t nl  = 0;
		char  *pos = buff;

		// lines pre modification
		while (nl < lineNum) {
			auto pos_n = strchr(pos, '\n') + 1; // strchr points to the \n, skip it
			fwrite(pos, 1, pos_n - pos, file);
			pos = pos_n;
			++nl;
		}

		// modified line
		UUID_name[8] = SEPARATOR;
		fwrite(UUID_name, 1, lineLen, file);
		fwrite("\n", 1, 1, file);

		// skip the old replace line
		pos = strchr(pos, '\n') + 1;

		// lines post modification
		while (true) {
			auto pos_n = strchr(pos, '\n') + 1; // strchr points to the \n, skip it
			if (pos_n == (char *)(1)) { // nullptr + 1
				break;
			}
			fwrite(pos, 1, pos_n - pos, file);
			pos = pos_n;
		}

		fwrite(pos, 1, buff + fileLen - pos, file);

		free(buff);

	} else {
		fseek(file, 0, SEEK_END);
		UUID_name[8] = SEPARATOR;
		fwrite("\n", 1, 1, file);
		fwrite(UUID_name, 1, lineLen, file);
	}

	fclose(file);
	return 0;
}
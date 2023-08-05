#include "user.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int saveUser(const userFull *user) {
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

	// now we have and actual file that exist

	auto namelen = strlen(user->uname);

	// space for the hex repr of the UUID the '|' separator and the entire name
	auto lineLen = 8 + 1 + namelen;
	char UUID_name[lineLen];

	stringifyHex(user->UUID, UUID_name, true);

	// temporarily use the null terminator here
	UUID_name[8]   = '\0';
	strncpy(&UUID_name[9], user->uname, namelen);
	size_t lineNum = 0;
	size_t colNum  = 0;

	if (findInFile(UUID_name, file, &lineNum, &colNum)) {

		fseek(file, 0, SEEK_END);
		auto fileLen = ftell(file);
		fseek(file, 0, SEEK_SET);

		// entire file + the entire line
		auto bufLen = fileLen + lineLen;
		char *buff = (char *)(malloc(bufLen));

		// read the file into the buffer
		fread(buff, 1, fileLen, file);

		buff[fileLen] = '\0';

		int   nl  = 0;
		char *pos = buff;

		while (nl < lineNum) {
			pos = strchr(pos, '\n') + 1;
			nl += pos != nullptr ? 1 : 0;
		}

		// i have the intial position of the line that i have to overwrite

		auto write_ptr = pos + lineLen;
		auto read_ptr  = strchr(pos, '\n'); // get the start of the next line

		while (true) {
			*write_ptr++ = *read_ptr++;
			if (write_ptr == buff + bufLen) {
				break;
			}
		}

		// replace the line with the old info
		UUID_name[8] = '|';
		for (int i = 0; i < lineLen; ++i) {
			pos[i] = UUID_name[i];
		}

		fseek(file, 0, SEEK_SET);
		fwrite(buff, 1, strlen(buff), file);
		// rewrite the data in the file

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
#include "user.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int saveUser(const userFull *user) {
	errno     = 0;
	auto file = fopen("./users.dat", "r+");

	// file does not exist, create it
	if (errno == ENOENT) { // no such file or directory
		errno = 0;
		file  = fopen("./users.dat", "w");
	}

	// opening failed for other reasons
	if (errno != 0) {
		log(LOG_ERROR, "File opening failed -> %s\n", strerror(errno));
		fclose(file);
		return 1;
	}

	// now we have and actual file that exist

	char UUIDstr[8 + 1];

	stringifyHex(user->UUID, UUIDstr, true);

	UUIDstr[8]     = '\0';
	size_t lineNum = 0;
	size_t colNum  = 0;

	if (findInFile(UUIDstr, file, &lineNum, &colNum)) {
		
		//replace the line with the old info
	} else {
		fseek(file, 0, SEEK_END);
		auto len = strlen(user->uname);
		UUIDstr[8] = SEPARATOR;
		fwrite(UUIDstr, 1, 9, file);
		fwrite(user->uname, 1, len, file);
		fwrite("\n", 1, 1, file);
	}

	fclose(file);
	return 0;
}
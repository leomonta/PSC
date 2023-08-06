#include "utils.hpp"

#include "constants.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHeaderStr(const char *head) {

#ifdef NDEBUG
	return
#else

	// for all the byte of the header (4 * 32bits )
	for (int i = 0; i < TOT_HEADER_LEN; ++i) {

		// for all the bits in a byte in reverse
		for (int j = 7; j >= 0; --j) {
			unsigned char bit = (head[i] >> j) & 1;
			printf("%u", bit);
		}
		
		// if we printed 32 bits / 4 bytes, newline
		if (i % 4 == 3) {
			printf("\n");
		} else {
			// else make space
			printf(" ");
		}
	}

	printf("\n");

#endif
}

void printHeaderStruct(const PSCheader *head) {

#ifdef NDEBUG
	return
#else

	printf("Header Version                 -> %d.%d\n", head->versionMajor, head->versionMinor);
	printf("Header Method and variant      -> %s - %s\n", METHODS_STR[head->method > 1], VARIANTS_STR[head->method & 0b001]);
	printf("Lenght of the attached data    -> %u\n", head->bodyLength);
	printf("UUID of the sender             -> %u\n", head->UUID);
	printf("Timestamp at which is was sent -> %u\n", head->UUID);

#endif
}

void stringifyHex(const uint32_t val, char *strloc, const bool lowercase) {

	char a = 'A';
	if (lowercase) {
		a = 'a';
	}

	// 8 quartets (0b0000) in 32 bits
	for (int i = 0; i < 8; ++i, strloc++) {
		uint8_t quartet = (uint8_t)(val >> (8 - i - 1) * 4); // consider the correct quartet starting from the right-most one
		quartet &= 0b1111;                                   // removes evetyhing other than the first 4 bits

		if (quartet < 10) {
			*strloc = (uint8_t)('0' + quartet);
		} else {
			*strloc = (uint8_t)(a + quartet - 10);
		}
	}
}

const char *strnstr(const char *haystack, const char *needle, const size_t count) {

	if (count == 0) {
		return nullptr;
	}

	// position we are checking on the needle string
	auto Itr = 0;

	for (size_t i = 0; i < count; ++i) {

		// match, advance the search
		if (haystack[i] == needle[Itr]) {
			++Itr;
			if (needle[Itr] == '\0') {
				// match
				return haystack + i - Itr + 1;
			}

		// no match, reset the search
		} else {
			i -= Itr;
			Itr = 0;
		}
	}

	// no match
	return nullptr;
}

bool findInFile(const char *toSearch, FILE *file, size_t *line, size_t *col) {

	*line = 0;

	size_t len     = 0;
	// the linestr is automatically allocated by getline, but we need to free it at the end
	char  *linestr = nullptr;

	bool res = false;

	// search file line by lide
	while (true) {
		auto read = getline(&linestr, &len, file);

		// reached eof
		if (read == -1) {
			res = false;
			break;
		}

		auto foundPos = strnstr(linestr, toSearch, read);
		if (foundPos != nullptr) {
			// save the column
			*col = foundPos - linestr;

			// found!
			res = true;
			break;
		}

		++(*line);
	}

	if (linestr) {
		free(linestr);
	}

	return res;
}
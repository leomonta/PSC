#include "utils.hpp"

#include "constants.hpp"

#include <stdio.h>
#include <stdlib.h>

void printHeaderStr(const char *head) {

#ifdef NDEBUG
	return
#else

	for (int i = 0; i < TOT_HEADER_LEN; ++i) {
		for (int j = 7; j >= 0; --j) {
			unsigned char bit = (head[i] >> j) & 1;
			printf("%u", bit);
		}
		if (i % 4 == 3) {
			printf("\n");
		} else {
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

	// 8 quartets 0x0000 in 32 bits
	for (int i = 0; i < 8; ++i, strloc++) {
		uint8_t quartet = val >> (8 - i - 1) * 4; // consiedare the correct quartet starting from the right-most
		quartet &= 0b1111;                        // removes evetyhing other than the first 4 bits

		if (quartet < 10) {
			*strloc = '0' + quartet;
		} else {
			*strloc = a + quartet - 10;
		}
	}
}

const char *strnstr(const char *haystack, const char *needle, const size_t count) {

	if (count == 0) {
		return nullptr;
	}

	auto Itr = 0;

	for (size_t i = 0; i < count; ++i) {

		if (haystack[i] == needle[Itr]) {
			++Itr;
			if (needle[Itr] == '\0') {
				// match
				return haystack + i - Itr + 1;
			}

		} else {
			// reset back the search
			i -= Itr;
			Itr = 0;
		}
	}

	// no match
	return nullptr;
}


bool findInFile(const char* toSearch, FILE *file, size_t *line, size_t *pos) {

	bool userAlreadySaved = false;

	size_t len  = 0;
	char  *linestr = nullptr;
	while (true) {
		auto read = getline(&linestr, &len, file);
		if (read == -1) {
			break;
		}

		auto foundPos = strnstr(linestr, toSearch, read);
		if (foundPos != nullptr) {
			return true;
		}
	}

	if (line) {
		free(line);
	}

	return false;
}
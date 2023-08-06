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

int unStringifyHex(const char *strloc, uint32_t *dest) {

	*dest = 0;

	/*
	   0110000 0                 1010000 P                 1110000 p
	   0110001 1    1000001 A    1010001 Q    1100001 a    1110001 q
	   0110010 2    1000010 B    1010010 R    1100010 b    1110010 r
	   0110011 3    1000011 C    1010011 S    1100011 c    1110011 s
	   0110100 4    1000100 D    1010100 T    1100100 d    1110100 t
	   0110101 5    1000101 E    1010101 U    1100101 e    1110101 u
	   0110110 6    1000110 F    1010110 V    1100110 f    1110110 v
	   0110111 7    1000111 G    1010111 W    1100111 g    1110111 w
	   0111000 8    1001000 H    1011000 X    1101000 h    1111000 x
	   0111001 9    1001001 I    1011001 Y    1101001 i    1111001 y
	                1001010 J    1011010 Z    1101010 j    1111010 z
	                1001011 K                 1101011 k
	                1001100 L                 1101100 l
	                1001101 M                 1101101 m
	                1001110 N                 1101110 n
	                1001111 O                 1101111 o
	*/

	for (int i = 0; i < 8; ++i, strloc++) {

		uint8_t quartet = *strloc;

		if (quartet <= '9' && quartet >= '0') {
			*dest += (quartet & 0b0000'1111) << ((8 - i - 1) * 4);
		} else if ((quartet >= 'A' && quartet <= 'F') || (quartet >= 'a' && quartet <= 'f')) { 
			*dest += (quartet & 0b0001'1111) << ((8 - i - 1) * 4);
		} else {
			return 1; // failure
		}
	}

	return 0;
}

void stringifyHex(const uint32_t val, char *strloc, const bool lowercase) {

	char a = 'A';
	if (lowercase) {
		a = 'a';
	}

	// 8 quartets (0b0000) in 32 bits
	for (int i = 0; i < 8; ++i, strloc++) {
		uint8_t quartet = (uint8_t)(val >> ((8 - i - 1) * 4)); // consider the correct quartet starting from the right-most one
		quartet &= 0b1111;                                     // removes evetyhing other than the first 4 bits

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

	size_t len    = 0;
	// the linestr is automatically allocated by getline, but we need to free it at the end
	char *linestr = nullptr;

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
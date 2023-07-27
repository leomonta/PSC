#include "utils.hpp"

#include "constants.hpp"

#include <stdio.h>

void printHeader(const char *head) {

#ifdef NDEBUG
	return
#else

	for (int i = 0; i < TOT_HEADER_LEN; ++i) {
		for (int j = 7; j >= 0; --j) {
			unsigned char bit = (head[i] >> j) & 1;
			printf("%u", bit);
		}
		if (i == 4) {
			printf("\n");
		}
		printf(" ");
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
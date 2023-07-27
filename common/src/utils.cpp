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
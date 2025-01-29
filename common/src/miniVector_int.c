

#include <minitVector_int>

#include <stdlib.h>
#include <string.h>

miniVector_int makeMiniVector_int(const size_t initialCount) {
	miniVector res = {
	    .data     = malloc(initialCount * sizeof(int)),
	    .capacity = initialCount,
	    .count    = 0,
	};

	return res;
}

void append_int(miniVector_int *vec, int *element) {
	if (vec->count == vec->capacity) {
		grow(vec);
	}

	memcpy(&(vec->data[vec->count]), element, sizeof(int));

	++(vec->count);
}

void grow_int(miniVector_int *vec) {
	vec->capacity *= 2;
	vec->data = realloc(vec->data, sizeof(int) * vec->capacity);
}

void destroyMiniVector_int(miniVector_int *vec) {
	// such logic
	free(vec->data);
}

int* getElement_int(const miniVector_int *vec, const size_t index) {
	if (index >= vec->count) {
		// invalid pos
		return nullptr;
	}

	return &vec->data[index];
}

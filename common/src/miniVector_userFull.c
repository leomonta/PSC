

#include <miniVector_userFull.h>

#include <stdlib.h>
#include <string.h>

miniVector_userFull makeMiniVector_userFull(const size_t initialCount) {
	miniVector_userFull res = {
	    .data     = malloc(initialCount * sizeof(userFull)),
	    .capacity = initialCount,
	    .count    = 0,
	};

	return res;
}

void append_userFull(miniVector_userFull *vec, userFull *element) {
	if (vec->count == vec->capacity) {
		grow_userFull(vec);
	}

	memcpy(&(vec->data[vec->count]), element, sizeof(userFull));

	++(vec->count);
}

void grow_userFull(miniVector_userFull *vec) {
	vec->capacity *= 2;
	vec->data = realloc(vec->data, sizeof(userFull) * vec->capacity);
}

void destroyMiniVector_userFull(miniVector_userFull *vec) {
	// such logic
	free(vec->data);
}

userFull* getElement_userFull(const miniVector_userFull *vec, const size_t index) {
	if (index >= vec->count) {
		// invalid pos
		return nullptr;
	}

	return &vec->data[index];
}

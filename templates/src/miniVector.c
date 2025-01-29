template <T>

#include <minitVector_#T#>

#include <stdlib.h>
#include <string.h>

miniVector_#T# makeMiniVector_#T#(const size_t initialCount) {
	miniVector res = {
	    .data     = malloc(initialCount * sizeof(T)),
	    .capacity = initialCount,
	    .count    = 0,
	};

	return res;
}

void append_#T#(miniVector_#T# *vec, T *element) {
	if (vec->count == vec->capacity) {
		grow(vec);
	}

	memcpy(&(vec->data[vec->count]), element, sizeof(T));

	++(vec->count);
}

void grow_#T#(miniVector_#T# *vec) {
	vec->capacity *= 2;
	vec->data = realloc(vec->data, sizeof(T) * vec->capacity);
}

void destroyMiniVector_#T#(miniVector_#T# *vec) {
	// such logic
	free(vec->data);
}

T* getElement_#T#(const miniVector_#T# *vec, const size_t index) {
	if (index >= vec->count) {
		// invalid pos
		return nullptr;
	}

	return &vec->data[index];
}

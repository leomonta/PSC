#include "miniVector.hpp"

#include <stdlib.h>
#include <string.h>

miniVector makeMiniVector(const size_t elementSize, const size_t initialCount) {
	miniVector res = {
	    .data        = malloc(initialCount * elementSize),
	    .capacity    = initialCount,
	    .elementSize = elementSize,
	    .count       = 0,
	};

	return res;
}

void append(miniVector *vec, void *element) {
	if (vec->count == vec->capacity) {
		grow(vec);
	}

	memcpy(vec->data + (vec->count * vec->elementSize), element, vec->elementSize);

	++(vec->count);
}

void grow(miniVector *vec) {
	vec->data = realloc(vec->data, vec->capacity * 2);
	vec->capacity *= 2;
}
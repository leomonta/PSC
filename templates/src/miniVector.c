INCL

#define miniVector r(miniVector)
#define makeMiniVector r(makeMiniVector)
#define append r(append)
#define grow r(grow)
#define destroy r(destroy)
#define getElement r(getElement)

#include <stdlib.h>
#include <string.h>

miniVector makeMiniVector(const size_t initialCount) {
	miniVector res = {
	    .data        = malloc(initialCount * sizeof(TYPE)),
	    .capacity    = initialCount,
	    .count       = 0,
	};

	return res;
}

void append(miniVector *vec, TYPE element) {
	if (vec->count == vec->capacity) {
		grow(vec);
	}

	memcpy(&(vec->data[vec->count]), &element, sizeof(TYPE));

	++(vec->count);
}

void grow(miniVector *vec) {
	vec->capacity *= 2;
	vec->data = realloc(vec->data, sizeof(TYPE) * vec->capacity);
}

void destroyMiniVector(miniVector *vec) {
	// such logic
	free(vec->data);
}

TYPE getElement(const miniVector *vec, const size_t index) {
	if (index >= vec->count) {
		// invalid pos
		return 0;
	}

	return vec->data[index];
}

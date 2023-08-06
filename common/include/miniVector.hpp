#pragma once

#include <stddef.h>

typedef struct miniVector {

	void  *data;        // data ptr
	size_t capacity;    // total allocated byte
	size_t elementSize; // size of an element
	size_t count;       // how many elements are stored at the moment

} miniVector;

/**
 * Make a mini vector with a preallocated array of elementSize * initalCount lenght
 * 
 * @param elementSize how big (in bytes) is a single element
 * @param initalCount how many bytes to preallocate;
 * 
 * @return a built miniVector
 */
miniVector makeMiniVector(const size_t elementSize, const size_t initialCount);

/**
 * append an element of the previously specified size at the end of the vector
 * 
 * @param vec the miniVecor where to append the data
 * @param element a pointer to the data to be appended
 */
void append(miniVector *vec, void *element);

/**
 * Doubles the capacity of the given vector
 * 
 * @param vec the vecotr to grow
 */
void grow(miniVector *vec);
#pragma once

#include <stddef.h>

INCL

#define miniVector r(miniVector)
#define makeMiniVector r(makeMiniVector)
#define append r(append)
#define grow r(grow)
#define destroy r(destroy)
#define getElement r(getElement)

typedef struct {

	TYPE  *data;        // data ptr
	size_t capacity;    // total allocated byte
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
miniVector makeMiniVector(const size_t initialCount);

/**
 * append an element of the previously specified size at the end of the vector
 *
 * @param vec the miniVecor where to append the data
 * @param element a pointer to the data to be appended
 */
void append(miniVector *vec, TYPE element);

/**
 * Doubles the capacity of the given vector
 *
 * @param vec the vector to grow
 */
void grow(miniVector *vec);

/**
 * Frees all the resource allocated by vec
 *
 * @param vec the vector the destroy
 */
void destroyMiniVector(miniVector *vec);

/**
 * Return the element at the specified position
 * 
 * @param vec the vector to get the element from
 * @param index the position the element should be
 * 
 * @return the the element at pos index
 */
TYPE getElement(const miniVector *vec, const size_t index);

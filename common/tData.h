#include <stddef.h>







// double macro expension yay!







#include tData


typedef struct {

 tData *data; // data ptr
 size_t capacity; // total allocated byte
 size_t count; // how many elements are stored at the moment

} miniVector_tData;

/**
 * Make a mini vector with a preallocated array of elementSize * initalCount lenght
 *
 * @param elementSize how big (in bytes) is a single element
 * @param initalCount how many bytes to preallocate;
 *
 * @return a built miniVector
 */
miniVector_tData makeMiniVector_tData(const size_t initialCount);

/**
 * append an element of the previously specified size at the end of the vector
 *
 * @param vec the miniVecor where to append the data
 * @param element a pointer to the data to be appended
 */
void append_tData(miniVector_tData *vec, tData element);

/**
 * Doubles the capacity of the given vector
 *
 * @param vec the vector to grow
 */
void grow_tData(miniVector_tData *vec);

/**
 * Frees all the resource allocated by vec
 *
 * @param vec the vector the destroy
 */
void destroyMiniVector_tData(miniVector_tData *vec);

/**
 * Return the element at the specified position
 * 
 * @param vec the vector to get the element from
 * @param index the position the element should be
 * 
 * @return the the element at pos index
 */
tData getElement_tData(const miniVector_tData *vec, const size_t index);

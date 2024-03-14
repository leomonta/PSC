#include <stddef.h>


typedef struct {

 int *data; // data ptr
 size_t capacity; // total allocated byte
 size_t count; // how many elements are stored at the moment

} miniVector_int;

/**
 * Make a mini vector with a preallocated array of elementSize * initalCount lenght
 *
 * @param elementSize how big (in bytes) is a single element
 * @param initalCount how many bytes to preallocate;
 *
 * @return a built miniVector
 */
miniVector_int makeMV(const size_t initialCount);

/**
 * append an element of the previously specified size at the end of the vector
 *
 * @param vec the miniVecor where to append the data
 * @param element a pointer to the data to be appended
 */
void append_int(miniVector_int *vec, int element);

/**
 * Doubles the capacity of the given vector
 *
 * @param vec the vector to grow
 */
void grow_int(miniVector_int *vec);

/**
 * Frees all the resource allocated by vec
 *
 * @param vec the vector the destroy
 */
void destroyMiniVector(miniVector_int *vec);

/**
 * Return the element at the specified position
 * 
 * @param vec the vector to get the element from
 * @param index the position the element should be
 * 
 * @return the the element at pos index
 */
int getElement_int(const miniVector_int *vec, const size_t index);
#include <stdlib.h>
#include <string.h>

miniVector_int makeMiniVector(const size_t initialCount) {
 miniVector_int res = {
     .data = malloc(initialCount * sizeof(int)),
     .capacity = initialCount,
     .count = 0,
 };

 return res;
}

void append_int(miniVector_int *vec, int element) {
 if (vec->count == vec->capacity) {
  grow_int(vec);
 }

 memcpy(&(vec->data[vec->count]), &element, sizeof(int));

 ++(vec->count);
}

void grow_int(miniVector_int *vec) {
 vec->capacity *= 2;
 vec->data = realloc(vec->data, sizeof(int) * vec->capacity);
}

void destroyMiniVector(miniVector_int *vec) {
 // such logic
 free(vec->data);
}

int getElement_int(const miniVector_int *vec, const size_t index) {
 if (index >= vec->count) {
  // invalid pos
  return 0;
 }

 return vec->data[index];
}

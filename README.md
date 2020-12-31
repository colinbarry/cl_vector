cl_vector
=========

cl_vector is an efficient, type-safe(ish) C89 vector class. It is deployed as a single include file containing two macros, one of which is instantiated to create an API for a specific element type, and the other an implementation for the API.

## Usage

### `CL_VECTOR_API(ELEMENT_TYPE, STRUCT_TAG, PREFIX)`
`CL_VECTOR_API` is a macro that creates the API for a vector of a specific element type. The struct will have a type of `STRUCT_TAG`, and each function will be prefixed with `PREFIX`. `STRUCT_TAG` must be a unique structure name, and `PREFIX` must generate functions with unique names.

This macro, or more specifically the output of this macro, must be visible to client code in order to create vectors or call functions on the vector.

### `CL_VECTOR_IMPL(ELEMENT_TYPE, STRUCT_TAG, PREFIX)`
`CL_VECTOR_IMPL` is a macro that creates the implementation for a vector of a specific element type. 

As this macro generates object code, it must be applied in exactly one .c file. The macro application must also see the result of a preceeding call to CL_VECTOR_API with the same arguments.

## Defining a vector type

To define a vector for ints, using the struct name IntVec and function prefix vec\_:

```c
/* intvec.h */
#include "cl_vector.h"

CL_VECTOR_API(int, IntVec, vec_)
```

```c
/* intvec.c */
#include "intvec.h"

CL_VECTOR_IMPL(int, IntVec, vec_)
```

## API

> N.B.: The function names listed below are unprefixed. S is  the struct tag type, and T the element type.

`S *new()` creates and returns a pointer to new heap-allocated, `construct`-ed vector. 

`void construct(S *vec)` initialises a S.

`void delete(S *vec)` `destruct`-s and deletes a previously heap-allocated S.

`void destruct(S *vec)` destructs a S.

`void setdeleter(S *vec, void(*deleter)(T element, void *userdata), void *userdata)`  Specifies an optional deleter callback function that will be called before an element is removed from the vector. The callback function must have the form `void callback(T element, void *userdata)`.

`int len(const S *vec)` returns the number of elements in the vector.

`int cap(const S *vec)` returns the capacity of the vector. The capacity of a vector is the number of elements it can hold before needing to resize its internal buffer.

`int compact(S *vec)` attempts to compact the vector, reducing the unused capacity so that `cap(vec) == len(vec)`.  Returns 0 on memory allocation failure, and 1 on success.

`T *data(const S *vec)` returns a raw pointer to the internal buffer data. This is not guaranteed to be valid after any calls to `compact`, `reserve`, `push`, `insert`, `erase`, `erasen`, and `clear`.

`int reserve(S *vec, int capacity)` attempts to expand the buffer capacity so that is can hold `capacity` elements without requiring further reallocations. Returns 0 on memory allocation failure, and 1 on success.

`int push(S *vec, T element)` pushes the `element` to the end of the vector.  Returns 0 on memory allocation failure, and 1 on success.

`int insert(S *vec, int pos, T element)` inserts the `element` into the vector at position `pos`.  Returns 0 on memory allocation failure, and 1 on success.

`T get(S *vec, int nth)` returns the `nth` element of the vector.

`void erase(S *vec, int nth)` removes the `nth` element from the vector.

`void erasen(S *vec, int nth, int num)` removes `num` elements from the vector, starting at the `nth` element.

`void clear(S *vec)` removes all elements (without reducing the capacity.)

## Example
```c
#include "cl_vector.h"
#include <stdio.h>
#include <stdlib.h>

CL_VECTOR_API(char, Vector, vec_)
CL_VECTOR_IMPL(char, Vector, vec_)

int main(int argc, char *argv[])
{
    struct Vector arr;
    int i;
    
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');

    for (i = 0; i < vec_len(&arr); ++i) {
        printf("[%i] = %c\n", i, vec_get(&arr, i));
    }

    vec_destruct(&arr);

    return EXIT_SUCCESS;
}
```

## Tests
See the /tests directory.

#include "../cl_vector.h"
#include "picotest.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

CL_VECTOR_API(char, Vector, vec_)
CL_VECTOR_IMPL(char, Vector, vec_)

DEF_TEST(construct_an_empty_array)
{
    struct Vector arr;
    
    vec_construct(&arr);
    ASSERT(vec_cap(&arr) == 0);
    ASSERT(vec_len(&arr) == 0);

    vec_destruct(&arr);
}

DEF_TEST(construct_an_empty_array_on_heap)
{
    struct Vector* arr;
    
    arr = vec_new();
    ASSERT(vec_cap(arr) == 0);
    ASSERT(vec_len(arr) == 0);

    vec_delete(arr);
}

DEF_TEST(push_element)
{
    struct Vector arr;
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');

    ASSERT(vec_len(&arr) == 3);
    ASSERT(vec_get(&arr, 0) == 'a');
    ASSERT(vec_get(&arr, 1) == 'b');
    ASSERT(vec_get(&arr, 2) == 'c');

    vec_destruct(&arr);
}

DEF_TEST(insert_element)
{
    struct Vector arr;
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');

    vec_insert(&arr, 0, 'z');
    vec_insert(&arr, 2, 'x');
    vec_insert(&arr, 5, 'y');

    ASSERT(vec_len(&arr) == 6);
    ASSERT(vec_get(&arr, 0) == 'z');
    ASSERT(vec_get(&arr, 1) == 'a');
    ASSERT(vec_get(&arr, 2) == 'x');
    ASSERT(vec_get(&arr, 3) == 'b');
    ASSERT(vec_get(&arr, 4) == 'c');
    ASSERT(vec_get(&arr, 5) == 'y');

    vec_destruct(&arr);
}

DEF_TEST(erase_element)
{
    struct Vector arr;
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');

    vec_erase(&arr, 0);
    ASSERT(vec_len(&arr) == 2);
    ASSERT(vec_get(&arr, 0) == 'b');
    ASSERT(vec_get(&arr, 1) == 'c');

    vec_erase(&arr, 1);
    ASSERT(vec_len(&arr) == 1);
    ASSERT(vec_get(&arr, 0) == 'b');

    vec_erase(&arr, 0);
    ASSERT(vec_len(&arr) == 0);
    
    vec_destruct(&arr);
}

DEF_TEST(erase_elements)
{
    struct Vector arr;
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');
    vec_push(&arr, 'd');
    vec_push(&arr, 'e');

    vec_erasen(&arr, 1, 3);
    ASSERT(vec_len(&arr) == 2);
    ASSERT(vec_get(&arr, 0) == 'a');
    ASSERT(vec_get(&arr, 1) == 'e');

    vec_erasen(&arr, 0, 2);
    ASSERT(vec_len(&arr) == 0);
    
    vec_destruct(&arr);
}

DEF_TEST(clear)
{
    struct Vector arr;
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');

    vec_clear(&arr);
    ASSERT(vec_len(&arr) == 0);
    ASSERT(vec_cap(&arr) != 0);

    vec_destruct(&arr);
}

DEF_TEST(compact)
{
    struct Vector arr;
    vec_construct(&arr);

    vec_push(&arr, 'a');
    vec_push(&arr, 'b');
    vec_push(&arr, 'c');

    vec_clear(&arr);
    vec_compact(&arr);
    ASSERT(vec_cap(&arr) == 0);

    vec_destruct(&arr);
}

struct TestObject {
    int data;
};

CL_VECTOR_API(struct TestObject*, ObjVector, oa_)
CL_VECTOR_IMPL(struct TestObject*, ObjVector, oa_)

static struct TestObject *obj_new(int data, int *numobjs)
{
    struct TestObject *obj = malloc(sizeof(*obj));
    obj->data = data;
    ++(*numobjs);
    return obj;
}

static void obj_delete(struct TestObject *obj, void *ud)
{
    (*(int*)ud)--;
    free(obj);
}

DEF_TEST(deleter)
{
    struct ObjVector arr;
    int numobjs = 0;

    oa_construct(&arr);
    oa_setdeleter(&arr, &obj_delete, &numobjs);

    ASSERT(numobjs == 0);

    oa_push(&arr, obj_new(2, &numobjs));
    oa_push(&arr, obj_new(3, &numobjs));
    oa_push(&arr, obj_new(5, &numobjs));
    oa_push(&arr, obj_new(7, &numobjs));
    oa_push(&arr, obj_new(11, &numobjs));

    ASSERT(numobjs == 5);

    /* deleter must be called when one element is deleted. */
    oa_erase(&arr, 3);
    ASSERT(numobjs == 4);
    ASSERT(oa_get(&arr, 0)->data == 2);
    ASSERT(oa_get(&arr, 1)->data == 3);
    ASSERT(oa_get(&arr, 2)->data == 5);
    ASSERT(oa_get(&arr, 3)->data == 11);

    /* deleter must be called when multiple element are deleted. */
    oa_erasen(&arr, 1, 2);
    ASSERT(numobjs == 2);
    ASSERT(oa_get(&arr, 0)->data == 2);
    ASSERT(oa_get(&arr, 1)->data == 11);

    /* deleter must be called when an array is cleared  */
    oa_clear(&arr);
    ASSERT(numobjs == 0);

    /** delete must be called when an array is destructed */
    oa_push(&arr, obj_new(2, &numobjs));
    oa_push(&arr, obj_new(3, &numobjs));
    oa_push(&arr, obj_new(5, &numobjs));
    ASSERT(numobjs == 3);

    oa_destruct(&arr);
    ASSERT(numobjs == 0);
}

DEF_SUITE(cl_vector)
{
    RUN_TEST(construct_an_empty_array);
    RUN_TEST(construct_an_empty_array_on_heap);
    RUN_TEST(push_element);
    RUN_TEST(insert_element);
    RUN_TEST(erase_element);
    RUN_TEST(erase_elements);
    RUN_TEST(clear);
    RUN_TEST(compact);
    RUN_TEST(deleter);
}


IMPORT_SUITE(cl_vector);

int main(int argc, char *argv[])
{
    BEGIN_TESTS();
    RUN_SUITE(cl_vector);

    printf("\n%i PASSED\n%i FAILED\n", passes, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

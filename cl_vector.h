#if !defined(CL_VECTOR_H_)
#define CL_VECTOR_H_

#include <string.h>
#include <assert.h>

#define CL_VECTOR_API(ELEMENT_TYPE, STRUCT_TAG, PREFIX)                       \
    struct STRUCT_TAG {                                                       \
        int size;                                                             \
        int capacity;                                                         \
        ELEMENT_TYPE *data;                                                   \
        void *userdata;                                                       \
        void (*deleter)(ELEMENT_TYPE element, void *userdata);                \
    };                                                                        \
                                                                              \
    struct STRUCT_TAG *PREFIX##new();                                         \
    void PREFIX##construct(struct STRUCT_TAG *vec);                           \
    void PREFIX##delete(struct STRUCT_TAG *vec);                              \
    void PREFIX##destruct(struct STRUCT_TAG *vec);                            \
    void PREFIX##setdeleter(struct STRUCT_TAG *vec,                           \
                            void(*deleter)(ELEMENT_TYPE, void *),             \
                            void* userdata);                                  \
    int PREFIX##len(const struct STRUCT_TAG *vec);                            \
    int PREFIX##cap(const struct STRUCT_TAG *vec);                            \
    int PREFIX##compact(struct STRUCT_TAG* vec);                              \
    ELEMENT_TYPE *PREFIX##data(const struct STRUCT_TAG *vec);                 \
    int PREFIX##reserve(struct STRUCT_TAG* vec, int capacity);                \
    int PREFIX##push(struct STRUCT_TAG *vec, ELEMENT_TYPE value);             \
    int PREFIX##insert(struct STRUCT_TAG *vec, int pos, ELEMENT_TYPE value);  \
    ELEMENT_TYPE PREFIX##get(const struct STRUCT_TAG *vec, int nth);          \
    void PREFIX##erase(struct STRUCT_TAG* vec, int nth);                      \
    void PREFIX##erasen(struct STRUCT_TAG* vec, int nth, int num);            \
    void PREFIX##clear(struct STRUCT_TAG *vec);


#define CL_VECTOR_IMPL(ELEMENT_TYPE, STRUCT_TAG, PREFIX)                      \
    struct STRUCT_TAG *PREFIX##new()                                          \
    {                                                                         \
        struct STRUCT_TAG *vec = malloc(sizeof *vec);                         \
        if (!vec)                                                             \
            return NULL;                                                      \
        PREFIX##construct(vec);                                               \
        return vec;                                                           \
    }                                                                         \
                                                                              \
    void PREFIX##construct(struct STRUCT_TAG *vec)                            \
    {                                                                         \
        vec->size = 0;                                                        \
        vec->capacity = 0;                                                    \
        vec->data = NULL;                                                     \
        vec->deleter = NULL;                                                  \
        vec->userdata = NULL;                                                 \
    }                                                                         \
                                                                              \
    void PREFIX##calldeleter(struct STRUCT_TAG *vec,                          \
                             int from,                                        \
                             int to)                                          \
    {                                                                         \
        if (vec->deleter) {                                                   \
            int i;                                                            \
            for (i = from; i < to; ++i) {                                     \
                (*vec->deleter)(*(vec->data + i),                             \
                                vec->userdata);                               \
            }                                                                 \
        }                                                                     \
    }                                                                         \
                                                                              \
    void PREFIX##destruct(struct STRUCT_TAG *vec)                             \
    {                                                                         \
        PREFIX##calldeleter(vec, 0, vec->size);                               \
        free(vec->data);                                                      \
    }                                                                         \
                                                                              \
    void PREFIX##delete(struct STRUCT_TAG *vec)                               \
    {                                                                         \
        PREFIX##destruct(vec);                                                \
        free(vec);                                                            \
    }                                                                         \
                                                                              \
    void PREFIX##setdeleter(struct STRUCT_TAG *vec,                           \
                            void(*deleter)(ELEMENT_TYPE, void *),             \
                            void *userdata)                                   \
    {                                                                         \
        vec->deleter = deleter;                                               \
        vec->userdata = userdata;                                             \
    }                                                                         \
                                                                              \
    int PREFIX##len(const struct STRUCT_TAG *vec)                             \
    {                                                                         \
        return vec->size;                                                     \
    }                                                                         \
                                                                              \
    int PREFIX##cap(const struct STRUCT_TAG *vec)                             \
    {                                                                         \
        return vec->capacity;                                                 \
    }                                                                         \
                                                                              \
    int PREFIX##compact(struct STRUCT_TAG* vec)                               \
    {                                                                         \
        ELEMENT_TYPE* ptr;                                                    \
        ptr = realloc(vec->data, sizeof(ELEMENT_TYPE) * vec->size);           \
        if (!ptr)                                                             \
            return 0;                                                         \
        vec->capacity = vec->size;                                            \
        vec->data = ptr;                                                      \
        return 1;                                                             \
    }                                                                         \
                                                                              \
    ELEMENT_TYPE *PREFIX##data(const struct STRUCT_TAG *vec)                  \
    {                                                                         \
        return vec->data;                                                     \
    }                                                                         \
                                                                              \
    int PREFIX##reserve(struct STRUCT_TAG* vec, int capacity)                 \
    {                                                                         \
        if (vec->capacity < capacity) {                                       \
            ELEMENT_TYPE* ptr;                                                \
            vec->capacity = capacity;                                         \
            ptr = realloc(vec->data, sizeof(ELEMENT_TYPE) * vec->capacity);   \
            if (!ptr)                                                         \
                return 0;                                                     \
            vec->data = ptr;                                                  \
        }                                                                     \
        return 1;                                                             \
    }                                                                         \
                                                                              \
    int PREFIX##push(struct STRUCT_TAG *vec, ELEMENT_TYPE value)              \
    {                                                                         \
        return PREFIX##insert(vec, vec->size, value);                         \
    }                                                                         \
                                                                              \
    int PREFIX##insert(struct STRUCT_TAG *vec, int pos, ELEMENT_TYPE value)   \
    {                                                                         \
        if (vec->size == vec->capacity) {                                     \
        int ok = PREFIX##reserve(vec, vec->capacity ? 2 * vec->capacity : 8); \
        if (!ok)                                                              \
            return 0;                                                         \
      }                                                                       \
                                                                              \
        memmove(vec->data + pos + 1,                                          \
                vec->data + pos,                                              \
                sizeof(ELEMENT_TYPE) * (vec->size - pos));                    \
        vec->data[pos] = value;                                               \
        ++vec->size;                                                          \
        return 1;                                                             \
    }                                                                         \
                                                                              \
    ELEMENT_TYPE PREFIX##get(const struct STRUCT_TAG *vec, int nth)           \
    {                                                                         \
        assert(nth >= 0 && nth < vec->size);                                  \
        return vec->data[nth];                                                \
    }                                                                         \
                                                                              \
    void PREFIX##erase(struct STRUCT_TAG* vec, int nth)                       \
    {                                                                         \
        PREFIX##erasen(vec, nth, 1);                                          \
    }                                                                         \
                                                                              \
    void PREFIX##erasen(struct STRUCT_TAG* vec, int nth, int num)             \
    {                                                                         \
        assert(nth >= 0 && nth + num <= vec->size);                           \
        assert(num >= 0);                                                     \
                                                                              \
        PREFIX##calldeleter(vec, nth, nth + num);                             \
                                                                              \
        memmove(vec->data + nth,                                              \
                vec->data + nth + num,                                        \
                sizeof(ELEMENT_TYPE) * (vec->size - nth - num));              \
        vec->size -= num;                                                     \
    }                                                                         \
                                                                              \
    void PREFIX##clear(struct STRUCT_TAG *vec)                                \
    {                                                                         \
        PREFIX##calldeleter(vec, 0, vec->size);                               \
        vec->size = 0;                                                        \
    }

#endif

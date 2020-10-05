/**
 * @file
 * @brief Definition and implementation of generic stack
 */

#ifdef STACK_TYPE

// TODO: Typed define guards?

#include <cassert>
#include <sys/types.h>

#define TYPED(baseName, type) baseName##_##type
#define TYPED_STACK(type) TYPED(Stack, type)

struct TYPED_STACK(STACK_TYPE) {
// TODO: Make "private" somehow?
    ssize_t size = 0;
    ssize_t capacity = 0;
    STACK_TYPE* data = nullptr;
};

void constructStack(TYPED_STACK(STACK_TYPE)* thiz, size_t initialCapacity = 0) {
    assert(thiz != nullptr);

    thiz->size = 0;
    thiz->capacity = initialCapacity;
    thiz->data = new STACK_TYPE[initialCapacity];
}

void destructStack(TYPED_STACK(STACK_TYPE)* thiz) {
    assert(thiz != nullptr);

    thiz->size = 0;
    thiz->capacity = 0;
    delete[] thiz->data;
    thiz->data = nullptr;
}

static void enlarge(TYPED_STACK(STACK_TYPE)* thiz) {
    assert(thiz != nullptr);

    if (thiz->size == thiz->capacity) {
        thiz->capacity = (thiz->capacity == 0) ? 1 : thiz->capacity * 2;
        STACK_TYPE* newData = new STACK_TYPE[thiz->capacity];
        for (ssize_t i = 0; i < thiz->size; ++i) {
            newData[i] = thiz->data[i];
        }
        delete[] thiz->data;
        thiz->data = newData;
    }
}

void push(TYPED_STACK(STACK_TYPE)* thiz, STACK_TYPE x) {
    assert(thiz != nullptr);

    enlarge(thiz);
    thiz->data[thiz->size++] = x;
}

STACK_TYPE pop(TYPED_STACK(STACK_TYPE)* thiz) {
    assert(thiz != nullptr);

    return thiz->data[--thiz->size];
}

#endif // STACK_TYPE

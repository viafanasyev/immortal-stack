/**
 * @file
 * @brief Definition and implementation of generic stack
 */

#ifdef STACK_TYPE

// TODO: Typed define guards?

#include <cassert>
#include <sys/types.h>
#include "environment.h"
#include "logger.h"

#define TYPED(baseName, type) baseName##_##type
#define TYPED_STACK(type) TYPED(Stack, type)

struct TYPED_STACK(STACK_TYPE) {
    /* !!! Private members !!! */
    ssize_t _size = 0;
    ssize_t _capacity = 0;
    STACK_TYPE* _data = nullptr;
};

void constructStack(TYPED_STACK(STACK_TYPE)* thiz, size_t initialCapacity = 0);

void destructStack(TYPED_STACK(STACK_TYPE)* thiz);

void enlarge(TYPED_STACK(STACK_TYPE)* thiz);

void push(TYPED_STACK(STACK_TYPE)* thiz, STACK_TYPE x);

STACK_TYPE pop(TYPED_STACK(STACK_TYPE)* thiz);

STACK_TYPE top(TYPED_STACK(STACK_TYPE)* thiz);

ssize_t getStackSize(TYPED_STACK(STACK_TYPE)* thiz);

ssize_t getStackCapacity(TYPED_STACK(STACK_TYPE)* thiz);

//----------------------------------------------------------------------------------------------------------------------

static inline bool isStackOk(TYPED_STACK(STACK_TYPE)* stack) {
    return (stack != nullptr) && (stack->_size != -1) && (stack->_capacity != -1) && (stack->_size <= stack->_capacity);
}

constexpr const char* stackLogFileName = "stack-dump.txt";

#define LOG_STACK(stack) do {                                                                                          \
    logPrintf("%s [" PTR_FORMAT "] (%s:%d)", #stack, (uintptr_t)stack, __FILENAME__, __LINE__);                        \
    if (stack == nullptr) {                                                                                            \
        logPrintf("\n");                                                                                               \
        break;                                                                                                         \
    }                                                                                                                  \
    logPrintf(" = {\n");                                                                                               \
    ssize_t size = stack->_size;                                                                                       \
    ssize_t capacity = stack->_capacity;                                                                               \
    auto data = stack->_data;                                                                                          \
    LOG_VALUE_INDENTED(size, "\t");                                                                                    \
    LOG_VALUE_INDENTED(capacity, "\t");                                                                                \
    LOG_ARRAY_INDENTED(data, capacity, "\t");                                                                          \
    logPrintf("}\n");                                                                                                  \
} while (0)

#define CHECK_STACK_CONDITION(stack, condition) do {                                                                   \
    if (!(condition)) {                                                                                                \
        logOpen(stackLogFileName);                                                                                     \
        LOG_STACK(stack);                                                                                              \
        logClose();                                                                                                    \
        assert(condition);                                                                                             \
    }                                                                                                                  \
} while (0)

#define CHECK_STACK_OK(stack) CHECK_STACK_CONDITION(stack, isStackOk(stack))

//----------------------------------------------------------------------------------------------------------------------

void constructStack(TYPED_STACK(STACK_TYPE)* const thiz, size_t initialCapacity) {
    CHECK_STACK_CONDITION(thiz, (thiz != nullptr) && (thiz->_data == nullptr));

    thiz->_size = 0;
    thiz->_capacity = initialCapacity;
    thiz->_data = new STACK_TYPE[initialCapacity];
}

void destructStack(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);

    thiz->_size = 0;
    thiz->_capacity = 0;
    delete[] thiz->_data;
    thiz->_data = nullptr;
}

void enlarge(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);

    if (thiz->_size == thiz->_capacity) {
        thiz->_capacity = (thiz->_capacity == 0) ? 1 : thiz->_capacity * 2;
        auto newData = new STACK_TYPE[thiz->_capacity];
        for (ssize_t i = 0; i < thiz->_size; ++i) {
            newData[i] = thiz->_data[i];
        }
        delete[] thiz->_data;
        thiz->_data = newData;
    }

    CHECK_STACK_OK(thiz);
}

void push(TYPED_STACK(STACK_TYPE)* const thiz, STACK_TYPE x) {
    CHECK_STACK_OK(thiz);

    if (thiz->_size == thiz->_capacity) {
        enlarge(thiz);
    }
    thiz->_data[thiz->_size++] = x;

    CHECK_STACK_OK(thiz);
}

STACK_TYPE pop(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);
    CHECK_STACK_CONDITION(thiz, thiz->_size > 0);

    return thiz->_data[--thiz->_size];
}

STACK_TYPE top(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);
    CHECK_STACK_CONDITION(thiz, thiz->_size > 0);

    return thiz->_data[thiz->_size - 1];
}

ssize_t getStackSize(TYPED_STACK(STACK_TYPE)* const thiz) {
    return thiz->_size;
}

ssize_t getStackCapacity(TYPED_STACK(STACK_TYPE)* const thiz) {
    return thiz->_capacity;
}

#endif // STACK_TYPE

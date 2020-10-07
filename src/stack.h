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

/**
 * Primitive analog of C++ templates.
 * Generates name of the struct/class from it's base name and type parameter.
 */
#define TYPED(baseName, type) baseName##_##type

/**
 * Generates name of the stack struct from type parameter (e.g. Stack_int).
 */
#define TYPED_STACK(type) TYPED(Stack, type)

/**
 * Generic stack that can contain any (almost) value that is specified by STACK_TYPE macro.
 * Stack allocates new memory if there's no empty space left to add new element.
 * Stack operations (construct/destruct, push, pop, etc) should be performed using the functions below.
 */
struct TYPED_STACK(STACK_TYPE) {
    /* !!! Private members !!! */

    /** Number of elements in stack */
    ssize_t _size = 0;

    /** Actual size of the stack data array */
    ssize_t _capacity = 0;

    /** Array with stack data */
    STACK_TYPE* _data = nullptr;
};

/**
 * Creates a new stack with a given initial size of the data array.
 * @param[in, out] thiz       pointer to the stack this operation should be performed on
 * @param[in] initialCapacity initial size of the data array
 */
void constructStack(TYPED_STACK(STACK_TYPE)* thiz, size_t initialCapacity = 0);

/**
 * Destructs the given stack. Frees the dynamic memory and resets all struct members to initial state.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 */
void destructStack(TYPED_STACK(STACK_TYPE)* thiz);

/**
 * Multiplier that is used in enlarge function.
 */
#define STACK_ENLARGE_MULTIPLIER 2

/**
 * Enlarges the internal data array of the given stack.
 * If the capacity of the data array is zero, then it's set to one.
 * Otherwise, capacity is multiplied by STACK_ENLARGE_MULTIPLIER.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 */
void enlarge(TYPED_STACK(STACK_TYPE)* thiz);

/**
 * Pushes the given element on top of the stack.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @param[in] x         value to put on top of the stack
 */
void push(TYPED_STACK(STACK_TYPE)* thiz, STACK_TYPE x);

/**
 * Removes value from top of the stack.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return value that was on top of the stack.
 */
STACK_TYPE pop(TYPED_STACK(STACK_TYPE)* thiz);

/**
 * Gives value from top of the stack without removing it (unlike pop function).
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return value that is located on top of the stack
 */
STACK_TYPE top(TYPED_STACK(STACK_TYPE)* thiz);

/**
 * Gives the number of elements in the given stack.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return size of the stack.
 */
ssize_t getStackSize(TYPED_STACK(STACK_TYPE)* thiz);

/**
 * Gives the actual size of the stack (size of the data holder array).
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return capacity of the stack.
 */
ssize_t getStackCapacity(TYPED_STACK(STACK_TYPE)* thiz);

//----------------------------------------------------------------------------------------------------------------------

/**
 * Checks if the given stack is in normal state (correct size and capacity and no nullptrs).
 * @param[in, out] stack stack to check
 * @return true, if the given stack is ok, false otherwise.
 */
static inline bool isStackOk(TYPED_STACK(STACK_TYPE)* stack) {
    return (stack != nullptr)                 &&
           (stack->_size != -1)               &&
           (stack->_capacity != -1)           &&
           (stack->_size <= stack->_capacity) &&
           (stack->_data != nullptr);
}

/** Name of the stack log file */
#define stackLogFileName "stack-dump.txt"

/**
 * Logs the given stack into the log file.          <br>
 * Logged stack example:                            <br>
 * <code>
 *     stack [0x00007FFC455B9830] (main.cpp:12) = { <br>
 *         size = 3                                 <br>
 *         capacity = 5                             <br>
 *         data [0x0000560884197ED0] = {            <br>
 *             [0] = 1                              <br>
 *             [1] = 2                              <br>
 *             [2] = 3                              <br>
 *             [3] = 0                              <br>
 *             [4] = 0                              <br>
 *         }                                        <br>
 *     }                                            <br>
 * </code>
 */
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

/**
 * Checks if the given condition is true fo this stack.
 * If the condition is false, logs the stack into the file and fails an assertion.
 */
#define CHECK_STACK_CONDITION(stack, condition) do {                                                                   \
    if (!(condition)) {                                                                                                \
        logOpen(stackLogFileName);                                                                                     \
        LOG_STACK(stack);                                                                                              \
        logClose();                                                                                                    \
        assert(condition);                                                                                             \
    }                                                                                                                  \
} while (0)

/**
 * Checks if the given stack is in normal state.
 *
 * @see isStackOk
 */
#define CHECK_STACK_OK(stack) CHECK_STACK_CONDITION(stack, isStackOk(stack))

//----------------------------------------------------------------------------------------------------------------------

/**
 * Creates a new stack with a given initial size of the data array.
 * @param[in, out] thiz       pointer to the stack this operation should be performed on
 * @param[in] initialCapacity initial size of the data array
 */
void constructStack(TYPED_STACK(STACK_TYPE)* const thiz, size_t initialCapacity) {
    CHECK_STACK_CONDITION(thiz, (thiz != nullptr) && (thiz->_data == nullptr));

    thiz->_size = 0;
    thiz->_capacity = initialCapacity;
    thiz->_data = new STACK_TYPE[initialCapacity];
}

/**
 * Destructs the given stack. Frees the dynamic memory and resets all struct members to initial state.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 */
void destructStack(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);

    thiz->_size = 0;
    thiz->_capacity = 0;
    delete[] thiz->_data;
    thiz->_data = nullptr;
}

/**
 * Enlarges the internal data array of the given stack.
 * If the capacity of the data array is zero, then it's set to one.
 * Otherwise, capacity is multiplied by STACK_ENLARGE_MULTIPLIER.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 */
void enlarge(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);

    if (thiz->_size == thiz->_capacity) {
        thiz->_capacity = (thiz->_capacity == 0) ? 1 : thiz->_capacity * STACK_ENLARGE_MULTIPLIER;
        auto newData = new STACK_TYPE[thiz->_capacity];
        for (ssize_t i = 0; i < thiz->_size; ++i) {
            newData[i] = thiz->_data[i];
        }
        delete[] thiz->_data;
        thiz->_data = newData;
    }

    CHECK_STACK_OK(thiz);
}

/**
 * Pushes the given element on top of the stack.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @param[in] x         value to put on top of the stack
 */
void push(TYPED_STACK(STACK_TYPE)* const thiz, STACK_TYPE x) {
    CHECK_STACK_OK(thiz);

    if (thiz->_size == thiz->_capacity) {
        enlarge(thiz);
    }
    thiz->_data[thiz->_size++] = x;

    CHECK_STACK_OK(thiz);
}

/**
 * Removes value from top of the stack.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return value that was on top of the stack.
 */
STACK_TYPE pop(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);
    CHECK_STACK_CONDITION(thiz, thiz->_size > 0);

    return thiz->_data[--thiz->_size];
}

/**
 * Gives value from top of the stack without removing it (unlike pop function).
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return value that is located on top of the stack
 */
STACK_TYPE top(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_OK(thiz);
    CHECK_STACK_CONDITION(thiz, thiz->_size > 0);

    return thiz->_data[thiz->_size - 1];
}

/**
 * Gives the number of elements in the given stack.
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return size of the stack.
 */
ssize_t getStackSize(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_CONDITION(thiz, thiz != nullptr);

    return thiz->_size;
}

/**
 * Gives the actual size of the stack (size of the data holder array).
 * @param[in, out] thiz pointer to the stack this operation should be performed on
 * @return capacity of the stack.
 */
ssize_t getStackCapacity(TYPED_STACK(STACK_TYPE)* const thiz) {
    CHECK_STACK_CONDITION(thiz, thiz != nullptr);

    return thiz->_capacity;
}

#endif // STACK_TYPE

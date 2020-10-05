/**
 * @file
 */

#include "testlib.h"

#define STACK_TYPE int
#include "../src/stack.h"
#undef STACK_TYPE

TEST(constructDestruct, simpleIntStack) {
    Stack_int s{ 100, 200 };
    const size_t initialCapacity = 42;
    constructStack(&s, initialCapacity);

    ASSERT_EQUALS(s.size, 0);
    ASSERT_EQUALS(s.capacity, initialCapacity);
    ASSERT_TRUE(s.data != nullptr);

    destructStack(&s);

    ASSERT_EQUALS(s.size, 0);
    ASSERT_EQUALS(s.capacity, 0);
    ASSERT_TRUE(s.data == nullptr);
}

TEST(pushPop, correctStackElementsOrder) {
    Stack_int s{};
    constructStack(&s);

    const int elements[] = { 5, 2, 6, 2, 1, 7, 2, 3 };
    const ssize_t elementsCount = sizeof(elements) / sizeof(*elements);

    for (int x : elements) {
        push(&s, x);
    }

    ASSERT_EQUALS(s.size, elementsCount);
    for (ssize_t i = elementsCount - 1; i >= 0; --i) {
        ASSERT_EQUALS(pop(&s), elements[i]);
    }
    ASSERT_EQUALS(s.size, 0);

    destructStack(&s);
}

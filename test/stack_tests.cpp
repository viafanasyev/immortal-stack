/**
 * @file
 */

#include <sys/types.h>
#include "testlib.h"

#define STACK_SECURITY_LEVEL 3
#define STACK_TYPE int
#include "../src/stack.h"
#undef STACK_TYPE
#define STACK_TYPE double
#include "../src/stack.h"
#undef STACK_TYPE

TEST(constructDestruct, simpleIntStack) {
    Stack_int s{ {}, 0, 100, 200, nullptr, {} };
    const size_t initialCapacity = 42;
    constructStack(&s, initialCapacity);

    ASSERT_EQUALS(getStackSize(&s), 0);
    ASSERT_EQUALS(getStackCapacity(&s), initialCapacity);
    ASSERT_NOT_NULL(s._data);

    destructStack(&s);

    ASSERT_EQUALS(getStackSize(&s), 0);
    ASSERT_EQUALS(getStackCapacity(&s), 0);
    ASSERT_NULL(s._data);
}

TEST(pushTopPop, correctStackElementsOrder) {
    Stack_int s{};
    constructStack(&s);

    const int elements[] = { 5, 2, 6, 2, 1, 7, 2, 3 };
    const ssize_t elementsCount = sizeof(elements) / sizeof(*elements);

    for (int x : elements) {
        push(&s, x);
    }

    ASSERT_EQUALS(getStackSize(&s), elementsCount);
    for (ssize_t i = elementsCount - 1; i >= 0; --i) {
        ASSERT_EQUALS(top(&s), elements[i]);
        ASSERT_EQUALS(pop(&s), elements[i]);
    }
    ASSERT_EQUALS(getStackSize(&s), 0);

    destructStack(&s);
}

TEST(canaryTest, canaryModifyingFailsAssertion) {
    struct {
        long long canaryKillerBefore[1]{};
        Stack_int s{};
        long long canaryKillerAfter[1]{};
    } canaryTesting;

    constructStack(&canaryTesting.s);
    int containedValue = 42;
    push(&canaryTesting.s, containedValue);

    int wrongIndexBefore = 1;
    canaryTesting.canaryKillerBefore[wrongIndexBefore] = 0;
    ASSERT_DIES(top(&canaryTesting.s));
    canaryTesting.canaryKillerBefore[wrongIndexBefore] = canaryValue; // Restoring canary to properly destruct stack

    ASSERT_EQUALS(top(&canaryTesting.s), containedValue); // Just checking that canary is ok before next test

    int wrongIndexAfter = -1;
    canaryTesting.canaryKillerAfter[wrongIndexAfter] = 0;
    ASSERT_DIES(top(&canaryTesting.s));
    canaryTesting.canaryKillerAfter[wrongIndexAfter] = canaryValue; // Restoring canary to properly destruct stack

    destructStack(&canaryTesting.s);
}

TEST(canaryTest, dataCanaryModifyingFailsAssertion) {
    Stack_int s{};

    constructStack(&s);
    int containedValue = 42;
    push(&s, containedValue);

    ((long long*)s._data)[0] = 0;
    ASSERT_DIES(top(&s));
    ((long long*)s._data)[0] = canaryValue; // Restoring canary to properly destruct stack

    ASSERT_EQUALS(top(&s), containedValue); // Just checking that canary is ok before next test

    ((long long*)(s._data + sizeof(long long) * canariesNumber + sizeof(int) * s._capacity))[0] = 0;
    ASSERT_DIES(top(&s));
    ((long long*)(s._data + sizeof(long long) * canariesNumber + sizeof(int) * s._capacity))[0] = canaryValue; // Restoring canary to properly destruct stack

    destructStack(&s);
}

TEST(hashTest, stackMembersModifyingFailsAssertion) {
    Stack_int s{};

    constructStack(&s);
    push(&s, 1);
    push(&s, 2);
    push(&s, 3);
    push(&s, 4);
    push(&s, 5);
    int topValue = top(&s);

    ssize_t realSize = s._size;
    s._size = realSize - 1;
    ASSERT_DIES(top(&s));
    s._size = realSize; // Restoring real value before next check

    ASSERT_EQUALS(top(&s), topValue); // Just checking that hash is ok before next test

    ssize_t realCapacity = s._capacity;
    s._capacity = realCapacity + 1;
    ASSERT_DIES(top(&s));
    s._capacity = realCapacity; // Restoring real value before next check

    ASSERT_EQUALS(top(&s), topValue); // Just checking that hash is ok before next test

    int realHash = s._hash;
    s._hash = realHash + 1;
    ASSERT_DIES(top(&s));
    s._hash = realHash; // Restoring real value to properly destruct stack

    destructStack(&s);
}

TEST(hashTest, stackDataModifyingFailsAssertion) {
    Stack_int s{};

    constructStack(&s);
    int containedValue = 42;
    push(&s, containedValue);

    ((int*)(s._data + sizeof(long long) * canariesNumber))[0] = containedValue + 1;
    ASSERT_DIES(top(&s));
    ((int*)(s._data + sizeof(long long) * canariesNumber))[0] = containedValue; // Restoring real value to properly destruct stack

    destructStack(&s);
}

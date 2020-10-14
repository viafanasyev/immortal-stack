/**
 * @file
 */

#define STACK_SECURITY_LEVEL 3
#define STACK_TYPE int
#include "stack.h"
#undef STACK_TYPE

int main() {
    /* NOTE: This is not a correct program - it should fail an assertion.
     * It just shows an example of stack dump file generation.
     * See stack-dump.txt after executing this program.
     */

    Stack_int s{};
    constructStack(&s);

    push(&s, 1);
    push(&s, 2);

    pop(&s);
    pop(&s);
    pop(&s);

    destructStack(&s);
    return 0;
}
